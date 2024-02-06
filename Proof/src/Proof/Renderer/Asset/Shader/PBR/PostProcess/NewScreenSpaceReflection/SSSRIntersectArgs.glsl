//https://github.com/qiutang98/flower/blob/main/source/shader/sssr/sssr_intersect_args.glsl
#Compute Shader

#version 450
#extension GL_GOOGLE_include_directive : enable
#extension GL_KHR_shader_subgroup_basic : enable
#extension GL_KHR_shader_subgroup_ballot : enable
#include <PBR/PostProcess/NewScreenSpaceReflection/SSSRCommon.glslh>

layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

void main()
{
	 // Prepare intersection args.
    uint rayCount = s_RayCounter.RayCount; // Ray count.

    u_IntersectCommand.args.x = (rayCount + 63) / 64;
    u_IntersectCommand.args.y = 1;
    u_IntersectCommand.args.z = 1;

    // Prepare denoise args.
    uint tileCount = s_RayCounter.DenoiseTileCount; // 8x8 tile count.
    
    u_IntersectCommand.args.x = tileCount;
    u_IntersectCommand.args.y = 1;
    u_IntersectCommand.args.z = 1;
}