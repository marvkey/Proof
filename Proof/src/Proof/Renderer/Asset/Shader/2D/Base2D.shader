#Vertex Shader
#version 450 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3)in float aTexIndex;

layout(location = 0) out vec4 outColor;
layout(location = 1) out vec2 outTexCoords;
layout(location = 2) out float outTexIndex;
layout(set = 0, binding = 0) uniform CameraData
{
	mat4 ProjectionMatrix;
	mat4 ViewMatrix;
	vec3 Position;

}Camera;
void main() {
	outColor = aColor;
	outTexCoords = aTexCoord;
	outTexIndex = aTexIndex;
	gl_Position = Camera.ProjectionMatrix * Camera.ViewMatrix * vec4(aPos, 1.0);
}

#Fragment Shader
#version 450 core
out vec4 FragColor;
layout(location = 0) in vec4 inColor;
layout(location = 1) in vec2 inTexCoord;
layout(location = 2) in float inTexIndex;
layout(set = 1, binding = 0) uniform texture2D textures[32];
void main() {
	vec4 temp = texture(u_TextureSlot[int(inTexIndex)], inTexCoord) * vec4(inColor);
	//if (temp.a < 0.01)
		//discard;
	FragColor = temp;
}