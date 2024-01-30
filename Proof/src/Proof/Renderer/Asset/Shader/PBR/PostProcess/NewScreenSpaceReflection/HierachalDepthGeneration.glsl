//https://github.com/GPUOpen-LibrariesAndSDKs/FidelityFX-SDK/blob/main/sdk/src/backends/vk/shaders/sssr/ffx_sssr_depth_downsample_pass.glsl
//https://github.com/GPUOpen-LibrariesAndSDKs/FidelityFX-SDK/blob/main/sdk/include/FidelityFX/gpu/sssr/ffx_sssr_depth_downsample.h
//https://github.com/GPUOpen-LibrariesAndSDKs/FidelityFX-SDK/blob/main/sdk/include/FidelityFX/gpu/sssr/ffx_sssr_callbacks_glsl.h#L631

layout(binding = 0, r32f) uniform image2D o_DepthHierarchy[8];//13
layout(binding = 1) uniform sampler2D u_DepthMap;

float GetInputDepth(uvec2 coordinate)
{
    return texelFetch(u_DepthMap, ivec2(coordinate), 0).r;
}

void WriteDepthHierarchy(uint index, uvec2 coordinate, float data)
{
    imageStore(o_DepthHierarchy[index], ivec2(coordinate), vec4(data));
}

void GetDepthHierarchyMipDimensions(uint mip, out vec2 imagesize)
{
    imagesize = imageSize(o_DepthHierarchy[mip]);
}

float GetMipsCount(vec2 texture_size)
{
    float max_dim = max(texture_size.x, texture_size.y);
    return 1.0 + floor(log2(max_dim));
}

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

}
layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;
void main()
{
    DepthDownsample(gl_LocalInvocationIndex, gl_WorkGroupID, gl_GlobalInvocationID);
}