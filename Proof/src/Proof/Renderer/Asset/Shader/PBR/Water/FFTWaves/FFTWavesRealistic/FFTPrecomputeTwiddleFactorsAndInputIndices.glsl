//https://github.com/Popov72/OceanDemo/blob/3b9866a1e8ac36e27025f5eb708f1fa5bf21564b/src/assets/ocean/fftPrecompute.wgsl
#Compute Shader

#version 450

#define PI 3.1415926

layout(local_size_x = 1, local_size_y = 8, local_size_z = 1) in;

// === Bindings ===
layout(binding = 0, rgba32f) uniform writeonly image2D PrecomputeBuffer;


layout(push_constant) restrict readonly uniform PushConstants {
     int Step;
    int Size;
}params;

// === Complex math ===
vec2 complexMult(vec2 a, vec2 b) 
{
    return vec2(a.r * b.r - a.g * b.g, a.r * b.g + a.g * b.r);
}

vec2 complexExp(vec2 a) {
    return vec2(cos(a.y), sin(a.y)) * exp(a.x);
}

void main() 
{
    ivec3 id = ivec3(gl_GlobalInvocationID);
    int b = params.Size >> (id.x + 1);

    vec2 mult = 2.0 * PI * vec2(0.0, -1.0) / float(params.Size);
    int i = (2 * b * (id.y / b) + (id.y % b)) % params.Size;
    vec2 twiddle = complexExp(mult * float((id.y / b) * b));

    imageStore(PrecomputeBuffer, ivec2(id.x, id.y), vec4(twiddle.x, twiddle.y, float(i), float(i + b)));
    imageStore(PrecomputeBuffer, ivec2(id.x, id.y + params.Size / 2), vec4(-twiddle.x, -twiddle.y, float(i), float(i + b)));
}
