#Vertex Shader

#version 450
layout(location = 0) in vec3 aPos;
//layout(location = 1) in vec3 aNormal;
//layout(location = 2) in vec2 aTexCoords;
//layout(location = 3) in vec3 aTangent;
//layout(location = 4) in vec3 aBitangent;
layout(location = 5) in mat4 aTransform;
layout(location = 0) out vec3 outWorldPos;
layout(set = 0, binding = 0) uniform CameraData
{
    mat4 Projection;
    mat4 View;
    vec3 Position;
}CameraUBO;

void main()
{
    outWorldPos = aPos;

    mat4 rotView = mat4(mat3(CameraUBO.View));
    vec4 clipPos = CameraUBO.Projection * rotView * vec4(outWorldPos, 1.0);
    gl_Position = clipPos.xyww;
}

#Fragment Shader
#version 450


layout(set = 0, binding = 1) uniform  samplerCube environmentMap;
layout(location = 0) in vec3 WorldPos;      
layout(location = 0) out vec4 outFragColor;
 
void main()
{

    vec3 envColor = textureLod(environmentMap, WorldPos,11).rgb;

    // HDR tonemap and gamma correct
    envColor = envColor / (envColor + vec3(1.0));
    envColor = pow(envColor, vec3(1.0 / 2.2));
    outFragColor = vec4(envColor,  1.0);
}

