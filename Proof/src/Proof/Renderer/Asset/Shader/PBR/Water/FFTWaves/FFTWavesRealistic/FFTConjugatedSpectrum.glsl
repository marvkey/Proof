//https://github.com/Popov72/OceanDemo/blob/3b9866a1e8ac36e27025f5eb708f1fa5bf21564b/src/assets/ocean/initialSpectrum2.wgsl

#Compute Shader

#version 450

layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in;

// === Bindings ===
layout(binding = 0, rgba32f) uniform writeonly image2D H0;

layout(std140, binding = 1) uniform Params {
    uint Size;
    float LengthScale;
    float CutoffHigh;
    float CutoffLow;
    float GravityAcceleration;
    float Depth;
};

layout(binding = 2) uniform sampler2D H0K;

void main() {
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);

    vec2 h0K = texelFetch(H0K, id, 0).xy;

    // Compute conjugate-symmetric index
    int size = int(Size);
    ivec2 minusK = ivec2((size - id.x) % size, (size - id.y) % size);
    vec2 h0MinusK = texelFetch(H0K, minusK, 0).xy;

    // Store real parts in .xy, conjugate of -k in .zw
    imageStore(H0, id, vec4(h0K.x, h0K.y, h0MinusK.x, -h0MinusK.y));
}
