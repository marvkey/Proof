#Compute Shader

#version 450
#extension GL_KHR_shader_subgroup_basic : enable
#extension GL_KHR_shader_subgroup_ballot : enable
#extension GL_KHR_shader_subgroup_quad : enable
#extension GL_KHR_shader_subgroup_shuffle : enable

#include <PBR/PostProcess/FidelityScreenSpaceReflection/FSSSRCommon.glslh>

layout (set = 0, binding = 0, r8) uniform image2D o_SSRExtractRoughness; // ssr roughness extract.
layout (set = 0, binding = 1, rgba16f) uniform image2D o_SSRRadiance;
layout (binding = 2) uniform sampler2D u_SSRVariance; // SSR variance help to filter.

void StoreRadiance(uvec2 coordinate, vec4 radiance)
{
    imageStore(o_SSRRadiance, ivec2(coordinate), radiance);
}
float LoadVarianceHistory(ivec3 coordinate) 
{
    return texelFetch(u_SSRVariance, coordinate.xy, coordinate.z).x;
}
bool IsBaseRay(uvec2 dispatch_thread_id, uint samples_per_quad) 
{
    switch (samples_per_quad) {
    case 1:
        return ((dispatch_thread_id.x & 1u) | (dispatch_thread_id.y & 1u)) == 0u; // Deactivates 3 out of 4 rays
    case 2:
        return (dispatch_thread_id.x & 1u) == (dispatch_thread_id.y & 1u); // Deactivates 2 out of 4 rays. Keeps diagonal.
    default: // case 4:
        return true;
    }
}

shared uint g_TileCount ;

void ClassifyTiles(uvec2 dispatch_thread_id, uvec2 group_thread_id, float roughness) 
{
    g_TileCount = 0;

    bool is_first_lane_of_wave = AWaveIsFirstLane();
    uint samplesPerQuad = u_PushData.SamplesPerQuad;
    // First we figure out on a per thread basis if we need to shoot a reflection ray.
    // Disable offscreen pixels
    bool needs_ray = !(dispatch_thread_id.x >= GetWorkSize().x || dispatch_thread_id.y >= GetWorkSize().y);

    // Dont shoot a ray on very rough surfaces.
    bool is_reflective_surface = IsReflectiveSurface(dispatch_thread_id, roughness);
    bool is_glossy_reflection = IsGlossyReflection(roughness);
    needs_ray = needs_ray && is_glossy_reflection && is_reflective_surface;

    // Also we dont need to run the denoiser on mirror reflections.
    bool needs_denoiser = needs_ray && !IsMirrorReflection(roughness);

    // Decide which ray to keep
    bool is_base_ray = IsBaseRay(dispatch_thread_id, samplesPerQuad);
    needs_ray = needs_ray && (!needs_denoiser || is_base_ray); // Make sure to not deactivate mirror reflection rays.

    bool temporalVariance = u_PushData.TemporalVarianceGuidedTracingEnabled > 0;
    if (temporalVariance && needs_denoiser && !needs_ray) {
        bool has_temporal_variance = LoadVarianceHistory(ivec3(dispatch_thread_id, 0)) > u_PushData.TemporalVarianceThreshold;
        needs_ray = needs_ray || has_temporal_variance;
    }

    FFX_GROUP_MEMORY_BARRIER(); // Wait until g_TileCount is cleared - allow some computations before and after

    // Now we know for each thread if it needs to shoot a ray and wether or not a denoiser pass has to run on this pixel.

    if (is_glossy_reflection && is_reflective_surface)
    {
        atomicAdd(g_TileCount, 1);
    }

    // Next we have to figure out for which pixels that ray is creating the values for. Thus, if we have to copy its value horizontal, vertical or across.
    bool require_copy = !needs_ray && needs_denoiser; // Our pixel only requires a copy if we want to run a denoiser on it but don't want to shoot a ray for it.
    bool copy_horizontal  = AWaveReadAtLaneIndexB1(require_copy, AWaveLaneIndex() ^ 1) && (samplesPerQuad != 4) && is_base_ray; // QuadReadAcrossX
    bool copy_vertical    = AWaveReadAtLaneIndexB1(require_copy, AWaveLaneIndex() ^ 2) && (samplesPerQuad == 1) && is_base_ray; // QuadReadAcrossY
    bool copy_diagonal    = AWaveReadAtLaneIndexB1(require_copy, AWaveLaneIndex() ^ 3) && (samplesPerQuad == 1) && is_base_ray; // QuadReadAcrossDiagonal

    // Thus, we need to compact the rays and append them all at once to the ray list.
    uint local_ray_index_in_wave = AWavePrefixCountBits(needs_ray);
    uint wave_ray_count = AWaveActiveCountBits(needs_ray);
    uint base_ray_index;
    if (is_first_lane_of_wave) {
        IncrementRayCounter(wave_ray_count, base_ray_index);
    }
    base_ray_index = AWaveReadLaneFirstU1(base_ray_index);
    if (needs_ray) {
        int ray_index = int(base_ray_index + local_ray_index_in_wave);
        StoreRay(ray_index, dispatch_thread_id, copy_horizontal, copy_vertical, copy_diagonal);
    }

    vec4 intersection_output = vec4(0.0f, 0.0f, 0.0f,0.0f);
    if (is_reflective_surface && !is_glossy_reflection)
    {
        // Fall back to environment map without preparing a ray
        vec2 uv = (dispatch_thread_id + 0.5) * u_ScreenData.InverseFullResolution;
        vec3 world_space_normal = LoadWorldSpaceNormal(ivec2(dispatch_thread_id));
        float  z = LoadDepth(ivec2(dispatch_thread_id), 0);
        vec3 screen_uv_space_ray_origin = vec3(uv, z);
        vec3 view_space_ray = ScreenSpaceToViewSpace(screen_uv_space_ray_origin);
        vec3 view_space_ray_direction = normalize(view_space_ray);
        vec3 view_space_surface_normal = vec4(u_Camera.View *  vec4( world_space_normal,0)).xyz;
        vec3 view_space_reflected_direction = reflect(view_space_ray_direction, view_space_surface_normal);
        vec3 world_space_reflected_direction = vec4(u_Camera.InverseView * vec4(view_space_reflected_direction, 0)).xyz;

        intersection_output.xyz = SampleEnvironmentMap(world_space_reflected_direction, sqrt(roughness));
    }

    StoreRadiance(dispatch_thread_id, intersection_output);

    FFX_GROUP_MEMORY_BARRIER(); // Wait until g_TileCount

    if ((group_thread_id.x == 0) && (group_thread_id.y == 0) && g_TileCount > 0)
    {
        uint tile_offset;
        IncrementDenoiserTileCounter(tile_offset);
        StoreDenoiserTile(int(tile_offset), ivec2(dispatch_thread_id.xy));
    }

}
void StoreExtractedRoughness(uvec2 coordinate, float roughness) 
{
    imageStore(o_SSRExtractRoughness, ivec2(coordinate), vec4(roughness, 0.0, 0.0, 0.0));
}
layout (local_size_x = 64, local_size_y = 1, local_size_z =1 ) in;

void main() {
    uvec2 group_thread_id = Remap8x8(gl_LocalInvocationIndex);
    uvec2 dispatch_thread_id = gl_WorkGroupID.xy * 8 + group_thread_id;
    float roughness = LoadRoughnessFromMaterialParametersInput(ivec3(dispatch_thread_id, 0));

    ClassifyTiles(dispatch_thread_id, group_thread_id, roughness);

    // Extract only the channel containing the roughness to avoid loading all 4 channels in the follow-up passes.
    StoreExtractedRoughness(dispatch_thread_id, roughness);
}
