#version 450 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;
layout(location = 5) in mat4 aTransform;
layout(location = 9) in mat4 aPrevTransform;

layout(set = 0, binding = 0) uniform CameraData
{
    mat4 Projection;
    mat4 View;
    vec3 Position;
} u_Camera;

void main() 
{
    outColor = aColor;
    outTexCoords = aTexCoord;
    outTextureIndex = aTextureIndex;
    gl_Position = u_Camera.Projection * u_Camera.View * vec4(aPos, 1.0);
}