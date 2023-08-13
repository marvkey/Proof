#Vertex Shader
#version 450
layout(push_constant) uniform PushConsts {
	uint CascadeIndex;
} u_CascadeInfo;

layout (location = 0) out vec2 outUV;
layout (location = 1) out uint outCascadeIndex;

out gl_PerVertex {
	vec4 gl_Position;   
};

void main() 
{
	outUV = vec2((gl_VertexIndex << 1) & 2, gl_VertexIndex & 2);
	outCascadeIndex = u_CascadeInfo.CascadeIndex;
	gl_Position = vec4(outUV * 2.0f - 1.0f, 0.0f, 1.0f);
}

#Fragment Shader
#version 450

layout (binding = 1) uniform sampler2DArray u_ShadowMap;

layout (location = 0) in vec2 inUV;
layout (location = 1) flat in uint inCascadeIndex;

layout (location = 0) out vec4 outFragColor;
float near_plane =-50;
float far_plane =50;
float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));	
}
void main() 
{
	float depth = texture(u_ShadowMap, vec3(inUV, float(inCascadeIndex))).r;
	//outFragColor = vec4(vec3(LinearizeDepth(depth) / far_plane), 1.0); // perspective
	outFragColor = vec4(vec3((depth)), 1.0);//orthographic
	//float threshold = 0.3; // Adjust threshold value
	//outFragColor = vec4(vec3(depth < threshold ? 0.0 : 1.0), 1.0);
}