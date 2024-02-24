#Compute Shader

#version 450

#define BLUE_NOISE_BUFFER_BEGIN_BINDING 0
#include <Extras/BlueNoiseInclude.glslh>
#include <Common.glslh>
layout(set = 0, binding = 0,rg32f) uniform image2D o_BlueNoiseTexture;

 

vec2 SampleRandomVector2D(uvec2 pixel) 
{
    vec2 u = vec2(
        mod(SampleRandomNumber(pixel.x, pixel.y, 0, 0u) + (u_FrameData.FrameCount & 0xFFu) * GOLDEN_RATIO, 1.0f),
        mod(SampleRandomNumber(pixel.x, pixel.y, 0, 1u) + (u_FrameData.FrameCount & 0xFFu) * GOLDEN_RATIO, 1.0f));
    return u;
}
void StoreBlueNoiseSample(uvec2 coordinate, vec2 blue_noise_sample) 
{
    imageStore(o_BlueNoiseTexture, ivec2(coordinate), vec4(blue_noise_sample, 0.0, 0.0));
}

void PrepareBlueNoiseTexture(uvec2 dispatch_thread_id) 
{
    StoreBlueNoiseSample(dispatch_thread_id, SampleRandomVector2D(dispatch_thread_id));
}

layout (local_size_x = 8,local_size_y =8, local_size_z = 1) in;

void main() 
{
    PrepareBlueNoiseTexture(gl_GlobalInvocationID.xy);
}