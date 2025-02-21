//https://github.com/achalpandeyy/OceanFFT/blob/master/Source/Shaders/CS_FFTHorizontal.comp
/*
    FFTOceanHorizontal.glsl - Vulkan Compute Shader
    
    This compute shader performs the horizontal pass of the Fast Fourier Transform (FFT) for ocean wave simulation.
    It applies a butterfly operation to process the frequency-domain data efficiently in parallel using Vulkan compute shaders.
*/
#Compute Shader
#version 450 core

// Workgroup size definition
#define WORK_GROUP_DIM 256

#define PI 3.14159265358979323846

layout (local_size_x = WORK_GROUP_DIM) in;

// Input and output textures for FFT processing
layout (binding = 0, rg32f) uniform readonly image2D u_Input;   // Frequency domain input
layout (binding = 1, rg32f) uniform writeonly image2D u_Output; // Transformed output

// Push constants to control FFT parameters
layout(push_constant) uniform FFTConstants 
{
    int TotalCount;  // Total number of elements in FFT
    int SubseqCount; // Sub-sequence count for butterfly operation
} u_PC;

// Function to perform complex multiplication
// Complex multiplication follows the rule:
// (a + bi) * (c + di) = (ac - bd) + (ad + bc)i
vec2 MultiplyComplex(vec2 a, vec2 b) 
{
    return vec2(a.x * b.x - a.y * b.y, a.y * b.x + a.x * b.y);
}


// Function to apply the butterfly operation, used in FFT processing
vec4 ButterflyOperation(vec2 a, vec2 b, vec2 twiddle) 
{
    vec2 twiddle_b = MultiplyComplex(twiddle, b);
    return vec4(a + twiddle_b, a - twiddle_b);
}

void main() {
    // Determine the pixel coordinate within the workgroup
    ivec2 pixel_coord = ivec2(gl_LocalInvocationID.x, gl_WorkGroupID.x);

    int thread_count = int(u_PC.TotalCount * 0.5f);
    int thread_idx = pixel_coord.x;

    // Compute input and output indices for the FFT stage
    int in_idx = thread_idx & (u_PC.SubseqCount - 1);
    int out_idx = ((thread_idx - in_idx) << 1) + in_idx;


    // Compute the twiddle factor:
    // Twiddle factor is e^(-i * 2π * k / N) = cos(angle) - i * sin(angle)
    // This rotates the input by a phase shift in frequency space
    float angle = -PI * (float(in_idx) / float(u_PC.SubseqCount));
    vec2 twiddle = vec2(cos(angle), sin(angle));

    // Load two complex numbers from the input texture
    vec4 a = imageLoad(u_Input, pixel_coord);
    vec4 b = imageLoad(u_Input, ivec2(pixel_coord.x + thread_count, pixel_coord.y));

     // Apply the butterfly operation to perform the FFT step
    // Each pair of complex numbers is transformed based on the twiddle factor
    vec4 result0 = ButterflyOperation(a.xy, b.xy, twiddle); // First pair transformation
    vec4 result1 = ButterflyOperation(a.zw, b.zw, twiddle); // Second pair transformation

    // Store results back to the output texture
    // Interleave the results to maintain proper order for the next stage
    imageStore(u_Output, ivec2(out_idx, pixel_coord.y), vec4(result0.xy, result1.xy));
    imageStore(u_Output, ivec2(out_idx + u_PC.SubseqCount, pixel_coord.y), vec4(result0.zw, result1.zw));
}