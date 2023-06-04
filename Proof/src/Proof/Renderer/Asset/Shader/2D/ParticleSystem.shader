#Vertex Shader
#version 450 core
layout(location = 4) in vec3 aPos;
layout(location = 5) in vec4 aColor;
layout(location = 6) in vec2 aTexCoord;
layout(location = 7) in float aTextureIndex;
layout(location = 0) in mat4 aModelMatrix;

layout(location = 0) out vec4 outColor;
layout(location = 1) out vec2 outTexCoords;
//layout(location = 2) out vec3 outPos; // not needed but if do
layout(location = 2) out float outTextureIndex;
layout(set = 0, binding = 0) uniform CameraData
{
	mat4 ProjectionMatrix;
	mat4 ViewMatrix;
	vec3 Position;

}Camera;

//(From chatgpt)
void main() {
	outColor = aColor;
	outTexCoords = aTexCoord;
	outTextureIndex = aTextureIndex;

 // Calculate the particle's position in world space
    vec4 worldPosition = aModelMatrix * vec4(aPos, 1.0);

    // Calculate the camera's position in world space
    vec3 cameraPosition = Camera.Position;

    // Calculate the vector from the particle to the camera
    vec3 toCamera = normalize(cameraPosition - worldPosition.xyz);

    // Calculate the right and up vectors for the billboard orientation
    vec3 right = vec3(aModelMatrix[0][0], aModelMatrix[1][0], aModelMatrix[2][0]);
    vec3 up = vec3(aModelMatrix[0][1], aModelMatrix[1][1], aModelMatrix[2][1]);

    // Calculate the billboard orientation using the right and up vectors
    vec3 billboardRight = normalize(cross(up, toCamera));
    vec3 billboardUp = normalize(cross(toCamera, billboardRight));

    // Transform the position by the billboard orientation and projection matrix
    vec3 billboardPosition = worldPosition.xyz + billboardRight * aPos.x + billboardUp * aPos.y;
    //gl_Position = Camera.ProjectionMatrix * Camera.ViewMatrix * vec4(billboardPosition, 1.0);


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
	//(before item becomes black)
	if (texColor.a < 0.1)
		discard;
	outFragColor = vec4(texColor.rgb, texColor.a);
}