#Compute Shader

#version 450
#extension GL_GOOGLE_include_directive : enable
#extension GL_KHR_shader_subgroup_basic : enable
#extension GL_KHR_shader_subgroup_ballot : enable
#include <PBR/PostProcess/FidelityScreenSpaceReflection/FSSSRCommon.glslh>
layout (set = 0, binding = 1) uniform texture2D u_SSSRRadianceMap;
layout (set = 0, binding = 2) uniform texture2D u_SSSRVariance;
layout (set = 0, binding = 3) uniform texture2D u_SSSRExtractRoughness;
layout (set = 0, binding = 4) uniform texture2D u_SSSRAverageRadiance;
layout (set = 0, binding = 5, rgba16f) uniform image2D o_SSSRRadianceMap;
layout (set = 0, binding = 6, r16f)    uniform image2D o_SSSRVariance;

vec3 LoadRadiance(ivec3 coordinate)
{
     return vec3(texelFetch(u_SSSRRadianceMap, coordinate.xy, coordinate.z).xyz);
}
float LoadVariance(ivec3 coords)
{
    return texelFetch(u_SSSRVariance, coords.xy, coords.z).x;
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

vec3 SampleAverageRadiance(vec2 uv)
{
    return vec3(textureLod(sampler2D(u_SSSRAverageRadiance, u_LinearSampler), uv, 0.0f).xyz);
}
void StoreRadiance(ivec2 coordinate, vec4 radiance)
{
    imageStore(o_SSSRRadianceMap, coordinate, radiance);
}
void StoreVariance(ivec2 coord, float value) 
{ 
    imageStore(o_SSSRVariance, coord, vec4(value, 0.0, 0.0, 0.0));
}
void StorePrefilteredReflections(ivec2 pixel_coordinate, vec3 radiance, float variance)
{
    StoreRadiance(pixel_coordinate, radiance.xyzz);
    StoreVariance(pixel_coordinate, variance.x);
}

shared uint  g_ffx_dnsr_shared_0[16][16];
shared uint  g_ffx_dnsr_shared_1[16][16];
shared uint  g_ffx_dnsr_shared_2[16][16];
shared uint  g_ffx_dnsr_shared_3[16][16];
shared float g_ffx_dnsr_shared_depth[16][16];

struct NeighborhoodSample 
{
    vec3  Radiance;
    float Variance;
    vec3  Normal;
    float Depth;
};

NeighborhoodSample  LoadFromGroupSharedMemory(ivec2 idx) 
{
    uvec2   packed_radiance          = uvec2(g_ffx_dnsr_shared_0[idx.y][idx.x], g_ffx_dnsr_shared_1[idx.y][idx.x]);
    vec4    unpacked_radiance        = FFX_DNSR_Reflections_UnpackFloat16_4(packed_radiance);
    uvec2   packed_normal_variance   = uvec2(g_ffx_dnsr_shared_2[idx.y][idx.x], g_ffx_dnsr_shared_3[idx.y][idx.x]);
    vec4    unpacked_normal_variance = FFX_DNSR_Reflections_UnpackFloat16_4(packed_normal_variance);

    NeighborhoodSample neighborSample;
    neighborSample.Radiance = unpacked_radiance.xyz;
    neighborSample.Normal   = unpacked_normal_variance.xyz;
    neighborSample.Variance = unpacked_normal_variance.w;
    neighborSample.Depth    = g_ffx_dnsr_shared_depth[idx.y][idx.x];
    return neighborSample;
}

void StoreInGroupSharedMemory(ivec2 group_thread_id, vec3 radiance, float variance, vec3 normal, float depth) 
{
    g_ffx_dnsr_shared_0[group_thread_id.y][group_thread_id.x]     = FFX_DNSR_Reflections_PackFloat16(radiance.xy);
    g_ffx_dnsr_shared_1[group_thread_id.y][group_thread_id.x]     = FFX_DNSR_Reflections_PackFloat16(radiance.zz);
    g_ffx_dnsr_shared_2[group_thread_id.y][group_thread_id.x]     = FFX_DNSR_Reflections_PackFloat16(normal.xy);
    g_ffx_dnsr_shared_3[group_thread_id.y][group_thread_id.x]     = FFX_DNSR_Reflections_PackFloat16(vec2(normal.z, variance));
    g_ffx_dnsr_shared_depth[group_thread_id.y][group_thread_id.x] = depth;
}

void LoadNeighborhood(
    ivec2 pixel_coordinate,
    out vec3 radiance,
    out float variance,
    out vec3 normal,
    out float depth,
    ivec2 screen_size) {

    radiance = LoadRadiance(ivec3(pixel_coordinate, 0));
    variance = LoadVariance(ivec3(pixel_coordinate, 0));

    normal = LoadWorldSpaceNormal(pixel_coordinate);

    vec2 uv = (pixel_coordinate.xy + (0.5f).xx) / vec2(screen_size.xy);
    depth = GetLinearDepth(uv, LoadDepth(pixel_coordinate, 0));
}

void InitializeGroupSharedMemory(ivec2 dispatch_thread_id, ivec2 group_thread_id, ivec2 screen_size) {
    // Load 16x16 region into shared memory using 4 8x8 blocks.
    ivec2 offset[4] = {ivec2(0, 0), ivec2(8, 0), ivec2(0, 8), ivec2(8, 8)};

    // Intermediate storage registers to cache the result of all loads
    vec3 radiance[4];
    float variance[4];
    vec3 normal[4];
    float depth[4];

    // Start in the upper left corner of the 16x16 region.
    dispatch_thread_id -= 4;

    // First store all loads in registers
    for (int i = 0; i < 4; ++i) {
        LoadNeighborhood(dispatch_thread_id + offset[i], radiance[i], variance[i], normal[i], depth[i], screen_size);
    }

    // Then move all registers to groupshared memory
    for (int j = 0; j < 4; ++j) {
        StoreInGroupSharedMemory(group_thread_id + offset[j], radiance[j], variance[j], normal[j], depth[j]); // X
    }
}

float GetEdgeStoppingNormalWeight(vec3 normal_p, vec3 normal_q) 
{
    return pow(max(dot(normal_p, normal_q), 0.0), FFX_DNSR_REFLECTIONS_PREFILTER_NORMAL_SIGMA);
}

float GetEdgeStoppingDepthWeight(float center_depth, float neighbor_depth) {
    return exp(-abs(center_depth - neighbor_depth) * center_depth * FFX_DNSR_REFLECTIONS_PREFILTER_DEPTH_SIGMA);
}

float GetRadianceWeight(vec3 center_radiance, vec3 neighbor_radiance, float variance) {
    return max(exp(-(FFX_DNSR_REFLECTIONS_RADIANCE_WEIGHT_BIAS + variance * FFX_DNSR_REFLECTIONS_RADIANCE_WEIGHT_VARIANCE_K)
                    * length(center_radiance - neighbor_radiance.xyz))
            , 1.0e-2);
}


void ReflectionsResolve(ivec2 group_thread_id, vec3 avg_radiance, NeighborhoodSample center,
                                   out vec3 resolved_radiance, out float resolved_variance) {
    // Initial weight is important to remove fireflies.
    // That removes quite a bit of energy but makes everything much more stable.
    float accumulated_weight = GetRadianceWeight(avg_radiance, center.Radiance.xyz, center.Variance);
    vec3 accumulated_radiance = center.Radiance.xyz * accumulated_weight;
    float accumulated_variance = center.Variance * accumulated_weight * accumulated_weight;
    // First 15 numbers of Halton(2,3) stretched to [-3,3]. Skipping the center, as we already have that in center_radiance and center_variance.
    const int sample_count = 15;
    const ivec2 sample_offsets[sample_count] = {ivec2(0, 1),  ivec2(-2, 1),  ivec2(2, -3), ivec2(-3, 0),  ivec2(1, 2), ivec2(-1, -2), ivec2(3, 0), ivec2(-3, 3),
                                                ivec2(0, -3), ivec2(-1, -1), ivec2(2, 1),  ivec2(-2, -2), ivec2(1, 0), ivec2(0, 2),   ivec2(3, -1)};
    float variance_weight = max(FFX_DNSR_REFLECTIONS_PREFILTER_VARIANCE_BIAS,
                                1.0 - exp(-(center.Variance * FFX_DNSR_REFLECTIONS_PREFILTER_VARIANCE_WEIGHT))
                                );
    for (int i = 0; i < sample_count; ++i) {
        ivec2 new_idx = group_thread_id + sample_offsets[i];
        NeighborhoodSample neighbor = LoadFromGroupSharedMemory(new_idx);

        float weight = 1.0;
        weight *= GetEdgeStoppingNormalWeight(center.Normal, neighbor.Normal);
        weight *= GetEdgeStoppingDepthWeight(center.Depth, neighbor.Depth);
        weight *= GetRadianceWeight(avg_radiance, neighbor.Radiance.xyz, center.Variance);
        weight *= variance_weight;

        // Accumulate all contributions.
        accumulated_weight += weight;
        accumulated_radiance += weight * neighbor.Radiance.xyz;
        accumulated_variance += weight * weight * neighbor.Variance;
    }

    accumulated_radiance /= accumulated_weight;
    accumulated_variance /= (accumulated_weight * accumulated_weight);
    resolved_radiance = accumulated_radiance;
    resolved_variance = accumulated_variance;
}

void FFX_DNSR_Reflections_Prefilter(ivec2 dispatch_thread_id, ivec2 group_thread_id, uvec2 screen_size) {
    float center_roughness = LoadRoughness(dispatch_thread_id);
    InitializeGroupSharedMemory(dispatch_thread_id, group_thread_id, ivec2(screen_size));
    FFX_GROUP_MEMORY_BARRIER();

    group_thread_id += 4; // Center threads in groupshared memory

    NeighborhoodSample center = LoadFromGroupSharedMemory(group_thread_id);

    vec3 resolved_radiance = center.Radiance.xyz;
    float resolved_variance = center.Variance;

    // Check if we have to denoise or if a simple copy is enough
    bool needs_denoiser = center.Variance > 0.0 && IsGlossyReflection(center_roughness) && !IsMirrorReflection(center_roughness);
    if (needs_denoiser) {
        vec2 uv8 = (vec2(dispatch_thread_id.xy) + vec2(0.5)) / FFX_DNSR_Reflections_RoundUp8(screen_size);
        vec3 avg_radiance = SampleAverageRadiance(uv8);
        ReflectionsResolve(group_thread_id, avg_radiance, center, resolved_radiance, resolved_variance);
    }

    StorePrefilteredReflections(dispatch_thread_id, resolved_radiance, resolved_variance);
}

void Prefilter(uint group_index, uint group_id, ivec2 group_thread_id) 
{
    uint packed_coords = GetDenoiserTile(group_id);
    ivec2 dispatch_thread_id = ivec2(packed_coords & 0xffffu, (packed_coords >> 16) & 0xffffu) + group_thread_id;
    ivec2 dispatch_group_id = dispatch_thread_id / 8;
    ivec2 remapped_group_thread_id = ivec2(Remap8x8(group_index));
    ivec2 remapped_dispatch_thread_id = ivec2(dispatch_group_id * 8 + remapped_group_thread_id);

    FFX_DNSR_Reflections_Prefilter(remapped_dispatch_thread_id, remapped_group_thread_id, GetWorkSize());
}

layout (local_size_x = 8, local_size_y = 8, local_size_z = 1) in;

void main()
{
    Prefilter(gl_LocalInvocationIndex, gl_WorkGroupID.x, ivec2(gl_LocalInvocationID.xy));
}