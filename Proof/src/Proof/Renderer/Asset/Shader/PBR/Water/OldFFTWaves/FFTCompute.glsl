
#Compute Shader
#version 450 core
#include <Common.glslh>
#include <PBR/Water/FFTWaves/FFTWaterCompute.glslh>

#define MODE_HORIZONTAL_FFT 0
#define MODE_VERTICAL_FFT 1
layout(push_constant) uniform PushData
{
     int Mode;
}u_Uniforms;
#define SIZE 1024
#define LOG_SIZE 10


layout(binding = 0, rgba16f) uniform image2DArray u_FourierTarget;

shared vec4 FftGroupBuffer[2][SIZE];

void ButterflyValues(uint stepp, uint index, out uvec2 indices, out vec2 twiddle) 
{
    uint b = SIZE >> (stepp + 1u);
    uint w = b * (index / b);
    uint i = (w + index) % SIZE;
    float sinVal, cosVal;
    SinCos(-TWO_PI / SIZE * float(w), sinVal, cosVal);
    twiddle = vec2(cosVal, sinVal);

    // This is what makes it the inverse FFT
    twiddle.y = -twiddle.y;
    indices = uvec2(i, i + b);
}



vec4 FFT(uint threadIndex, vec4 fftInput) 
{
    FftGroupBuffer[0u][threadIndex] = fftInput;
    groupMemoryBarrier();
    barrier();
    bool flag = false;

    for (uint stepIndex = 0; stepIndex < LOG_SIZE; ++stepIndex) 
    {
        uvec2 inputsIndices;
        vec2 twiddle;
        ButterflyValues(stepIndex, threadIndex, inputsIndices, twiddle);

        vec4 v = FftGroupBuffer[flag ? 1 : 0][inputsIndices.y];
        FftGroupBuffer[flag ? 0 : 1][threadIndex] = FftGroupBuffer[flag ? 1 : 0][inputsIndices.x] 
            + vec4(ComplexMult(twiddle, v.xy), ComplexMult(twiddle, v.zw));

        flag = !flag;
        groupMemoryBarrier();
        barrier();
    }

    return FftGroupBuffer[flag ? 1 : 0][threadIndex];
}

void HorizontalFFT() 
{
    uvec3 id = gl_GlobalInvocationID;
    for (int i = 0; i < 8; ++i) 
    {
        vec4 inputt = imageLoad(u_FourierTarget, ivec3(id.xy, i));
        vec4 result = FFT(id.x, inputt);
        imageStore(u_FourierTarget, ivec3(id.xy, i), result);
    }
}

void VerticalFFT() 
{
    uvec3 id = gl_GlobalInvocationID;
    for (int i = 0; i < 8; ++i) 
    {
        vec4 inputt = imageLoad(u_FourierTarget, ivec3(id.yx, i));
        vec4 result = FFT(id.x, inputt);
        imageStore(u_FourierTarget, ivec3(id.yx, i), result);
    }
}

layout (local_size_x = 8, local_size_y = 8, local_size_z = 1) in;
void main()
{
    if (u_Uniforms.Mode == MODE_HORIZONTAL_FFT)
    {
        HorizontalFFT();
    } 
    else if (u_Uniforms.Mode == MODE_VERTICAL_FFT)
    {
        VerticalFFT();
    }
}