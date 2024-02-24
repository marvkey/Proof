#Compute Shader

#version 450
#extension GL_GOOGLE_include_directive : enable
#extension GL_KHR_shader_subgroup_basic : enable
#extension GL_KHR_shader_subgroup_ballot : enable
#include <PBR/PostProcess/FidelityScreenSpaceReflection/FSSSRCommon.glslh>
layout (set = 0, binding = 0) uniform texture2D u_PreviewDepthMap;
layout (set = 0, binding = 1) uniform texture2D u_PrevSSSRRadianceMap;
layout (set = 0, binding = 2) uniform texture2D u_PrevNormalMap;
layout (set = 0, binding = 3) uniform texture2D u_SSSRVarianceHistory;
layout (set = 0, binding = 4) uniform texture2D u_SSSRExtractRoughness;
layout (set = 0, binding = 5) uniform texture2D u_SSSRRadianceMap;
layout (set = 0, binding = 6) uniform texture2D u_SSSRVariance;
layout (set = 0, binding = 7) uniform texture2D u_SampleCount;
layout (set = 0, binding = 8, rgba16f) uniform image2D o_SSSRReprojectedRadiance; // ssr reproject output radiance.
layout (set = 0, binding = 9, r11f_g11f_b10f) uniform image2D o_SSSRAverageRadiance; 
layout (set = 0, binding = 10, r16f) uniform image2D o_SSRVariance;
layout (set = 0, binding = 11, r16f) uniform image2D o_SSRSampleCount;
layout (set = 0, binding = 12) uniform texture2D u_SSSRPrevExtractRoughness;
layout (set = 0, binding = 13) uniform texture2D u_VelocityMap; 


vec3 LoadRadiance(ivec2 coords)
{
    return vec3(texelFetch(u_SSSRRadianceMap, coords, 0).xyz);
}
vec2 LoadMotionVector(ivec2 dispatchThreadId)
{
    return texelFetch(u_VelocityMap, dispatchThreadId, 0).rg;
}

vec3 SampleWorldSpaceNormalHistory(vec2 uv)
{
    return normalize(vec3(kNormalUnpackMul * textureLod(sampler2D(u_PrevNormalMap, u_LinearSampler), uv, 0.0f).xyz + kNormalUnpackAdd));
}

vec3 LoadWorldSpaceNormalHistory(ivec2 coords) 
{
    return normalize(vec3(kNormalUnpackMul * texelFetch(u_PrevNormalMap, coords.xy, 0).xyz + kNormalUnpackAdd));
}

vec3 SampleRadianceHistory(vec2 uv)
{
    return vec3(textureLod(sampler2D(u_PrevSSSRRadianceMap, u_LinearSampler), uv, 0.0f).xyz);
}

vec3 LoadRadianceHistory(ivec2 coords) 
{ 
    return vec3(texelFetch(u_PrevSSSRRadianceMap, coords, 0).rgb); 
}

float SampleRoughnessHistory(vec2 uv)
{
    float rawRoughness = float(textureLod(sampler2D(u_SSSRPrevExtractRoughness, u_LinearSampler), uv, 0.0f).x);
    if (kIsRoughnessPerceptual)
    {
        rawRoughness *= rawRoughness;
    }

    return rawRoughness;
}

float LoadRoughness(ivec2 pixel_coordinate)
{

    float rawRoughness = float(textureLod(sampler2D(u_SSSRExtractRoughness, u_LinearSampler), pixel_coordinate, 0.0f).x);
    if (kIsRoughnessPerceptual)
    {
        rawRoughness *= rawRoughness;
    }

    return rawRoughness;
}
float SampleDepthHistory(vec2 uv)
{
    return textureLod(sampler2D(u_PreviewDepthMap, u_LinearSampler), uv, 0.0f).x;
}

float LoadRayLength(ivec2 pixel_coordinate)
{
    return float(texelFetch(u_SSSRRadianceMap, pixel_coordinate, 0).w);
}

float SampleVarianceHistory(vec2 uv)
{
    return float(textureLod(sampler2D(u_SSSRVariance, u_LinearSampler), uv, 0.0f).x);
}

float SampleNumSamplesHistory(vec2 uv)
{
    return float(textureLod(sampler2D(u_SampleCount, u_LinearSampler), uv, 0.0f).x);
}
float LoadDepthHistory(ivec2 pixel_coordinate)
{
    return texelFetch(u_PreviewDepthMap, pixel_coordinate, 0).x;
}
void StoreRadianceReprojected(ivec2 pixel_coordinate, vec3 value)
{
    imageStore(o_SSSRReprojectedRadiance, pixel_coordinate, vec4(value, 0.0f));
}

void StoreAverageRadiance(ivec2 pixel_coordinate, vec3 value)
{
    imageStore(o_SSSRAverageRadiance, pixel_coordinate, vec4(value, 0.0f));
}

void StoreVariance(ivec2 coord, float value) 
{ 
    imageStore(o_SSRVariance, coord, vec4(value, 0.0, 0.0, 0.0));
}

void StoreNumSamples(ivec2 coord, float value) 
{ 
    imageStore(o_SSRSampleCount, coord, vec4(value, 0.0, 0.0, 0.0));
}

shared uint G_ffx_dnsr_shared_0[16][16];
shared uint G_ffx_dnsr_shared_1[16][16];

struct NeighborhoodSample 
{
    vec3 Radiance;
};
NeighborhoodSample LoadFromGroupSharedMemory(ivec2 idx) 
{
    uvec2  packed_radiance          = uvec2(G_ffx_dnsr_shared_0[idx.y][idx.x], G_ffx_dnsr_shared_1[idx.y][idx.x]);
    vec4 unpacked_radiance        = FFX_DNSR_Reflections_UnpackFloat16_4(packed_radiance);

    NeighborhoodSample neighborSample;
    neighborSample.Radiance = unpacked_radiance.xyz;
    return neighborSample;
}

void StoreInGroupSharedMemory(ivec2 group_thread_id, vec3 radiance) 
{
    G_ffx_dnsr_shared_0[group_thread_id.y][group_thread_id.x]     = FFX_DNSR_Reflections_PackFloat16(radiance.xy);
    G_ffx_dnsr_shared_1[group_thread_id.y][group_thread_id.x]     = FFX_DNSR_Reflections_PackFloat16(radiance.zz);
}

void StoreInGroupSharedMemory(ivec2 group_thread_id, vec4 radiance_variance) 
{
    G_ffx_dnsr_shared_0[group_thread_id.y][group_thread_id.x]     = FFX_DNSR_Reflections_PackFloat16(radiance_variance.xy);
    G_ffx_dnsr_shared_1[group_thread_id.y][group_thread_id.x]     = FFX_DNSR_Reflections_PackFloat16(radiance_variance.zw);
}

void InitializeGroupSharedMemory(ivec2 dispatch_thread_id, ivec2 group_thread_id, ivec2 screen_size) 
{
    // Load 16x16 region into shared memory using 4 8x8 blocks.
    ivec2 offset[4] = {ivec2(0, 0), ivec2(8, 0), ivec2(0, 8), ivec2(8, 8)};

    // Intermediate storage registers to cache the result of all loads
    vec3 radiance[4];

    // Start in the upper left corner of the 16x16 region.
    dispatch_thread_id -= 4;

    // First store all loads in registers
    for (int i = 0; i < 4; ++i) {
        radiance[i] = LoadRadiance(dispatch_thread_id + offset[i]);
    }

    // Then move all registers to FFX_GROUPSHARED memory
    for (int j = 0; j < 4; ++j) {
        StoreInGroupSharedMemory(group_thread_id + offset[j], radiance[j]); // X
    }
}

vec4 LoadFromGroupSharedMemoryRaw(ivec2 idx) {
    uvec2 packed_radiance = uvec2(G_ffx_dnsr_shared_0[idx.y][idx.x], G_ffx_dnsr_shared_1[idx.y][idx.x]);
    return FFX_DNSR_Reflections_UnpackFloat16_4(packed_radiance);
}

float GetLuminanceWeight(vec3 val) {
    float luma   = FFX_DNSR_Reflections_Luminance(val.xyz);
    float weight = max(exp(-luma * FFX_DNSR_REFLECTIONS_AVG_RADIANCE_LUMINANCE_WEIGHT), 1.0e-2);
    return weight;
}

vec2 GetSurfaceReprojection(ivec2 dispatch_thread_id, vec2 uv, vec2 motion_vector) {
    // Reflector position reprojection
    vec2 history_uv = uv + motion_vector;
    return history_uv;
}

vec2 GetHitPositionReprojection(ivec2 dispatch_thread_id, vec2 uv, float reflected_ray_length) {
    float z = LoadDepth(dispatch_thread_id);
    vec3 view_space_ray = ScreenSpaceToViewSpace(vec3(uv, z));

    // We start out with reconstructing the ray length in view space.
    // This includes the portion from the camera to the reflecting surface as well as the portion from the surface to the hit position.
    float surface_depth = length(view_space_ray);
    float ray_length = surface_depth + reflected_ray_length;

    // We then perform a parallax correction by shooting a ray
    // of the same length "straight through" the reflecting surface
    // and reprojecting the tip of that ray to the previous frame.
    view_space_ray /= surface_depth; // == normalize(view_space_ray)
    view_space_ray *= ray_length;
    vec3 world_hit_position = ViewSpaceToWorldSpace(vec4(view_space_ray, 1.0)); // This is the "fake" hit position if we would follow the ray straight through the surface.
    vec3 prev_hit_position = WorldSpaceToScreenSpacePrevious(world_hit_position);
    vec2 history_uv = prev_hit_position.xy;
    return history_uv;
}

float GetDisocclusionFactor(vec3 normal, vec3 history_normal, float linear_depth, float history_linear_depth) {
    float factor = 1.0                                                            //
                   * exp(-abs(1.0 - max(0.0, dot(normal, history_normal))) * FFX_DNSR_REFLECTIONS_DISOCCLUSION_NORMAL_WEIGHT) //
                   * exp(-abs(history_linear_depth - linear_depth) / linear_depth * FFX_DNSR_REFLECTIONS_DISOCCLUSION_DEPTH_WEIGHT);
    return factor;
}

struct Moments 
{
    vec3 Mean;
    vec3 Variance;
};

Moments EstimateLocalNeighborhoodInGroup(ivec2 group_thread_id) 
{
    Moments estimate;
    estimate.Mean = vec3(0.0);
    estimate.Variance = vec3(0.0);
    float accumulated_weight = 0.0;
    for (int j = -kLocalNeighborhoodRadius; j <= kLocalNeighborhoodRadius; ++j) {
        for (int i = -kLocalNeighborhoodRadius; i <= kLocalNeighborhoodRadius; ++i) {
            ivec2 new_idx = group_thread_id + ivec2(i, j);
            vec4 radiance = LoadFromGroupSharedMemoryRaw(new_idx);
            vec3 radiance_xyz = radiance.xyz;
            float weight = LocalNeighborhoodKernelWeight(float(i)) * LocalNeighborhoodKernelWeight(float(j));
            accumulated_weight += weight;
            estimate.Mean += radiance_xyz * weight;
            estimate.Variance += radiance_xyz * radiance_xyz * weight;
        }
    }
    estimate.Mean /= accumulated_weight;
    estimate.Variance /= accumulated_weight;

    estimate.Variance = abs(estimate.Variance - estimate.Mean * estimate.Mean);
    return estimate;
}

float dot2(vec3 a) { return dot(a, a); }

void PickReprojection(ivec2 dispatch_thread_id,
                                            ivec2 group_thread_id,
                                            uvec2 screen_size,
                                            float roughness,
                                            float ray_length,
                                            out float disocclusion_factor,
                                            out vec2 reprojection_uv,
                                            out vec3 reprojection) {
    Moments  local_neighborhood = EstimateLocalNeighborhoodInGroup(group_thread_id);

    vec2 uv = (vec2(dispatch_thread_id) + vec2(0.5)) / vec2(screen_size);
    vec3 normal = LoadWorldSpaceNormal(dispatch_thread_id);
    vec3 history_normal;
    float history_linear_depth;

    {
        vec2 motion_vector = LoadMotionVector(dispatch_thread_id);
        vec2 surface_reprojection_uv = GetSurfaceReprojection(dispatch_thread_id, uv, motion_vector);
        vec2 hit_reprojection_uv = GetHitPositionReprojection(dispatch_thread_id, uv, ray_length);
        vec3 surface_normal = SampleWorldSpaceNormalHistory(surface_reprojection_uv);
        vec3 hit_normal = SampleWorldSpaceNormalHistory(hit_reprojection_uv);
        vec3 surface_history = SampleRadianceHistory(surface_reprojection_uv);
        vec3 hit_history = SampleRadianceHistory(hit_reprojection_uv);
        float hit_normal_similarity = dot(normalize(hit_normal), normalize(normal));
        float surface_normal_similarity = dot(normalize(surface_normal), normalize(normal));
        float hit_roughness = SampleRoughnessHistory(hit_reprojection_uv);
        float surface_roughness = SampleRoughnessHistory(surface_reprojection_uv);

        if (hit_normal_similarity > FFX_DNSR_REFLECTIONS_REPROJECTION_NORMAL_SIMILARITY_THRESHOLD &&
            hit_normal_similarity + 1.0e-3 > surface_normal_similarity &&
            abs(hit_roughness - roughness) < abs(surface_roughness - roughness) + 1.0e-3) {
            history_normal = hit_normal;
            float hit_history_depth = SampleDepthHistory(hit_reprojection_uv);
            float hit_history_linear_depth = GetLinearDepth(hit_reprojection_uv, hit_history_depth);
            history_linear_depth = hit_history_linear_depth;
            reprojection_uv = hit_reprojection_uv;
            reprojection = hit_history;
        } else {
            if (dot2(surface_history - local_neighborhood.Mean) <
                FFX_DNSR_REFLECTIONS_REPROJECT_SURFACE_DISCARD_VARIANCE_WEIGHT * length(local_neighborhood.Variance)) {
                history_normal = surface_normal;
                float surface_history_depth = SampleDepthHistory(surface_reprojection_uv);
                float surface_history_linear_depth = GetLinearDepth(surface_reprojection_uv, surface_history_depth);
                history_linear_depth = surface_history_linear_depth;
                reprojection_uv = surface_reprojection_uv;
                reprojection = surface_history;
            } else {
                disocclusion_factor = 0.0;
                return;
            }
        }
    }
    float depth = LoadDepth(dispatch_thread_id);
    float linear_depth = GetLinearDepth(uv, depth);
    disocclusion_factor = GetDisocclusionFactor(normal, history_normal, linear_depth, history_linear_depth);

    if (disocclusion_factor > FFX_DNSR_REFLECTIONS_DISOCCLUSION_THRESHOLD) // Early out, good enough
        return;

    if (disocclusion_factor < FFX_DNSR_REFLECTIONS_DISOCCLUSION_THRESHOLD) {
        vec2 closest_uv = reprojection_uv;
        vec2 dudv = 1.0 / vec2(screen_size);
        const int search_radius = 1;
        for (int y = -search_radius; y <= search_radius; y++) {
            for (int x = -search_radius; x <= search_radius; x++) {
                vec2 uv = reprojection_uv + vec2(x, y) * dudv;
                vec3 history_normal = SampleWorldSpaceNormalHistory(uv);
                float history_depth = SampleDepthHistory(uv);
                float history_linear_depth = GetLinearDepth(uv, history_depth);
                float weight = GetDisocclusionFactor(normal, history_normal, linear_depth, history_linear_depth);
                if (weight > disocclusion_factor) {
                    disocclusion_factor = weight;
                    closest_uv = uv;
                    reprojection_uv = closest_uv;
                }
            }
        }
        reprojection = SampleRadianceHistory(reprojection_uv);
    }

    if (disocclusion_factor < FFX_DNSR_REFLECTIONS_DISOCCLUSION_THRESHOLD) 
    {
        // If we've got a discarded history, try to construct a better sample out of 2x2 interpolation neighborhood
        // Helps quite a bit on the edges in movement
        float uvx = ffxFract(float(screen_size.x) * reprojection_uv.x + 0.5);
        float uvy = ffxFract(float(screen_size.y) * reprojection_uv.y + 0.5);
        ivec2 reproject_texel_coords = ivec2(screen_size * reprojection_uv - 0.5);
        vec3 reprojection00 = LoadRadianceHistory(reproject_texel_coords + ivec2(0, 0));
        vec3 reprojection10 = LoadRadianceHistory(reproject_texel_coords + ivec2(1, 0));
        vec3 reprojection01 = LoadRadianceHistory(reproject_texel_coords + ivec2(0, 1));
        vec3 reprojection11 = LoadRadianceHistory(reproject_texel_coords + ivec2(1, 1));

        vec3 normal00 = LoadWorldSpaceNormalHistory(reproject_texel_coords + ivec2(0, 0));
        vec3 normal10 = LoadWorldSpaceNormalHistory(reproject_texel_coords + ivec2(1, 0));
        vec3 normal01 = LoadWorldSpaceNormalHistory(reproject_texel_coords + ivec2(0, 1));
        vec3 normal11 = LoadWorldSpaceNormalHistory(reproject_texel_coords + ivec2(1, 1));

        float depth00 = GetLinearDepth(reprojection_uv, LoadDepthHistory(reproject_texel_coords + ivec2(0, 0)));
        float depth10 = GetLinearDepth(reprojection_uv, LoadDepthHistory(reproject_texel_coords + ivec2(1, 0)));
        float depth01 = GetLinearDepth(reprojection_uv, LoadDepthHistory(reproject_texel_coords + ivec2(0, 1)));
        float depth11 = GetLinearDepth(reprojection_uv, LoadDepthHistory(reproject_texel_coords + ivec2(1, 1)));
        vec4 w = vec4(1.0, 1.0, 1.0, 1.0);
        // Initialize with occlusion weights
        w.x = GetDisocclusionFactor(normal, normal00, linear_depth, depth00) > FFX_DNSR_REFLECTIONS_DISOCCLUSION_THRESHOLD / 2.0 ? 1.0 : 0.0;
        w.y = GetDisocclusionFactor(normal, normal10, linear_depth, depth10) > FFX_DNSR_REFLECTIONS_DISOCCLUSION_THRESHOLD / 2.0 ? 1.0 : 0.0;
        w.z = GetDisocclusionFactor(normal, normal01, linear_depth, depth01) > FFX_DNSR_REFLECTIONS_DISOCCLUSION_THRESHOLD / 2.0 ? 1.0 : 0.0;
        w.w = GetDisocclusionFactor(normal, normal11, linear_depth, depth11) > FFX_DNSR_REFLECTIONS_DISOCCLUSION_THRESHOLD / 2.0 ? 1.0 : 0.0;
        // And then mix in bilinear weights
        w.x = w.x * (1.0 - uvx) * (1.0 - uvy);
        w.y = w.y * (uvx) * (1.0 - uvy);
        w.z = w.z * (1.0 - uvx) * (uvy);
        w.w = w.w * (uvx) * (uvy);
        float ws = max(w.x + w.y + w.z + w.w, 1.0e-3);
        // normalize
        w /= ws;

        vec3 history_normal;
        float history_linear_depth;
        reprojection = reprojection00 * w.x + reprojection10 * w.y + reprojection01 * w.z + reprojection11 * w.w;
        history_linear_depth = depth00 * w.x + depth10 * w.y + depth01 * w.z + depth11 * w.w;
        history_normal = normal00 * w.x + normal10 * w.y + normal01 * w.z + normal11 * w.w;
        disocclusion_factor = GetDisocclusionFactor(normal, history_normal, linear_depth, history_linear_depth);
    }

    disocclusion_factor = disocclusion_factor < FFX_DNSR_REFLECTIONS_DISOCCLUSION_THRESHOLD ? 0.0 : disocclusion_factor;

}

void FFX_DNSR_Reflections_Reproject(in ivec2 dispatch_thread_id, in ivec2 group_thread_id, in uvec2 screen_size, in float temporal_stability_factor, in int max_samples) {
    InitializeGroupSharedMemory(dispatch_thread_id, group_thread_id, ivec2(screen_size));

    groupMemoryBarrier();
    barrier();

    group_thread_id += 4; // Center threads in FFX_GROUPSHARED memory

    float variance = 1.0;
    float num_samples = 0.0;
    float roughness = LoadRoughness(dispatch_thread_id);
    vec3 normal = LoadWorldSpaceNormal(dispatch_thread_id);
    vec3 radiance = LoadRadiance(dispatch_thread_id);
    const float ray_length = LoadRayLength(dispatch_thread_id);

    if (IsGlossyReflection(roughness)) 
    {
        float disocclusion_factor;
        vec2 reprojection_uv;
        vec3 reprojection;
        PickReprojection(dispatch_thread_id, group_thread_id, screen_size, roughness, ray_length, disocclusion_factor, reprojection_uv, reprojection);
        if (reprojection_uv.x > 0.0 && reprojection_uv.y > 0.0 && reprojection_uv.x < 1.0 && reprojection_uv.y < 1.0) 
        {
            float prev_variance = SampleVarianceHistory(reprojection_uv);
            num_samples = SampleNumSamplesHistory(reprojection_uv) * disocclusion_factor;
            float s_max_samples = max(8.0, max_samples * FFX_DNSR_REFLECTIONS_SAMPLES_FOR_ROUGHNESS(roughness));
            num_samples = min(s_max_samples, num_samples + 1.0);
            float new_variance = FFX_DNSR_Reflections_ComputeTemporalVariance(radiance.xyz, reprojection.xyz);
            if (disocclusion_factor < FFX_DNSR_REFLECTIONS_DISOCCLUSION_THRESHOLD) 
            {
                StoreRadianceReprojected(dispatch_thread_id, vec3(0.0, 0.0, 0.0));
                StoreVariance(dispatch_thread_id, 1.0);
                StoreNumSamples(dispatch_thread_id, 1.0);
            } 
            else 
            {
                float variance_mix = mix(new_variance, prev_variance, 1.0 / num_samples);
                StoreRadianceReprojected(dispatch_thread_id, reprojection);
                StoreVariance(dispatch_thread_id, variance_mix);
                StoreNumSamples(dispatch_thread_id, num_samples);
                // Mix in reprojection for radiance mip computation 
                radiance = mix(radiance, reprojection, 0.3);
            }
        } 
        else 
        {
            StoreRadianceReprojected(dispatch_thread_id, vec3(0.0, 0.0, 0.0));
            StoreVariance(dispatch_thread_id, 1.0);
            StoreNumSamples(dispatch_thread_id, 1.0);
        }
    }

    // Downsample 8x8 -> 1 radiance using FFX_GROUPSHARED memory
    // Initialize FFX_GROUPSHARED array for downsampling
    float weight = GetLuminanceWeight(radiance.xyz);
    radiance.xyz *= weight;
    if ( (dispatch_thread_id.x >= int(screen_size.x)) || (dispatch_thread_id.y >= int(screen_size.y)) || any(isinf(radiance)) || any(isnan(radiance)) || (weight > 1.0e3)) 
    {
        radiance = vec3(0.0, 0.0, 0.0);
        weight   = 0.0;
    }

    group_thread_id -= 4; // Center threads in FFX_GROUPSHARED memory

    StoreInGroupSharedMemory(group_thread_id, vec4(radiance.xyz, weight));
    FFX_GROUP_MEMORY_BARRIER();

    for (int i = 2; i <= 8; i *= 2) 
    {
        int ox = group_thread_id.x * i;
        int oy = group_thread_id.y * i;
        int ix = group_thread_id.x * i + i / 2;
        int iy = group_thread_id.y * i + i / 2;
        if (ix < 8 && iy < 8) 
        {
            vec4 rad_weight00 = LoadFromGroupSharedMemoryRaw(ivec2(ox, oy));
            vec4 rad_weight10 = LoadFromGroupSharedMemoryRaw(ivec2(ox, iy));
            vec4 rad_weight01 = LoadFromGroupSharedMemoryRaw(ivec2(ix, oy));
            vec4 rad_weight11 = LoadFromGroupSharedMemoryRaw(ivec2(ix, iy));
            vec4 sum = rad_weight00 + rad_weight01 + rad_weight10 + rad_weight11;
            StoreInGroupSharedMemory(ivec2(ox, oy), sum);
        }
        FFX_GROUP_MEMORY_BARRIER();
    }

    if ((group_thread_id.x == 0) && (group_thread_id.y == 0)) 
    {
        vec4 sum = LoadFromGroupSharedMemoryRaw(ivec2(0, 0));
        float weight_acc = max(sum.w, 1.0e-3);
        vec3 radiance_avg = sum.xyz / weight_acc;
        StoreAverageRadiance(dispatch_thread_id.xy / 8, radiance_avg);
    }

}


void Reproject(uint group_index, uint group_id, uvec2 group_thread_id) 
{
    uint packed_coords = GetDenoiserTile(group_id);
    ivec2 dispatch_thread_id = ivec2(packed_coords & 0xffffu, (packed_coords >> 16) & 0xffffu) + ivec2(group_thread_id);
    ivec2 dispatch_group_id = dispatch_thread_id / 8;
    ivec2 remapped_group_thread_id = ivec2(Remap8x8(group_index));
    ivec2 remapped_dispatch_thread_id = dispatch_group_id * 8 + remapped_group_thread_id;

    FFX_DNSR_Reflections_Reproject(remapped_dispatch_thread_id, remapped_group_thread_id, GetWorkSize(), kTemporalStableReprojectFactor, 32);
}

layout (local_size_x = 8, local_size_y = 8, local_size_z = 1) in;


void main()
{
    Reproject(gl_LocalInvocationIndex, gl_WorkGroupID.x, gl_LocalInvocationID.xy);
}
