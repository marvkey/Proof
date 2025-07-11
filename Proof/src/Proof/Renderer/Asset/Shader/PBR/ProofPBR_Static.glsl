#Vertex Shader
#version 450
#include <PBR/PBRShaderBases/PBR.Vertex.glsl>

void Vertex(inout PBRVertexInput vertexinput)
{
    
}


#Fragment Shader
#version 450 core
#include <PBR/PBRShaderBases/PBR.Fragment.glsl>

// pbr map
layout(set = 0, binding = 5) uniform sampler2D u_AlbedoMap;
layout(set = 0, binding = 6) uniform sampler2D u_NormalMap;
layout(set = 0, binding = 7) uniform sampler2D u_MetallicMap;
layout(set = 0, binding = 8) uniform sampler2D u_RoughnessMap;


layout(push_constant) uniform Material
{
    vec3 Albedo;
    float Metalness;

    float Roughness;
    float Emission;
    bool EmissionOverrideColorToggle;
    bool NormalTexToggle;

    vec2 TextureTiling;
    vec2 TextureOffset;

    vec3 EmissionOverrideColor; // if EmissionOverrideColorToggle is equal to true then we will override the emission color
} u_MaterialUniform;

void Fragment(inout PBRData pbrData)
{
    vec2 texCoords = PBR_Input.TexCoords * u_MaterialUniform.TextureTiling + u_MaterialUniform.TextureOffset;

    pbrData.Albedo = texture(u_AlbedoMap, texCoords).rgb * u_MaterialUniform.Albedo;
    pbrData.Albedo = texture(u_AlbedoMap, texCoords).rgb * u_MaterialUniform.Albedo;
    pbrData.Metalness = texture(u_MetallicMap, texCoords).r * u_MaterialUniform.Metalness;
    pbrData.Roughness = texture(u_RoughnessMap, texCoords).r * max(u_MaterialUniform.Roughness,0.00);

    pbrData.Normal = normalize(PBR_Input.Normal);
    if (u_MaterialUniform.NormalTexToggle)
	{
		pbrData.Normal = normalize(texture(u_NormalMap, texCoords).rgb * 2.0f - 1.0f);
		pbrData.Normal = normalize(PBR_Input.WorldNormals * pbrData.Normal);
	}

    pbrData.TexCoords = texCoords;
    pbrData.Emission = u_MaterialUniform.Emission;
    
    if(u_MaterialUniform.EmissionOverrideColorToggle == false)
    {
        pbrData.EmissionColour = u_MaterialUniform.Albedo;//emision
    }
    else
    {
        pbrData.EmissionColour = u_MaterialUniform.EmissionOverrideColor ;//emision
    }
    pbrData.OutFinalColor = 1;

}


void PreEndFragment()
{
}

        
void LightLateUpdate(inout vec3 lightDir, inout vec3 diffuseBRDF, inout vec3 specularBRDF,DirectionalLight currentLight)
{

}
