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
out vec2 TexCoords;
out vec3 Normal;

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
    MaterialEnabled = aMaterialEnabled;

    vec4 worldPos = aTransform * vec4(aPos,1.0);
    FragPos = worldPos.xyz;
    TexCoords = aTexCoords;

    mat3 normalMatrix = transpose(inverse(mat3(aTransform)));
    Normal = normalMatrix * aNormal;

    gl_Position = Camera.ProjectionMatrix * Camera.ViewMatrix * worldPos;
}


#Fragment Shader
#version 450 core
layout(location = 0) out vec4 gPosition;
layout(location = 1) out vec4 gNormal;
layout(location = 2) out vec4 gAlbedoSpec;
in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D DiffuseShader;

in vec3  MaterialColour;
in float Materialmetallic;
in float Materialroughness;
in float MaterialAO;
flat in int MaterialEnabled;

void main() {
    // store the fragment position vector in the first gbuffer texture
    gPosition.rgb = FragPos;
    gPosition.a =1;
    // also store the per-fragment normals into the gbuffer
    gNormal.rgb = normalize(Normal);
    gNormal.a =1;
    // and the diffuse per-fragment color
    if(MaterialEnabled==1){
        gAlbedoSpec.rgb = texture(albedoMap,TexCoords).rgb*MaterialColour;
   // // store specular intensity in gAlbedoSpec's alpha component
        gAlbedoSpec.a = texture(metallicMap,TexCoords).r*Materialmetallic;
    }else{
        gAlbedoSpec = texture(DiffuseShader,TexCoords);
    }
}