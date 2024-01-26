
//GPU Pro 5- Advanced Rendering Techniques CHAPTER 4 preintegration page 173
//https://github.com/InCloudsBelly/X2_RenderingEngine/blob/e7c349b70bd95af3ab673556cdb56cb2cc40b48e/Resources/Shaders/Pre-Integration.glsl
#Compute Shader
#version 450 core

layout(push_constant) uniform Info
{
	vec2 HZBInvRes;
	vec2 InvRes;
	vec2 ProjectionParams; //(x) = Near plane, (y) = Far plane // Reversed
    int PrevMip;
} u_Info;

layout(binding = 0, r8)  writeonly uniform image2D o_VisibilityImage;
layout(binding = 1) uniform sampler2D u_VisibilityTex;
layout(binding = 2) uniform sampler2D u_HZB;

float LinearizeDepth(float d)
{
	return u_Info.ProjectionParams.x * u_Info.ProjectionParams.y / (u_Info.ProjectionParams.y + d * (u_Info.ProjectionParams.x - u_Info.ProjectionParams.y));
}

layout(local_size_x = 8, local_size_y = 8) in;
void main()
{
    ivec2 base = ivec2(gl_GlobalInvocationID);
	vec2 hzbUV = u_Info.HZBInvRes * base;
	vec2 uv = u_Info.InvRes * base;
    
	vec4 fineZ;
	fineZ.x = LinearizeDepth(textureLod(u_HZB, hzbUV + u_Info.HZBInvRes * vec2(-0.5, -0.5), u_Info.PrevMip).x);
	fineZ.y = LinearizeDepth(textureLod(u_HZB, hzbUV + u_Info.HZBInvRes * vec2(-0.5,  0.0), u_Info.PrevMip).x);
	fineZ.z = LinearizeDepth(textureLod(u_HZB, hzbUV + u_Info.HZBInvRes * vec2( 0.0, -0.5), u_Info.PrevMip).x);
	fineZ.w = LinearizeDepth(textureLod(u_HZB, hzbUV + u_Info.HZBInvRes * vec2( 0.5,  0.5), u_Info.PrevMip).x);

	/* Fetch fine visibility from previous visibility map LOD */
	vec4 visibility;
	visibility.x = textureLod(u_VisibilityTex, uv + u_Info.InvRes * vec2(-0.5, -0.5), u_Info.PrevMip).r;
    visibility.y = textureLod(u_VisibilityTex, uv + u_Info.InvRes * vec2(-0.5,  0.0), u_Info.PrevMip).r;
    visibility.z = textureLod(u_VisibilityTex, uv + u_Info.InvRes * vec2( 0.0, -0.5), u_Info.PrevMip).r;
    visibility.w = textureLod(u_VisibilityTex, uv + u_Info.InvRes * vec2( 0.5,  0.5), u_Info.PrevMip).r;

	/* Integrate visibility */
	float maxZ = max(max(fineZ.x, fineZ.y), max(fineZ.z, fineZ.w));
	vec4 integration = (fineZ / maxZ) * visibility;

	/* Compute coarse visibility (with SIMD 'dot' intrinsic) */
	float coarseVisibility = dot(vec4(0.25), integration);

	imageStore(o_VisibilityImage, base, coarseVisibility.xxxx);
}