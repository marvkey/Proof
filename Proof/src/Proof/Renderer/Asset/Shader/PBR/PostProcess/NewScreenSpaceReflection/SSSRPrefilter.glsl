//https://github.com/qiutang98/flower/blob/0414798840c1c4aef4e742f521696378695e7897/source/shader/sssr/sssr_prefilter.glsl
//https://github.com/GPUOpen-LibrariesAndSDKs/FidelityFX-SDK/blob/d7531ae47d8b36a5d4025663e731a47a38be882f/sdk/include/FidelityFX/gpu/denoiser/ffx_denoiser_reflections_prefilter.h#L120
#compute shader

#extension GL_GOOGLE_include_directive : enable
#extension GL_KHR_shader_subgroup_basic : enable
#extension GL_KHR_shader_subgroup_ballot : enable
#include <PBR/PostProcess/NewScreenSpaceReflection/SSSRCommon.glslh>


vec3 SampleAverageRadiance(vec2 uv) 
{
    return texture(sampler2D(inSSRAverageRadiance, linearClampBorder0000Sampler), uv).rgb;
}

float LoadRoughness(ivec2 coord) 
{
    return texelFetch(inSSRExtractRoughness, coord, 0).r;
}

void StorePrefilteredReflections(ivec2 coord, vec3 radiance, float variance) 
{
    imageStore(SSRPrefilterRadiance, coord, vec4(radiance, 0.0));
    imageStore(SSRPrefilterVariance, coord, vec4(variance, 0.0, 0.0, 0.0));
}

shared vec4 sharedData0[16][16];
shared vec4 sharedData1[16][16];

struct NeighborhoodSample 
{
    vec3 radiance;
    float variance;
    vec3 normal;
    float depth;
};

void LoadNeighborhood(
    ivec2 coord,
    out vec3 radiance,
    out float variance,
    out vec3 normal,
    out float depth,
    ivec2 screenSize) 
{
    radiance = texelFetch(inSSRIntersection, coord, 0).xyz;
    variance = texelFetch(inSSRVariance, coord, 0).x;
    normal = texelFetch(inGbufferB, coord, 0).xyz;

    float deviceZ = texelFetch(inDepth, coord, 0).r;
    depth = linearizeDepth(deviceZ, frameData);
}

void StoreInGroupSharedMemory(ivec2 idx, vec3 radiance, float variance, vec3 normal, float depth) 
{
    sharedData0[idx.y][idx.x] = vec4(radiance, variance);
    sharedData1[idx.y][idx.x] = vec4(normal, depth);
}

NeighborhoodSample LoadFromGroupSharedMemory(ivec2 idx) 
{
    vec4 sample0 = sharedData0[idx.y][idx.x];
    vec4 sample1 = sharedData1[idx.y][idx.x];

    NeighborhoodSample result;

    result.radiance = sample0.xyz;
    result.variance = sample0.w;

    result.normal = sample1.xyz;
    result.depth  = sample1.w;

    return result;
}

void InitializeGroupSharedMemory(ivec2 dispatchThreadId, ivec2 groupThreadId, ivec2 screenSize) 
{
    // Load 16x16 region into shared memory using 4 8x8 blocks.
    ivec2 offset[4] = {ivec2(0, 0), ivec2(8, 0), ivec2(0, 8), ivec2(8, 8)};

    // Intermediate storage registers to cache the result of all loads
    vec3 radiance[4];
    float variance[4];

    vec3 normal[4];
    float depth[4];

    // Start in the upper left corner of the 16x16 region.
    dispatchThreadId -= 4;

    // First store all loads in registers
    for (int i = 0; i < 4; ++i) 
    {
        loadNeighborhood(dispatchThreadId + offset[i], radiance[i], variance[i], normal[i], depth[i], screenSize);
    }

    // Then move all registers to groupshared memory
    for (int j = 0; j < 4; ++j) 
    {
        storeInGroupSharedMemory(groupThreadId + offset[j], radiance[j], variance[j], normal[j], depth[j]);
    }
}

float GetEdgeStoppingNormalWeight(vec3 normalP, vec3 normalQ) 
{
    return pow(max(dot(normalP, normalQ), 0.0), kPrefilterNormalSigma);
}

float GetEdgeStoppingDepthWeight(float centerDepth, float neighborDepth) 
{
    return exp(-abs(centerDepth - neighborDepth) * centerDepth * kPrefilterDepthSigma);
}

float GetRadianceWeight(vec3 centerRadiance, vec3 neighborRadiance, float variance) 
{
    return max(exp(-(kRadianceWeightBias + variance * kRadianceWeightVarianceK) * length(centerRadiance - neighborRadiance.xyz)), 1.0e-2);
}

void Resolve(ivec2 groupThreadId, vec3 avgRadiance, NeighborhoodSample center, out vec3 resolvedRadiance, out float resolvedVariance)
{
    // Initial weight is important to remove fireflies.
    // That removes quite a bit of energy but makes everything much more stable.
    float accumulatedWeight = getRadianceWeight(avgRadiance, center.radiance, center.variance);

    vec3 accumulatedRadiance = center.radiance * accumulatedWeight;
    float  accumulatedVariance = center.variance * accumulatedWeight * accumulatedWeight;

    // First 15 numbers of Halton(2,3) streteched to [-3,3]. Skipping the center, as we already have that in center_radiance and center_variance.
    const uint kSampleCount = 15;
    const ivec2 kSampleOffsets[kSampleCount] = ivec2[](
        ivec2( 0,  1),  
        ivec2(-2,  1),  
        ivec2( 2, -3), 
        ivec2(-3,  0),  
        ivec2( 1,  2), 
        ivec2(-1, -2), 
        ivec2( 3,  0), 
        ivec2(-3,  3),
        ivec2( 0, -3), 
        ivec2(-1, -1), 
        ivec2( 2,  1),  
        ivec2(-2, -2), 
        ivec2( 1,  0), 
        ivec2( 0,  2),   
        ivec2( 3, -1)
    );
    float varianceWeight = max(kPrefilterVarianceBias, 1.0 - exp(-(center.variance * kPrefilterVarianceWeight)));
    for (int i = 0; i < kSampleCount; ++i) 
    {
        ivec2 newIdx = groupThreadId + kSampleOffsets[i];
        NeighborhoodSample neighbor = loadFromGroupSharedMemory(newIdx);

        float weight = 1.0;
        weight *= getEdgeStoppingNormalWeight(vec3(center.normal), vec3(neighbor.normal));
        weight *= getEdgeStoppingDepthWeight(center.depth, neighbor.depth);
        weight *= getRadianceWeight(avgRadiance, neighbor.radiance.xyz, center.variance);
        weight *= varianceWeight;

        // Accumulate all contributions.
        accumulatedWeight += weight;
        accumulatedRadiance += weight * neighbor.radiance;
        accumulatedVariance += weight * weight * neighbor.variance;
    }

    accumulatedRadiance /= accumulatedWeight;
    accumulatedVariance /= (accumulatedWeight * accumulatedWeight);
    resolvedRadiance = accumulatedRadiance;
    resolvedVariance = accumulatedVariance;
}

void Prefilter(ivec2 dispatchThreadId, ivec2 groupThreadId, uvec2 screenSize) 
{
    float centerRoughness = loadRoughness(dispatchThreadId).r;
    initializeGroupSharedMemory(dispatchThreadId, groupThreadId, ivec2(screenSize));

    groupMemoryBarrier();
    barrier();

    groupThreadId += 4; // Center threads in groupshared memory

    NeighborhoodSample center = loadFromGroupSharedMemory(groupThreadId);

    vec3 resolvedRadiance = center.radiance;
    float resolvedVariance = center.variance;

    // Check if we have to denoise or if a simple copy is enough
    bool bNeedDenoiser = center.variance > 0.0 && isGlossyReflection(centerRoughness) && !isMirrorReflection(centerRoughness);
    if (bNeedDenoiser) 
    {
        vec2 uv8 = (vec2(dispatchThreadId.xy) + 0.5) / roundUp8(screenSize);

        vec3 avgRadiance = sampleAverageRadiance(uv8);
        resolve(groupThreadId, avgRadiance, center, resolvedRadiance, resolvedVariance);
    }

    storePrefilteredReflections(dispatchThreadId, resolvedRadiance, resolvedVariance);
}

layout (local_size_x = 8, local_size_y = 8) in;
void main()
{
    uint packedCoords = ssboDenoiseTileList.data[int(gl_WorkGroupID)];

    ivec2 dispatchThreadId = ivec2(packedCoords & 0xffffu, (packedCoords >> 16) & 0xffffu) + ivec2(gl_LocalInvocationID.xy);
    ivec2 dispatchGroupId = dispatchThreadId / 8;

    uvec2 remappedGroupThreadId = remap8x8(gl_LocalInvocationIndex);
    uvec2 remappedDispatchThreadId = dispatchGroupId * 8 + remappedGroupThreadId;

    uvec2 screenSize = textureSize(inDepth, 0);
    Prefilter(ivec2(remappedDispatchThreadId), ivec2(remappedGroupThreadId), screenSize);
}