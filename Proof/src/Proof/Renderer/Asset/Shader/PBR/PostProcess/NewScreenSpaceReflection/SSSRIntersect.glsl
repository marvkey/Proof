//https://github.com/qiutang98/flower/blob/main/source/shader/sssr/sssr_intersect.glsl
#Compute Shader

#version 450
#extension GL_GOOGLE_include_directive : enable
#extension GL_KHR_shader_subgroup_basic : enable
#extension GL_KHR_shader_subgroup_ballot : enable
#include <PBR/PostProcess/NewScreenSpaceReflection/SSSRCommon.glslh>
layout (set = 0, binding = 1)  uniform texture2D u_GeometryTexture;
layout (set = 0, binding = 2, rgba16f) uniform image2D o_SSRIntersectionImage; // ssr intersect result.
layout (set = 0, binding = 3) uniform texture2D u_SSSRExtractRoughnessImage; // current frame ssr roughness.
layout (set = 0, binding = 4) uniform texture2D u_VelocityMap; 

#define M_PI                               3.14159265358979f

vec3 UnpackWorldNormal(vec3 pack)
{
	return normalize(pack * 2.0 - vec3(1.0));
}
vec2 GetHizMipResolution(int mipLevel) 
{
    // https://community.khronos.org/t/cost-of-texturesize/65968
    return vec2(textureSize(u_HZBMap, mipLevel));
}

float LoadDepth(ivec2 coord, int mip)
{
    return texelFetch(u_HZBMap, coord, mip).r; // use cloest depth.
}
// http://jcgt.org/published/0007/04/01/paper.pdf by Eric Heitz
// Input Ve: view direction
// Input alpha_x, alpha_y: roughness parameters
// Input U1, U2: uniform random numbers
// Output Ne: normal sampled with PDF D_Ve(Ne) = G1(Ve) * max(0, dot(Ve, Ne)) * D(Ne) / Ve.z
vec3 ImportanceSampleGGXVNDF(vec3 Ve, float alpha_x, float alpha_y, float U1, float U2) 
{
    // Section 3.2: transforming the view direction to the hemisphere configuration
    vec3 Vh = normalize(vec3(alpha_x * Ve.x, alpha_y * Ve.y, Ve.z));

    // Section 4.1: orthonormal basis (with special case if cross product is zero)
    float lensq = Vh.x * Vh.x + Vh.y * Vh.y;
    vec3 T1 = lensq > 0 ? vec3(-Vh.y, Vh.x, 0) * inversesqrt(lensq) : vec3(1, 0, 0);
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
vec3 GetReflectionDir(const vec3 viewDir, const vec3 viewNormal, float roughness, ivec2 sampleCoord, uvec2 screenSize)
{
    //uvec2 offset = uvec2(vec2(0.754877669, 0.569840296) * (frameData.frameIndex.x % frameData.jitterPeriod) * uvec2(screenSize));
    uvec2 offset = uvec2(vec2(0.754877669, 0.569840296) * (u_FrameData.FrameCount % u_Camera.JitterPeriod) * uvec2(screenSize));
    uvec2 offsetId = uvec2(sampleCoord) + offset;
    offsetId.x = offsetId.x % screenSize.x;
    offsetId.y = offsetId.y % screenSize.y;

    vec2 u;
    u.x = SampleRandomNumber(offsetId.x, offsetId.y, 0, 0u);
    u.y = SampleRandomNumber(offsetId.x, offsetId.y, 0, 1u);

    mat3 tbnTransform = CreateTBN(viewNormal);
    vec3 viewDirTbn = tbnTransform * (-viewDir);

    vec3 sampledNormalTbn = ImportanceSampleGGXVNDF(viewDirTbn, roughness, roughness, u.x, u.y);

    vec3 reflectedDirTbn = reflect(-viewDirTbn, sampledNormalTbn);
    return transpose(tbnTransform) * reflectedDirTbn;
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
    t.z = direction.z < 0.0 ? t.z : 3.402823466e+38; // reverse z.
    // t.z = direction.z > 0.0 ? t.z : 3.402823466e+38; // No reverse z.

    // Choose nearest intersection with a boundary.
    float tMin = min(min(t.x, t.y), t.z);

    bool bAboveSurface = surfaceZ < position.z; // reverse z.
    // bool bAboveSurface = surfaceZ > position.z; // No reverse z.

    // Decide whether we are able to advance the ray until we hit the xy boundaries or if we had to clamp it at the surface.
    // We use the asuint comparison to avoid NaN / Inf logic, also we actually care about bitwise equality here to see if t_min is the t.z we fed into the min3 above.
    bool bSkipTile = floatBitsToUint(tMin) != floatBitsToUint(t.z) && bAboveSurface;

    // Make sure to only advance the ray if we're still above the surface.
    currentT = bAboveSurface ? tMin : currentT;

    // Advance ray.
    position = origin + currentT * direction;

    return bSkipTile;
}

vec3 HizMarching(
    vec3 origin, 
    vec3 dir, 
    bool bMirror, 
    vec2 screenSize,
    int mostDetailedMip,
    uint minTraversalOccupancy,
    uint maxTraversalIntersections,
    out bool bValidHit)
{
    vec3 invDir;
    invDir.x = dir.x != 0.0 ? 1.0 / dir.x : 3.402823466e+38;
    invDir.y = dir.y != 0.0 ? 1.0 / dir.y : 3.402823466e+38;
    invDir.z = dir.z != 0.0 ? 1.0 / dir.z : 3.402823466e+38;

    int currentMip = mostDetailedMip;

    vec2 currentMipRes = GetHizMipResolution(currentMip);
    vec2 currentMipResInv = 1.0 / currentMipRes;

    // Slightly offset ensure ray step into pixel cell.
    vec2 uvOffset = 0.005f * exp2(mostDetailedMip) / screenSize * sign(dir.xy);

    // Offset applied depending on current mip resolution to move the boundary to the left/right upper/lower border depending on ray direction.
    vec2 floorOffset;
    floorOffset.x = dir.x < 0.0 ? 0.0 : 1.0;
    floorOffset.y = dir.y < 0.0 ? 0.0 : 1.0;

    float currentT;
    vec3 position;
    // Init advance ray avoid self hit.
    {
        vec2 currentMipPosition = currentMipRes * origin.xy;

        vec2 xyPlane = floor(currentMipPosition) + floorOffset;
        xyPlane = xyPlane * currentMipResInv + uvOffset;

        // o + d * t = p' => t = (p' - o) / d
        vec2 t = xyPlane * invDir.xy - origin.xy * invDir.xy;
        currentT = min(t.x, t.y);
        position = origin + currentT * dir;
    }

    bool bExitDueToLowOccupancy = false;
    int i = 0;
    while(i < maxTraversalIntersections && !bExitDueToLowOccupancy && currentMip >= mostDetailedMip)
    {
        vec2 currentMipPosition = currentMipRes * position.xy;

        float surfaceZ = LoadDepth(ivec2(currentMipPosition), currentMip);
        bExitDueToLowOccupancy = !bMirror && subgroupBallotBitCount(subgroupBallot(true)) <= minTraversalOccupancy;

        bool bSkipTile = AdvanceRay(
            origin, 
            dir, 
            invDir, 
            currentMipPosition, 
            currentMipResInv, 
            floorOffset, 
            uvOffset, 
            surfaceZ, 
            position, 
            currentT);

        currentMip += bSkipTile ? 1 : -1; 
        currentMipRes = GetHizMipResolution(currentMip);
        currentMipResInv = 1.0 / currentMipRes;

        ++i;
    }

    bValidHit = (i <= maxTraversalIntersections);

    return position;
}

float ValidateHit(
    vec3 hit, 
    vec2 uv, 
    vec3 worldSpaceRayDirection, 
    vec2 screenSize, 
    float depthBufferThickness) 
{
    // Reject the hit if we didnt advance the ray significantly to avoid immediate self reflection
    vec2 manhattanDist = abs(hit.xy - uv);
    vec2 manhattanDistEdge = 2.0f / screenSize;
    if((manhattanDist.x < manhattanDistEdge.x) && (manhattanDist.y < manhattanDistEdge.y)) 
    {
        return 0;
    }

    ivec2 texelCoords = ivec2(screenSize * hit.xy);

    // Don't lookup radiance from the background.
    float surfaceZ = texelFetch(u_DepthMap, texelCoords, 0).r;
    if(surfaceZ <= 0.0)
    {
        return 0;
    }

    // We check if we hit the surface from the back, these should be rejected.
    vec3 hitNormal = UnpackWorldNormal(texelFetch(u_NormalMap, texelCoords, 0).rgb);
    if (dot(hitNormal, worldSpaceRayDirection) > 0) 
    {
        return 0;
    }

    vec3 viewSpaceSurface = GetWorldPos(hit.xy, surfaceZ);
    vec3 viewSpaceHit = GetWorldPos(hit.xy,hit.z);
    float distance = length(viewSpaceSurface - viewSpaceHit);

    // Fade out hits near the screen borders
    vec2 fov = 0.05 * vec2(screenSize.y / screenSize.x, 1);
    vec2 border = smoothstep(vec2(0), fov, hit.xy) * (1 - smoothstep(vec2(1 - fov), vec2(1), hit.xy));
    float vignette = border.x * border.y;

    // We accept all hits that are within a reasonable minimum distance below the surface.
    // Add constant in linear space to avoid growing of the reflections toward the reflected objects.
    float confidence = 1 - smoothstep(0, depthBufferThickness, distance);
    confidence *= confidence;

    return vignette * confidence;
}
vec3 LoadInputColor(ivec3 coords)
{
    return texelFetch(u_GeometryTexture,coords.xy,coords.z).xyz;
}



layout (local_size_x = 64) in;
void main()
{
    uint rayIndex = gl_GlobalInvocationID.x;

    if(rayIndex >= s_RayCounter.RayCount)
    {
        return;
    }

    uint packedCoords = s_Raylist.Data[rayIndex];

    uvec2 rayCoord;
    bool bCopyHorizontal;
    bool bCopyVertical;
    bool bCopyDiagonal;

    UnpackRayCoords(packedCoords, rayCoord, bCopyHorizontal, bCopyVertical, bCopyDiagonal);
    const uvec2 screenSize = imageSize(o_SSRIntersectionImage);
    const vec2 screenSizeInv = 1.0 / vec2(screenSize);
    const vec2 uv = (rayCoord + 0.5) * screenSizeInv;

    const vec3 worldNormal = UnpackWorldNormal(texelFetch(u_NormalMap, ivec2(rayCoord), 0).xyz); 
    const float roughness = texelFetch(u_SSSRExtractRoughnessImage, ivec2(rayCoord), 0).r;


    const bool bMirrorPlane = IsMirrorReflection(roughness);
    const int mostDetailedMip = bMirrorPlane ? 0 : int(u_PushData.MostDetailedMip); 

    const float z = LoadDepth(ivec2(rayCoord), mostDetailedMip);
    const vec3 screenSpaceUVzStart = vec3(uv, z);

    const vec3 viewPos = GetViewPos(uv, z);
    //const vec3 viewPos = ScreenSpaceToViewSpace(vec3(uv, z));
    const vec3 viewDir = normalize(viewPos);
    const vec3 viewNormal = normalize((u_Camera.View * vec4(worldNormal, 0.0)).rgb);
    const vec3 viewReflectedDir = GetReflectionDir(viewDir, viewNormal, roughness, ivec2(rayCoord), screenSize);
    const vec3 viewEnd = viewPos + viewReflectedDir;
    const vec3 screenSpaceUVzEnd = ProjectPosition(viewEnd, u_Camera.Projection);

    // Now get the screen space step dir.
    const vec3 screenSpaceUVz = screenSpaceUVzEnd - screenSpaceUVzStart;

    const bool bGlossy = IsGlossyReflection(roughness);

    bool bValidHit = false;
    vec3 hit;
    if(bGlossy && roughness < 0.2f) // Skip out ray hit.
    {
        hit = HizMarching(
            screenSpaceUVzStart, 
            screenSpaceUVz, 
            bMirrorPlane, 
            vec2(screenSize),
            mostDetailedMip,
            kMinTraversalOccupancy,
            kMaxTraversalIterations,
            bValidHit
        );
    }
    else
    {
        // Same with src pos, so ray length will be zero.
        hit = vec3(uv, z);
    }
    
    vec3 worldOrigin = GetWorldPos(uv, z);
    vec3 worldHit = GetWorldPos(hit.xy, hit.z);
    vec3 worldRay = worldHit - worldOrigin;

    float confidence = bValidHit ? ValidateHit(hit, uv, worldRay, vec2(screenSize), kDepthBufferThickness) : 0;
   // confidence =1;
    float worldRayLength = max(0, length(worldRay));

    vec3 reflectionRadiance = vec3(0);

    if (confidence > 0) 
    {


        vec2 historyUv = hit.xy + texelFetch(u_VelocityMap, ivec2(screenSize * hit.xy), 0).rg;
        if(historyUv.x >= 0 && historyUv.y >= 0 && historyUv.x <= 1 && historyUv.y <= 1)
        {
            // Found an intersection with the depth buffer -> We can lookup the color from lit scene.
            reflectionRadiance = texelFetch(u_GeometryTexture, ivec2(screenSize * historyUv.xy), 0).rgb;
            // reflectionRadiance *= M_PI; // Scale ssr hit result to keep energy full.
        }
        else
        {
            confidence = 0.0;
        }

        //reflectionRadiance = LoadInputColor(ivec3(screenSize * hit.xy, 0));

    }

    vec3 worldSpaceReflectedDir = (u_Camera.InverseView * vec4(viewReflectedDir, 0.0)).xyz;
    //vec3 envFallback = GetIBLContribution(roughness, worldSpaceReflectedDir, normalize(u_Camera.Position.xyz - worldOrigin), worldOrigin);
    vec3 envFallback = GetIBLContribution(roughness, worldNormal, normalize(u_Camera.Position.xyz - worldOrigin));

    reflectionRadiance = mix(envFallback, reflectionRadiance, confidence);

    vec4 newSample = vec4(reflectionRadiance, worldRayLength);

    imageStore(o_SSRIntersectionImage, ivec2(rayCoord), newSample);

    uvec2 copyTarget = rayCoord ^ 0x1; // Flip last bit to find the mirrored coords along the x and y axis within a quad.
    if (bCopyHorizontal) 
    {
        uvec2 copyCoords = uvec2(copyTarget.x, rayCoord.y);
        imageStore(o_SSRIntersectionImage, ivec2(copyCoords), newSample);
    }
    if (bCopyVertical) 
    {
        uvec2 copyCoords = uvec2(rayCoord.x, copyTarget.y);
        imageStore(o_SSRIntersectionImage, ivec2(copyCoords), newSample);
    }
    if (bCopyDiagonal)
    {
        uvec2 copyCoords = copyTarget;
        imageStore(o_SSRIntersectionImage, ivec2(copyCoords), newSample);
    }
}