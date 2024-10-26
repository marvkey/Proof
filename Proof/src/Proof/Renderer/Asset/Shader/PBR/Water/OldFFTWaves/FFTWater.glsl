#Vertex Shader
#version 450
#include <Common.glslh>
#include <PBR/PBRShaderBases/PBR.Vertex.glsl>
layout(set = 0, binding = 0) uniform sampler2DArray u_DiplacementTextures;
layout(set = 0, binding = 1) uniform sampler2DArray u_SlopeTextures;

layout(std140, set = 0, binding = 1) uniform FFTTileBuffer
{
    float Tile0;
    float Tile1;
    float Tile2;
    float Tile3;

    bool DebugTile0;
    bool DebugTIle1;
    bool DebugTIle2;
    bool DebugTIle3;

    int DebugLayer0;
    int DebugLayer1;
    int DebugLayer2;
    int DebugLayer3;

} u_TileData;

struct VertexOutput
{
   float Depth;
};
layout(location = 23) out VertexOutput Output;

void Vertex(inout PBRVertexInput vertexInput)
{
    vec2 uv = vec2(0.0,0.0);
	vec4 worldPos = aTransform * vec4(vertexInput.Vertex, 1.0);

    vec3 displacement1 = textureLod(u_DisplacementTextures, vec3(worldPos.xz * u_TileData.Tile0, 0.0), 0.0) * u_TileData.DebugLayer0 * u_ContributeDisplacement0;
    vec3 displacement2 = textureLod(u_DisplacementTextures, vec3(worldPos.xz * u_TileData.Tile1, 1.0), 0.0) * u_TileData.DebugLayer1 * u_ContributeDisplacement1;
    vec3 displacement3 = textureLod(u_DisplacementTextures, vec3(worldPos.xz * u_TileData.Tile2, 2.0), 0.0) * u_TileData.DebugLayer2 * u_ContributeDisplacement2;
    vec3 displacement4 = textureLod(u_DisplacementTextures, vec3(worldPos.xz * u_TileData.Tile3, 3.0), 0.0) * u_TileData.DebugLayer3 * u_ContributeDisplacement3;

    vec3 displacement = displacement1 + displacement2 + displacement3 + displacement4;

    vec4 clipPos = WorldPositionToClipPosition(vertexInput.Vertex);

    float depth = 1.0 - Linear01Depth(clipPos.z / clipPos.w);

    displacement = mix(vec3(0.0), displacement, pow(clamp(depth, 0.0, 1.0), u_DisplacementDepthAttenuation));

    vertexInput.Vertex.xyz += (GetWorldToObjectMatrix(aTransform) * vec4(displacement, 0.0)).xyz;
    vertexInput.TexCoords = worldPos.xz;

    Output.Depth = depth;

}
/*
#TessellationControl Shader
#version 450
#include <Common.glslh>
#include <PBR/PBRShaderBases/PBR.TessellationControl.glsl>
struct TessellationFactors 
{
    float Edge[3];
    float Inside;
};
float TessellationHeuristic(vec3 cp0, vec3 cp1) 
{
    float edgeLength = distance(cp0, cp1);
    vec3 edgeCenter = (cp0 + cp1) * 0.5;
    float viewDistance = distance(edgeCenter, u_WorldSpaceCameraPos);

    return edgeLength * u_ScreenParams.y / (u_TessellationEdgeLength * pow(viewDistance * 0.5, 1.2));
}

bool TriangleIsBelowClipPlane(vec3 p0, vec3 p1, vec3 p2, int planeIndex, float bias) 
{
    vec4 plane = u_CameraWorldClipPlanes[planeIndex];

    return dot(vec4(p0, 1.0), plane) < bias &&
           dot(vec4(p1, 1.0), plane) < bias &&
           dot(vec4(p2, 1.0), plane) < bias;
}

bool CullTriangle(vec3 p0, vec3 p1, vec3 p2, float bias) {
    return TriangleIsBelowClipPlane(p0, p1, p2, 0, bias) ||
           TriangleIsBelowClipPlane(p0, p1, p2, 1, bias) ||
           TriangleIsBelowClipPlane(p0, p1, p2, 2, bias) ||
           TriangleIsBelowClipPlane(p0, p1, p2, 3, bias);
}

void TessellationControl(PBRVertexOutput pbrInput[3])
{

}

#TessellationEvaluation Shader
#version 450
#include <Common.glslh>
#include <PBR/PBRShaderBases/PBR.TessellationEvaluation.glsl>


#define DP_INTERPOLATE(fieldName) data.fieldName = \
    data.fieldName = patch[0].fieldName * barycentricCoordinates.x + \
                        patch[1].fieldName * barycentricCoordinates.y + \
                        patch[2].fieldName * barycentricCoordinates.z;  

void TessellationEvaluation(PBRVertexOutput pbrInput[3])
{

}

*/

#Fragment Shader
#version 450 core

//#include <PBR/PBRShaderBases/PBR.Fragment.glsl>
#include <PBR/PBRShaderBases/PBR.FragmentTransperant.glsl>
layout(std140, set = 0, binding = 1) uniform FFTTileBuffer
{
    float Tile0;
    float Tile1;
    float Tile2;
    float Tile3;

    bool DebugTile0;
    bool DebugTIle1;
    bool DebugTIle2;
    bool DebugTIle3;

    int DebugLayer0;
    int DebugLayer1;
    int DebugLayer2;
    int DebugLayer3;

    float FoamSubtract0;
    float FoamSubtract1;
    float FoamSubtract2;
    float FoamSubtract3;

    float FoamDepthAttenuation;

} u_TileData;

layout(set = 0, binding = 0) uniform sampler2D u_DiplacementTextures;
float SchlickFresnel(vec3 normal, vec3 viewDir) 
{
    // 0.02f comes from the reflectivity bias of water kinda idk it's from a paper somewhere i'm not gonna link it tho lmaooo
	return 0.02f + (1 - 0.02f) * (pow(1 - DotClamped(normal, viewDir), 5.0f));
}

float SmithMaskingBeckmann(vec3 H, vec33 S, float roughness) 
{
    float hdots = max(0.001f, DotClamped(H, S));
    float a = hdots / (roughness * sqrt(1 - hdots * hdots));
    float a2 = a * a;

    return a < 1.6f ? (1.0f - 1.259f * a + 0.396f * a2) / (3.535f * a + 2.181 * a2) : 0.0f;
}

float Beckmann(float ndoth, float roughness) 
{
    float exp_arg = (ndoth * ndoth - 1) / (roughness * roughness * ndoth * ndoth);

    return exp(exp_arg) / (PI * roughness * roughness * ndoth * ndoth * ndoth * ndoth);
}
struct VertexOutput
{
   float Depth;
};
layout(location = 23) in VertexOutput Input;

layout(set = 0, binding = 0) uniform sampler2DArray u_DiplacementTextures;
layout(set = 0, binding = 1) uniform sampler2DArray u_SlopeTextures;
void Fragment(inout PBRData pbrData)
{
    vec4 displacementFoam1 = texture(u_DisplacementTextures, vec3(PBR_Input.TexCoords * u_TileData.Tile0, 0)) * u_TileData.DebugLayer0;
    displacementFoam1.a += u_TileData.u_FoamSubtract0;
    vec4 displacementFoam2 = texture(u_DisplacementTextures, vec3(PBR_Input.TexCoords * u_TileData.Tile1, 1)) * u_TileData.DebugLayer1;
    displacementFoam2.a += u_TileData.u_FoamSubtract1;
    vec4 displacementFoam3 = texture(u_DisplacementTextures, vec3(PBR_Input.TexCoords * u_TileData.Tile2, 2)) * u_TileData.DebugLayer2;
    displacementFoam3.a += u_TileData.u_FoamSubtract2;
    vec4 displacementFoam4 = texture(u_DisplacementTextures, vec3(PBR_Input.TexCoords * u_TileData.Tile3, 3)) * u_TileData.DebugLayer3;
    displacementFoam4.a += u_TileData.u_FoamSubtract3;

    vec4 displacementFoam = displacementFoam1 + displacementFoam2 + displacementFoam3 + displacementFoam4;

    vec2 slopes1 = texture(u_SlopeTextures, vec3(PBR_Input.TexCoords * u_TileData.Tile0, 0)) * u_TileData.u_DebugLayer0;
    vec2 slopes2 = texture(u_SlopeTextures, vec3(PBR_Input.TexCoords * u_TileData.Tile1, 1)) * u_TileData.u_DebugLayer1;
    vec2 slopes3 = texture(u_SlopeTextures, vec3(PBR_Input.TexCoords * u_TileData.Tile2, 2)) * u_TileData.u_DebugLayer2;
    vec2 slopes4 = texture(u_SlopeTextures, vec3(PBR_Input.TexCoords * u_TileData.Tile3, 3)) * u_TileData.u_DebugLayer3;
    vec2 slopes = slopes1 + slopes2 + slopes3 + slopes4;

    //slopes *= u_NormalStrength;
    float foam = mix(0.0f, clamp(displacementFoam.a, 0.0f, 1.0f), pow(depth, u_TileData.FoamDepthAttenuation));


    // Set up the normals
pbrData.Normal = normalize(vec3(-slopes.x, 1.0f, -slopes.y));
pbrData.Normal = normalize(mix(vec3(0, 1, 0), pbrData.Normal, pow(clamp(depth, 0.0f, 1.0f), u_NormalDepthAttenuation)));
pbrData.Normal = normalize(normalize(pbrData.Normal) * mat3(u_ObjectToWorldMatrix));

float NdotL = max(dot(pbrData.Normal, lightDir), 0.0f);

// Compute roughness and other parameters
pbrData.Roughness = u_Roughness + foam * u_FoamRoughnessModifier;
float ndoth = max(0.0001f, dot(pbrData.Normal, halfwayDir));

float viewMask = SmithMaskingBeckmann(halfwayDir, viewDir, pbrData.Roughness);
float lightMask = SmithMaskingBeckmann(halfwayDir, lightDir, pbrData.Roughness);
float G = 1.0f / (1.0f + viewMask + lightMask);

float eta = 1.33f;
float R = ((eta - 1.0f) * (eta - 1.0f)) / ((eta + 1.0f) * (eta + 1.0f));
float numerator = pow(1.0f - dot(pbrData.Normal, viewDir), 5.0f * exp(-2.69f * pbrData.Roughness));
float F = R + (1.0f - R) * numerator / (1.0f + 22.7f * pow(pbrData.Roughness, 1.5f));
F = clamp(F, 0.0f, 1.0f);

float3 specular = u_SunIrradiance * F * G * Beckmann(ndoth, pbrData.Roughness);
specular /= 4.0f * max(0.001f, NdotL);
specular *= NdotL;

float3 envReflection = texture(u_EnvironmentMap, reflect(-viewDir, pbrData.Normal)).rgb;
envReflection *= u_EnvironmentLightStrength;

float H = max(0.0f, displacementFoam.y) * u_HeightModifier;
vec3 scatterColor = u_ScatterColor;
vec3 bubbleColor = u_BubbleColor;
float bubbleDensity = u_BubbleDensity;

float k1 = u_WavePeakScatterStrength * H * pow(max(dot(lightDir, -viewDir), 0.0f), 4.0f) * pow(0.5f - 0.5f * dot(lightDir, pbrData.Normal), 3.0f);
float k2 = u_ScatterStrength * pow(max(dot(viewDir, pbrData.Normal), 0.0f), 2.0f);
float k3 = u_ScatterShadowStrength * NdotL;
float k4 = bubbleDensity;

float3 scatter = (k1 + k2) * scatterColor * u_SunIrradiance * (1.0f / (1.0f + lightMask));
scatter += k3 * scatterColor * u_SunIrradiance + k4 * bubbleColor * u_SunIrradiance;

float3 finalOutput = (1.0f - F) * scatter + specular + F * envReflection;
finalOutput = max(vec3(0.0f), finalOutput);
finalOutput = mix(finalOutput, u_FoamColor, clamp(foam, 0.0f, 1.0f));

// Populate the PBRData structure
pbrData.Albedo = finalOutput;
pbrData.Metalness = 0.0f;  // Assuming non-metallic for water
pbrData.Emission = 0.0f;
pbrData.EmissionColour = vec3(0.0f);
pbrData.Alpha = 1.0f;  // Assuming fully opaque unless otherwise specified
pbrData.UseOnlyAlbedo = false;
}

