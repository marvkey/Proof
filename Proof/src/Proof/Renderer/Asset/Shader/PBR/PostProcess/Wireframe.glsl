#Vertex Shader
#version 450

#include <Common.glslh>

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;
layout(location = 5) in mat4 aTransform;
void main() {

   
    vec4 worldPos = aTransform * vec4(aPosition, 1.0);
    gl_Position =  u_Camera.Projection * u_Camera.View * vec4(worldPos.xyz, 1.0);
}

#Fragment Shader
#version 450 core
layout(location = 0) out vec4 out_FragColor;

layout (push_constant) uniform Material
{
        vec4 Color;
} u_MaterialUniform;


void main()
{
    out_FragColor = u_MaterialUniform.Color;
   
}   
