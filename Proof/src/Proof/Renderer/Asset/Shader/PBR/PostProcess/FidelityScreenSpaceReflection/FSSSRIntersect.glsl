#Compute Shader

#version 450
#extension GL_KHR_shader_subgroup_basic : enable
#extension GL_KHR_shader_subgroup_ballot : enable
#extension GL_KHR_shader_subgroup_quad : enable
#extension GL_KHR_shader_subgroup_shuffle : enable

#include <PBR/PostProcess/FidelityScreenSpaceReflection/FSSSRCommon.glslh>
layout (set = 0, binding = 0, rgba16f) uniform image2D o_SSRIntersectionImage; // ssr intersect result.
layout (set = 0, binding = 1) uniform texture2D u_SSSRExtractRoughnessImage; // current frame ssr roughness.
layout (set = 0, binding = 2)  uniform texture2D u_DirectLightedWorld;
float LoadExtractedRoughness(ivec3 coordinate)
{
    return texelFetch(u_SSSRExtractRoughnessImage, coordinate.xy, coordinate.z).x;
}
vec3 LoadInputColor(ivec3 coordinate)
{
    return texelFetch(u_DirectLightedWorld, coordinate.xy, coordinate.z).xyz;
}

void StoreRadiance(uvec2 coordinate, vec4 radiance)
{
    imageStore(o_SSRIntersectionImage, ivec2(coordinate), radiance);
}
float rsqrt(float x)
{
    return float(1.0) / sqrt(x);
}
vec2 GetMipResolution(vec2 screen_dimensions, int mip_level) 
{
    return screen_dimensions * pow(0.5, mip_level);
}
#define M_PI                               3.14159265358979f
#define FFX_SSSR_FLOAT_MAX                 3.402823466e+38
#define FFX_SSSR_DEPTH_HIERARCHY_MAX_MIP   6
// http://jcgt.org/published/0007/04/01/paper.pdf by Eric Heitz
// Input Ve: view direction
// Input alpha_x, alpha_y: roughness parameters
// Input U1, U2: uniform random numbers
// Output Ne: normal sampled with PDF D_Ve(Ne) = G1(Ve) * max(0, dot(Ve, Ne)) * D(Ne) / Ve.z
vec3 SampleGGXVNDF(vec3 Ve, float alpha_x, float alpha_y, float U1, float U2) 
{
    // Section 3.2: transforming the view direction to the hemisphere configuration
    vec3 Vh = normalize(vec3(alpha_x * Ve.x, alpha_y * Ve.y, Ve.z));
    // Section 4.1: orthonormal basis (with special case if cross product is zero)
    float lensq = Vh.x * Vh.x + Vh.y * Vh.y;
    vec3 T1 = lensq > 0 ? vec3(-Vh.y, Vh.x, 0) * rsqrt(lensq) : vec3(1, 0, 0);
    vec3 T2 = cross(Vh, T1);
    // Section 4.2: parameterization of the projected area
    float r = sqrt(U1);
    float phi = 2.0 * M_PI * U2;
    float t1 = r * cos(phi);
    float t2 = r * sin(phi);
    float s = 0.5 * (1.0 + Vh.z);
    t2 = (1.0 - s) * sqrt(1.0 - t1 * t1) + s * t2;
    // Section 4.3: reprojection onto hemisphere
    vec3 Nh = t1 * T1 + t2 * T2 + sqrt(max(0.0, 1.0 - t1 * t1 - t2 * t2)) * Vh;
    // Section 3.4: transforming the normal back to the ellipsoid configuration
    vec3 Ne = normalize(vec3(alpha_x * Nh.x, alpha_y * Nh.y, max(0.0, Nh.z)));
    return Ne;
}

vec3 Sample_GGX_VNDF_Ellipsoid(vec3 Ve, float alpha_x, float alpha_y, float U1, float U2) 
{
    return SampleGGXVNDF(Ve, alpha_x, alpha_y, U1, U2);
}

vec3 Sample_GGX_VNDF_Hemisphere(vec3 Ve, float alpha, float U1, float U2) 
{
    return Sample_GGX_VNDF_Ellipsoid(Ve, alpha, alpha, U1, U2);
}

vec3 SampleReflectionVector(vec3 view_direction, vec3 normal, float roughness, ivec2 dispatch_thread_id) 
{
    vec3 U;
    vec3 N = normal;
    if (abs(N.z) > 0.0) {
        float k = sqrt(N.y * N.y + N.z * N.z);
        U.x = 0.0; U.y = -N.z / k; U.z = N.y / k;
    }
    else {
        float k = sqrt(N.x * N.x + N.y * N.y);
        U.x = N.y / k; U.y = -N.x / k; U.z = 0.0;
    }

    // TBN 3x3 matrix
    vec3 TBN_row0 = U;
    vec3 TBN_row1 = cross(N, U);
    vec3 TBN_row2 = N;

    // TBN * -view_direction
    vec3 view_direction_tbn = vec3(dot(TBN_row0, -view_direction), dot(TBN_row1, -view_direction), dot(TBN_row2, -view_direction));

    vec2 u = SampleRandomBlueNoiseVector2D(dispatch_thread_id);

    vec3 sampled_normal_tbn = Sample_GGX_VNDF_Hemisphere(view_direction_tbn, roughness, u.x, u.y);
    #ifdef PERFECT_REFLECTIONS
        sampled_normal_tbn = vec3(0, 0, 1); // Overwrite normal sample to produce perfect reflection.
    #endif

    vec3 reflected_direction_tbn = reflect(-view_direction_tbn, sampled_normal_tbn);

    // Transpose of TBN
    vec3 TBN_col0 = vec3(TBN_row0[0], TBN_row1[0], TBN_row2[0]);
    vec3 TBN_col1 = vec3(TBN_row0[1], TBN_row1[1], TBN_row2[1]);
    vec3 TBN_col2 = vec3(TBN_row0[2], TBN_row1[2], TBN_row2[2]);

    // transpose(TBN) * reflected_direction_tbn
    return vec3(dot(TBN_col0, reflected_direction_tbn), dot(TBN_col1, reflected_direction_tbn), dot(TBN_col2, reflected_direction_tbn));
}


void FFX_SSSR_InitialAdvanceRay(vec3 origin, vec3 direction, vec3 inv_direction, vec2 current_mip_resolution, vec2 current_mip_resolution_inv, vec2 floor_offset, vec2 uv_offset, out vec3 position, out float current_t) 
{
    vec2 current_mip_position = current_mip_resolution * origin.xy;

    // Intersect ray with the half box that is pointing away from the ray origin.
    vec2 xy_plane = floor(current_mip_position) + floor_offset;
    xy_plane = xy_plane * current_mip_resolution_inv + uv_offset;

    // o + d * t = p' => t = (p' - o) / d
    vec2 t = xy_plane * inv_direction.xy - origin.xy * inv_direction.xy;
    current_t = min(t.x, t.y);
    position = origin + current_t * direction;
}

bool FFX_SSSR_AdvanceRay(vec3 origin, vec3 direction, vec3 inv_direction, vec2 current_mip_position, vec2 current_mip_resolution_inv, vec2 floor_offset, vec2 uv_offset, float surface_z, inout vec3 position, inout float current_t) {
    // Create boundary planes
    vec2 xy_plane = floor(current_mip_position) + floor_offset;
    xy_plane = xy_plane * current_mip_resolution_inv + uv_offset;
    vec3 boundary_planes = vec3(xy_plane, surface_z);

    // Intersect ray with the half box that is pointing away from the ray origin.
    // o + d * t = p' => t = (p' - o) / d
    vec3 t = boundary_planes * inv_direction - origin * inv_direction;

    // Prevent using z plane when shooting out of the depth buffer.
#if FFX_SSSR_OPTION_INVERTED_DEPTH
    t.z = direction.z < 0 ? t.z : FFX_SSSR_FLOAT_MAX;
#else
    t.z = direction.z > 0 ? t.z : FFX_SSSR_FLOAT_MAX;
#endif

    // Choose nearest intersection with a boundary.
    float t_min = min(min(t.x, t.y), t.z);

#if FFX_SSSR_OPTION_INVERTED_DEPTH
    // Larger z means closer to the camera.
    bool above_surface = surface_z < position.z;
#else
    // Smaller z means closer to the camera.
    bool above_surface = surface_z > position.z;
#endif

    // Decide whether we are able to advance the ray until we hit the xy boundaries or if we had to clamp it at the surface.
    // We use the asuint comparison to avoid NaN / Inf logic, also we actually care about bitwise equality here to see if t_min is the t.z we fed into the min3 above.
    bool skipped_tile = floatBitsToUint(t_min) != floatBitsToUint(t.z) && above_surface;

    // Make sure to only advance the ray if we're still above the surface.
    current_t = above_surface ? t_min : current_t;

    // Advance ray
    position = origin + current_t * direction;

    return skipped_tile;
}

vec2 FFX_SSSR_GetMipResolution(vec2 screen_dimensions, int mip_level) 
{
    return screen_dimensions * pow(0.5, mip_level);
}

// Requires origin and direction of the ray to be in screen space [0, 1] x [0, 1]
vec3 FFX_SSSR_HierarchicalRaymarch(vec3 origin, vec3 direction, bool is_mirror, vec2 screen_size, int most_detailed_mip, uint min_traversal_occupancy, uint max_traversal_intersections, out bool valid_hit) 
{
    const vec3 inv_direction = direction != vec3(0.0f, 0.0f, 0.0f) ? vec3(1.0f, 1.0f, 1.0f) / direction : vec3(FFX_SSSR_FLOAT_MAX, FFX_SSSR_FLOAT_MAX, FFX_SSSR_FLOAT_MAX);

    // Start on mip with highest detail.
    int current_mip = most_detailed_mip;

    // Could recompute these every iteration, but it's faster to hoist them out and update them.
    vec2 current_mip_resolution = FFX_SSSR_GetMipResolution(screen_size, current_mip);
    vec2 current_mip_resolution_inv = 1 / current_mip_resolution;

    // Offset to the bounding boxes uv space to intersect the ray with the center of the next pixel.
    // This means we ever so slightly over shoot into the next region.
    vec2 uv_offset = 0.005 * exp2(most_detailed_mip) / screen_size;
    uv_offset.x = direction.x < 0.0f ? -uv_offset.x : uv_offset.x;
    uv_offset.y = direction.y < 0.0f ? -uv_offset.y : uv_offset.y;

    // Offset applied depending on current mip resolution to move the boundary to the left/right upper/lower border depending on ray direction.
    vec2 floor_offset;
    floor_offset.x = direction.x < 0.0f ? 0.0f : 1.0f;
    floor_offset.y = direction.y < 0.0f ? 0.0f : 1.0f;

    // Initially advance ray to avoid immediate self intersections.
    float current_t;
    vec3 position;
    FFX_SSSR_InitialAdvanceRay(origin, direction, inv_direction, current_mip_resolution, current_mip_resolution_inv, floor_offset, uv_offset, position, current_t);

    bool exit_due_to_low_occupancy = false;
    int i = 0;
    while (i < max_traversal_intersections && current_mip >= most_detailed_mip && !exit_due_to_low_occupancy) {
        vec2 current_mip_position = current_mip_resolution * position.xy;
        float surface_z = LoadDepth(ivec2(current_mip_position), current_mip);
        exit_due_to_low_occupancy = !is_mirror && AWaveActiveCountBits(true) <= min_traversal_occupancy;
        bool skipped_tile = FFX_SSSR_AdvanceRay(origin, direction, inv_direction, current_mip_position, current_mip_resolution_inv, floor_offset, uv_offset, surface_z, position, current_t);
        
        // Don't increase mip further than this because we did not generate it
        bool nextMipIsOutOfRange = skipped_tile && (current_mip >= FFX_SSSR_DEPTH_HIERARCHY_MAX_MIP);
        if (!nextMipIsOutOfRange)
        {
            current_mip += skipped_tile ? 1 : -1;
            current_mip_resolution *= skipped_tile ? 0.5 : 2;
            current_mip_resolution_inv *= skipped_tile ? 2 : 0.5;;
        }

        ++i;
    }

    valid_hit = (i <= max_traversal_intersections);

    return position;
}

float FFX_SSSR_ValidateHit(vec3 hit, vec2 uv, vec3 world_space_ray_direction, vec2 screen_size, float depth_buffer_thickness) 
{
    // Reject hits outside the view frustum
    if ((hit.x < 0.0f) || (hit.y < 0.0f) || (hit.x > 1.0f) || (hit.y > 1.0f)) {
        return 0.0f;
    }

    // Reject the hit if we didnt advance the ray significantly to avoid immediate self reflection
     vec2 manhattan_dist= abs(hit.xy - uv);
    if((manhattan_dist.x < (2.0f / screen_size.x)) && (manhattan_dist.y < (2.0f / screen_size.y)) ) {
        return 0.0;
    }

    // Don't lookup radiance from the background.
    ivec2 texel_coords = ivec2(screen_size * hit.xy);
    float surface_z = LoadDepth(texel_coords / 2, 1);
#if FFX_SSSR_OPTION_INVERTED_DEPTH
    if (surface_z == 0.0) {
#else
    if (surface_z == 1.0) {
#endif
        return 0;
    }

    // We check if we hit the surface from the back, these should be rejected.
    vec3 hit_normal = LoadWorldSpaceNormal(texel_coords);
    if (dot(hit_normal, world_space_ray_direction) > 0) {
        return 0;
    }

    vec3 view_space_surface = ScreenSpaceToViewSpace(vec3(hit.xy, surface_z));
    vec3 view_space_hit = ScreenSpaceToViewSpace(hit);
    float distance = length(view_space_surface - view_space_hit);

    // Fade out hits near the screen borders
    vec2 fov = 0.05 * vec2(screen_size.y / screen_size.x, 1);
    vec2 border = smoothstep(vec2(0.0f, 0.0f), fov, hit.xy) * (1 - smoothstep(vec2(1.0f, 1.0f) - fov, vec2(1.0f, 1.0f), hit.xy));
    float vignette = border.x * border.y;

    // We accept all hits that are within a reasonable minimum distance below the surface.
    // Add constant in linear space to avoid growing of the reflections toward the reflected objects.
    float confidence = 1.0f - smoothstep(0.0f, depth_buffer_thickness, distance);
    confidence *= confidence;

    return vignette * confidence;
}

void Intersect(uint group_index, uint group_id)
{
    uint ray_index = group_id * 64 + group_index;
    if(!IsRayIndexValid(ray_index))
    {
        return;
    }

    uint packed_coords = GetRaylist(ray_index);

    uvec2 coords;
    bool copy_horizontal;
    bool copy_vertical;
    bool copy_diagonal;
    UnpackRayCoords(packed_coords, coords, copy_horizontal, copy_vertical, copy_diagonal);

    const uvec2 screen_size = uvec2(u_ScreenData.FullResolution);

    vec2 uv = (coords + 0.5) * u_ScreenData.InverseFullResolution;

    vec3 world_space_normal = LoadWorldSpaceNormal(ivec2(coords));
    float roughness = LoadExtractedRoughness(ivec3(coords, 0));
    bool is_mirror = IsMirrorReflection(roughness);

    int most_detailed_mip = is_mirror ? 0 : int(u_PushData.MostDetailedMip);
    vec2 mip_resolution = GetMipResolution(screen_size, most_detailed_mip);
    float z = LoadDepth(ivec2(uv * mip_resolution), most_detailed_mip);

    vec3 screen_uv_space_ray_origin = vec3(uv, z);
    vec3 view_space_ray = ScreenSpaceToViewSpace(screen_uv_space_ray_origin);
    vec3 view_space_ray_direction = normalize(view_space_ray);

    vec3 view_space_surface_normal =  ( u_Camera.View * vec4(world_space_normal, 0)).xyz;
    vec3 view_space_reflected_direction = SampleReflectionVector(view_space_ray_direction, view_space_surface_normal, roughness, ivec2(coords));
    vec3 screen_space_ray_direction = ProjectDirection(view_space_ray, view_space_reflected_direction, screen_uv_space_ray_origin, u_Camera.Projection);

    //====SSSR====
    bool valid_hit = false;
    vec3 hit = FFX_SSSR_HierarchicalRaymarch(screen_uv_space_ray_origin, screen_space_ray_direction, is_mirror, screen_size, most_detailed_mip, kMinTraversalOccupancy, kMaxTraversalIterations, valid_hit);

    vec3 world_space_origin   = ScreenSpaceToWorldSpace(screen_uv_space_ray_origin);
    vec3 world_space_hit      = ScreenSpaceToWorldSpace(hit);
    vec3 world_space_ray      = world_space_hit - world_space_origin.xyz;

    float confidence = valid_hit ? FFX_SSSR_ValidateHit(hit, uv, world_space_ray, screen_size, kDepthBufferThickness) : 0;
    float world_ray_length = max(0, length(world_space_ray));

    confidence = 1.0;
    vec3 reflection_radiance = vec3(0.0f, 0.0f, 0.0f);
    if (confidence > 0.0f) 
    {
        // Found an intersection with the depth buffer -> We can lookup the color from lit scene.
        reflection_radiance = LoadInputColor(ivec3(screen_size * hit.xy, 0))* 2;
        //reflection_radiance =vec3(0,1,0.0);
    }

    // Sample environment map.
    vec3 world_space_reflected_direction = (u_Camera.InverseView * vec4(view_space_reflected_direction, 0)).xyz;
    vec3 environment_lookup = SampleEnvironmentMap(world_space_reflected_direction, 0.0f);
    reflection_radiance = mix(environment_lookup, reflection_radiance, confidence);

    vec4 new_sample = vec4(reflection_radiance,world_ray_length);

    StoreRadiance(coords, new_sample);

    uvec2 copy_target = coords ^ 1; // Flip last bit to find the mirrored coords along the x and y axis within a quad.
    if (copy_horizontal) 
    {
        uvec2 copy_coords = uvec2(copy_target.x, coords.y);
        StoreRadiance(copy_coords, new_sample);
    }
    if (copy_vertical) 
    {
        uvec2 copy_coords = uvec2(coords.x, copy_target.y);
        StoreRadiance(copy_coords, new_sample);
    }
    if (copy_diagonal) 
    {
        uvec2 copy_coords = copy_target;
        StoreRadiance(copy_coords, new_sample);
    }
}

layout (local_size_x = 8, local_size_y = 8, local_size_z = 1) in;

void main()
{
    Intersect(gl_LocalInvocationIndex, gl_WorkGroupID.x);
}