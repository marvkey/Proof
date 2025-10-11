#Vertex Shader
#version 450
#include <PBR/PBRShaderBases/PBR.Vertex.glsl>


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

    float GlitchRate; //0.5
    float GlitchScale;//50

    vec3 Axis;

} u_MaterialUniform;

void Vertex(inout PBRVertexInput vertexinput)
{
   float time = u_FrameData.AppTimeSeconds;

    // base position
    vec3 pos = vertexinput.VertexPosition;

    // fake "digital noise" movement based on position & time
    float wave = sin(dot(pos.xyz, vec3(12.3, 7.9, 5.5)) + time * u_MaterialUniform.GlitchRate * 8.0);
    float wave2 = cos(dot(pos.xyz, vec3(8.1, 6.2, 9.3)) + time * u_MaterialUniform.GlitchRate * 5.5);

    // combine the noise for more chaotic motion
    float glitch = (wave + wave2) * 0.5 * (u_MaterialUniform.GlitchScale * 0.001);

    // displace along normal and optional axis
    pos += vertexinput.Normal * glitch;
    pos += u_MaterialUniform.Axis * glitch * 0.5;

    vertexinput.VertexPosition = pos;
}


#Fragment Shader
#version 450 core
#include <PBR/PBRShaderBases/PBR.Fragment.glsl>

layout(set = 0, binding = 5) uniform sampler2D u_AlbedoMap;
layout(set = 0, binding = 6) uniform sampler2D u_NormalMap;
layout(set = 0, binding = 7) uniform sampler2D u_MetallicMap;
layout(set = 0, binding = 8) uniform sampler2D u_RoughnessMap;

void Fragment(inout PBRData pbrData)
{
/*
   float time = u_FrameData.AppTimeSeconds;

    vec2 texCoords = PBR_Input.TexCoords;

    // === RGB GLITCH CHANNEL OFFSET ===
    float offset = sin(time * u_MaterialUniform.GlitchRate * 5.0 + texCoords.y * 20.0) * 0.005;

    vec3 albedoColor;
    albedoColor.r = texture(u_AlbedoMap, texCoords + vec2(offset, 0.0)).r;
    albedoColor.g = texture(u_AlbedoMap, texCoords).g;
    albedoColor.b = texture(u_AlbedoMap, texCoords - vec2(offset, 0.0)).b;

    // === FLICKER CUTOUT ===
    float flicker = fract(sin(dot(texCoords, vec2(12.9898, 78.233))) * 43758.5453 + time * u_MaterialUniform.GlitchRate * 20.0);
    if (flicker < 0.02)
        discard; // randomly hides pixels for digital breakup

    // === STANDARD PBR SETUP ===
    pbrData.Albedo = albedoColor * u_MaterialUniform.Albedo.rgb;
    pbrData.Metallic = texture(u_MetallicMap, texCoords).r * u_MaterialUniform.Metalness;
    pbrData.Roughness = texture(u_RoughnessMap, texCoords).r * u_MaterialUniform.Roughness;

    if (u_MaterialUniform.NormalTexToggle)
    {
        pbrData.Normal = GetNormalFromMap(texture(u_NormalMap, texCoords), PBR_Input.TBN);
    }
    else
    {
        pbrData.Normal = normalize(PBR_Input.WorldNormals);
    }

    // === EMISSION ===
    pbrData.Emission = albedoColor * u_MaterialUniform.Emission;

    pbrData.Alpha = texture(u_AlbedoMap, texCoords).a * u_MaterialUniform.Albedo.a;
    */
}

void PreEndFragment()
{
}

        
void LightLateUpdate(inout vec3 lightDir, inout vec3 diffuseBRDF, inout vec3 specularBRDF,DirectionalLight currentLight)
{

}