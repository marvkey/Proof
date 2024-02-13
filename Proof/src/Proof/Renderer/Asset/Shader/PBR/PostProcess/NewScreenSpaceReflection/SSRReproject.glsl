//https://github.com/qiutang98/flower/blob/0414798840c1c4aef4e742f521696378695e7897/source/shader/sssr/sssr_reproject.glsl
//https://github.com/GPUOpen-LibrariesAndSDKs/FidelityFX-SDK/blob/d7531ae47d8b36a5d4025663e731a47a38be882f/sdk/include/FidelityFX/gpu/denoiser/ffx_denoiser_reflections_reproject.h#L23
#Compute Shader

#version 450
#extension GL_GOOGLE_include_directive : enable
#extension GL_KHR_shader_subgroup_basic : enable
#extension GL_KHR_shader_subgroup_ballot : enable
#include <PBR/PostProcess/NewScreenSpaceReflection/SSSRCommon.glslh>

layout (set = 0, binding = 0)  uniform texture2D u_SSSRIntersectionMap;
layout (set = 0, binding = 1)  uniform texture2D u_PreviewDepthMap;
layout (set = 0, binding = 2)  uniform texture2D u_PrevSSSRRadianceMap;
layout (set = 0, binding = 3)  uniform texture2D u_PrevNormalMap;
layout (set = 0, binding = 4)  uniform texture2D u_SSSRVarianceHistory;
layout (set = 0, binding = 5) uniform texture2D u_SSSRExtractRoughness;
layout (set = 0, binding = 8) uniform texture2D u_PrevSampleCount; // ssr prevframe sample count. for reproject and temporal pass.
layout (set = 0, binding = 9, rgba16f) uniform image2D o_SSSRReprojectedRadiance; // ssr reproject output radiance.
layout (set = 0, binding = 10, r11f_g11f_b10f) uniform image2D o_SSSRAverageRadiance; 
layout (set = 0, binding = 11, r16f) uniform image2D o_SSRVariance;
layout (set = 0, binding = 12, r16f) uniform image2D o_SSRSampleCount;
layout (set = 0, binding = 13) uniform texture2D u_SSSRPrevExtractRoughness;
layout (set = 0, binding = 14) uniform texture2D u_VelocityMap; 


// Vulkan linearize z.
// NOTE: viewspace z range is [-zFar, -zNear], linear z is viewspace z mul -1 result on vulkan.
// if no exist reverse z:
//       linearZ = zNear * zFar / (zFar +  deviceZ * (zNear - zFar));
//  when reverse z enable, then the function is:
//       linearZ = zNear * zFar / (zNear + deviceZ * (zFar - zNear));
float LinearizeDepth(float z, float n, float f)
{
    return n * f / (z * (f - n) + n);
}

float LinearizeDepth(float z)
{
    const float n = u_Camera.NearPlane;
    const float f = u_Camera.FarPlane;
    return LinearizeDepth(z, n, f);
}

float LinearizeDepthPrev(float z)
{
    const float n = u_Camera.NearPlane;
    const float f = u_Camera.FarPlane;
    return LinearizeDepth(z, n, f);
}

vec3 LoadRadiance(ivec2 coords)
{
    return vec3(texelFetch(u_SSSRIntersectionMap, coords, 0).xyz);
}

float LoadDepth(ivec2 coords) 
{
    return texelFetch(u_DepthMap, coords, 0).x;
}

float SampleDepthHistory(vec2 uv) 
{ 
    return texture(sampler2D(u_PreviewDepthMap, u_LinearClampBorder1111Sampler), uv).r; 
}

float LoadDepthHistory(ivec2 coords) 
{ 
    return texelFetch(u_PreviewDepthMap, coords, 0).x;
}

vec3 SampleRadianceHistory(vec2 uv)
{
    return vec3(texture(sampler2D(u_PrevSSSRRadianceMap, u_LinearClampBorder0000Sampler), uv).rgb);
}

vec3 LoadRadianceHistory(ivec2 coords) 
{ 
    return vec3(texelFetch(u_PrevSSSRRadianceMap, coords, 0).rgb); 
}

vec3 SampleWorldSpaceNormalHistory(vec2 uv)
{
    return normalize(vec3(texture(sampler2D(u_PrevNormalMap, u_LinearClampBorder0000Sampler), uv).rgb));
}

vec3 LoadWorldSpaceNormalHistory(ivec2 coords) 
{
    return vec3(texelFetch(u_PrevNormalMap, coords, 0).rgb);
}

float SampleVarianceHistory(vec2 uv)
{
    return texture(sampler2D(u_SSSRVarianceHistory, u_LinearClampBorder0000Sampler), uv).r; 
}

float SampleNumSamplesHistory(vec2 uv)
{
    return texture(sampler2D(u_PrevSampleCount, u_LinearClampBorder0000Sampler), uv).r; 
}

void StoreRadianceReprojected(ivec2 coord, vec3 value) 
{ 
    imageStore(o_SSSRReprojectedRadiance, coord, vec4(value, 1.0));
}

void StoreAverageRadiance(ivec2 coord, vec3 value) 
{ 
    imageStore(o_SSSRAverageRadiance, coord, vec4(value, 1.0));
}

void StoreVariance(ivec2 coord, float value) 
{ 
    imageStore(o_SSRVariance, coord, vec4(value, 0.0, 0.0, 0.0));
}

void StoreNumSamples(ivec2 coord, float value) 
{ 
    imageStore(o_SSRSampleCount, coord, vec4(value, 0.0, 0.0, 0.0));
}

// 16x16 tile in 8x8 group.
shared vec4 sharedData[16][16];

// Radiance load.
vec3 LoadFromGroupSharedMemory(ivec2 idx) 
{
    return (sharedData[idx.y][idx.x]).xyz;
}

// Radiance store.
void StoreInGroupSharedMemory(ivec2 idx, vec3 radiance) 
{
    (sharedData[idx.y][idx.x]).xyz = radiance;
}

// Radiance and variance store.
void StoreInGroupSharedMemory(ivec2 idx, vec4 radianceVariance) 
{
    sharedData[idx.y][idx.x] = radianceVariance;
}

vec4 LoadFromGroupSharedMemoryRaw(ivec2 idx) 
{
    return sharedData[idx.y][idx.x];
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
        radiance[i] = LoadRadiance(dispatchThreadId + offset[i]);
    }

    // Then move all registers to groupshared memory
    for (int j = 0; j < 4; ++j) 
    {
        StoreInGroupSharedMemory(groupThreadId + offset[j], radiance[j]);
    }
}

// 8x8 downsample luminance weight.
float GetLuminanceWeight(vec3 val) 
{
    float luma = LuminanceSSR(val.xyz);
    float weight = max(exp(-luma * kAverageRadianceLuminanceWeight), 1.0e-2);

    return weight;
}

vec2 GetSurfaceReprojection(vec2 uv, vec2 motionVector) 
{
    vec2 historyUv = uv + motionVector;

    return historyUv;
}

float GetDisocclusionFactor(vec3 normal, vec3 historyNormal, float linearDepth, float historyLinearDepth) 
{
    return 
        exp(-abs(1.0 - max(0.0, dot(normal, historyNormal))) * kDisocclusionNormalWeight) *
        exp(-abs(historyLinearDepth - linearDepth) / linearDepth * kDisocclusionDepthWeight);
}


float GetLinearDepth(vec2 uv, float depth)
{
    vec3 viewSpacePos = InvProjectPosition(vec3(uv, depth), u_Camera.InverseProjection);
    return abs(viewSpacePos.z);
}

vec2 GetHitPositionReprojection(ivec2 dispatchThreadId, vec2 uv, float reflectedRayLength) 
{
    float z = LoadDepth(dispatchThreadId);

    // Viewspace ray position.
    //vec3 viewSpaceRay = getViewPos(uv, z, frameData);
    vec3 viewSpaceRay = ScreenSpaceToViewSpace(vec3(uv,z));
    // We start out with reconstructing the ray length in view space.
    // This includes the portion from the camera to the reflecting surface as well as the portion from the surface to the hit position.
    float surfaceDepth = length(viewSpaceRay);
    float rayLength = surfaceDepth + reflectedRayLength;

    // We then perform a parallax correction by shooting a ray
    // of the same length "straight through" the reflecting surface
    // and reprojecting the tip of that ray to the previous frame.
    viewSpaceRay /= surfaceDepth; // == normalize(viewSpaceRay)
    viewSpaceRay *= rayLength;

    // This is the "fake" hit position if we would follow the ray straight through the surface.
    vec3 worldHitPosition = (u_Camera.InverseView * vec4(viewSpaceRay, 1.0)).xyz; 
    
    // Project to prev frame position.
    vec3 prevHitPosition =  ProjectPosition(worldHitPosition, u_Camera.PrevViewProjectionMatrix);

    return prevHitPosition.xy;
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

    // 9x9 tent.
    float accumulatedWeight = float(0);
    for (int j = -kLocalNeighborhoodRadius; j <= kLocalNeighborhoodRadius; ++j) 
    {
        for (int i = -kLocalNeighborhoodRadius; i <= kLocalNeighborhoodRadius; ++i) 
        {
            // TODO: Optimize. Can pre-compute.
            float weight = LocalNeighborhoodKernelWeight(i) * LocalNeighborhoodKernelWeight(j);

            ivec2 newIdx  = groupThreadId + ivec2(i, j);
            vec3 radiance = LoadFromGroupSharedMemory(newIdx);

            // Accumulate.
            accumulatedWeight += weight;
            estimate.mean += radiance * weight;
            estimate.variance += radiance * radiance * weight;
        }
    }

    // Weight mean.
    estimate.mean /= accumulatedWeight;

    // Variance compute.
    estimate.variance /= accumulatedWeight;
    estimate.variance = abs(estimate.variance - estimate.mean * estimate.mean);

    return estimate;
}

void PickReprojection(
    ivec2 dispatchThreadId,
    ivec2 groupThreadId, 
    uvec2 screenSize,   
    float roughness,
    float rayLength,
    out float disocclusionFactor,
    out vec2 reprojectionUV, 
    out vec3 reprojection) 
{
    Moments localNeighborhood = EstimateLocalNeighborhoodInGroup(groupThreadId);
    vec2 uv = vec2(dispatchThreadId.x + 0.5, dispatchThreadId.y + 0.5) / screenSize;

    vec3 normal = texelFetch(u_NormalMap, dispatchThreadId, 0).rgb;

    vec3 historyNormal;
    float historyLinearDepth;
    {
        const vec2 motionVector = texelFetch(u_VelocityMap, dispatchThreadId, 0).rg; 

        // Then get surface prev-frame uv.
        const vec2 surfaceReprojectionUV = GetSurfaceReprojection(uv, motionVector);

        // Compute prev-frame hit uv.
        const vec2 hitReprojectionUV = GetHitPositionReprojection(dispatchThreadId, uv, rayLength);

        // linear sample surface normal and hit normal. from prev-frame.
        const vec3 surfaceNormal = SampleWorldSpaceNormalHistory(surfaceReprojectionUV);
        const vec3 hitNormal = SampleWorldSpaceNormalHistory(hitReprojectionUV);

        // linear sample radiance from prev-frame.
        const vec3 surfaceHistory = SampleRadianceHistory(surfaceReprojectionUV);
        const vec3 hitHistory = SampleRadianceHistory(hitReprojectionUV);

        // Compute normal similarity.
        const float surfaceNormalSimilarity = dot(normalize(vec3(surfaceNormal)), normalize(vec3(normal)));
        const float hitNormalSimilarity = dot(normalize(vec3(hitNormal)), normalize(vec3(normal)));
        
        // linear sample roughness from prev-frame.
        const float surfaceRoughness = float(texture(sampler2D(u_SSSRPrevExtractRoughness, u_LinearClampBorder0000Sampler), surfaceReprojectionUV).r);
        const float hitRoughness = float(texture(sampler2D(u_SSSRPrevExtractRoughness, u_LinearClampBorder0000Sampler), hitReprojectionUV).r);
        
        // Choose reprojection uv based on similarity to the local neighborhood.
        if (hitNormalSimilarity > kReprojectionNormalSimilarityThreshold  // Candidate for mirror reflection parallax
            && (hitNormalSimilarity + 1.0e-3) > surfaceNormalSimilarity    
            && abs(hitRoughness - roughness) < abs(surfaceRoughness - roughness) + 1.0e-3
        ) 
        {
            historyNormal = hitNormal;

            float hitHistoryDepth = SampleDepthHistory(hitReprojectionUV);
            float hitHistoryLinearDepth = LinearizeDepthPrev(hitHistoryDepth);

            historyLinearDepth = hitHistoryLinearDepth;
            reprojectionUV = hitReprojectionUV;
            reprojection = hitHistory;
        } 
        else 
        {
            // Reject surface reprojection based on simple distance
            vec3 surfaceHistoryDiff = surfaceHistory - localNeighborhood.mean;
            if (dot(surfaceHistoryDiff, surfaceHistoryDiff) < kReprojectSurfaceDiscardVarianceWeight * length(localNeighborhood.variance)) 
            {
                historyNormal = surfaceNormal;

                float surfaceHistoryDepth = SampleDepthHistory(surfaceReprojectionUV);
                float surfaceHistoryLinearDepth = LinearizeDepthPrev(surfaceHistoryDepth);

                historyLinearDepth = surfaceHistoryLinearDepth;
                reprojectionUV = surfaceReprojectionUV;
                reprojection = surfaceHistory;
            } 
            else 
            {
                disocclusionFactor = 0.0;
                return;
            }
        }
    }

    float depth = LoadDepth(dispatchThreadId);
    float linearDepth = LinearizeDepthPrev(depth);

    // Determine disocclusion factor based on history
    disocclusionFactor = GetDisocclusionFactor(normal, historyNormal, linearDepth, historyLinearDepth);

    if (disocclusionFactor > kDisocclusionThreshold) // Early out, good enough
    {
        return;
    }

    // Try to find the closest sample in the vicinity if we are not convinced of a disocclusion
    if (disocclusionFactor < kDisocclusionThreshold) 
    {
        vec2 closestUv = reprojectionUV;
        vec2 dudv = 1.0 / vec2(screenSize);

        const int kSearchRadius = 1;
        for (int y = -kSearchRadius; y <= kSearchRadius; y++) 
        {
            for (int x = -kSearchRadius; x <= kSearchRadius; x++) 
            {
                vec2 uv = reprojectionUV + vec2(x, y) * dudv;

                vec3 historyNormal = SampleWorldSpaceNormalHistory(uv);
                float historyDepth = SampleDepthHistory(uv);

                float historyLinearDepth = LinearizeDepthPrev(historyDepth);

                float weight = GetDisocclusionFactor(normal, historyNormal, linearDepth, historyLinearDepth);
                if (weight > disocclusionFactor) 
                {
                    disocclusionFactor = weight;
                    closestUv = uv;
                    reprojectionUV = closestUv;
                }
            }
        }
        reprojection = SampleRadianceHistory(reprojectionUV);
    }

    // Rare slow path - triggered only on the edges.
    // Try to get rid of potential leaks at bilinear interpolation level.
    if (disocclusionFactor < kDisocclusionThreshold)
    {
        // If we've got a discarded history, try to construct a better sample out of 2x2 interpolation neighborhood
        // Helps quite a bit on the edges in movement
        float uvx = fract(float(screenSize.x) * reprojectionUV.x + 0.5);
        float uvy = fract(float(screenSize.y) * reprojectionUV.y + 0.5);

        ivec2 reprojectTexelCoords = ivec2(screenSize * reprojectionUV - 0.5);

        vec3 reprojection00 = LoadRadianceHistory(reprojectTexelCoords + ivec2(0, 0));
        vec3 reprojection10 = LoadRadianceHistory(reprojectTexelCoords + ivec2(1, 0));
        vec3 reprojection01 = LoadRadianceHistory(reprojectTexelCoords + ivec2(0, 1));
        vec3 reprojection11 = LoadRadianceHistory(reprojectTexelCoords + ivec2(1, 1));

        vec3 normal00 = LoadWorldSpaceNormalHistory(reprojectTexelCoords + ivec2(0, 0));
        vec3 normal10 = LoadWorldSpaceNormalHistory(reprojectTexelCoords + ivec2(1, 0));
        vec3 normal01 = LoadWorldSpaceNormalHistory(reprojectTexelCoords + ivec2(0, 1));
        vec3 normal11 = LoadWorldSpaceNormalHistory(reprojectTexelCoords + ivec2(1, 1));

        float depth00 = LinearizeDepthPrev(LoadDepthHistory(reprojectTexelCoords + ivec2(0, 0)));
        float depth10 = LinearizeDepthPrev(LoadDepthHistory(reprojectTexelCoords + ivec2(1, 0)));
        float depth01 = LinearizeDepthPrev(LoadDepthHistory(reprojectTexelCoords + ivec2(0, 1)));
        float depth11 = LinearizeDepthPrev(LoadDepthHistory(reprojectTexelCoords + ivec2(1, 1)));

        vec4 w = vec4(1.0);

        // Initialize with occlusion weights
        w.x = GetDisocclusionFactor(normal, normal00, linearDepth, depth00) > (kDisocclusionThreshold / 2.0) ? 1.0 : 0.0;
        w.y = GetDisocclusionFactor(normal, normal10, linearDepth, depth10) > (kDisocclusionThreshold / 2.0) ? 1.0 : 0.0;
        w.z = GetDisocclusionFactor(normal, normal01, linearDepth, depth01) > (kDisocclusionThreshold / 2.0) ? 1.0 : 0.0;
        w.w = GetDisocclusionFactor(normal, normal11, linearDepth, depth11) > (kDisocclusionThreshold / 2.0) ? 1.0 : 0.0;
        
        // And then mix in bilinear weights
        w.x = w.x * (1.0 - uvx) * (1.0 - uvy);
        w.y = w.y * (uvx) * (1.0 - uvy);
        w.z = w.z * (1.0 - uvx) * (uvy);
        w.w = w.w * (uvx) * (uvy);

        // Get final max weight.
        float ws = max(w.x + w.y + w.z + w.w, 1.0e-3);

        // normalize
        w /= ws;

        vec3 historyNormal;
        float historyLinearDepth;

        reprojection       = reprojection00 * w.x + reprojection10 * w.y + reprojection01 * w.z + reprojection11 * w.w;
        historyLinearDepth = depth00 * w.x + depth10 * w.y + depth01 * w.z + depth11 * w.w;
        historyNormal      = normal00 * w.x + normal10 * w.y + normal01 * w.z + normal11 * w.w;
        disocclusionFactor = GetDisocclusionFactor(normal, historyNormal, linearDepth, historyLinearDepth);
    }
    disocclusionFactor = disocclusionFactor < kDisocclusionThreshold ? 0.0 : disocclusionFactor;
}

void Reproject(ivec2 dispatchThreadId, ivec2 groupThreadId, uvec2 screenSize, float temporalStabilityFactor, int maxSamples) 
{
    InitializeGroupSharedMemory(dispatchThreadId, groupThreadId, ivec2(screenSize));

    groupMemoryBarrier();
    barrier();

    // Center threads in groupshared memory
    groupThreadId += ivec2(4);

    float variance   = 1.0;
    float numSamples = 0.0;
    float roughness  = float(texelFetch(u_SSSRExtractRoughness, dispatchThreadId, 0).r);
    
    vec3 normal = texelFetch(u_NormalMap, dispatchThreadId, 0).rgb;

    vec4 intersectResult = texelFetch(u_SSSRIntersectionMap, dispatchThreadId, 0);
    vec3 radiance = vec3(intersectResult.xyz);
    const float rayLength = float(intersectResult.w);

    if (IsGlossyReflection(roughness)) 
    {
        float disocclusionFactor;
        vec2 reprojectionUV;
        vec3 reprojection;

        PickReprojection(
            /* in  */ dispatchThreadId,
            /* in  */ groupThreadId,
            /* in  */ screenSize,
            /* in  */ roughness,
            /* in  */ rayLength,
            /* out */ disocclusionFactor,
            /* out */ reprojectionUV,
            /* out */ reprojection
        );

        if (reprojectionUV.x > 0.0 && reprojectionUV.y > 0.0 && reprojectionUV.x < 1.0 && reprojectionUV.y < 1.0) 
        {
            float prevVariance = SampleVarianceHistory(reprojectionUV);
            numSamples = SampleNumSamplesHistory(reprojectionUV) * disocclusionFactor;

            // Config sample nums.
            float sMaxSamples = max(8.0, float(maxSamples) * (1.0 - exp(-roughness * 100.0)));
            numSamples = min(sMaxSamples, numSamples + 1);

            float newVariance  = ComputeTemporalVariance(radiance.xyz, reprojection.xyz);
            if (disocclusionFactor < kDisocclusionThreshold) 
            {
                StoreRadianceReprojected(dispatchThreadId, vec3(0.0));
                StoreVariance(dispatchThreadId, 1.0);
                StoreNumSamples(dispatchThreadId, 1.0);
            } 
            else 
            {
                float varianceMix = mix(newVariance, prevVariance, 1.0 / numSamples);

                StoreRadianceReprojected(dispatchThreadId, reprojection);
                StoreVariance(dispatchThreadId, varianceMix);
                StoreNumSamples(dispatchThreadId, numSamples);
                
                // Mix in reprojection for radiance mip computation 
                radiance = mix(radiance, reprojection, 0.3);
            }
        } 
        else 
        {
            StoreRadianceReprojected(dispatchThreadId, vec3(0.0));
            StoreVariance(dispatchThreadId, 1.0);
            StoreNumSamples(dispatchThreadId, 1.0);
        }
    }
    
    // Downsample 8x8 -> 1 radiance using groupshared memory
    // Initialize groupshared array for downsampling
    float weight = GetLuminanceWeight(radiance.xyz);
    radiance.xyz *= weight;

    if (
        any(bvec2(dispatchThreadId.x >= screenSize.x, dispatchThreadId.y >= screenSize.y))
     || any(isinf(radiance)) 
     || any(isnan(radiance)) 
     || weight > 1.0e3) 
    {
        radiance = vec3(0.0);
        weight   = 0.0;
    }

    groupThreadId -= 4; // Center threads in groupshared memory

    StoreInGroupSharedMemory(groupThreadId, vec4(radiance.xyz, weight));

    groupMemoryBarrier();
    barrier();

    for (int i = 2; i <= 8; i = i * 2) 
    {
        int ox = groupThreadId.x * i;
        int oy = groupThreadId.y * i;
        int ix = groupThreadId.x * i + i / 2;
        int iy = groupThreadId.y * i + i / 2;
        if (ix < 8 && iy < 8) 
        {
            vec4 rad_weight00 = LoadFromGroupSharedMemoryRaw(ivec2(ox, oy));
            vec4 rad_weight10 = LoadFromGroupSharedMemoryRaw(ivec2(ox, iy));
            vec4 rad_weight01 = LoadFromGroupSharedMemoryRaw(ivec2(ix, oy));
            vec4 rad_weight11 = LoadFromGroupSharedMemoryRaw(ivec2(ix, iy));

            vec4 sum = rad_weight00 + rad_weight01 + rad_weight10 + rad_weight11;
            StoreInGroupSharedMemory(ivec2(ox, oy), sum);
        }

        groupMemoryBarrier();
        barrier();
    }

    if (groupThreadId.x == 0 && groupThreadId.y == 0) 
    {
        vec4 sum = LoadFromGroupSharedMemoryRaw(ivec2(0, 0));
        float weightAcc = max(sum.w, 1.0e-3);

        vec3 radianceAvg = sum.xyz / weightAcc;
        StoreAverageRadiance(dispatchThreadId.xy / 8, radianceAvg);
    }
}

layout (local_size_x = 8, local_size_y = 8) in;
void main()
{
/*
    uint packedCoords = s_DenoiseTileList.Data[int(gl_WorkGroupID)];

    ivec2 dispatchThreadId = ivec2(packedCoords & 0xffffu, (packedCoords >> 16) & 0xffffu) + ivec2(gl_LocalInvocationID.xy);
    ivec2 dispatchGroupId = dispatchThreadId / 8;

    uvec2 remappedGroupThreadId = Remap8x8(gl_LocalInvocationIndex);
    uvec2 remappedDispatchThreadId = dispatchGroupId * 8 + remappedGroupThreadId;

    uvec2 screenSize = textureSize(u_DepthMap, 0);
    Reproject(ivec2(remappedDispatchThreadId), ivec2(remappedGroupThreadId), screenSize, kTemporalStableReprojectFactor, kTemporalPeriod);
    */
}