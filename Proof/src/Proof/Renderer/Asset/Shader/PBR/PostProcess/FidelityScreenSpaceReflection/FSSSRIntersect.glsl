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

float rsqrt(float x)
{
    return float(1.0) / sqrt(x);
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

vec3 SampleGGXVNDFEllipsoid(vec3 ve, float alphaX, float alphaY, float u1, float u2) 
{
    return SampleGGXVNDF(ve, alphaX, alphaY, u1, u2);
}

vec3 sampleGGXVNDFHemisphere(vec3 ve, float alpha, float u1, float u2) 
{
    return SampleGGXVNDFEllipsoid(ve, alpha, alpha, u1, u2);
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

    vec3 sampled_normal_tbn = sampleGGXVNDFHemisphere(view_direction_tbn, roughness, u.x, u.y);
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

void InitialAdvanceRay(vec3 origin, vec3 direction, vec3 inv_direction, vec2 current_mip_resolution, vec2 current_mip_resolution_inv, vec2 floor_offset, vec2 uv_offset, out vec3 position, out float current_t)
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

// NOTE: Hiz ray intersection is accurate, but need more step to get good result.
//       Maybe we just need some fast intersect like linear search with only 16 tap.
//       Eg, unreal engine 4's SSR use this tech, full screen SSR just cost 0.5ms in 2K.
bool AdvanceRay(
    vec3 origin, 
    vec3 direction, 
    vec3 invDirection, 
    vec2 currentMipPosition, 
    vec2 currentMipResolutionInv, 
    vec2 floorOffset, 
    vec2 uvOffset, 
    float surfaceZ, 
    inout vec3 position, 
    inout float currentT) 
{
    vec2 xyPlane = floor(currentMipPosition) + floorOffset;
    xyPlane = xyPlane * currentMipResolutionInv + uvOffset;
    vec3 boundaryPlanes = vec3(xyPlane, surfaceZ);

    // Intersect ray with the half box that is pointing away from the ray origin.
    // o + d * t = p' => t = (p' - o) / d
    vec3 t = boundaryPlanes * invDirection - origin * invDirection;

    // Prevent using z plane when shooting out of the depth buffer.
    #if FFX_SSSR_OPTION_INVERTED_DEPTH
        t.z = direction.z < 0 ? t.z : FFX_SSSR_FLOAT_MAX;
    #else
        t.z = direction.z > 0 ? t.z : FFX_SSSR_FLOAT_MAX;
    #endif

    // Choose nearest intersection with a boundary.
    float tMin = min(min(t.x, t.y), t.z);

    #if FFX_SSSR_OPTION_INVERTED_DEPTH
    // Larger z means closer to the camera.
        bool bAboveSurface = surfaceZ < position.z;
    #else
        // Smaller z means closer to the camera.
        bool bAboveSurface = surfaceZ > position.z;
    #endif

    // Decide whether we are able to advance the ray until we hit the xy boundaries or if we had to clamp it at the surface.
    // We use the asuint comparison to avoid NaN / Inf logic, also we actually care about bitwise equality here to see if t_min is the t.z we fed into the min3 above.
    bool bSkipTile = floatBitsToUint(tMin) != floatBitsToUint(t.z) && bAboveSurface;

    // Make sure to only advance the ray if we're still above the surface.
    currentT = bAboveSurface ? tMin : currentT;

    // Advance ray.
    position = origin + currentT * direction;

    return bSkipTile;
}

vec2 GetMipResolution(vec2 screen_dimensions, int mip_level) 
{
    return screen_dimensions * pow(0.5, mip_level);
}

vec3 HizMarching( vec3 origin, 
    vec3 direction, 
    bool bMirror, 
    vec2 screenSize,
    int mostDetailedMip,
    uint minTraversalOccupancy,
    uint maxTraversalIntersections,
    out bool bValidHit) 
{
    const vec3 invDirection = direction != vec3(0.0) ? vec3(1.0) / direction : vec3(FFX_SSSR_FLOAT_MAX, FFX_SSSR_FLOAT_MAX, FFX_SSSR_FLOAT_MAX);

    // Start on mip with highest detail.
    int currentMip = mostDetailedMip;

    // Hoisted variables for performance.
    vec2 currentMipResolution = GetMipResolution(screenSize, currentMip);
    vec2 currentMipResolutionInv = 1.0 / currentMipResolution;

    // Offset to intersect ray with center of next pixel.
    vec2 uvOffset = 0.005 * exp2(float(mostDetailedMip)) / screenSize;
    uvOffset.x *= direction.x < 0.0f ? -uvOffset.x : uvOffset.x;
    uvOffset.y *= direction.y < 0.0f ? -uvOffset.y : uvOffset.y;

    // Offset depending on current mip resolution to move boundary to left/right upper/lower border based on ray direction.
    vec2 floorOffset;
    floorOffset.x = direction.x < 0.0f ? 0.0f : 1.0f;
    floorOffset.y = direction.y < 0.0f ? 0.0f : 1.0f;
    // Initially advance ray to avoid immediate self-intersections.
    float currentT;
    vec3 position;

    InitialAdvanceRay(origin, direction, invDirection, currentMipResolution, currentMipResolutionInv, floorOffset, uvOffset, position, currentT);

    bool exitDueToLowOccupancy = false;
    int i = 0;
    while (i < int(maxTraversalIntersections) && currentMip >= mostDetailedMip && !exitDueToLowOccupancy) {
        vec2 currentMipPosition = currentMipResolution * position.xy;
        float surfaceZ = LoadDepth(ivec2(currentMipPosition), currentMip);
        exitDueToLowOccupancy = !bMirror && AWaveActiveCountBits(true) <= minTraversalOccupancy;
        bool skippedTile = AdvanceRay(origin, direction, invDirection, currentMipPosition, currentMipResolutionInv, floorOffset, uvOffset, surfaceZ, position, currentT);
        
        // Don't increase mip further than this because we did not generate it
        bool nextMipIsOutOfRange = skippedTile && (currentMip >= FFX_SSSR_DEPTH_HIERARCHY_MAX_MIP);
        if (!nextMipIsOutOfRange) 
        {
            currentMip += skippedTile ? 1 : -1;
            currentMipResolution *= skippedTile ? 0.5 : 2.0;
            currentMipResolutionInv *= skippedTile ? 2.0 : 0.5;
        }

        ++i;
    }

    bValidHit = (i <= int(maxTraversalIntersections));
    return position;
}

float ValidateHit(
    vec3 hit, 
    vec2 uv, 
    vec3 worldSpaceRayDirection, 
    vec2 screenSize, 
    float depthBufferThickness) 
{

        // Reject hits outside the view frustum
    if ((hit.x < 0.0) || (hit.y < 0.0) || (hit.x > 1.0) || (hit.y > 1.0)) {
        return 0.0;
    }

    // Reject the hit if we didn't advance the ray significantly to avoid immediate self-reflection
    vec2 manhattanDist = abs(hit.xy - uv);
    if ((manhattanDist.x < (2.0 / screenSize.x)) && (manhattanDist.y < (2.0 / screenSize.y))) {
        return 0.0;
    }
    // Don't lookup radiance from the background.
    ivec2 texelCoords = ivec2(screenSize * hit.xy);
    float surfaceZ = LoadDepth(texelCoords / 2, 1);

    #if FFX_SSSR_OPTION_INVERTED_DEPTH
    if (surfaceZ == 0.0) {
#else
    if (surfaceZ == 1.0) {
#endif
        //return 0;
    }


        // We check if we hit the surface from the back, these should be rejected.
    vec3 hitNormal = LoadWorldSpaceNormal(texelCoords);
    if (dot(hitNormal, worldSpaceRayDirection) > 0.0) {
        return 0.0;
    }

    vec3 viewSpaceSurface = ScreenSpaceToViewSpace(vec3(hit.xy, surfaceZ));
    vec3 viewSpaceHit = ScreenSpaceToViewSpace(hit);
    float distancee = length(viewSpaceSurface - viewSpaceHit);

    // Fade out hits near the screen borders
    vec2 fov = 0.05 * vec2(screenSize.y / screenSize.x, 1.0);
    vec2 border = smoothstep(vec2(0.0), fov, hit.xy) * (1.0 - smoothstep(vec2(1.0) - fov, vec2(1.0), hit.xy));
    float vignette = border.x * border.y;

    // We accept all hits that are within a reasonable minimum distance below the surface.
    // Add constant in linear space to avoid growing of the reflections toward the reflected objects.
    float confidence = 1.0 - smoothstep(0.0, depthBufferThickness, distancee);
    confidence *= confidence;

    return vignette * confidence ;
}
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
void Intersect(uint groupIndex, uint groupId) 
{
    uint rayIndex = groupId * 64u + groupIndex;
    if (!IsRayIndexValid(rayIndex)) {
        return;
    }

    uint packedCoords = GetRaylist(rayIndex);

    uvec2 coords;
    bool copyHorizontal;
    bool copyVertical;
    bool copyDiagonal;
    UnpackRayCoords(packedCoords, coords, copyHorizontal, copyVertical, copyDiagonal);

    const uvec2 screenSize = uvec2(u_ScreenData.FullResolution);
    const vec2 screenSizeInv = 1.0 / vec2(screenSize);

    vec2 uv = (vec2(coords) + 0.5) * screenSizeInv;

    vec3 worldSpaceNormal = LoadWorldSpaceNormal(ivec2(coords));
    float roughness = LoadExtractedRoughness(ivec3(coords, 0));
    bool isMirror = IsMirrorReflection(roughness);

    int mostDetailedMip = isMirror ? 0 : int(u_PushData.MostDetailedMip);
    vec2 mipResolution = GetMipResolution(screenSize, mostDetailedMip);
    float z = LoadDepth(ivec2(uv * mipResolution), mostDetailedMip);

    vec3 screenUvSpaceRayOrigin = vec3(uv, z);
    vec3 viewSpaceRay = ScreenSpaceToViewSpace(screenUvSpaceRayOrigin);
    vec3 viewSpaceRayDirection = normalize(viewSpaceRay);

    vec3 viewSpaceSurfaceNormal = (u_Camera.View * vec4(worldSpaceNormal, 0.0)).xyz;
    vec3 viewSpaceReflectedDirection = SampleReflectionVector(viewSpaceRayDirection, viewSpaceSurfaceNormal, roughness, ivec2(coords));
    vec3 screenSpaceRayDirection = ProjectDirection(viewSpaceRay, viewSpaceReflectedDirection, screenUvSpaceRayOrigin, u_Camera.Projection);


    //====SSSR====
    bool validHit = false;
    vec3 hit = HizMarching(screenUvSpaceRayOrigin, screenSpaceRayDirection, isMirror , screenSize, mostDetailedMip, kMinTraversalOccupancy, kMaxTraversalIterations, validHit);

    vec3 worldSpaceOrigin = ScreenSpaceToWorldSpace(screenUvSpaceRayOrigin);
    vec3 worldSpaceHit = ScreenSpaceToWorldSpace(hit);
    vec3 worldSpaceRay = worldSpaceHit - worldSpaceOrigin;
    //everyting above works

    float confidence = validHit ? ValidateHit(hit, uv, worldSpaceRay, screenSize, kDepthBufferThickness) : 0.0;
    float worldRayLength = max(0.0, length(worldSpaceRay));
    vec3 reflectionRadiance = vec3(0.0);
        reflectionRadiance = LoadInputColor(ivec3(screenSize * hit.xy, 0));

    if (confidence > 0.0) 
    {
        //reflectionRadiance = vec3(0,0,1);

        // Found an intersection with the depth buffer -> We can lookup the color from lit scene.
    }
    // Sample environment map.
    vec3 worldSpaceReflectedDirection = (u_Camera.InverseView * vec4(viewSpaceReflectedDirection, 0.0)).xyz;
    vec3 environmentLookup = SampleEnvironmentMap(worldSpaceReflectedDirection, 0.0);
    //reflectionRadiance = mix(environmentLookup, reflectionRadiance, confidence);

    vec4 newSample = vec4(reflectionRadiance,1.0);

    StoreRadiance(coords, newSample);

    uvec2 copyTarget = coords ^ uvec2(1); // Flip last bit to find the mirrored coords along the x and y axis within a quad.
    if (copyHorizontal) {
        uvec2 copyCoords = uvec2(copyTarget.x, coords.y);
        StoreRadiance(copyCoords, newSample);
    }
    if (copyVertical) {
        uvec2 copyCoords = uvec2(coords.x, copyTarget.y);
        StoreRadiance(copyCoords, newSample);
    }
    if (copyDiagonal) {
        uvec2 copyCoords = copyTarget;
        StoreRadiance(copyCoords, newSample);
    }

}

layout (local_size_x = 8, local_size_y = 8, local_size_z = 1) in;

void main()
{
    Intersect(gl_LocalInvocationIndex, gl_WorkGroupID.x);
}