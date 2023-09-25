// Basic Texture Shader
#Vertex Shader

#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;



layout(set = 0, binding = 0) uniform CameraData
{
	mat4 Projection;
	mat4 View;
	vec3 Position;
}u_Camera;

layout (location = 0) out vec4 v_Color;

void main()
{
	v_Color = a_Color;
	gl_Position =u_Camera.Projection * u_Camera.View * vec4(a_Position, 1.0);
}

#Fragment Shader

#version 450 core

layout(location = 0) out vec4 color;

layout (location = 0) in vec4 v_Color;

void main()
{
	color = v_Color;
}