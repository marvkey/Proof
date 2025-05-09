#Compute Shader

#version 460
layout(local_size_x = 64, local_size_y = 1, local_size_z = 1) in;

#define MAX_CASCADES 8

layout(set = 0, binding = 0) uniform sampler2DArray u_Displacements;
struct SBWaveHeightQuery 
{
    uint ID_Low;
    uint ID_High;
    float WaveHeight;
    uint pad1;

    vec4 WorldPosition; // vec4 for padding
    vec4 WaveDisplacement; // vec4 for padding 

};
layout(std430, set = 0, binding = 1) buffer WaveQueryBuffer {

    SBWaveHeightQuery queries[];
};

layout(push_constant) restrict readonly uniform PushConstants 
{
    uint NumQueries;
    int NumCascades;

}u_PC;

layout(set = 0, binding = 2) uniform ScalesWaterUniform
{
   vec4 MapScales[MAX_CASCADES];
};


void main() {
    uint idx = gl_GlobalInvocationID.x;
    if (idx >= u_PC.NumQueries)
        return;

    SBWaveHeightQuery q = queries[idx];
    float waveHeight = 0.0;
    vec3 totalDisplacement = vec3(0.0);

    for (int i = 0; i < u_PC.NumCascades; ++i) {
        vec4 scales = MapScales[i];

        // Reconstruct tile length
        vec2 tileLength = vec2(1.0 / scales.x, 1.0 / scales.y);
        vec2 worldPos = q.WorldPosition.xz;

        // Wrap world position
        vec2 wrapped = mod(mod(worldPos, tileLength) + tileLength, tileLength);
        vec2 uv = wrapped / tileLength;
        

        vec3 displacement = texture(u_Displacements, vec3(uv, float(i))).xyz * scales.z;
       // waveHeight += texture(u_Displacements, vec3(uv, float(i))).y * scales.z;
        waveHeight += displacement.y;
        totalDisplacement += displacement;
    }

    queries[idx].WaveHeight = waveHeight;
    queries[idx].WaveDisplacement.xyz = totalDisplacement;
}
