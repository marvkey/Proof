//https://github.com/Popov72/OceanDemo/blob/3b9866a1e8ac36e27025f5eb708f1fa5bf21564b/src/assets/ocean/fftInverseFFT.wgsl

#Compute Shader
#version 450

layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in;

#define PI 3.1415926


layout(push_constant) restrict readonly uniform PushConstants 
{
    int Step;
    int Size;
}params;
// Read-only textures
layout(binding = 3) uniform sampler2D PrecomputedData;
layout(binding = 5) uniform sampler2D InputBuffer;

// Write-only image
layout(binding = 6, rg32f) uniform writeonly image2D OutputBuffer;

// === Complex multiplication ===
vec2 complexMult(vec2 a, vec2 b) {
    return vec2(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x);
}

// === Main ===
void main() {
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);
    
    // Load precomputed twiddle factor data
    vec4 data = texelFetch(PrecomputedData, ivec2(params.Step, id.x), 0);
    ivec2 inputsIndices = ivec2(data.b, data.a);  // .ba becomes .zw in GLSL

    vec2 input0 = texelFetch(InputBuffer, ivec2(inputsIndices.x, id.y), 0).xy;
    vec2 input1 = texelFetch(InputBuffer, ivec2(inputsIndices.y, id.y), 0).xy;

    vec2 twiddle = vec2(data.r, -data.g); // Invert imaginary part

   // Perform butterfly operation
    vec2 result = input0 + complexMult(vec2(data.r, -data.g), input1);
    imageStore(OutputBuffer, id, vec4(result, 0.0, 0.0));
}
