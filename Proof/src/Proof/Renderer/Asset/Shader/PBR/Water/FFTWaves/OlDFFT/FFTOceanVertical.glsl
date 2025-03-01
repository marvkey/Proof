/*
    Ocean Simulation - Vertical FFT Compute Shader
    ------------------------------------------------
    This compute shader performs the vertical Fast Fourier Transform (FFT) pass
    in the ocean simulation pipeline. It is used to transform frequency-domain
    data into spatial-domain wave heights for realistic ocean rendering.

    This shader executes the FFT using the Cooley-Tukey algorithm with a butterfly
    operation, optimized using push constants for better performance
*/

#Compute Shader
#version 450 core

//https://github.com/achalpandeyy/OceanFFT/blob/master/Source/Shaders/CS_FFTVertical.comp

// Compute Shader for Vertical FFT Pass in Ocean Simulation
// Optimized using Push Constants for better performance.

#define WORK_GROUP_DIM 256  // Workgroup size
#define PI 3.14159265358979323846

layout (local_size_x = WORK_GROUP_DIM) in;

// Input texture (two-component complex values stored as (real, imaginary))
layout (binding = 0, rg32f) uniform readonly image2D u_Input;

// Output texture (two-component complex values stored as (real, imaginary))
layout (binding = 1, rg32f) uniform writeonly image2D u_Output;

// Push constants for dynamic parameters
layout(push_constant) uniform PushConstants {
    int TotalCount;   // Total number of elements in the sequence
    int SubseqCount;  // Number of elements in the subsequence
} u_PC;

// Function to multiply two complex numbers
vec2 MultiplyComplex(vec2 a, vec2 b)
{
	return vec2(a[0] * b[0] - a[1] * b[1], a[1] * b[0] + a[0] * b[1]);
}


// Function to perform the butterfly operation (used in FFT)
vec4 ButterflyOperation(vec2 a, vec2 b, vec2 twiddle) {
    vec2 twiddleB = MultiplyComplex(twiddle, b); // Multiply b by the twiddle factor
    return vec4(a + twiddleB, a - twiddleB);    // Combine results
}

void main()
{
	 // Calculate the current pixel coordinate
    ivec2 pixelCoord = ivec2(gl_WorkGroupID.x, gl_LocalInvocationID.x);

    // Extract push constant values
    int totalCount = u_PC.TotalCount;
    int subseqCount = u_PC.SubseqCount;

    // Calculate thread count and current thread index
    int threadCount = int(totalCount * 0.5f);
    int threadIdx = pixelCoord.y;

    // Calculate input and output indices for the butterfly operation
    int inIdx = threadIdx & (subseqCount - 1); // Bitwise AND to wrap around
    int outIdx = ((threadIdx - inIdx) << 1) + inIdx; // Shift and add to calculate output index

    // Calculate the twiddle factor (complex exponential)
    float angle = -PI * (float(inIdx) / float(subseqCount));
    vec2 twiddle = vec2(cos(angle), sin(angle)); // Euler's formula: e^(iθ) = cos(θ) + i*sin(θ)

    // Load input data (two complex numbers from the input texture)
    vec4 a = imageLoad(u_Input, ivec2(pixelCoord.x, pixelCoord.y));
    vec4 b = imageLoad(u_Input, ivec2(pixelCoord.x, pixelCoord.y + threadCount));

    // Perform the butterfly operation on the two complex sequences
    vec4 result0 = ButterflyOperation(a.xy, b.xy, twiddle); // First pair of complex numbers
    vec4 result1 = ButterflyOperation(a.zw, b.zw, twiddle); // Second pair of complex numbers

    // Store the results back into the output texture
    imageStore(u_Output, ivec2(pixelCoord.x, outIdx), vec4(result0.xy, result1.xy)); // First half
    imageStore(u_Output, ivec2(pixelCoord.x, outIdx + subseqCount), vec4(result0.zw, result1.zw)); // Second half
}