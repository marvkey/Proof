#Compute Shader
#version 450 core
#include <Common.glslh>
#include <PBR/Water/FFTWaves/FFTWaterCompute.glslh>

#define MODE_ASSEMBLE_MAPS 0
#define MODE_ACCUMULATE_FOAM 1
layout(push_constant) uniform PushData
{
     int Mode;
}u_Uniforms;
vec4 Permute(vec4 data, vec3 id) 
{
    return data * (1.0f - 2.0f * mod(id.x + id.y, 2.0));
}

void AssmebleMaps()
{
    uvec3 id = uvec3(gl_GlobalInvocationID.xy, 0);

    for (int i = 0; i < 4; ++i) 
    {
        vec4 htildeDisplacement = Permute(imageLoad(u_SpectrumTextures, ivec3(id.xy, i * 2u)), id);
        vec4 htildeSlope = Permute(imageLoad(u_SpectrumTextures, ivec3(id.xy, i * 2u + 1u)), id);

        vec2 dxdz = htildeDisplacement.rg;
        vec2 dydxz = htildeDisplacement.ba;
        vec2 dyxdyz = htildeSlope.rg;
        vec2 dxxdzz = htildeSlope.ba;

        float jacobian = (1.0 + u_FFTData.Lambda.x * dxxdzz.x) * (1.0 + u_FFTData.Lambda.y * dxxdzz.y) - u_FFTData.Lambda.x * u_FFTData.Lambda.y * dydxz.y * dydxz.y;

        vec3 displacement = vec3(u_FFTData.Lambda.x * dxdz.x, dydxz.x, u_FFTData.Lambda.y * dxdz.y);

        vec2 slopes = dyxdyz.xy / (1.0 + abs(dxxdzz * u_FFTData.Lambda));
        float covariance = slopes.x * slopes.y;

        float foam = imageLoad(u_DisplacementTextures, ivec3(id.xy, i)).a;
        foam *= exp(-u_FFTData.FoamDecayRate);
        foam = clamp(foam, 0.0, 1.0);

        float biasedJacobian = max(0.0, -(jacobian - u_FFTData.FoamBias));

        if (biasedJacobian > u_FFTData.FoamThreshold)
            foam += u_FFTData.FoamAdd * biasedJacobian;

        imageStore(u_DisplacementTextures, ivec3(id.xy, i), vec4(displacement, foam));
        imageStore(u_SlopeTextures, ivec3(id.xy, i), vec4(slopes, 0.0, 0.0));

        if (i == 0) 
        {
            imageStore(u_BuoyancyData, ivec2(id.xy), vec4(displacement.y, 0.0, 0.0, 0.0));
        }
    }
}

void AccumulateFoam() 
{
    uvec3 id = uvec3(gl_GlobalInvocationID.xy, 1);
    imageStore(u_SpectrumTextures, ivec3(id), vec4(0.0));
}

layout(local_size_x = 8, local_size_y = 8, local_size_y = 1) in;

void main()
{
    if (u_Uniforms.Mode == MODE_ASSEMBLE_MAPS)
    {
        AssmebleMaps();
    } 
    else if (u_Uniforms.Mode == MODE_ACCUMULATE_FOAM)
    {
        AccumulateFoam();
    }
}