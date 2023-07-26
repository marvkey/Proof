#Vertex Shader

#version 450
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;
layout(location = 5) in mat4 aTransform;

layout(location = 0) out Vertex
{
    vec3 WorldPos;
    vec2 TexCoords;
    vec3 Normal;
    vec3 Tangent;
    vec3 Bitangent;
    vec3 CameraPosition;
} Vout;

layout(set = 1, binding = 0) uniform CameraData
{
    mat4 ProjectionMatrix;
    mat4 ViewMatrix;
    vec3 Position;
}u_Camera;
void main() {
    Vout.Tangent = aTangent;
    Vout.Bitangent = aBitangent;
    Vout.TexCoords = aTexCoords;
    Vout.WorldPos = vec3(aTransform * vec4(aPosition, 1.0));
    Vout.Normal = mat3(aTransform) * aNormal;
    Vout.CameraPosition = u_Camera.Position;
    gl_Position = u_Camera.ProjectionMatrix * u_Camera.ViewMatrix * vec4(Vout.WorldPos, 1.0);
}

#Fragment Shader
#version 450 core

//https://github.com/Nadrin/PBR/blob/master/data/shaders/glsl/pbr_fs.glsl
//https://learnopengl.com/code_viewer_gh.php?code=src/6.pbr/2.2.2.ibl_specular_textured/2.2.2.pbr.fs
//https://cdn2.unrealengine.com/Resources/files/2013SiggraphPresentationsNotes-26915738.pdf
//https://github.com/Shot511/RapidGL/blob/master/src/demos/22_pbr/pbr-lighting.glh

layout(location = 0) out vec4 out_FragColor;

layout(set = 0, binding = 5) uniform sampler2D u_AlbedoMap;
layout(set = 0, binding = 6) uniform sampler2D u_NormalMap;
layout(set = 0, binding = 7) uniform sampler2D u_MetallicMap;
layout(set = 0, binding = 8) uniform sampler2D u_RoughnessMap;

layout(set = 1, binding = 2) uniform samplerCube u_IrradianceMap;
layout(set = 1, binding = 3) uniform samplerCube u_PrefilterMap;
layout(set = 1, binding = 4) uniform sampler2D u_BRDFLUT;

layout(location = 0) in Vertex
{
    vec3 WorldPos;
    vec2 TexCoords;
    vec3 Normal;
    vec3 Tangent;
    vec3 Bitangent;
    vec3 CameraPosition;
} Fs_in;
struct DirectionalLight {
    vec3 Color;
    float Intensity;
    vec3 Direction;
};

layout(std140, set = 1, binding = 1) buffer DirectionalLightStorageBuffer {

    DirectionalLight Lights[];
} DirectionalLightData;
const float PI = 3.1415926535897932384626433832795f;
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
vec3 FresnelSchlick(float cos_theta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(max(1.0 - cos_theta, 0.0), 5.0);
}

vec3 FresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}

float DistributionGGX(vec3 n, vec3 h, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(n, h), 0.0);
    float NdotH2 = NdotH * NdotH;

    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / max(denom, 1e-5);
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / max(denom, 1e-5);
}

float GeometrySmith(vec3 n, vec3 wo, vec3 wi, float roughness)
{
    float NdotV = max(dot(n, wo), 0.0);
    float NdotL = max(dot(n, wi), 0.0);

    float ggx1 = GeometrySchlickGGX(NdotL, roughness);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);

    return ggx1 * ggx2;
}

// Constant normal incidence Fresnel factor for all dielectrics.
const vec3  Fidelectric = vec3(0.04);
void main()
{
    vec3 albedo = u_MaterialUniform.AlbedoTexToggle == true ? texture(u_AlbedoMap, Fs_in.TexCoords).rgb * u_MaterialUniform.Albedo : u_MaterialUniform.Albedo;
    float metallic = u_MaterialUniform.MetallnesTexToggle == true ? texture(u_MetallicMap, Fs_in.TexCoords).r * u_MaterialUniform.Metalness : u_MaterialUniform.Metalness;
    float roughness = u_MaterialUniform.RoghnessTexToggle == true ? texture(u_RoughnessMap, Fs_in.TexCoords).r * max(u_MaterialUniform.Roughness,0.001) : max(u_MaterialUniform.Roughness,0.001); // max to keep specular
    vec3 normal = CalculateNormal();

    vec3 view = normalize(Fs_in.CameraPosition - Fs_in.WorldPos);
    vec3 reflection = reflect(-view, normal);
    vec3 F0 = mix(Fidelectric, albedo, metallic);
    
    // Angle between surface normal and outgoing view Direction.
	float cosView = max(0.0, dot(normal, view));

    int numDirectionalLights = DirectionalLightData.Lights.length();
    vec3 directLighting = vec3(0);
    for (int i = 0; i < 1; i++)
    {
        //https://github.com/Shot511/RapidGL/blob/master/src/demos/22_pbr/pbr-lighting.glh
        DirectionalLight currentLight= DirectionalLightData.Lights[i];
        vec3 wi= normalize(currentLight.Direction);
        vec3 halfwayVector = normalize(view + wi);
        vec3 radiance = currentLight.Color * currentLight.Intensity;

      
        vec3 F  = FresnelSchlick(max(dot(halfwayVector, view), 0.0), F0);

      // cook-torrance brdf
        float NDF = DistributionGGX(normal, halfwayVector, roughness);
        float G   = GeometrySmith(normal, view, wi, roughness);

        float NdotL    = max(dot(normal, wi), 0.0);
        vec3  num      = NDF * G * F;
        float denom    = 4.0 * cosView * NdotL;
        vec3  specular = num / max(denom, 0.00001); // avoid dividing by zero

     

        vec3 kd = (1.0 - F) * (1.0 - metallic);

       directLighting += (kd * albedo / PI + specular) * radiance * NdotL;
    }
    
    vec3 iblEfeect = vec3(0);
    //IBL
    {
        //https://github.com/kidrigger/Blaze/blob/canon/Blaze/shaders/forward/fPBR.frag
        //https://github.com/Shot511/RapidGL/blob/master/src/demos/22_pbr/pbr-lighting.glh

        // fresnel reflectance
       
        vec3 F= FresnelSchlickRoughness(cosView, F0, roughness);
        vec3 kd = (1.0 - F) * (1.0 - metallic);

        int specularTextureLevels = textureQueryLevels(u_PrefilterMap);
		vec3 prefilteredColor = texture(u_PrefilterMap, reflection * specularTextureLevels).rgb;
        vec2 envBRDF  = texture(u_BRDFLUT, vec2(cosView, roughness)).rg;
		vec3 specular = prefilteredColor * (F * envBRDF.x + envBRDF.y);
        
        vec3 diffuse = texture(u_IrradianceMap, normal).rgb * albedo;

        float emmision = 0.0f;
        float ao= 1.0f;
        iblEfeect =  (kd * diffuse + specular) * ao + emmision;
        //iblEfeect*= vec3(0.025); // maybe for effect
    }
    
    vec3 finalColor = iblEfeect + directLighting;

    // HDR tonemapping
    finalColor = finalColor / (finalColor + vec3(1.0));
    // gamma correct
    finalColor = pow(finalColor, vec3(1.0/2.2)); 
    out_FragColor = vec4(finalColor ,1.0);
}
