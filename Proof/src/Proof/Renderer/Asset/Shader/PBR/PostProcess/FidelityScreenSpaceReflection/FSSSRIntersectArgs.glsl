#Compute Shader

#version 450
#extension GL_GOOGLE_include_directive : enable
#extension GL_KHR_shader_subgroup_basic : enable
#extension GL_KHR_shader_subgroup_ballot : enable
#include <PBR/PostProcess/FidelityScreenSpaceReflection/FSSSRCommon.glslh>

layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main()
{
	  { // Prepare intersection args
        uint ray_count = GetRayCounter(0);

        WriteIntersectIndirectArgs(0, (ray_count + 63) / 64);
        WriteIntersectIndirectArgs(1, 1);
        WriteIntersectIndirectArgs(2, 1);

        WriteRayCounter(0, 0);
        WriteRayCounter(1, ray_count);
    }
    { // Prepare denoiser args
        uint tile_count = GetRayCounter(2);

        WriteIntersectIndirectArgs(3, tile_count);
        WriteIntersectIndirectArgs(4, 1);
        WriteIntersectIndirectArgs(5, 1);

        WriteRayCounter(2, 0);
        WriteRayCounter(3, tile_count);
    }

}