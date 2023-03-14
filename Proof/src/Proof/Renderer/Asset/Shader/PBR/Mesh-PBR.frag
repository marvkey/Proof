#version 450
//https://github.com/Shot511/RapidGL/blob/master/src/demos/22_pbr/pbr-lighting.glh
//https://www.youtube.com/watch?v=RRE-F57fbXw soruce 
layout(location = 0) in vec3 WorldPos;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 Normal;
layout(location = 3) in vec3 CameraPoition;

layout(set = 0, binding = 4) uniform samplerCube irradianceMap;
layout(set = 0, binding = 5) uniform samplerCube prefilterMap;
layout(set = 0, binding = 6) uniform sampler2D BRDFLUT;

layout(location = 0) out vec4 outFragColor;
layout(set = 1, binding = 0) uniform sampler2D tex1;

struct BaseLight {
    vec3 Color;
    float Intensity;
};
struct DirectionalLight {
    BaseLight Base;
    vec3 Direction;
};
//layout(std140, set = 0, binding = 1) buffer DirectionalLightBuffer {
//
//    DirectionalLight Lights[];
//} DirectionalLightData;
layout(std140, set = 0, binding = 1) uniform DirectionalLightBuffer
{
   DirectionalLight Light;
}DirectionalLightData;


const float PI = 3.14159265359;
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float nom = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}   
vec3 pbr(BaseLight base, vec3 direction, vec3 normal, vec3 world_pos, vec3 meshColor, float roughness, float metallic, vec3 camPos)
{
    vec3 wo = normalize(camPos - world_pos);
    vec3 wi = normalize(direction);
    vec3 h = normalize(wo + wi);
    vec3 radiance = base.Color * base.Intensity;

    // fresnel reflectance
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, meshColor, metallic);
    vec3 F = fresnelSchlick(max(dot(h, wo), 0.0), F0);

    // cook-torrance brdf
    float NDF = DistributionGGX(normal, h, roughness);
    float G = GeometrySmith(normal, wo, wi, roughness);

    float NdotL = max(dot(normal, wi), 0.0);
    vec3  num = NDF * G * F;
    float denom = 4.0 * max(dot(normal, wo), 0.0) * NdotL;
    vec3  specular = num / max(denom, 1e-5);

    vec3 ks = F;
    vec3 kd = vec3(1.0) - ks;
    kd = kd * (1.0 - metallic);

    return (kd * meshColor / PI + specular) * radiance * NdotL;
}
vec3 calcDirectionalLight(DirectionalLight light, vec3 normal, vec3 world_pos, vec3 meshColor, float roughness, float metallic, vec3 camPos)
{
    //return pbr(light.Base, light.Direction, normal, world_pos, meshColor, roughness, metallic, camPos);
    return pbr(light.Base,light.Direction, normal, world_pos, meshColor, roughness, metallic, camPos);
}

vec3 IndirectLightingDiffuse(vec3 normal, vec3 world_pos, vec3 color, float roughness,float metallic,vec3 cameraPos )
{
    //mips in range of 0-4, need to add more casue some of our cubemps can have  mips
    const float MAX_REFLECTION_LOD = 4.0; 
    float ao        = 1;

    vec3 wo = normalize(cameraPos  - world_pos);
    vec3 r  = reflect(-wo, normal);

    // fresnel reflectance
    vec3 F0 = vec3(0.04);
         F0 = mix(F0, color, metallic);

    vec3 F  = fresnelSchlickRoughness(max(dot(normal, wo), 0.0), F0, roughness);
    vec3 ks = F;
    vec3 kd = 1.0 - ks;
         kd = kd * (1.0 - metallic);
    
    // diffuse IBL term
    vec3 irradiance = texture(irradianceMap, normal).rgb;
    vec3 diffuse    = color * irradiance;

    // specular IBL term
    vec3 prefiletered_color = textureLod(prefilterMap, r, roughness * MAX_REFLECTION_LOD).rgb;
    vec2 brdf               = texture(BRDFLUT, vec2(max(dot(normal, wo), 0.0), roughness)).rg;
    vec3 specular           =  prefiletered_color * (F * brdf.x + brdf.y);


    vec3 emision =vec3(0);
    // total indirect lighting
    return (kd * diffuse + specular) * ao + emision;
}
// ----------------------------------------------------------------------------
void main()
{
    vec3 meshColor = texture(tex1, texCoord).xyz;
    vec3 N = normalize(Normal);
    vec3 V = normalize(CameraPoition - WorldPos);
    vec3 Lo = vec3(0);
    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
    vec3 F0 = vec3(0.04);
    //  when rougness is 0 metallic will not repson to light because 
    // metallic on responds to specullar lgiht and there will be no spe5uclar light in the scne
    float metallic = 0;
    float roughness = 0.5;
    Lo += calcDirectionalLight(DirectionalLightData.Light, N, WorldPos, meshColor, roughness, metallic, CameraPoition);
  //  vec3 ambient = textureLod(irradianceMap,N,0).rgb * meshColor;
    vec3 ambient = IndirectLightingDiffuse(N,WorldPos,meshColor,roughness,metallic,CameraPoition);

    vec3 color = ambient + Lo;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0 / 2.2));

    outFragColor = vec4(color, 1.0);
}