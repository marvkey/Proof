//https://github.com/Popov72/OceanDemo/blob/3b9866a1e8ac36e27025f5eb708f1fa5bf21564b/src/assets/ocean/fftInverseFFT2.wgsl
#Compute Shader

#version 450

layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in;

layout(push_constant) restrict readonly uniform PushConstants 
{
    int Step;
    int Size;
}params;

// === Bindings ===
layout(binding = 3) uniform sampler2D PrecomputedData;
layout(binding = 5) uniform sampler2D InputBuffer;
layout(binding = 6, rg32f) uniform writeonly image2D OutputBuffer;

// === Complex Multiplication ===
vec2 complexMult(vec2 a, vec2 b) {
    return vec2(a.x * b.x - a.y * b.y,
                a.x * b.y + a.y * b.x);
}

// === Main ===
void main() {
    ivec3 id = ivec3(gl_GlobalInvocationID);
    ivec2 coord = ivec2(params.Step, id.y);

    vec4 data = texelFetch(PrecomputedData, coord, 0);
    ivec2 inputIndices = ivec2(int(data.b), int(data.a)); // WGSL .ba → GLSL .b, .a

    vec2 input0 = texelFetch(InputBuffer, ivec2(id.x, inputIndices.x), 0).xy;
    vec2 input1 = texelFetch(InputBuffer, ivec2(id.x, inputIndices.y), 0).xy;

    vec2 twiddle = vec2(data.r, -data.g); // Invert imaginary
    vec2 result = input0 + complexMult(twiddle, input1);

    imageStore(OutputBuffer, ivec2(id.xy), vec4(result, 0.0, 0.0));
}
