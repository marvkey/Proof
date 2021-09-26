#Vertex Shader
#version 450 core

layout(location = 0) in vec3 aPos;

layout(std140,binding = 1) uniform CameraData
{
    mat4 ProjectionMatrix;
    mat4 ViewMatrix;
    vec3 Position;
}Camera;

out vec3 WorldPos;

void main() {
	WorldPos = aPos;

	mat4 rotView = mat4(mat3(Camera.ViewMatrix));
	vec4 clipPos = Camera.ProjectionMatrix * rotView * vec4(WorldPos,1.0);

	gl_Position = clipPos.xyww;
}
#Fragment Shader
#version 450 core

out vec4 FragColor;
in vec3 WorldPos;

uniform samplerCube environmentMap;

void main() {
    vec3 envColor = texture(environmentMap,WorldPos).rgb;

    // HDR tonemap and gamma correct
    envColor = envColor / (envColor + vec3(1.0));
    envColor = pow(envColor,vec3(1.0 / 2.2));

    FragColor = vec4(envColor,1.0);
}