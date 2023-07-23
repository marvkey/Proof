#Vertex Shader

#version 450
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;
layout(location = 5) in mat4 aTransform;

layout(location = 0) out vec3 outWorldPos;
layout(location = 1) out vec2 outTexCoords;
layout(location = 2) out vec3 outNormal;
layout(location = 3) out vec3 outTangent;
layout(location = 4) out vec3 outBitangent;
layout(location = 5) out vec3 outCameraPostion;
layout(set = 1, binding = 0) uniform CameraData
{
    mat4 ProjectionMatrix;
    mat4 ViewMatrix;
    vec3 Position;
}u_Camera;
void main() {
    outTangent = aTangent;
    outBitangent = aBitangent;
    outTexCoords = aTexCoords;
    outWorldPos = vec3(aTransform * vec4(aPosition, 1.0));
    outNormal = mat3(aTransform) * aNormal;
    outCameraPostion = u_Camera.Position;
    gl_Position = u_Camera.ProjectionMatrix * u_Camera.ViewMatrix * vec4(outWorldPos, 1.0);
}

#Fragment Shader
#version 450 core

layout(location = 0) in vec3 in_WorldPos;
layout(location = 1) in vec2 in_TexCoords;
layout(location = 2) in vec3 in_Normal;
layout(location = 3) in vec3 in_CameraPosition;

layout(location = 0) out vec4 out_FragColor;

layout(set = 0, binding = 5) uniform sampler2D u_AlbedoMap;
layout(set = 0, binding = 6) uniform sampler2D u_NormalMap;
layout(set = 0, binding = 7) uniform sampler2D u_MetallicMap;
layout(set = 0, binding = 8) uniform sampler2D u_RoughnessMap;

//layout(set = 1, binding = 1) uniform samplerCube u_IrradianceMap;
//layout(set = 1, binding = 2) uniform samplerCube u_PrefilterMap;
//layout(set = 1, binding = 3) uniform sampler2D u_BRDFLUT;

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

void main()
{
    vec3 albedo = texture(u_AlbedoMap, in_TexCoords).rgb * u_MaterialUniform.Albedo;

    out_FragColor = vec4(albedo,1.0);
}
