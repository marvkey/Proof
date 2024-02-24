#Compute Shader

#version 450
#extension GL_GOOGLE_include_directive : enable
#extension GL_KHR_shader_subgroup_basic : enable
#extension GL_KHR_shader_subgroup_ballot : enable
#include <PBR/PostProcess/FidelityScreenSpaceReflection/FSSSRCommon.glslh>
layout (set = 0, binding = 0) uniform texture2D u_SSSRRadianceMap;
layout (set = 0, binding = 1)   uniform texture2D u_SSSRAverageRadiance;
layout (set = 0, binding = 2)  uniform texture2D u_SSSRReprojectedRadiance;
layout (set = 0, binding = 3) uniform texture2D u_SSSRVariance;
layout (set = 0, binding = 4) uniform texture2D u_SSSRExtractRoughness;
layout (set = 0, binding = 6) uniform texture2D u_SSSRNumSamples;
layout (set = 0, binding = 7, rgba16f) uniform image2D o_SSSRRadianceMap;
layout (set = 0, binding = 8, r16f)    uniform image2D o_SSSRVariance;
#define RADIANCE_THRESHOLD 0.0001

shared uint g_ffx_dnsr_shared_0[16][16];
shared uint g_ffx_dnsr_shared_1[16][16];
float LoadNumSamples(ivec2 pixel_coordinate)
{
    return float(texelFetch(u_SSSRNumSamples, pixel_coordinate, 0).x);
}
float LoadVariance(ivec3 coords)
{
    return texelFetch(u_SSSRVariance, coords.xy, coords.z).x;
}
float LoadVariance(ivec2 coords)
{
    return texelFetch(u_SSSRVariance, coords.xy, 0).x;
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

vec3 LoadRadiance(ivec2 coordinate)
{
     return vec3(texelFetch(u_SSSRRadianceMap, coordinate.xy, 0).xyz);
}

vec3 SampleAverageRadiance(vec2 uv)
{
    return vec3(textureLod(sampler2D(u_SSSRAverageRadiance, u_LinearSampler), uv, 0.0f).xyz);
}

vec3 LoadRadianceReprojected(ivec2 pixel_coordinate)
{
    return vec3(texelFetch(u_SSSRReprojectedRadiance, pixel_coordinate, 0).xyz);
}
void StoreRadiance(ivec2 coordinate, vec4 radiance)
{
    imageStore(o_SSSRRadianceMap, coordinate, radiance);
}
void StoreVariance(ivec2 coord, float value) 
{ 
    imageStore(o_SSSRVariance, coord, vec4(value, 0.0, 0.0, 0.0));
}
void StoreTemporalAccumulation(ivec2 pixel_coordinate, vec3 radiance, float variance)
{
    StoreRadiance(pixel_coordinate, radiance.xyzz);
    StoreVariance(pixel_coordinate, variance.x);
}
struct NeighborhoodSample 
{
    vec3 Radiance;
};

NeighborhoodSample LoadFromGroupSharedMemory(ivec2 idx) 
{
    uvec2 packed_radiance = uvec2(g_ffx_dnsr_shared_0[idx.y][idx.x], g_ffx_dnsr_shared_1[idx.y][idx.x]);
    vec3 unpacked_radiance = FFX_DNSR_Reflections_UnpackFloat16_4(packed_radiance).xyz;

    NeighborhoodSample neighborSample;
    neighborSample.Radiance = unpacked_radiance;
    return neighborSample;
}


struct Moments 
{
    vec3 Mean;
    vec3 Variance;
};

Moments EstimateLocalNeighborhoodInGroup(ivec2 group_thread_id) 
{
    Moments estimate;
    estimate.Mean                 = vec3(0.0f, 0.0f, 0.0f);
    estimate.Variance             = vec3(0.0f, 0.0f, 0.0f);
    float accumulated_weight = float(0.0f);
    for (int j = -FFX_DNSR_REFLECTIONS_LOCAL_NEIGHBORHOOD_RADIUS; j <= FFX_DNSR_REFLECTIONS_LOCAL_NEIGHBORHOOD_RADIUS; ++j) 
    {
        for (int i = -FFX_DNSR_REFLECTIONS_LOCAL_NEIGHBORHOOD_RADIUS; i <= FFX_DNSR_REFLECTIONS_LOCAL_NEIGHBORHOOD_RADIUS; ++i) 
        {
            ivec2 new_idx  = group_thread_id + ivec2(i, j);
            vec3 radiance = LoadFromGroupSharedMemory(new_idx).Radiance;
            float   weight   = LocalNeighborhoodKernelWeight(float(i)) * LocalNeighborhoodKernelWeight(float(j));
            accumulated_weight  += weight;
            estimate.Mean       += radiance * weight;
            estimate.Variance   += radiance * radiance * weight;
        }
    }
    estimate.Mean     /= accumulated_weight;
    estimate.Variance /= accumulated_weight;

    estimate.Variance = abs(estimate.Variance - estimate.Mean * estimate.Mean);
    return estimate;
}

void StoreInGroupSharedMemory(ivec2 group_thread_id, vec3 radiance) 
{
    g_ffx_dnsr_shared_0[group_thread_id.y][group_thread_id.x] = FFX_DNSR_Reflections_PackFloat16(radiance.xy);
    g_ffx_dnsr_shared_1[group_thread_id.y][group_thread_id.x] = FFX_DNSR_Reflections_PackFloat16(radiance.zz);
}

void LoadNeighborhood(ivec2 pixel_coordinate, out vec3 radiance) 
{ 
    radiance = LoadRadiance(pixel_coordinate); 
}

void InitializeGroupSharedMemory(ivec2 dispatch_thread_id, ivec2 group_thread_id, ivec2 screen_size) {
    // Load 16x16 region into shared memory using 4 8x8 blocks.
    ivec2 offset[4] = {ivec2(0, 0), ivec2(8, 0), ivec2(0, 8), ivec2(8, 8)};

    // Intermediate storage registers to cache the result of all loads
    vec3 radiance[4];

    // Start in the upper left corner of the 16x16 region.
    dispatch_thread_id -= 4;

    // First store all loads in registers
    for (int i = 0; i < 4; ++i) 
    {
        LoadNeighborhood(dispatch_thread_id + offset[i], radiance[i]);
    }

    // Then move all registers to FFX_GROUPSHARED memory
    for (int j = 0; j < 4; ++j) 
    {
        StoreInGroupSharedMemory(group_thread_id + offset[j], radiance[j]);
    }
}

void FFX_DNSR_Reflections_ResolveTemporal(ivec2 dispatch_thread_id, ivec2 group_thread_id, uvec2 screen_size, uvec2 inv_screen_size, float history_clip_weight) 
{
    InitializeGroupSharedMemory(dispatch_thread_id, group_thread_id, ivec2(screen_size));
    FFX_GROUP_MEMORY_BARRIER();

    group_thread_id += 4; // Center threads in FFX_GROUPSHARED memory

    vec3 radiance = LoadFromGroupSharedMemory(group_thread_id).Radiance;
    float radianceSum = radiance.x + radiance.y + radiance.z;
    if (radianceSum < RADIANCE_THRESHOLD)
    {
        StoreTemporalAccumulation(dispatch_thread_id, vec3(0.0f, 0.0f, 0.0f), float(0.0f));
        return;
    }

    NeighborhoodSample center = LoadFromGroupSharedMemory(group_thread_id);
    vec3 new_signal   = center.Radiance;
    float roughness    = LoadRoughness(dispatch_thread_id);
    float new_variance = LoadVariance(dispatch_thread_id);
    
    if (IsGlossyReflection(roughness)) 
    {
        float num_samples  = LoadNumSamples(dispatch_thread_id);
        vec2 uv8          = (vec2(dispatch_thread_id.xy) + (0.5).xx) / FFX_DNSR_Reflections_RoundUp8(screen_size);
        vec3 avg_radiance = SampleAverageRadiance(uv8);

        vec3 old_signal = LoadRadianceReprojected(dispatch_thread_id);
        Moments local_neighborhood  = EstimateLocalNeighborhoodInGroup(group_thread_id);
        // Clip history based on the curren local statistics
        vec3 color_std          = vec3((sqrt(local_neighborhood.Variance.xyz) + length(local_neighborhood.Mean.xyz - avg_radiance)) * history_clip_weight * 1.4f);
                             local_neighborhood.Mean.xyz = mix(local_neighborhood.Mean.xyz, avg_radiance, vec3(0.2f, 0.2f, 0.2f));
        vec3 radiance_min       = local_neighborhood.Mean.xyz - color_std;
        vec3 radiance_max       = local_neighborhood.Mean.xyz + color_std;
        vec3 clipped_old_signal = FFX_DNSR_Reflections_ClipAABB(radiance_min, radiance_max, old_signal.xyz);
        float accumulation_speed = float(1.0f) / max(num_samples, float(1.0f));
        float weight             = (float(1.0f) - accumulation_speed);
        // Blend with average for small sample count
        new_signal.xyz = mix(new_signal.xyz, avg_radiance, float(1.0f) / max(num_samples + float(1.0f), float(1.0f)));
        // Clip outliers
        {
            vec3 radiance_min = avg_radiance.xyz - color_std * vec3(1.0f, 1.0f, 1.0f);
            vec3 radiance_max = avg_radiance.xyz + color_std * vec3(1.0f, 1.0f, 1.0f);
            new_signal.xyz = FFX_DNSR_Reflections_ClipAABB(radiance_min, radiance_max, new_signal.xyz);
        }
        // Blend with history
        new_signal   = mix(new_signal, clipped_old_signal, weight);
        new_variance = mix(FFX_DNSR_Reflections_ComputeTemporalVariance(new_signal.xyz, clipped_old_signal.xyz), new_variance, weight);
        if (any(isinf(new_signal)) || any(isnan(new_signal)) || isinf(new_variance) || isnan(new_variance)) {
            new_signal   = vec3(0.0f, 0.0f, 0.0f);
            new_variance = float(0.0f);
        }

    }
    StoreTemporalAccumulation(dispatch_thread_id, new_signal, new_variance);
}

void ResolveTemporal(uint group_index, uint group_id, uvec2 group_thread_id) 
{
    uint  packed_coords = GetDenoiserTile(group_id);
    ivec2  dispatch_thread_id = ivec2(packed_coords & 0xffffu, (packed_coords >> 16) & 0xffffu) + ivec2(group_thread_id);
    ivec2  dispatch_group_id = dispatch_thread_id / 8;
    ivec2 remapped_group_thread_id = ivec2(Remap8x8(group_index));
    ivec2 remapped_dispatch_thread_id = dispatch_group_id * 8 + remapped_group_thread_id;

    uvec2 invertedWorkSize = uint(1)/GetWorkSize();
    FFX_DNSR_Reflections_ResolveTemporal(remapped_dispatch_thread_id, remapped_group_thread_id, GetWorkSize(), invertedWorkSize, kTemporalStableReprojectFactor);
}

layout (local_size_x = 8, local_size_y = 8, local_size_z = 1) in;

void main()
{
    ResolveTemporal(gl_LocalInvocationIndex, gl_WorkGroupID.x, gl_LocalInvocationID.xy);
}