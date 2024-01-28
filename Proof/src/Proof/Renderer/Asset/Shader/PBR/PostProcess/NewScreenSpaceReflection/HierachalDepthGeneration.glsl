//https://github.com/GPUOpen-LibrariesAndSDKs/FidelityFX-SDK/blob/main/sdk/src/backends/vk/shaders/sssr/ffx_sssr_depth_downsample_pass.glsl
//https://github.com/GPUOpen-LibrariesAndSDKs/FidelityFX-SDK/blob/main/sdk/include/FidelityFX/gpu/sssr/ffx_sssr_depth_downsample.h
layout(binding = 0) uniform sampler2D u_DepthMap;

void DepthDownsample(uint group_index, uvec3 group_id, uvec3 dispatch_thread_id)
{
	vec2 depthImageSize = textureSize(u_DepthMap);

}
layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;
void main()
{
    DepthDownsample(gl_LocalInvocationIndex, gl_WorkGroupID, gl_GlobalInvocationID);
}