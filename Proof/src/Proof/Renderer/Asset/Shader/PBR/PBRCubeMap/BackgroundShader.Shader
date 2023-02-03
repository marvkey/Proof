#Vertex Shader

#version 450
layout(location = 0) in vec3 aPos;

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
    gl_Position = CameraUBO.Projection * CameraUBO.View * vec4(outWorldPos, 1.0);
}

#Fragment Shader
#version 450


layout(set = 0, binding = 1) uniform  samplerCube environmentMap;
layout(location = 0) in vec3 WorldPos;
layout(location = 0) out vec4 outFragColor;

void main()
{
    vec3 envColor = textureLod(environmentMap, WorldPos, 0.0).rgb;

    // HDR tonemap and gamma correct
    envColor = envColor / (envColor + vec3(1.0));
    envColor = pow(envColor, vec3(1.0 / 2.2));
    vec3 color = vec3(0.2, 0.5, 0.7);
    outFragColor = vec4(envColor,  1.0);
}

