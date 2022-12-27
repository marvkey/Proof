#Vertex Shader

#version 450

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec4 Color;
layout(location = 2) in mat4 aTransform;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform CameraData
{
	mat4 ProjectionMatrix;
	mat4 ViewMatrix;
	vec3 Position;
}CameraUBO;

void main() {
	outColor = color;
	gl_Position = CameraUBO.ProjectionMatrix * CameraUBO.ViewMatrix * aTransform * vec4(aPosition, 1.0);

}

#Fragment Shader

#version 450
layout(location = 0) out vec4 outFragColor;
layout(location = 1) in vec4 outColor;

void main() {
	outFragColor = outColor;
}