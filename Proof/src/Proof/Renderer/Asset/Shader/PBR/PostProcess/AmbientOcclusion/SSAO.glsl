//https://github.com/mateeeeeee/Adria-DX12/blob/master/Adria/Resources/NewShaders/Postprocess/SSAO.hlsl
//https://john-chapman-graphics.blogspot.com/2013/01/ssao-tutorial.html
//https://learnopengl.com/code_viewer_gh.php?code=src/5.advanced_lighting/9.ssao/9.ssao_lighting.fs
//https://learnopengl.com/code_viewer_gh.php?code=src/5.advanced_lighting/9.ssao/9.ssao.fs
#Compute Shader
#version 450 core

#include <Common.glslh>

layout (push_constant) uniform Uniforms
{
	float Radius;
	float Bias;
	uint KernelSize;
	uint NoiseScale;
} u_Uniforms;
#define BLOCK_SIZE 16

// Unroll all loops for performance - this is important
//#pragma optionNV(unroll all)

layout(binding = 0) uniform sampler2D u_DepthMap;
layout(binding = 1) uniform sampler2D u_ViewNormalMap;
layout(binding = 2) uniform sampler2D u_NoiseImage;
layout(set = 0, binding = 3) buffer KernalBuffer 
{
    vec3 Samples[];
} u_Kernal;
layout(binding = 4, r8) uniform image2D o_SSAOImage;

layout(local_size_x = BLOCK_SIZE, local_size_y = BLOCK_SIZE, local_size_z = 1) in;
void main() 
{
    uvec2 pixelCoords = uvec2(gl_GlobalInvocationID.xy);
    vec2 texCoords = vec2(pixelCoords) / vec2(imageSize(o_SSAOImage));

    uvec2 resolution = uvec2(u_ScreenData.FullResolution.x /u_Uniforms.NoiseScale ,  u_ScreenData.FullResolution.y/u_Uniforms.NoiseScale);
	vec2 uv = (vec2(gl_GlobalInvocationID.xy) + 0.5) * 1.0 / vec2(resolution);
	vec3 viewNormal = texture(u_ViewNormalMap, uv).rgb * 2.0 - 1.0;
	viewNormal = normalize(viewNormal);

	float depth = texture(u_DepthMap, uv).r;
	vec3 viewPosition = GetViewPosition(uv, depth);
	vec3 randomVector = normalize(2.0 * texture(u_NoiseImage, uv * u_Uniforms.NoiseScale).xyz - 1.0);

	vec3 tangent = normalize(randomVector - viewNormal * dot(randomVector, viewNormal));
	vec3 bitangent = cross(viewNormal, tangent);
	mat3 TBN = mat3(tangent, bitangent, viewNormal);

	float occlusion = 0.0;
	#pragma unroll u_Uniforms.KernelSize

	for (int i = 0; i < u_Uniforms.KernelSize; i++)
	{
		vec3 sampleDir = u_Kernal.Samples[i].xyz * TBN;
		vec3 samplePos = viewPosition + sampleDir * u_Uniforms.Radius;
		vec4 offset = vec4(samplePos, 1.0);
		offset = u_Camera.Projection * offset;
		offset.xy = ((offset.xy / offset.w) * vec2(1.0f, -1.0f)) * 0.5f + 0.5f;
		float sampleDepth = texture(u_DepthMap, offset.xy).r;
		sampleDepth = GetViewPosition(offset.xy, sampleDepth).z;
		float rangeCheck = smoothstep(0.0, 1.0, u_Uniforms.Radius / abs(viewPosition.z - sampleDepth));
		//occlusion += rangeCheck * step(sampleDepth, samplePos.z - 0.01);
		occlusion += (sampleDepth >= samplePos.z + u_Uniforms.Bias? 1.0 : 0.0) * rangeCheck;           

	}
	float ssaoPower  = 2;
	occlusion = 1.0 - (occlusion / float(u_Uniforms.KernelSize));
	float AO =pow(abs(occlusion), ssaoPower);
	//o_SSAOImage[gl_GlobalInvocationID.xy] = pow(abs(occlusion), ssaoPower);

	//imageStore(o_SSAOImage, ivec3(gl_GlobalInvocationID),uvec4(AO,viewPosition.z,0,1.0) );
	imageStore(o_SSAOImage, ivec2(gl_GlobalInvocationID.xy),uvec4(AO,0,0,1.0) );

}