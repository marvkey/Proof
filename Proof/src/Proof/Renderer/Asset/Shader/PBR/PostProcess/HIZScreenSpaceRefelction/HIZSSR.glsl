#Compute Shader

#version 450
#include <Common.glslh>
#extension GL_EXT_samplerless_texture_functions : enable

layout(set = 0, binding = 0) uniform texture2D u_NormalMap;
layout(set = 0, binding = 1) uniform sampler2D u_HZBMap;
layout(set = 0, binding = 2) uniform sampler2D u_WorldTexture;
layout(set = 0, binding = 3, rgba32f) writeonly uniform image2D o_SSROutput;
layout (set = 0, binding = 4)  uniform samplerCube u_CubeMapPrefilter;
layout (set = 0, binding = 5) uniform texture2D u_MetalnessRoughness;

#define MAX_ITERATION 2000
#define STEP_SIZE 2
#define MAX_THICKNESS 0.001
/*
// changeable
vec3 SampleEnvironmentMap(vec3 direction, float roughness)
{
    ivec2 cubeSize = textureSize(u_CubeMapPrefilter, 0);
    int maxMipLevel = int(log2(float(cubeSize.x > 0 ? cubeSize.y : 1)));
    float lod = clamp(roughness * float(maxMipLevel), 0.0, float(maxMipLevel));
    return textureLod(samplerCube(u_CubeMapPrefilter, u_LinearClampEdgeSampler), direction, lod).xyz * kIblFactor;
    return textureLod(samplerCube(u_CubeMapPrefilter, u_LinearClampEdgeSampler), direction, lod).xyz * kIblFactor;
}
*/
uint c_Iterations = 100;
// hi-z with min-z buffer
// Compute the position, the reflection direction, maxTraceDistance of the sample in texture space.
void ComputePosAndReflection(uvec2 tid,
                             vec3 vSampleNormalInVS,
                             sampler2D tex_depth,
                             out vec3 outSamplePosInTS,
                             out vec3 outReflDirInTS,
                             out float outMaxDistance)
{
    float sampleDepth = texture(tex_depth, (vec2(tid) + 0.5) / u_ScreenData.FullResolution).x;
    vec4 samplePosInCS = vec4(((vec2(tid) + 0.5) / u_ScreenData.FullResolution) * 2.0 - 1.0, sampleDepth, 1.0);
    samplePosInCS.y *= -1.0;

    vec4 samplePosInVS = u_Camera.InverseProjection * samplePosInCS;
    samplePosInVS /= samplePosInVS.w;

    vec3 vCamToSampleInVS = normalize(samplePosInVS.xyz);
    vec4 vReflectionInVS = vec4(reflect(vCamToSampleInVS, vSampleNormalInVS), 0.0);

    vec4 vReflectionEndPosInVS = samplePosInVS + vReflectionInVS * 1000.0;
    vReflectionEndPosInVS /= (vReflectionEndPosInVS.z < 0.0 ? vReflectionEndPosInVS.z : 1.0);
    vec4 vReflectionEndPosInCS = u_Camera.Projection * vec4(vReflectionEndPosInVS.xyz, 1.0);
    vReflectionEndPosInCS /= vReflectionEndPosInCS.w;
    vec3 vReflectionDir = normalize((vReflectionEndPosInCS - samplePosInCS).xyz);

    // Transform to texture space
    samplePosInCS.xy *= vec2(0.5, -0.5);
    samplePosInCS.xy += vec2(0.5, 0.5);
    
    vReflectionDir.xy *= vec2(0.5, -0.5);
    
    outSamplePosInTS = samplePosInCS.xyz;
    outReflDirInTS = vReflectionDir;
    
    // Compute the maximum distance to trace before the ray goes outside of the visible area.
    outMaxDistance = outReflDirInTS.x >= 0.0 ? (1.0 - outSamplePosInTS.x) / outReflDirInTS.x : -outSamplePosInTS.x / outReflDirInTS.x;
    outMaxDistance = min(outMaxDistance, outReflDirInTS.y < 0.0 ? (-outSamplePosInTS.y / outReflDirInTS.y) : ((1.0 - outSamplePosInTS.y) / outReflDirInTS.y));
    outMaxDistance = min(outMaxDistance, outReflDirInTS.z < 0.0 ? (-outSamplePosInTS.z / outReflDirInTS.z) : ((1.0 - outSamplePosInTS.z) / outReflDirInTS.z));
}
vec3 IntersectDepthPlane(vec3 o, vec3 d, float z)
{
	return o + d * z;
}

vec2 GetCell(vec2 pos, vec2 cell_count)
{
	return vec2(floor(pos*cell_count));
}

vec3 IntersectCellBoundary(vec3 o, vec3 d, vec2 cell, vec2 cell_count, vec2 crossStep, vec2 crossOffset)
{
	vec3 intersection = vec3(0);
	
	vec2 index = cell + crossStep;
	vec2 boundary = index / cell_count;
	boundary += crossOffset;
	
	vec2 delta = boundary - o.xy;
	delta /= d.xy;
	float t = min(delta.x, delta.y);
	
	intersection = IntersectDepthPlane(o, d, t);
	
	return intersection;
}

float GetMinimumDepthPlane(vec2 p, int mipLevel, sampler2D tex_hi_z) 
{
    return textureLod(tex_hi_z, p, float(mipLevel)).x;
}

vec2 GetMinMaxDepthPlane(vec2 p, int mipLevel, sampler2D tex_hi_z) 
{
    return textureLod(tex_hi_z, p, float(mipLevel)).xy;
}

vec2 GetCellCount(int mipLevel, sampler2D tex_hi_z) {
    return vec2(textureSize(tex_hi_z, mipLevel));
}

 
bool CrossedCellBoundary(vec2 oldCellIndex, vec2 newCellIndex)
{
	return (oldCellIndex.x != newCellIndex.x) || (oldCellIndex.y != newCellIndex.y);
}

float FindIntersection_HiZ(vec3 samplePosInTS,
                            vec3 vReflDirInTS,
                            float maxTraceDistance,
                            sampler2D tex_hi_z,
                            out vec3 intersection)
{
    const int maxLevel = textureQueryLevels(tex_hi_z) - 1;

    vec2 crossStep = vec2(vReflDirInTS.x >= 0.0 ? 1.0 : -1.0, vReflDirInTS.y >= 0.0 ? 1.0 : -1.0);
    vec2 crossOffset = crossStep / (u_ScreenData.FullResolution * 128.0);
    crossStep = clamp(crossStep, 0.0, 1.0);

    vec3 ray = samplePosInTS;
    float minZ = ray.z;
    float maxZ = ray.z + vReflDirInTS.z * maxTraceDistance;
    float deltaZ = maxZ - minZ;

    vec3 o = ray;
    vec3 d = vReflDirInTS * maxTraceDistance;

    int startLevel = 2;
    int stopLevel = 0;
    vec2 startCellCount = GetCellCount(startLevel, tex_hi_z);

    vec2 rayCell = GetCell(ray.xy, startCellCount);
    ray = IntersectCellBoundary(o, d, rayCell, startCellCount, crossStep, crossOffset * 64.0);

    int level = startLevel;
    uint iter = 0u;
    bool isBackwardRay = vReflDirInTS.z < 0.0;
    float rayDir = isBackwardRay ? -1.0 : 1.0;

    while (level >= stopLevel && ray.z * rayDir <= maxZ * rayDir && iter < c_Iterations)
    {
        vec2 cellCount = GetCellCount(level, tex_hi_z);
        vec2 oldCellIdx = GetCell(ray.xy, cellCount);
    
        float cell_minZ = GetMinimumDepthPlane((oldCellIdx + 0.5) / cellCount, level, tex_hi_z);
        vec3 tmpRay = ((cell_minZ > ray.z) && !isBackwardRay) ? IntersectDepthPlane(o, d, (cell_minZ - minZ) / deltaZ) : ray;
    
        vec2 newCellIdx = GetCell(tmpRay.xy, cellCount);
    
        float thickness = level == 0 ? (ray.z - cell_minZ) : 0.0;
        bool crossed = (isBackwardRay && (cell_minZ > ray.z)) || (thickness > MAX_THICKNESS) || CrossedCellBoundary(oldCellIdx, newCellIdx);
        ray = crossed ? IntersectCellBoundary(o, d, oldCellIdx, cellCount, crossStep, crossOffset) : tmpRay;
        level = crossed ? min(maxLevel, level + 1) : level - 1;
    
        ++iter;
    }

    bool intersected = (level < stopLevel);
    intersection = ray;

    float intensity = intersected ? 1.0 : 0.0;

    return intensity;

}

vec4 ComputeReflectedColor(float intensity,
                            vec3 intersection,
                            vec4 skyColor,
                            sampler2D tex_scene_color)
{
    vec4 ssr_color = texture(tex_scene_color, intersection.xy);

    return mix(skyColor, ssr_color, intensity);
}
layout (local_size_x = 8, local_size_y = 8, local_size_z = 1) in;

void main() 
{
    uvec2 tid = uvec2(gl_GlobalInvocationID.xy);
    
    vec4 NormalAndReflectionMask = texelFetch(u_NormalMap, ivec2(tid), 0);
    vec4 color = texelFetch(u_WorldTexture, ivec2(tid), 0);
    vec4 normalInWS = vec4(normalize(NormalAndReflectionMask.xyz), 0.0);
    vec3 normal = (u_Camera.View * normalInWS).xyz;
    float reflection_mask = NormalAndReflectionMask.w;
    float rawRoughness = texelFetch(u_MetalnessRoughness, ivec2(tid), int(0)).g;
    
    vec4 skyColor = vec4(textureLod(u_CubeMapPrefilter, vec3(vec2(tid),0),rawRoughness).rgb,1.0);
    
    vec4 reflectionColor = vec4(0.0);
    if (reflection_mask != 0.0) {
        reflectionColor = skyColor;
        vec3 samplePosInTS = vec3(0.0);
        vec3 vReflDirInTS = vec3(0.0);
        float maxTraceDistance = 0.0;
        // Compute the position, the reflection vector, maxTraceDistance of this sample in texture space.
        ComputePosAndReflection(tid, normal, u_HZBMap, samplePosInTS, vReflDirInTS, maxTraceDistance);
        
        // Find intersection in texture space by tracing the reflection ray
        vec3 intersection = vec3(0.0);
        // if (vReflDirInTS.z > 0.0) {
        float intensity = FindIntersection_HiZ(samplePosInTS, vReflDirInTS, maxTraceDistance, u_HZBMap, intersection);
        
        // Compute reflection color if intersected
        reflectionColor = ComputeReflectedColor(intensity, intersection, skyColor, u_WorldTexture);
        // }
    }
    
    // Add the reflection color to the color of the sample.
    vec4 finalColor = color + reflectionColor;
    
    imageStore(o_SSROutput, ivec2(tid), finalColor);
}