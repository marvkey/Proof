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

layout(std140,binding = 1) uniform CameraData
{
    mat4 ProjectionMatrix;
    mat4 ViewMatrix;
    vec3 Position;
}Camera;
out VS_OUT
{
    vec2 TexCoords;
    vec3 Tangent;
    vec3 Bitangent;
    vec3 Normal;
    vec3 FragPos;
    vec3 CameraPosition;
} vs_in;
struct Material{
    vec3 AlbedoColour;
    float Metallness;
    float Roughness;
    float AO;
};

out Material m_Material;
void main() {

    vs_in.TexCoords = aTexCoords;
    vs_in.Normal = mat3(aTransform) * aNormal;
    vs_in.FragPos = vec3(aTransform * vec4(aPos,1.0));
    vs_in.Bitangent = aBitangent;
    vs_in.Tangent = aTangent;
    vs_in.CameraPosition = Camera.Position;

    m_Material.AlbedoColour = aAlbedoColour;
    m_Material.Metallness = aMetallness;
    m_Material.Roughness =aRoughness;
    m_Material.AO = aAO;

    gl_Position = Camera.ProjectionMatrix * Camera.ViewMatrix * vec4(FragPos,1.0);
}
#Fragment Shader

#version 450 core
out vec4 FragColor;
in Material m_Material;
in VS_OUT{
    vec2 TexCoords;
    vec3 Tangent;
    vec3 Bitangent;
    vec3 Normal;
    vec3 FragPos;
    vec3 CameraPosition;
} fs_in;
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;

uniform int AmountLight;
uniform vec3 lightPositions[150];
uniform vec3 lightColors[150];


// IBL
uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D brdfLUT;
float NormalDistrubution(float alpha, vec3 normal, vec3 h);
void main(){

}

float NormalDistrubution(float alpha, vec3 normal, vec3 h){
    float numerator = pow(alpha,2.0);
    float 
    return 0;
}