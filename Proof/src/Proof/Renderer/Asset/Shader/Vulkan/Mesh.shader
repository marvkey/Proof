#Vertex Shader

#version 450

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

layout(location = 0) out vec4 outColor;

//push constants block
layout(push_constant) uniform constants
{
	vec4 color;
	mat4 render;
} PushConstants;

layout(set =0,binding = 0) uniform CameraData
{
	mat4 ProjectionMatrix;
	mat4 ViewMatrix;
	vec3 Position;
}CameraUBO;
void main() {
	outColor = vec4(aNormal,1.0);
	gl_Position = CameraUBO.ProjectionMatrix* CameraUBO.ViewMatrix * vec4(aPosition, 1.0f);
}

#Fragment Shader

#version 450

layout(location = 0) in vec4 inColor;

layout(location = 0) out vec4 outFragColor;

layout(push_constant) uniform constants
{
	vec4 color;
	mat4 render;
} PushConstants;
void main() {
	outFragColor = inColor;
}