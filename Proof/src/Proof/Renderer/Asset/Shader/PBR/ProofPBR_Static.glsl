#Vertex Shader

#version 450
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;
layout(location = 5) in mat4 aTransform;

struct VertexOutput
{

    vec3 WorldPos;
    vec3 Normal;
    vec2 TexCoords;
    vec3 Tangent;
    vec3 Bitangent;
    vec3 CameraPosition;
};
layout(location = 0) out VertexOutput Output;

layout(set = 1, binding = 0) uniform CameraData
{
    mat4 ProjectionMatrix;
    mat4 ViewMatrix;
    vec3 Position;
    float NearPlane;
    float FarPlane;
}u_Camera;
//https://computergraphics.stackexchange.com/questions/8979/strange-dark-spot-when-lighting-in-deferred-rendering-pbr-shaders
void main() {

    Output.Tangent = aTangent;
    Output.Bitangent = aBitangent;
    Output.TexCoords = aTexCoords;
    //Output.TexCoords = vec2(aTexCoords.x,1 - aTexCoords.y);
    Output.WorldPos = vec4(aTransform * vec4(aPosition, 1.0)).xyz;

    mat3 normalMatrix = transpose(inverse(mat3(aTransform)));
    Output.Normal = normalMatrix * aNormal.xyz;
    Output.CameraPosition = u_Camera.Position;
    gl_Position = u_Camera.ProjectionMatrix * u_Camera.ViewMatrix * vec4(Output.WorldPos, 1.0);
}

#Fragment Shader
#version 450 core

//https://github.com/Nadrin/PBR/blob/master/data/shaders/glsl/pbr_fs.glsl
//https://learnopengl.com/code_viewer_gh.php?code=src/6.pbr/2.2.2.ibl_specular_textured/2.2.2.pbr.fs
//https://cdn2.unrealengine.com/Resources/files/2013SiggraphPresentationsNotes-26915738.pdf
//https://github.com/Shot511/RapidGL/blob/master/src/demos/22_pbr/pbr-lighting.glh
//https://github.com/Angelo1211/HybridRenderingEngine/blob/master/assets/shaders/PBRClusteredShader.frag
layout(location = 0) out vec4 out_FragColor;

layout(set = 0, binding = 5) uniform sampler2D u_AlbedoMap;
layout(set = 0, binding = 6) uniform sampler2D u_NormalMap;
layout(set = 0, binding = 7) uniform sampler2D u_MetallicMap;
layout(set = 0, binding = 8) uniform sampler2D u_RoughnessMap;

layout(set = 2, binding = 2) uniform samplerCube u_IrradianceMap;
layout(set = 2, binding = 3) uniform samplerCube u_PrefilterMap;
layout(set = 2, binding = 4) uniform sampler2D u_BRDFLUT;

#include <PBR/PBR.glslh>

struct VertexOutput
{

    vec3 WorldPos;
    vec3 Normal;
    vec2 TexCoords;
    vec3 Tangent;
    vec3 Bitangent;
    vec3 CameraPosition;
};
layout(location = 0) in VertexOutput Fs_in;

struct DirectionalLight {
    vec3 Color;
    float Intensity;
    vec3 Direction;
};

layout(std140, set = 1, binding = 1) buffer DirectionalLightStorageBuffer {

    DirectionalLight Lights[];
} DirectionalLightData;
layout(set = 3, binding = 1) uniform SkyBoxData
{
    vec3 TintColor;
    float Lod;
    float Intensity;
    float Roation;
}u_SkyBoxInfo;
const float Epsilon = 0.00001;
layout(push_constant) uniform Material
{
    vec3 Albedo;
    float Metalness;
    float Roughness;
    bool AlbedoTexToggle;
    bool NormalTexToggle;
    bool RoghnessTexToggle;
    bool MetallnesTexToggle;
    vec2 Tiling;
    vec2 Offset;
    //int padding[2]; // Padding to ensure proper alignment
}u_MaterialUniform;

vec3 CalculateNormal()
{
    if (u_MaterialUniform.NormalTexToggle == false)
        return normalize(Fs_in.Normal);
    vec3 tangentNormal = texture(u_NormalMap, Fs_in.TexCoords).xyz*2.0 -1.0f;
    vec3 Q1 = dFdx(Fs_in.WorldPos);
    vec3 Q2 = dFdy(Fs_in.WorldPos);
    vec2 st1 = dFdx(Fs_in.TexCoords);
    vec2 st2 = dFdy(Fs_in.TexCoords);

    vec3 N = normalize(Fs_in.Normal);
    vec3 T = normalize(Q1 * st2.t - Q2 * st1.t);
    vec3 B = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}


// Constant normal incidence Fresnel factor for all dielectrics.
const vec3  Fidelectric = vec3(0.04);
void main()
{
    vec3 albedo = u_MaterialUniform.AlbedoTexToggle == true ? texture(u_AlbedoMap, Fs_in.TexCoords).rgb * u_MaterialUniform.Albedo : u_MaterialUniform.Albedo;
    float metallic = u_MaterialUniform.MetallnesTexToggle == true ? texture(u_MetallicMap, Fs_in.TexCoords).r * u_MaterialUniform.Metalness : u_MaterialUniform.Metalness;
    float roughness = u_MaterialUniform.RoghnessTexToggle == true ? texture(u_RoughnessMap, Fs_in.TexCoords).r * max(u_MaterialUniform.Roughness,0.00) : max(u_MaterialUniform.Roughness,0.00); // max to keep specular
    vec3 normal = CalculateNormal();

    vec3 view = normalize(Fs_in.CameraPosition - Fs_in.WorldPos);
    vec3 reflection = reflect(-view, normal);
    const vec3 F0 = mix(Fidelectric, albedo, metallic);
    
    // Angle between surface normal and outgoing view Direction.
	const float NdotV = max(dot(normal, view),0.0);

    int numDirectionalLights = DirectionalLightData.Lights.length();
    vec3 directLighting = vec3(0);
    for (int i = 0; i < 1; i++)
    {
        //https://github.com/Angelo1211/HybridRenderingEngine/blob/master/assets/shaders/PBRClusteredShader.frag
        DirectionalLight currentLight= DirectionalLightData.Lights[i];
        vec3 lightDirection = -normalize(currentLight.Direction);
        vec3 halfway = normalize(view + lightDirection);
        float nDotL = max(dot(normal, lightDirection), 0.0);
        vec3 radiance = currentLight.Color * currentLight.Intensity;
        
        float NDF = DistributionGGX(normal, halfway, roughness);
        float G   = GeometrySmith(NdotV, nDotL, roughness);
        vec3  F   = FresnelSchlick(max(dot(halfway,view), 0.0), F0);

        vec3 kD = 1.0 - F;
        kD *= 1.0 - metallic;

        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * NdotV * nDotL;
        vec3 specular = numerator / max (denominator, 0.0001);

        vec3 lightEffect = (kD * albedo / PI + specular )  * radiance * nDotL;
        directLighting +=lightEffect;
    }
    
    vec3 iblEfeect = vec3(0);
    //IBL
    {
        vec3 Lr = 2.0 *dot(view,normal) * normal - view;
        //https://github.com/kidrigger/Blaze/blob/canon/Blaze/shaders/forward/fPBR.frag
        //https://github.com/Shot511/RapidGL/blob/master/src/demos/22_pbr/pbr-lighting.glh

        // fresnel reflectance
        vec3 F= FresnelSchlickRoughness(NdotV, F0, roughness);
        vec3 irradiance = texture(u_IrradianceMap, normal).rgb ;
        vec3 kd = (1 - F) * (1.0 - metallic);

        vec3 diffuseIBL = kd * albedo * irradiance;

        int specularTextureLevels = textureQueryLevels(u_PrefilterMap);
		vec3 specularIrradiance = textureLod(u_PrefilterMap, Lr,specularTextureLevels * roughness).rgb *u_SkyBoxInfo.TintColor * u_SkyBoxInfo.Intensity ;

        vec2 specularBRDF  = texture(u_BRDFLUT, vec2(NdotV,roughness)).rg;

        //TODO Fix why brdf is causing shader bug
        //of a grey dot
		//vec3 specularIBL = specularIrradiance * (F0* specularBRDF.x + specularBRDF.y) ; 
		vec3 specularIBL = specularIrradiance * (F0) ; 

        iblEfeect += specularIBL + diffuseIBL;
    }
    
    vec3 finalColor =  iblEfeect ;

    // HDR tonemapping
    finalColor = finalColor / (finalColor + vec3(1.0));
    // gamma correct
    finalColor = pow(finalColor, vec3(1.0/2.2)); 
    out_FragColor = vec4(finalColor ,1.0);
}
