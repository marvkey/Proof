#Vertex Shader
#version 450 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

layout(location = 5) in mat4 aTransform;

layout(location = 9) in vec3 aAlbedoColour;
layout(location = 10) in float aMetallness;
layout(location = 11) in float aRoughness;
layout(location = 12) in float aAO;
layout(location = 13) in int aMaterialEnabled;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec3 CameraPosition;

out vec3  MaterialColour;
out float Materialmetallic;
out float Materialroughness;
out float MaterialAO;
flat out int MaterialEnabled;
layout(std140,binding = 1) uniform CameraData
{
    mat4 ProjectionMatrix;
    mat4 ViewMatrix;
    vec3 Position;
}Camera;

void main() {
    MaterialColour = aAlbedoColour;
    Materialmetallic = aMetallness;
    Materialroughness = aRoughness;
    MaterialAO = aAO;
    MaterialEnabled =aMaterialEnabled;
    TexCoords = aTexCoords;
    FragPos = vec3(aTransform * vec4(aPos,1.0));
    Normal = mat3(aTransform) * aNormal;
    CameraPosition = Camera.Position;

    gl_Position = Camera.ProjectionMatrix * Camera.ViewMatrix * aTransform* vec4(aPos,1.0);
}


#Fragment Shader

#version 450 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec3 CameraPosition;

in vec3  MaterialColour;
in float Materialmetallic;
in float Materialroughness;
in float MaterialAO;
flat in int MaterialEnabled;

const float PI = 3.14159265359;
// lights
uniform int AmountLight;
uniform vec3 lightPositions[150];
uniform vec3 lightColors[150];

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D DiffuseShader;
// IBL
uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D brdfLUT;
struct DirectionalLight {
    vec3 Direction;
    vec3 Ambient;
    float Intensity;
};

struct PointLight {
    vec3 Position;
    vec3 Ambient;

    float Constant;
    float Linear;
    float Quadratic;

    float Radius;
    float  Intensity;
};

struct SpotLight {
    vec3 Position;
    vec3 Direction;
    vec3 Ambient;

    float CutOff;
    float OuterCutOff;

    float Constant;
    float Linear;
    float Quadratic;

    float Radius;
    float Intensity;
};
uniform int v_NrDirectionalLight;
uniform DirectionalLight v_DirectionalLight[150];

uniform int v_NrPointLight;
uniform PointLight v_PointLight[150];

uniform int v_NrSpotLight;
uniform SpotLight v_SpotLight[150];

vec3 CalcDirLight(DirectionalLight light,vec3 normal,vec3 viewDir,vec3 matcolour,float shininess);
vec3 CalcPointLight(PointLight light,vec3 normal,vec3 fragPos,vec3 viewDir,vec3 matcolour,float shininess);
vec3 CalcSpotLight(SpotLight light,vec3 normal,vec3 fragPos,vec3 viewDir,vec3 matcolour,float shininess);
float DistributionGGX(vec3 N,vec3 H,float roughness);
float GeometrySchlickGGX(float NdotV,float roughness);
float GeometrySmith(vec3 N,vec3 V,vec3 L,float roughness);
vec3 fresnelSchlick(float cosTheta,vec3 F0);
vec3 fresnelSchlickRoughness(float cosTheta,vec3 F0,float roughness) {
    return F0 + (max(vec3(1.0 - roughness),F0) - F0) * pow(clamp(1.0 - cosTheta,0.0,1.0),5.0);
}
void main() {

    vec3 NewMaterialColour = texture(albedoMap,TexCoords).rgb * MaterialColour;
    float NewMaterialmetallic = texture(metallicMap,TexCoords).r * Materialmetallic;
    float NewMaterialroughness = texture(roughnessMap,TexCoords).r * Materialroughness;
    int NewMaterialEnabled = MaterialEnabled;
    vec3 N = texture(normalMap,TexCoords).rgb;
    N = normalize(Normal);
    vec3 V = normalize(CameraPosition - FragPos);
    vec3 R = reflect(-V,N);
    vec3 F0 = vec3(0.04);
    F0 = mix(F0,NewMaterialColour,NewMaterialmetallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);
    for (int i = 0; i < AmountLight; ++i) {
        // calculate per-light radiance
        vec3 L = normalize(lightPositions[i] - FragPos);
        vec3 H = normalize(V + L);
        float distance = length(lightPositions[i] - FragPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = lightColors[i] * attenuation;

        // cook-torrance brdf
        float NDF = DistributionGGX(N,H,NewMaterialroughness);
        float G = GeometrySmith(N,V,L,NewMaterialroughness);
        vec3 F = fresnelSchlick(max(dot(H,V),0.0),F0);

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - NewMaterialmetallic;

        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N,V),0.0) * max(dot(N,L),0.0) + 0.0001;
        vec3 specular = numerator / denominator;

        // add to outgoing radiance Lo
        float NdotL = max(dot(N,L),0.0);
        Lo += (kD * NewMaterialColour / PI + specular) * radiance * NdotL;
    }
   // ambient lighting (we now use IBL as the ambient term)
    vec3 F = fresnelSchlickRoughness(max(dot(N,V),0.0),F0,NewMaterialroughness);

    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - NewMaterialmetallic;

    vec3 irradiance = texture(irradianceMap,N).rgb;
    vec3 diffuse = irradiance * NewMaterialColour;

    // sample both the pre-filter map and the BRDF lut and combine them together as per the Split-Sum approximation to get the IBL specular part.
    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(prefilterMap,R,NewMaterialroughness * MAX_REFLECTION_LOD).rgb;
    vec2 brdf = texture(brdfLUT,vec2(max(dot(N,V),0.0),NewMaterialroughness)).rg;
    vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

    vec3 ambient = (kD * diffuse + specular) * NewMaterialmetallic;

    vec3 color = ambient + Lo;

    color = color / (color + vec3(1.0));
    color = pow(color,vec3(1.0 / 2.2));
    if(NewMaterialEnabled==1)// has material
        FragColor = vec4(color,1.0);
    else
        FragColor = texture(DiffuseShader,TexCoords);

}

float DistributionGGX(vec3 N,vec3 H,float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N,H),0.0);
    float NdotH2 = NdotH * NdotH;

    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / denom;
}

float GeometrySchlickGGX(float NdotV,float roughness) {
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}
float GeometrySmith(vec3 N,vec3 V,vec3 L,float roughness) {
    float NdotV = max(dot(N,V),0.0);
    float NdotL = max(dot(N,L),0.0);
    float ggx2 = GeometrySchlickGGX(NdotV,roughness);
    float ggx1 = GeometrySchlickGGX(NdotL,roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta,vec3 F0) {
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta,0.0,1.0),5.0);
}

vec3 CalcPointLight(PointLight light,vec3 normal,vec3 fragPos,vec3 viewDir,vec3 matcolour,float shininess) {
    
    vec3 lightDir = normalize(light.Position - fragPos);
    float diff = max(dot(normal,lightDir),0.0);
    vec3 reflectDir = reflect(-lightDir,normal);
    float spec = pow(max(dot(viewDir,reflectDir),0.0),shininess);

    float distance = length(light.Position - fragPos);
    float attenuation = 1.0 / (light.Constant + light.Linear * distance + light.Quadratic * (distance * distance));

    vec3 ambient = light.Ambient * matcolour*light.Intensity;               // THIS 3 NNED TO BE MULTPLIED DIFFFRENTILY
    vec3 diffuse = diff * light.Ambient * matcolour*light.Intensity;        // THIS 3 NNED TO BE MULTPLIED DIFFFRENTILY
    vec3 specular =spec * light.Ambient * matcolour*light.Intensity;       // THIS 3 NNED TO BE MULTPLIED DIFFFRENTILY

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
    
}
vec3 CalcDirLight(DirectionalLight light,vec3 normal,vec3 viewDir,vec3 matcolour,float shininess) {
    vec3 lightDir = normalize(-light.Direction);
    float diff = max(dot(normal,lightDir),0.0);
    vec3 reflectDir = reflect(-lightDir,normal);
    float spec = pow(max(dot(viewDir,reflectDir),0.0),shininess);
    vec3 ambient = light.Ambient * matcolour;                 // THIS 3 NNED TO BE MULTPLIED DIFFFRENTILY
    vec3 diffuse = diff * light.Ambient * matcolour;         // THIS 3 NNED TO BE MULTPLIED DIFFFRENTILY
    vec3 specular = spec * light.Ambient * matcolour;       // THIS 3 NNED TO BE MULTPLIED DIFFFRENTILY
    return (ambient + diffuse + specular) * light.Intensity;
}
vec3 CalcSpotLight(SpotLight light,vec3 normal,vec3 fragPos,vec3 viewDir,vec3 matcolour,float shininess) {
    vec3 lightDir = normalize(light.Position - fragPos);
    float diff = max(dot(normal,lightDir),0.0);

    vec3 reflectDir = reflect(-lightDir,normal);
    float spec = pow(max(dot(viewDir,reflectDir),0.0),shininess);
    float distance = length(light.Position - fragPos);
    float attenuation = 1.0 / (light.Constant + light.Linear * distance + light.Quadratic * (distance * distance));
    float theta = dot(lightDir,normalize(-light.Direction));

    float epsilon = (light.CutOff - light.OuterCutOff);
    float intensity = clamp((theta - light.OuterCutOff) / epsilon,0.0,1.0);
    vec3 ambient = light.Ambient * matcolour;
    vec3 diffuse = diff * light.Ambient * matcolour;
    vec3 specular = spec *  light.Ambient* matcolour;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return(ambient + diffuse + specular)*light.Intensity;
}