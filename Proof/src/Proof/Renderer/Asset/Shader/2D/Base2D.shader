#Vertex Shader
#version 450 core
layout(location = 0) in mat4 aTransfomr;
layout(location = 4) in vec3 aPos;
layout(location = 5) in vec4 aColor;
layout(location = 6) in vec2 aTexCoord;
layout(location = 7) in float aTextureIndex;

layout(location = 0) out vec4 outColor;
layout(location = 1) out vec2 outTexCoords;
layout(location = 2) out float outTextureIndex;
layout(set = 0, binding = 0) uniform CameraData
{
	mat4 ProjectionMatrix;
	mat4 ViewMatrix;
	vec3 Position;

}Camera;
void main() {
	outColor = aColor;
	outTexCoords = aTexCoord;
	outTextureIndex = aTextureIndex;
	gl_Position = Camera.ProjectionMatrix * Camera.ViewMatrix * vec4(aPos, 1.0);
}

#Fragment Shader
#version 450 core
layout(location = 0) out vec4 outFragColor;

layout(location = 0) in vec4 inColor;
layout(location = 1) in vec2 inTexCoord;
layout(location = 2) in float  inTextureIndex;
layout(set = 0, binding = 1) uniform sampler2D u_Textures[32];

void main() {
	vec4  texColor = texture(u_Textures[int(inTextureIndex)], inTexCoord) * inColor;
//	vec4  texColor =  inColor;
	//(before item becomes black)
	if (texColor.a < 0.1)
		discard;
	outFragColor = vec4(texColor.rgb, texColor.a);
}