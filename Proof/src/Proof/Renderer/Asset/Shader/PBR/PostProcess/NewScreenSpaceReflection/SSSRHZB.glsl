//https://github.com/GPUOpen-LibrariesAndSDKs/FidelityFX-SDK/blob/main/sdk/src/backends/vk/shaders/sssr/ffx_sssr_depth_downsample_pass.glsl
//https://github.com/GPUOpen-LibrariesAndSDKs/FidelityFX-SDK/blob/main/sdk/include/FidelityFX/gpu/sssr/ffx_sssr_depth_downsample.h
//https://github.com/GPUOpen-LibrariesAndSDKs/FidelityFX-SDK/blob/main/sdk/include/FidelityFX/gpu/sssr/ffx_sssr_callbacks_glsl.h#L631

#Compute Shader
#version 460

#extension GL_GOOGLE_include_directive : enable
#extension GL_KHR_shader_subgroup_basic : enable
#extension GL_KHR_shader_subgroup_ballot : enable
#include <PBR/PostProcess/NewScreenSpaceReflection/SSSRCommon.glslh>

layout(binding = 0, r32f) uniform image2D o_DepthHierarchy[13];//13
  //layout (set = 0, binding = 1, r32f)     uniform image2D rw_depth_hierarchy[13];

#define FFX_GPU
#define FFX_GLSL
#include <ffx_core.h>

FFX_GROUPSHARED FfxUInt32 spdCounter;
FFX_GROUPSHARED FfxFloat32 spdIntermediate[16][16];

// Define fetch and store functions
FfxUInt32 SpdGetAtomicCounter() {
	return spdCounter;
}
#define FFX_SSSR_OPTION_INVERTED_DEPTH 1


void SpdStoreIntermediate(FfxUInt32 x, FfxUInt32 y, FfxFloat32x4 value) {
	spdIntermediate[x][y] = value.x;
}

FfxFloat32x4 SpdLoadIntermediate(FfxUInt32 x, FfxUInt32 y) {
	FfxFloat32 f = spdIntermediate[x][y];
	return FfxFloat32x4(f.x, f.x, f.x, f.x);
}

FfxFloat32x4 SpdReduce4(FfxFloat32x4 v0, FfxFloat32x4 v1, FfxFloat32x4 v2, FfxFloat32x4 v3) {
#if FFX_SSSR_OPTION_INVERTED_DEPTH
    return max(max(v0, v1), max(v2, v3));
#else
    return min(min(v0, v1), min(v2, v3));
#endif
}

void SpdIncreaseAtomicCounter(FfxUInt32 slice)
{
	SPDIncreaseAtomicCounter(spdCounter);
}
FfxFloat32x4 SpdLoad(FfxInt32x2 coordinate, FfxUInt32 slice)
{
    return FfxFloat32x4(imageLoad(o_DepthHierarchy[6], coordinate).x);   // 5 -> 6 as we store a copy of the depth buffer at index 0
}

void SpdStore(FfxInt32x2 pix, FfxFloat32x4 outValue, FfxUInt32 coordinate, FfxUInt32 slice)
{
    imageStore(o_DepthHierarchy[coordinate + 1], pix, FfxFloat32x4(outValue.x));    // + 1 as we store a copy of the depth buffer at index 0
}


float GetInputDepth(uvec2 coordinate)
{
    return texelFetch(u_DepthMap, ivec2(coordinate), 0).r;
}
FfxFloat32x4 SpdLoadSourceImage(FfxInt32x2 coordinate, FfxUInt32 slice)
{
    return GetInputDepth(coordinate).xxxx;
}
void WriteDepthHierarchy(uint index, uvec2 coordinate, float data)
{
    imageStore(o_DepthHierarchy[index], ivec2(coordinate), vec4(data));
}

void GetDepthHierarchyMipDimensions(uint mip, out vec2 imagesize)
{
    imagesize = imageSize(o_DepthHierarchy[mip]);
}

#include <spd/ffx_spd.h>
float GetMipsCount(vec2 texture_size)
{
    float max_dim = max(texture_size.x, texture_size.y);
    return 1.0 + floor(log2(max_dim));
}

//float GetMipLevelCount(vec2 texture_size) {
//    return float(floor(log2(float(max(texture_size.x, texture_size.y))))) + 1;
//}

uint GetThreadgroupCount(uvec2 image_size)
{
	// Each threadgroup works on 64x64 texels
	return ((image_size.x + uint(63)) / uint(64)) * ((image_size.y + uint(63)) / uint(64));
}

void DepthDownsample(uint group_index, uvec3 group_id, uvec3 dispatch_thread_id)
{
	vec2 depthImageSize = textureSize(u_DepthMap,0);
	
	uvec2 uDepthImageSize = uvec2(depthImageSize);

	for(int i =0; i < 2; i++)
	{
		//going through 8 mips
		for (int j =0; j<8; ++j)
		{
			uvec2 idx = uvec2(uint(2) * dispatch_thread_id.x + uint(i), uint(8) * dispatch_thread_id.y + uint(j));
			if (idx.x < uDepthImageSize.x && idx.y < uDepthImageSize.y)
			{
				WriteDepthHierarchy(uint(0), idx,GetInputDepth(idx));
			}
		}
	}

	vec2 mipImageSize = vec2(0.0f,0.0f);
	GetDepthHierarchyMipDimensions(uint(0),mipImageSize);
	float mips_count = GetMipsCount(mipImageSize);
	uint threadgroup_count = GetThreadgroupCount(uvec2(mipImageSize));

	SpdDownsample(
		FfxUInt32x2(group_id.xy),
		FfxUInt32(group_index),
		FfxUInt32(mips_count),
		FfxUInt32(threadgroup_count),
		0);
}
layout(local_size_x = 32, local_size_y = 8, local_size_z = 1) in;
void main()
{
    DepthDownsample(gl_LocalInvocationIndex, gl_WorkGroupID, gl_GlobalInvocationID);
}