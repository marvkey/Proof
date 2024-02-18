//https://github.com/GPUOpen-LibrariesAndSDKs/FidelityFX-SDK/blob/d7531ae47d8b36a5d4025663e731a47a38be882f/sdk/include/FidelityFX/gpu/denoiser/ffx_denoiser_reflections_resolve_temporal.h#L104
//https://github.com/qiutang98/flower/blob/0414798840c1c4aef4e742f521696378695e7897/source/shader/sssr/sssr_temporal.glsl
#Compute Shader

#version 450
#extension GL_GOOGLE_include_directive : enable
#extension GL_KHR_shader_subgroup_basic : enable
#extension GL_KHR_shader_subgroup_ballot : enable
#include <PBR/PostProcess/NewScreenSpaceReflection/SSSRCommon.glslh>
layout (binding = 0) uniform texture2D u_SSSRAverageRadiance;
layout (binding = 1) uniform texture2D U_SSSRPrefilterRadiance;
layout (binding = 2) uniform texture2D u_SSSRReprojectedRadiance;
layout (binding = 3) uniform texture2D u_SSSRPrefilterVariance;
layout (binding = 4) uniform texture2D u_PrevSampleCount;
layout (binding = 5) uniform texture2D u_SSSRExtractRoughness;
layout (binding = 6 , rgba16f) uniform image2D o_SSSRTemporalFilterRadiance;
layout (binding = 7 , r16f) uniform image2D o_SSSRTemporalFilterVariance;

vec3 SampleAverageRadiance(vec2 uv) 
{ 
    return texture(sampler2D(u_SSSRAverageRadiance, u_LinearClampBorder0000Sampler), uv).xyz;
}

vec3 LoadRadiance(ivec2 coord) 
{ 
    return texelFetch(U_SSSRPrefilterRadiance, coord, 0).xyz;
}

vec3 LoadRadianceReprojected(ivec2 coord) 
{ 
    return texelFetch(u_SSSRReprojectedRadiance, coord, 0).xyz;
}

float LoadVariance(ivec2 coord) 
{ 
    return texelFetch(u_SSSRPrefilterVariance, coord, 0).x;
}

float LoadNumSamples(ivec2 coord) 
{ 
    return texelFetch(u_PrevSampleCount, coord, 0).x;
}

void StoreTemporalAccumulation(ivec2 coord, vec3 radiance, float variance) 
{
    imageStore(o_SSSRTemporalFilterRadiance, coord, vec4(radiance, 0.0));
    imageStore(o_SSSRTemporalFilterVariance, coord, vec4(variance, 0.0, 0.0, 0.0));
}

shared vec3 sharedData[16][16];

vec3 LoadFromGroupSharedMemory(ivec2 idx) 
{
    return sharedData[idx.y][idx.x];
}

void StoreInGroupSharedMemory(ivec2 idx, vec3 radiance) 
{
    sharedData[idx.y][idx.x] = radiance;
}

struct Moments 
{
    vec3 mean;
    vec3 variance;
};

Moments EstimateLocalNeighborhoodInGroup(ivec2 groupThreadId) 
{
    Moments estimate;
    estimate.mean = vec3(0);
    estimate.variance = vec3(0);

    float accumulatedWeight = 0.0;

    // 9x9 Tent.
    for (int j = -kLocalNeighborhoodRadius; j <= kLocalNeighborhoodRadius; ++j) 
    {
        for (int i = -kLocalNeighborhoodRadius; i <= kLocalNeighborhoodRadius; ++i) 
        {
            ivec2 newIdx  = groupThreadId + ivec2(i, j);

            vec3 radiance = LoadFromGroupSharedMemory(newIdx);
            float weight  = LocalNeighborhoodKernelWeight(i) * LocalNeighborhoodKernelWeight(j);

            accumulatedWeight += weight;
            estimate.mean     += radiance * weight;
            estimate.variance += radiance * radiance * weight;
        }
    }
    estimate.mean     /= accumulatedWeight;
    estimate.variance /= accumulatedWeight;

    estimate.variance = abs(estimate.variance - estimate.mean * estimate.mean);
    return estimate;
}

void LoadNeighborhood(ivec2 coord, out vec3 radiance) 
{ 
    radiance = texelFetch(U_SSSRPrefilterRadiance, coord, 0).xyz;
}

void InitializeGroupSharedMemory(ivec2 dispatchThreadId, ivec2 groupThreadId, ivec2 screenSize) 
{
    // Load 16x16 region into shared memory using 4 8x8 blocks.
    ivec2 offset[4] = {ivec2(0, 0), ivec2(8, 0), ivec2(0, 8), ivec2(8, 8)};

    // Intermediate storage registers to cache the result of all loads
    vec3 radiance[4];

    // Start in the upper left corner of the 16x16 region.
    dispatchThreadId -= 4;

    // First store all loads in registers
    for (int i = 0; i < 4; ++i) 
    {
        LoadNeighborhood(dispatchThreadId + offset[i], radiance[i]);
    }

    // Then move all registers to groupshared memory
    for (int j = 0; j < 4; ++j) 
    {
        StoreInGroupSharedMemory(groupThreadId + offset[j], radiance[j]);
    }
}

vec3 ClipAABB(vec3 aabbMin, vec3 aabbMax, vec3 prevSample) 
{
    // Main idea behind clipping - it prevents clustering when neighbor color space
    // is distant from history sample

    // Here we find intersection between color vector and aabb color box

    // Note: only clips towards aabb center
    vec3 aabbCenter = 0.5 * (aabbMax + aabbMin);
    vec3 extentClip = 0.5 * (aabbMax - aabbMin) + 0.001;

    // Find color vector
    vec3 colorVector = prevSample - aabbCenter;

    // Transform into clip space
    vec3 colorVectorClip = colorVector / extentClip;
    // Find max absolute component
    colorVectorClip = abs(colorVectorClip);

    float maxAbsUnit = max(max(colorVectorClip.x, colorVectorClip.y), colorVectorClip.z);

    if (maxAbsUnit > 1.0) 
    {
        return aabbCenter + colorVector / maxAbsUnit; // clip towards color vector
    } 
    else 
    {
        return prevSample; // point is inside aabb
    }
}
// Rounds value to the nearest multiple of 8
uvec2 RoundUp8(uvec2 value) 
{
    uvec2 roundDown = value & ~0x7;
    return (roundDown == value) ? value : value + 8;
}
void ResolveTemporal(ivec2 dispatchThreadId, ivec2 groupThreadId, uvec2 screenSize, float historyClipWeight) 
{
    InitializeGroupSharedMemory(dispatchThreadId, groupThreadId, ivec2(screenSize));

    groupMemoryBarrier();
    barrier();

    // Center threads in groupshared memory
    groupThreadId += 4; 

    vec3  newSignal = LoadFromGroupSharedMemory(groupThreadId);

    float roughness  = texelFetch(u_SSSRExtractRoughness, dispatchThreadId, 0).x;
    float newVariance = LoadVariance(dispatchThreadId);
    
    if (IsGlossyReflection(roughness)) 
    {
        float numSamples = LoadNumSamples(dispatchThreadId);
        vec2 uv8 = (vec2(dispatchThreadId.xy) + 0.5) / RoundUp8(screenSize);
        vec3 avgRadiance = SampleAverageRadiance(uv8);

        vec3 oldSignal = LoadRadianceReprojected(dispatchThreadId);

        Moments localNeighborhood = EstimateLocalNeighborhoodInGroup(groupThreadId);

        // Clip history based on the curren local statistics
        vec3 colorStd = (sqrt(localNeighborhood.variance.xyz) + length(localNeighborhood.mean.xyz - avgRadiance)) * historyClipWeight * 1.4;
        localNeighborhood.mean.xyz = mix(localNeighborhood.mean.xyz, avgRadiance, 0.2);

        vec3 radianceMin = localNeighborhood.mean.xyz - colorStd;
        vec3 radianceMax = localNeighborhood.mean.xyz + colorStd;

        vec3 clippedOldSignal = ClipAABB(radianceMin, radianceMax, oldSignal.xyz);

        float accumulationSpeed = 1.0 / max(numSamples, 1.0);

        float weight  = (1.0 - accumulationSpeed);

        // Blend with average for small sample count
        newSignal.xyz = mix(newSignal.xyz, avgRadiance, 1.0 / max(numSamples + 1.0f, 1.0));

        // Clip outliers
        {
            vec3 radianceMin = avgRadiance.xyz - colorStd * 1.0;
            vec3 radianceMax = avgRadiance.xyz + colorStd * 1.0;
            newSignal.xyz  = ClipAABB(radianceMin, radianceMax, newSignal.xyz);
        }

        // Blend with history
        newSignal = mix(newSignal, clippedOldSignal, weight);
        newVariance = mix(ComputeTemporalVariance(newSignal.xyz, clippedOldSignal.xyz), newVariance, weight);

        if (any(isinf(newSignal)) || any(isnan(newSignal)) || isinf(newVariance) || isnan(newVariance)) 
        {
            newSignal   = vec3(0.0);
            newVariance = 0.0;
        }
    }
    StoreTemporalAccumulation(dispatchThreadId, newSignal, newVariance);
}

layout (local_size_x = 8, local_size_y = 8) in;
void main()
{
    uint packedCoords = s_DenoiseTileList.Data[int(gl_WorkGroupID)];

    ivec2 dispatchThreadId = ivec2(packedCoords & 0xffffu, (packedCoords >> 16) & 0xffffu) + ivec2(gl_LocalInvocationID.xy);
    ivec2 dispatchGroupId = dispatchThreadId / 8;

    uvec2 remappedGroupThreadId = Remap8x8(gl_LocalInvocationIndex);
    uvec2 remappedDispatchThreadId = dispatchGroupId * 8 + remappedGroupThreadId;

    uvec2 screenSize = textureSize(u_DepthMap, 0);
    ResolveTemporal(ivec2(remappedDispatchThreadId), ivec2(remappedGroupThreadId), screenSize, kTemporalStableFactor);
}