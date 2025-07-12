#Vertex Shader
#version 450
#include <PBR/PBRShaderBases/PBR.Vertex.glsl>

void Vertex(inout PBRVertexInput vertexinput)
{
    
}

#Fragment Shader
#version 450 core
#include <PBR/PBRShaderBases/PBR.Fragment.glsl>
//https://www.youtube.com/watch?v=XjH-UoyaTgs&list=PLFt_AvWsXl0eBW2EiBtl_sxmDtSgZBxB3&index=17

struct TerrainLayer
{
    float StartHeight;
    float BlendStrength;
    float ColorTint;
    float TextureScale;
    vec3 Colour;
};

layout(std140, set = 0, binding = 0) uniform TerrainInfos
{
	uint LayerCount;
	float MinHeight;
	float MaxHeight;
} u_TerrainInfo;

layout(std140, set = 0, binding = 1) buffer TerrainLayers
{
	TerrainLayer Layers[];
} s_TerrainLayers;

layout(set = 0, binding = 3) uniform sampler2D u_Textures[12];

float InverseLerp(float a, float b, float value) 
{
    return clamp((value - a) / (b - a), 0.0, 1.0);
}

vec3 triplaner(vec3 worldPos, float scale, vec3 blendAxes, int textureIndex)
{
    vec3 scaleWorldPos = worldPos / scale;

    vec3 xProjection = texture(u_Textures[textureIndex], scaleWorldPos.yz).rgb * blendAxes.x;
    vec3 yProjection = texture(u_Textures[textureIndex], scaleWorldPos.xz).rgb * blendAxes.x;
    vec3 zProjection = texture(u_Textures[textureIndex], scaleWorldPos.xy).rgb * blendAxes.z;

    return xProjection + yProjection + zProjection;
}
void Fragment(inout PBRData pbrData)
{
     pbrData.Albedo = vec3(1.0f);

     float heightPercent = InverseLerp(u_TerrainInfo.MinHeight,u_TerrainInfo.MaxHeight,PBR_Input.WorldPosition.y);

    vec3 blendAxes = abs(PBR_Input.VecWorldNormal);
    blendAxes /= blendAxes.x + blendAxes.y + blendAxes.z;

     for(int layerIndex = 0; layerIndex < u_TerrainInfo.LayerCount; layerIndex++)
     {
        TerrainLayer layer = s_TerrainLayers.Layers[layerIndex];
        float drawStrength = InverseLerp(-layer.BlendStrength/2 - Epsilon,layer.BlendStrength/2,heightPercent -layer.StartHeight); // make sure base lends is not 0

        vec3 baseColor = layer.Colour * layer.ColorTint;
        vec3 textureColor = triplaner(PBR_Input.WorldPosition, layer.TextureScale, blendAxes, layerIndex);
        pbrData.Albedo = pbrData.Albedo * (1.0 - drawStrength) + (baseColor + textureColor) * drawStrength;
     }
}


void PreEndFragment()
{
}
        
void LightLateUpdate(inout vec3 lightDir, inout vec3 diffuseBRDF, inout vec3 specularBRDF,DirectionalLight currentLight)
{

}
