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

layout(set =0,binding = 0) uniform CameraData
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

layout(location = 0) in vec3 outWorldPos;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 materialColor;
layout(location = 3) in vec3 Normal;
layout(location = 4) in vec3 CameraPoition;

layout(location = 0) out vec4 outFragColor;
layout(set = 1, binding = 0) uniform sampler2D tex1;

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
//https://www.youtube.com/watch?v=RRE-F57fbXw soruce 
const float PI = 3.14159265359;
float DistributionGGX(float roughness, vec3 N, vec3 H);
float GeometrySchlickGGX(float roughness, vec3 N, vec3 X);
float GeometrySmith(float roughness, vec3 N, vec3 V, vec3 L);
vec3 FresnelSchlick(vec3 F0, vec3 V, vec3 H);
vec3 PBR(vec3 normal, vec3 viewVector, DirectionalLight light, vec3 halfVector, vec3 F0, float roughnes, float metallic, vec3 meshColor);

void main() {
    //vec3 f0 = vec3(1.0);
    vec3 f0 = vec3(0.04);
    vec3 meshColor = texture(tex1, texCoord).xyz * materialColor;

    vec3 pbrNormal = normalize(Normal);
    vec3 viewVector = normalize(CameraPoition - outWorldPos);
    vec3 outGoingLight = vec3(0);
    //for (int i = 0; i < DirectionalLightData.Lights.length(); i++)
    //{
    //    DirectionalLight currentLight = DirectionalLightData.Lights[i];
    //    outGoingLight += PBR(pbrNormal, viewVector, currentLight, halfWayVecotr, f0, 0, 0, meshColor);
    //}
    DirectionalLight currentLight = DirectionalLightData.Light;
    currentLight.Direction = normalize(currentLight.Direction);
    vec3 halfWayVecotr = normalize(viewVector + currentLight.Direction);

    outGoingLight = PBR(pbrNormal, viewVector, currentLight, halfWayVecotr, f0, 0.5, 0, meshColor);

    vec3 ambient = vec3(0.03) * meshColor;

    // ambient lighting (note that the next IBL tutorial will replace 
    // this ambient lighting with environment lighting).
    //vec3 color = ambient + outGoingLight;
    vec3 color = outGoingLight;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0 / 2.2));

    outFragColor = vec4(outGoingLight, 1.0);
}

float DistributionGGX(float roughness, vec3 N, vec3 H)
{
    float numerator = pow(roughness, 2.0);
    float NdotH = max(dot(N, H), 0.0);
    float denominator = PI * pow(pow(NdotH, 2.0) * (pow(roughness, 2.0) - 1.0) + 1.0, 2.0);
    denominator = max(denominator, 0.000001);
    return numerator / denominator;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float roughness, vec3 N, vec3 X)
{
    float numerator = max(dot(N, X), 0.0);
    float k = roughness / 2.0;
    float denominator = max(dot(N, X), 0.0) * (1.0 - k) + k;
    denominator = max(denominator, 0.000001);
    return numerator / denominator;
}
// ----------------------------------------------------------------------------
float GeometrySmith(float roughness, vec3 N, vec3 V, vec3 L)
{
    return GeometrySchlickGGX(roughness, N, V) * GeometrySchlickGGX(roughness, N, L);

}
// ----------------------------------------------------------------------------
vec3 FresnelSchlick(vec3 F0, vec3 V, vec3 H)
{
    return F0 + (vec3(1.0) - F0) * pow(1 - max(dot(V, H), 0.0), 5.0);
}

vec3 PBR(vec3 normal, vec3 viewVector, DirectionalLight light, vec3 halfVector, vec3 F0, float roughnes, float metallic, vec3 meshColor) {
    vec3 ks = FresnelSchlick(F0, viewVector, halfVector);
    vec3 kd = (vec3(1.0) - ks) * (1.0 - metallic);

    vec3 lambert = meshColor / PI;

    vec3 cookTorranceNumerator = GeometrySchlickGGX(roughnes, normal, halfVector) * GeometrySmith(roughnes, normal, viewVector, light.Direction) * FresnelSchlick(F0, viewVector, halfVector);
    float cookTorranceDenominator = 4.0 * max(dot(viewVector, normal), 0.0) * max(dot(light.Direction, normal), 0.0);
    cookTorranceDenominator = max(cookTorranceDenominator, 0.000001);
    vec3 cookTorance = cookTorranceNumerator / cookTorranceDenominator;

    vec3 BRDF = kd * lambert + cookTorance;
    vec3 emiivemeh = vec3(0.0);
    vec3 outGoingLight = emiivemeh + BRDF * light.Color * max(dot(light.Direction, normal), 0.0);
    return outGoingLight;
}