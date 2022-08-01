#Vertex Shader

#version 450

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

layout(location = 0) out vec3 outColor;

//push constants block
layout(push_constant) uniform constants
{
	vec4 data;
	mat4 render_matrix;
} PushConstants;

void main() {
	gl_Position = PushConstants.render_matrix * vec4(aPosition, 1.0f);
	outColor = aNormal;
}

#Fragment Shader

#version 450

layout(location = 0) in vec3 inColor;

layout(location = 0) out vec4 outFragColor;

void main() {
	outFragColor = vec4(0.5,0.2,0.9, 1.0f);
}