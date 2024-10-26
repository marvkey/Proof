#Compute Shader
#version 450 core
#include <Common.glslh>
#include <Water/FFTWaves/FFT/Oceanography.glslh>


layout(set = 0, binding = 0, rgba32f) uniform image2DArray u_Result;

layout(set = 0, binding = 1) uniform sampler2DArray u_H0;
layout(set = 0, binding = 2) uniform sampler2DArray u_WavesData;



vec2 ComplexMult(vec2 a, vec2 b) {
    return vec2(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x);
}

void CalculateForCascade(ivec3 id) {
    vec4 wave = texelFetch(u_WavesData, id, 0);
    
    float phase = wave.w * u_FrameData.AppTimeSeconds;
    vec2 exponent = vec2(cos(phase), sin(phase));
    vec4 h0 = texelFetch(u_H0, id, 0);
    vec2 h = ComplexMult(h0.xy, exponent) + ComplexMult(h0.zw, vec2(exponent.x, -exponent.y));
    vec2 ih = vec2(-h.y, h.x);
    
    float oneOverKLength = 1.0 / max(0.001, length(wave.xz));
    
    float lambda = wave.y;
    vec2 displacementX = lambda * ih * wave.x * oneOverKLength;
    vec2 displacementY = h;
    vec2 displacementZ = lambda * ih * wave.z * oneOverKLength;
    
    vec2 displacementX_dx = -lambda * h * wave.x * wave.x * oneOverKLength;
    vec2 displacementY_dx = ih * wave.x;
    vec2 displacementZ_dx = -lambda * h * wave.x * wave.z * oneOverKLength;
         
    vec2 displacementY_dz = ih * wave.z;
    vec2 displacementZ_dz = -lambda * h * wave.z * wave.z * oneOverKLength;
    
    imageStore(u_Result, ivec3(id.xy, id.z * 2), vec4(vec2(displacementX.x - displacementY.y, displacementX.y + displacementY.x),
                                                   vec2(displacementZ.x - displacementZ_dx.y, displacementZ.y + displacementZ_dx.x)));
    
    imageStore(u_Result, ivec3(id.xy, id.z * 2 + 1), vec4(vec2(displacementY_dx.x - displacementY_dz.y, displacementY_dx.y + displacementY_dz.x),
                                                       vec2(displacementX_dx.x - displacementZ_dz.y, displacementX_dx.y + displacementZ_dz.x)));
}

layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in;
void main() {
    ivec3 id = ivec3(gl_GlobalInvocationID);
    for (uint i = 0; i < u_WaveParams.CascadesCount; i++) 
    {
        CalculateForCascade(ivec3(id.xy, int(i)));
    }
}