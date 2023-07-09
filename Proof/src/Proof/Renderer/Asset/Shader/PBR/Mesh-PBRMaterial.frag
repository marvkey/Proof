#version 450
//https://github.com/Shot511/RapidGL/blob/master/src/demos/22_pbr/pbr-lighting.glh
//https://www.youtube.com/watch?v=RRE-F57fbXw soruce 
layout(location = 0) in vec3 WorldPos;
layout(location = 1) in vec2 TexCoords;
layout(location = 2) in vec3 Normal;
layout(location = 3) in vec3 CameraPoition;

layout(location = 4) in vec3 outTangent;
layout(location = 5) in vec3 outBitangent;
layout(location = 0) out vec4 outFragColor;
layout(set = 0, binding = 4) uniform samplerCube irradianceMap;
layout(set = 0, binding = 5) uniform samplerCube prefilterMap;
layout(set = 0, binding = 6) uniform sampler2D BRDFLUT;

//layout(set = 3, binding = 3) uniform sampler2D samplers[];

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


const float PI = 3.1415926535897932384626433832795f;
// ----------------------------------------------------------------------------
// Easy trick to get tangent-normals to world-space to keep PBR code simplified.
// Don't worry if you don't get what's going on; you generally want to do normal 
// mapping the usual way for performance anyways; I do plan make a note of this 
// technique somewhere later in the normal mapping tutorial.
vec3 getNormalFromMap(sampler2D map)
{
    vec3 tangentNormal = texture(map, TexCoords).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(WorldPos);
    vec3 Q2  = dFdy(WorldPos);
    vec2 st1 = dFdx(TexCoords);
    vec2 st2 = dFdy(TexCoords);

    vec3 N   = normalize(Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}
// ----------------------------------------------------------------------------
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
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
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}   
// ----------------------------------------------------------------------------
struct Material{
    vec3 Color;
    float Metallic;
    float Roughness;
    int UsePBR;
    vec2 Tiling;
    vec2 Offset;
};
//push constants block
layout( push_constant ) uniform constants
{
	Material Mat;
} MaterialData;


layout(set = 1, binding = 0) uniform sampler2D AlbedoMap;
layout(set = 1, binding = 1) uniform sampler2D NormalMap;
layout(set = 1, binding = 2) uniform sampler2D MetallicMap;
layout(set = 1, binding = 3) uniform sampler2D RoughnessMap;

void main()
{
     Material currentMaterial = MaterialData.Mat;
   
    //currentMaterial.Color = texture(AlbedoMap,texCoord).xyz * currentMaterial.Color;
    //currentMaterial.Roughness = texture(RoughnessMap, texCoord).r * currentMaterial.Roughness;
    //currentMaterial.Metallic = texture(MetallicMap, texCoord).r * currentMaterial.Metallic;

    // vec3 albedo = pow(texture(albedoMap, TexCoords).rgb, vec3(2.2));
    vec3 albedo = currentMaterial.Color;
    float metallic = currentMaterial.Metallic;
    float roughness = currentMaterial.Roughness;
    if(currentMaterial.UsePBR == 0)
    {
        vec2 tiledTexCoord = mod(TexCoords * currentMaterial.Tiling + currentMaterial.Offset, vec2(1.0));
        //vec2 scale = TexCoords * vec2(3,3);
        outFragColor = vec4(albedo, 1.0)*texture(AlbedoMap, tiledTexCoord);
        return;
    }
    //vec3 N = getNormalFromMap();
    vec3 N = normalize(Normal);
    vec3 V = normalize(CameraPoition - WorldPos);
    vec3 R = reflect(-V, N); 

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < 1; i++) 
    {
        // calculate per-light radiance
        vec3 L = normalize(DirectionalLightData.Light.Direction - WorldPos);
        vec3 H = normalize(V + L);
        float distance = length(DirectionalLightData.Light.Direction - WorldPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = DirectionalLightData.Light.Base.Color * attenuation;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);   
        float G   = GeometrySmith(N, V, L, roughness);    
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);        
        
        vec3 numerator    = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
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
        float NdotL = max(dot(N, L), 0.0);        

        // add to outgoing radiance Lo
        Lo += (kD * albedo / PI + specular) * radiance * NdotL; // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    }   
     
    
    // ambient lighting (we now use IBL as the ambient term)
    vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
    
    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;	  
    
    vec3 irradiance = texture(irradianceMap, N).rgb;
    vec3 diffuse      = irradiance * albedo;
    
    // sample both the pre-filter map and the BRDF lut and combine them together as per the Split-Sum approximation to get the IBL specular part.
    const float MAX_REFLECTION_LOD = 12.0;
    vec3 prefilteredColor = textureLod(prefilterMap, R,  roughness * MAX_REFLECTION_LOD).rgb;    
    vec2 brdf  = texture(BRDFLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
    vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

    vec3 ambient = (kD * diffuse + specular) * 1;
    
    vec3 color = ambient + Lo;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2)); 

    outFragColor = vec4(color , 1.0);
}

