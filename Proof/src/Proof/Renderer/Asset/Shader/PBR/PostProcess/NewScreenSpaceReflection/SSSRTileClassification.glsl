//https://github.com/qiutang98/flower/blob/main/source/shader/sssr/sssr_tile.glsl
//https://github.com/GPUOpen-LibrariesAndSDKs/FidelityFX-SDK/blob/main/sdk/include/FidelityFX/gpu/sssr/ffx_sssr_classify_tiles.h
#Compute Shader

#version 450
#extension GL_KHR_shader_subgroup_basic : enable
#extension GL_KHR_shader_subgroup_ballot : enable
#extension GL_KHR_shader_subgroup_quad : enable
#extension GL_KHR_shader_subgroup_shuffle : enable

#include <PBR/PostProcess/NewScreenSpaceReflection/SSSRCommon.glslh>
layout (set = 0, binding = 0, r8) uniform image2D o_SSRExtractRoughness; // ssr roughness extract.
layout (binding = 2) uniform sampler2D u_SSRVarianceHistory; // SSR variance help to filter.
layout (binding = 3) uniform sampler2D u_MetalnessRoughness;
layout (set = 0, binding = 4, rgba16f) uniform image2D o_SSRIntersection;

float LoadDepth(ivec2 coord, int mip)
{
    return texelFetch(u_HZBMap, coord, mip).r; // use cloest depth.
}

bool IsBaseRay(uvec2 dispatchThreadId, uint samplesPerQuad) 
{
    switch (samplesPerQuad) 
    {
    case 1: // Deactivates 3 out of 4 rays
        return ((dispatchThreadId.x & 1) | (dispatchThreadId.y & 1)) == 0; 
    case 2: // Deactivates 2 out of 4 rays. Keeps diagonal.
        return (dispatchThreadId.x & 1) == (dispatchThreadId.y & 1); 
    default: // Other case 4. all is base ray.
        return true;
    }
}
shared uint sharedTileCount;
bool IsReflectiveSurface(uvec2 pixel_coordinate, float roughness)
{
    //https://github.com/GPUOpen-LibrariesAndSDKs/FidelityFX-SDK/blob/main/sdk/include/FidelityFX/gpu/sssr/ffx_sssr_callbacks_glsl.h#L480
    //line(480)
    #if INVERTED_DEPTH
        const float far_plane = 0.0f;
        return texelFetch(u_HZBMap, ivec2(pixel_coordinate), 0).r > far_plane;
    #else //  FFX_SSSR_OPTION_INVERTED_DEPTH
        const float far_plane = 1.0f;
        return texelFetch(u_HZBMap, ivec2(pixel_coordinate), 0).r < far_plane;
    #endif //  FFX_SSSR_OPTION_INVERTED_DEPTH
}
// For 8-bit unorm texture, float error range = 1.0 / 255.0 = 0.004
#define kShadingModelRangeCheck        0.005

// Shading model count is 50, Step value is 0.02
#define kShadingModelUnvalid           0.00

bool isShadingModelValid(float v)
{
    return v > (kShadingModelUnvalid + kShadingModelRangeCheck);
}

void ClassifyTiles(uvec2 dispatchThreadId, uvec2 groupThreadId, float roughness)
{
    sharedTileCount = 0;
    const uint samplesPerQuad = u_PushData.SamplesPerQuad;

    const uvec2 workSize = textureSize(u_MetalnessRoughness, 0);

    bool bFirstLaneWave = subgroupElect();

    // First, we figure out on a per-thread basis if we need to shoot a reflection ray.
    // Disable offscreen pixels
    bool bNeedsRay = !(dispatchThreadId.x >= workSize.x || dispatchThreadId.y >= workSize.y);

    // Don't shoot a ray on very rough surfaces.
    bool bReflectiveSurface = IsReflectiveSurface(dispatchThreadId, roughness);
    bool bGlossyReflection = IsGlossyReflection(roughness);
    bNeedsRay = bNeedsRay && bGlossyReflection && bReflectiveSurface;

    // Also, we don't need to run the denoiser on mirror reflections.
    const bool bNeedsDenoiser = bNeedsRay && !IsMirrorReflection(roughness);

    // Decide which ray to keep
    bool bBaseRay = IsBaseRay(dispatchThreadId, samplesPerQuad);
    bNeedsRay = bNeedsRay && (!bNeedsDenoiser || bBaseRay); // Make sure not to deactivate mirror reflection rays.

    
    // Create ray if temporal variance guided enable.
    const bool bTemporalVarianceGuidedEnable = u_PushData.TemporalVarianceGuidedTracingEnabled > 0;
    if (bTemporalVarianceGuidedEnable && bNeedsDenoiser && !bNeedsRay) 
    {
        bool bHasTemporalVariance = texelFetch(u_SSRVarianceHistory, ivec2(dispatchThreadId), 0).r > u_PushData.TemporalVarianceThreshold;
        bNeedsRay = bNeedsRay || bHasTemporalVariance;
    }
      // Sync for shared tile count clear done.
    groupMemoryBarrier();
    barrier();
    // Now we know for each thread if it needs to shoot a ray and whether or not a denoiser pass has to run on this pixel.

    if (bGlossyReflection && bReflectiveSurface) 
    {
        atomicAdd(sharedTileCount, 1);
    }

    // Next, we have to figure out for which pixels that ray is creating the values for.
    // Thus, if we have to copy its value horizontally, vertically, or across.
    bool requireCopy = !bNeedsRay && bNeedsDenoiser; // Our pixel only requires a copy if we want to run a denoiser on it but don't want to shoot a ray for it.
    bool copyHorizontal = AWaveReadAtLaneIndexB1(requireCopy, AWaveLaneIndex() ^ 1) && (samplesPerQuad != 4) && bBaseRay; // QuadReadAcrossX
    bool copyVertical = AWaveReadAtLaneIndexB1(requireCopy, AWaveLaneIndex() ^ 2) && (samplesPerQuad == 1) && bBaseRay; // QuadReadAcrossY
    bool copyDiagonal = AWaveReadAtLaneIndexB1(requireCopy, AWaveLaneIndex() ^ 3) && (samplesPerQuad == 1) && bBaseRay; // QuadReadAcrossDiagonal

    // Thus, we need to compact the rays and append them all at once to the ray list.
    uint localRayIndexWave = AWavePrefixCountBits(bNeedsRay);
    uint waveRayCount = AWaveActiveCountBits(bNeedsRay);
    uint baseRayIndex;
    if (bFirstLaneWave) 
    {
        IncrementRayCounter(waveRayCount, baseRayIndex);
    }
    baseRayIndex = AWaveReadLaneFirstU1(baseRayIndex);
    if (bNeedsRay) 
    {
        int ray_index = int(baseRayIndex + localRayIndexWave);
        AddRay(ray_index, ivec2(dispatchThreadId.xy), copyHorizontal, copyVertical, copyDiagonal);
    }

    vec4 intersectionOutput = vec4(0.0, 0.0, 0.0, 0.0);
    if (bReflectiveSurface && !bGlossyReflection) 
    {
        // Fall back to the environment map without preparing a ray
        vec2 uv = (vec2(dispatchThreadId.xy) + 0.5) * 1/workSize;
        vec3 worldSpaceNormal = LoadWorldSpaceNormal(ivec2(dispatchThreadId.xy));
        float z = LoadDepth(ivec2(dispatchThreadId.xy), 0);
        vec3 screen_uv_space_ray_origin = vec3(uv, z);
        vec3 view_space_ray = ScreenSpaceToViewSpace(screen_uv_space_ray_origin);
        vec3 view_space_ray_direction = normalize(view_space_ray);
        vec3 view_space_surface_normal = vec4(u_Camera.View *  vec4(worldSpaceNormal, 0)).xyz;
        vec3 view_space_reflected_direction = reflect(view_space_ray_direction, view_space_surface_normal);
        vec3 world_space_reflected_direction  = vec4(u_Camera.InverseView * vec4(view_space_reflected_direction, 0)).xyz;

        intersectionOutput.xyz = SampleEnvironmentMap(world_space_reflected_direction, sqrt(roughness));
    }

    imageStore(o_SSRIntersection, ivec2(dispatchThreadId), intersectionOutput);

    barrier(); // Wait until g_TileCount

    
    // Add denoise tile info.
    if ((groupThreadId.x == 0) && (groupThreadId.y == 0) && (sharedTileCount > 0)) 
    {
        uint tileOffset = AddDenoiseTileCount();
        AddDenoiserTile(tileOffset, dispatchThreadId);
    }
/*
// Shared tile count clear.
    sharedTileCount = 0;

    const uvec2 workSize = textureSize(u_MetalnessRoughness, 0);
    const uint samplesPerQuad = u_PushData.SamplesPerQuad;

    const bool bAllInScreen = (dispatchThreadId.x < workSize.x) && (dispatchThreadId.y < workSize.y);
    //const bool bCanReflective = isShadingModelValid(texelFetch(inGbufferA, ivec2(dispatchThreadId), 0).a); // in the api copied a is if the shaidn gmodel if valid
    const bool bCanReflective = IsReflectiveSurface(dispatchThreadId,roughness);

    const bool bMirrorPlane = IsMirrorReflection(roughness);
    const bool bBaseRay = IsBaseRay(dispatchThreadId, samplesPerQuad);

      // Only need ray in these case.
    bool bNeedRay = bAllInScreen && bCanReflective;

    // Only run denoise for no mirror plane.
    bool bNeedDenoise = bNeedRay && (!bMirrorPlane); 

    // If is mirror plane, always full ray.
    // If not mirror plane, add deactivate check.
    bNeedRay = bNeedRay && (!bNeedDenoise || bBaseRay);

    // Create ray if temporal variance guided enable.
    const bool bTemporalVarianceGuidedEnable = u_PushData.TemporalVarianceGuidedTracingEnabled > 0;
    if(bTemporalVarianceGuidedEnable && bNeedDenoise && (!bNeedRay))
    {
        const bool bTemporalVarianceRequire = texelFetch(u_SSRVarianceHistory, ivec2(dispatchThreadId), 0).r > u_PushData.TemporalVarianceThreshold;
        bNeedRay = bNeedRay || bTemporalVarianceRequire;
    }

    // Sync for shared tile count clear done.
    groupMemoryBarrier();
    barrier();

    // Add tile count flag when plane is reflective.
    if(bCanReflective)
    {
        atomicAdd(sharedTileCount, 1);
    }

      // Our pixel only requires a copy if we want to run a denoiser on it but don't want to shoot a ray for it.
    const bool bRequireCopy = (!bNeedRay) && bNeedDenoise;

    // Issue: https://github.com/google/shaderc/issues/1277
    // Read quad data 
    // Exist one bad fact here after compile:
    //
    //  uint* _485 = &samplesPerQuadd;
    //  uint _486 = *_485;
    //  bool _488 = _486 != 4;
    //  bool _489 = *bBaseRay;
    //  bool _490 = _488 && _489;
    //  
    //  if(_490) 
    //  {
    //      bool _498 = *bRequireCopy;
    //      bool _499 = GroupNonUniformQuadSwap(Subgroup, _498, 0);
    //  }
    //  bool _494 = Phi(_490, _493);
    //  *bCopyHorizontal@3 = _494;

    // bool bCopyHorizontal = ((samplesPerQuad != 4) && bBaseRay) && subgroupQuadSwapHorizontal(bRequireCopy);
    // bool bCopyVertical   = ((samplesPerQuad == 1) && bBaseRay) &&   subgroupQuadSwapVertical(bRequireCopy);
    // bool bCopyDiagonal   = ((samplesPerQuad == 1) && bBaseRay) &&   subgroupQuadSwapDiagonal(bRequireCopy);

    bool bCopyH = subgroupQuadSwapHorizontal(bRequireCopy);
    bool bCopyV = subgroupQuadSwapVertical(bRequireCopy);
    bool bCopyD = subgroupQuadSwapDiagonal(bRequireCopy);

    bool bCopyHorizontal = bCopyH && ((samplesPerQuad != 4) && bBaseRay);
    bool bCopyVertical   = bCopyV && ((samplesPerQuad == 1) && bBaseRay);
    bool bCopyDiagonal   = bCopyD && ((samplesPerQuad== 1) && bBaseRay);

    
    // Compact ray and append to ray list.
    uvec4 ballotNeedRay = subgroupBallot(bNeedRay);
    uint waveRayCount = subgroupBallotBitCount(ballotNeedRay); 
    uint localRayIndexInWave = subgroupBallotExclusiveBitCount(ballotNeedRay);

    uint baseRayIndex;
    if(subgroupElect())
    {
        baseRayIndex = AddRayCount(waveRayCount);
    }
    baseRayIndex = subgroupBroadcastFirst(baseRayIndex);

    if(bNeedRay)
    {
        uint rayIndex = baseRayIndex + localRayIndexInWave;
        AddRay(rayIndex, dispatchThreadId, bCopyHorizontal, bCopyVertical, bCopyDiagonal);
    }
    vec4 intersectionOutput = vec4(0);
    imageStore(o_SSRIntersection, ivec2(dispatchThreadId), intersectionOutput);
    
    // Sync for sharedTileCount add done.
    groupMemoryBarrier();
    barrier();

    // Add denoise tile info.
    if ((groupThreadId.x == 0) && (groupThreadId.y == 0) && (sharedTileCount > 0)) 
    {
        uint tileOffset = AddDenoiseTileCount();
        AddDenoiserTile(tileOffset, dispatchThreadId);
    }
    */
}
layout (local_size_x = 8, local_size_y = 8) in;
void main()
{
    uvec2 groupThreadId = Remap8x8(gl_LocalInvocationIndex);
    uvec2 dispatchId = groupThreadId + gl_WorkGroupID.xy * 8;
    ivec2 workPos = ivec2(dispatchId);

    float perceptualRoughness = texelFetch(u_MetalnessRoughness, workPos, 0).g;
    ClassifyTiles(dispatchId, groupThreadId, perceptualRoughness);

    // Also store roughness.
    imageStore(o_SSRExtractRoughness, workPos, vec4(perceptualRoughness, 0.0f, 0.0f, 0.0f));
}