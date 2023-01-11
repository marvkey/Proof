#Vertex Shader

#version 450

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;
layout(location = 5) in mat4 aTransform;
layout(location = 9) in vec3 aMaterialColor;

layout(location = 0) out vec3 outWorldPos;
layout(location = 1) out vec2 outTexCoords;
layout(location = 2) out vec3 outmaterialColor;
layout(location = 3) out vec3 outNormal;
layout(location = 4) out vec3 outCameraPostion;

layout(set = 0, binding = 0) uniform CameraData
{
    mat4 ProjectionMatrix;
    mat4 ViewMatrix;
    vec3 Position;
    float padding;
}CameraUBO;
void main() {
    outmaterialColor = aMaterialColor;
    outTexCoords = aTexCoords;
    outWorldPos = vec3(aTransform * vec4(aPosition, 1.0));
    outNormal = mat3(aTransform) * aNormal;
    outCameraPostion = CameraUBO.Position;
    gl_Position = CameraUBO.ProjectionMatrix * CameraUBO.ViewMatrix * vec4(outWorldPos, 1.0);

}

#Fragment Shader

#version 450

layout(location = 0) in vec3 WorldPos;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 materialColor;
layout(location = 3) in vec3 Normal;
layout(location = 4) in vec3 CameraPoition;

layout(location = 0) out vec4 outFragColor;
layout(set = 1, binding = 0) uniform sampler2D tex1;

struct BaseLight {
    vec3 Color;
    float Intensity;
};
struct DirectionalLight {
    vec3 Color;
    float Intensity;
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

vec3 pbr(BaseLight base, vec3 direction, vec3 normal, vec3 world_pos)
{
    vec3  albedo = u_has_albedo_map ? texture(u_albedo_map, in_texcoord).rgb : u_albedo;
    float metallic = u_has_metallic_map ? texture(u_metallic_map, in_texcoord).r : u_metallic;
    float roughness = u_has_roughness_map ? texture(u_roughness_map, in_texcoord).r : u_roughness;
    normal = u_has_normal_map ? getNormalFromMap() : normal;

    vec3 wo = normalize(u_cam_pos - world_pos);
    vec3 wi = normalize(direction);
    vec3 h = normalize(wo + wi);
    vec3 radiance = base.color * base.intensity;

    // fresnel reflectance
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);
    vec3 F = fresnelSchlick(max(dot(h, wo), 0.0), F0);

    // cook-torrance brdf
    float NDF = distributionGGX(normal, h, roughness);
    float G = geometrySmith(normal, wo, wi, roughness);

    float NdotL = max(dot(normal, wi), 0.0);
    vec3  num = NDF * G * F;
    float denom = 4.0 * max(dot(normal, wo), 0.0) * NdotL;
    vec3  specular = num / max(denom, 1e-5);

    vec3 ks = F;
    vec3 kd = 1.0 - ks;
    kd = kd * (1.0 - metallic);

    return (kd * albedo / PI + specular) * radiance * NdotL;
}
//https://www.youtube.com/watch?v=RRE-F57fbXw soruce 
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
// ----------------------------------------------------------------------------
void main()
{
    vec3 meshColor = texture(tex1, texCoord).xyz * materialColor;
    vec3 N = normalize(Normal);
    vec3 V = normalize(CameraPoition - WorldPos);

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
    vec3 F0 = vec3(0.04);
    //  when rougness is 0 metallic will not repson to light because 
    // metallic on responds to specullar lgiht and there will be no speuclar light in the scne
    float metallic = 1.0;
    float roughness = 0.1;
    F0 = mix(F0, meshColor, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);
   // for (int i = 0; i < 4; ++i)
   // {
    DirectionalLight currentLight = DirectionalLightData.Light;
    vec3 L = normalize(currentLight.Direction);
    vec3 H = normalize(V + L);
    // Cook-Torrance BRDF
    float NDF = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);
    vec3 F = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);

    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0000001; // + 0.0001 to prevent divide by zero
    vec3 specular = numerator / denominator;

    // kS is equal to Fresnel
    vec3 kS = F;
    // for energy conservation, the diffuse and specular light can't
    // be above 1.0 (unless the surface emits light); to preserve this
    // relationship the diffuse component (kD) should equal 1.0 - kS.
    vec3 kD = vec3(1.0) - kS;
    // multiply kD by the inverse metalness such that only non-metals 
    // have diffuse lighting, or a linear blend if partly metal (pure metals
    // have no diffuse light).
    kD *= 1.0 - metallic;

    // scale light by NdotL
    float NdotL = max(dot(L, N), 0.0);
    vec3 lambert = meshColor / PI;
    // add to outgoing radiance Lo
    Lo += (kD * lambert + specular) * NdotL * currentLight.Color * currentLight.Intensity;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again


    // ambient lighting (note that the next IBL tutorial will replace 
    // this ambient lighting with environment lighting).
    vec3 ambient = vec3(0.03) * meshColor;

    vec3 color = ambient + Lo;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0 / 2.2));

    outFragColor = vec4(color, 1.0);
}

