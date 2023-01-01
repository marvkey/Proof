#Vertex Shader

#version 450

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;
layout(location = 5) in mat4 aTransform;
layout(location = 9) in vec3 aMaterialColor;

layout(location = 0) out vec4 outColor;
layout(location = 1) out vec2 outTexCoords;
layout(location = 2) out vec3 outmaterialColor;


layout(set =0,binding = 0) uniform CameraData
{
	mat4 ProjectionMatrix;
	mat4 ViewMatrix;
	vec3 Position;
}CameraUBO;
void main() {
	outmaterialColor = aMaterialColor;
	outTexCoords = aTexCoords;
	outColor = vec4(aNormal,1.0);
	gl_Position = CameraUBO.ProjectionMatrix * CameraUBO.ViewMatrix * aTransform * vec4(aPosition, 1.0);

}

#Fragment Shader

#version 450

layout(location = 0) in vec4 inColor;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 materialColor;

layout(location = 0) out vec4 outFragColor;
layout(set = 1, binding = 0) uniform sampler2D tex1;
void main() {
	vec3 color = texture(tex1, texCoord).xyz;
	outFragColor = vec4(color,1.0)* vec4(materialColor,1.0);
}