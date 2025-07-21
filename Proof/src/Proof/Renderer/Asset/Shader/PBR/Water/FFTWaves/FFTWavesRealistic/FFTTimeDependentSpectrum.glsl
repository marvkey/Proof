//https://github.com/Popov72/OceanDemo/blob/3b9866a1e8ac36e27025f5eb708f1fa5bf21564b/src/assets/ocean/timeDependentSpectrum.wgsl

#Compute Shader

#version 450

layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in;

// === Bindings ===
layout(binding = 0) uniform sampler2D H0;
layout(binding = 1) uniform sampler2D WavesData;

layout(push_constant) restrict readonly uniform PushConstants {
    float Time;
}u_PC;

layout(binding = 5, rg32f) uniform writeonly image2D DxDz;
layout(binding = 6, rg32f) uniform writeonly image2D DyDxz;
layout(binding = 7, rg32f) uniform writeonly image2D DyxDyz;
layout(binding = 8, rg32f) uniform writeonly image2D DxxDzz;

// === Complex Multiplication ===
vec2 complexMult(vec2 a, vec2 b) {
    return vec2(a.x * b.x - a.y * b.y,
                a.x * b.y + a.y * b.x);
}

void main() {
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);

    vec4 wave = texelFetch(WavesData, id, 0);
    float phase = wave.w * u_PC.Time;
    vec2 exponent = vec2(cos(phase), sin(phase));

    vec4 h0 = texelFetch(H0, id, 0);
    vec2 h0_pos = h0.xy;
    vec2 h0_neg = h0.zw;

    vec2 h = complexMult(h0_pos, exponent) + complexMult(h0_neg, vec2(exponent.x, -exponent.y));
    vec2 ih = vec2(-h.y, h.x); // i * h

    // Displacements
    vec2 dispX = ih * wave.x * wave.y;
    vec2 dispY = h;
    vec2 dispZ = ih * wave.z * wave.y;

    // Derivatives
    vec2 dispX_dx = -h * wave.x * wave.x * wave.y;
    vec2 dispY_dx = ih * wave.x;
    vec2 dispZ_dx = -h * wave.x * wave.z * wave.y;

    vec2 dispY_dz = ih * wave.z;
    vec2 dispZ_dz = -h * wave.z * wave.z * wave.y;

    // Output
    imageStore(DxDz, id, vec4(dispX.x - dispZ.y, dispX.y + dispZ.x, 0.0, 0.0));
    imageStore(DyDxz, id, vec4(dispY.x - dispZ_dx.y, dispY.y + dispZ_dx.x, 0.0, 0.0));
    imageStore(DyxDyz, id, vec4(dispY_dx.x - dispY_dz.y, dispY_dx.y + dispY_dz.x, 0.0, 0.0));
    imageStore(DxxDzz, id, vec4(dispX_dx.x - dispZ_dz.y, dispX_dx.y + dispZ_dz.x, 0.0, 0.0));

 
}
