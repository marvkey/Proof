
#Vertex Shader

#version 450
#extension GL_ARB_separate_shader_objects : enable
#include <Common.glslh>

layout(set = 0, binding = 0) uniform  ModelBufferObject { mat4 model; };

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

void main()
{
  gl_Position = u_Camera.Projection * u_Camera.View * model * vec4(aPosition, 1.0);
}


#Fragment Shader

#version 450
layout(location = 0) out vec4 outColor;
void main() {
  outColor = vec4(1.0, 0.2, 0.2, 1.0); // bright red
}