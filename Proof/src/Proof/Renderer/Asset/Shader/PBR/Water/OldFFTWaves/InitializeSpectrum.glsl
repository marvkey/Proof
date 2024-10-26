#Compute Shader
#version 450 core
#include <Common.glslh>
#include <PBR/Water/FFTWaves/FFTWaterCompute.glslh>

#define MODE_INITIALIZE_SPECTRUM 0
#define MODE_PACK_SPECTRUM_CONJUGATE 1
#define MODE_UPDATE_SPECTRUM_FOR_FFT 2

layout(push_constant) uniform PushData
{
     int Mode;
}u_Uniforms;
void InitializeSpectrum()
{
    uvec3 id = gl_GlobalInvocationID.xyz;
    uint seed = id.x + u_FFTData.N * id.y + u_FFTData.N;
    seed += uint(u_FFTData.Seed);

    float lengthScales[4] = { u_FFTData.LengthScale0, u_FFTData.LengthScale1, u_FFTData.LengthScale2, u_FFTData.LengthScale3 };

    for (uint i = 0; i < 4u; i++) 
    {
        float halfN = float(u_FFTData.N) / 2.0;

        float deltaK = 2.0 * 3.141592653589793 / lengthScales[i];
        vec2 K = (vec2(id.xy) - halfN) * deltaK;
        float kLength = length(K);

        seed += i + uint(Hash(seed) * 10.0);
        vec4 uniformRandSamples = vec4(Hash(seed), Hash(seed * 2u),Hash(seed * 3u), Hash(seed * 4u));
        vec2 gauss1 = UniformToGaussian(uniformRandSamples.x, uniformRandSamples.y);
        vec2 gauss2 = UniformToGaussian(uniformRandSamples.z, uniformRandSamples.w);

        if (u_FFTData.LowCutoff <= kLength && kLength <= u_FFTData.HighCutoff) 
        {
            float kAngle = atan(K.y, K.x);
            float omega = Dispersion(kLength);
            float dOmegadk = DispersionDerivative(kLength);

            SpectrumParameters spectrumParams = s_Spectrum.Spectrums[i * 2u];
            float spectrum = Jonswap(omega, spectrumParams) * DirectionSpectrum(kAngle, omega, spectrumParams) * ShortWavesFade(kLength, spectrumParams);
            
            if (s_Spectrum.Spectrums[i * 2u + 1u].Scale > 0.0) 
            {
                spectrumParams = s_Spectrum.Spectrums[i * 2u + 1u];
                spectrum += Jonswap(omega, spectrumParams) * DirectionSpectrum(kAngle, omega, spectrumParams) * ShortWavesFade(kLength, spectrumParams);
            }
            
            imageStore(u_InitialSpectrumTextures, ivec3(id.xy, i), vec4(gauss2.x, gauss1.y, sqrt(2.0 * spectrum * abs(dOmegadk) / kLength * deltaK * deltaK), 0.0));
        } 
        else 
        {
            imageStore(u_InitialSpectrumTextures, ivec3(id.xy, i), vec4(0.0));
        }
    }
}

void PackSpectrumConjugate() 
{
    uvec3 id = gl_GlobalInvocationID.xyz;

    for (uint i = 0; i < 4; ++i) {
        // Read the texture data from the image
        vec4 h0 = imageLoad(u_InitialSpectrumTextures, ivec3(id.xy, int(i)));
        vec4 h0conj = imageLoad(u_InitialSpectrumTextures, ivec3(((u_FFTData.N - id.x) % u_FFTData.N), ((u_FFTData.N - id.y) % u_FFTData.N), int(i)));

        // Write the updated data to the image
        imageStore(u_InitialSpectrumTextures, ivec3(id.xy, int(i)), vec4(h0.xy, h0conj.x, -h0conj.y));
    }
}

void UpdateSectrumForFFT()
{
  uvec3 id = gl_GlobalInvocationID.xyz;

    float lengthScales[4] = { u_FFTData.LengthScale0, u_FFTData.LengthScale1, u_FFTData.LengthScale2, u_FFTData.LengthScale3 };

    for (int i = 0; i < 4u; ++i) {
        vec4 initialSignal = imageLoad(u_InitialSpectrumTextures, ivec3(id.xy, i));
        vec2 h0 = initialSignal.xy;
        vec2 h0conj = initialSignal.zw;

        float halfN = float(u_FFTData.N) / 2.0f;
        vec2 K = (vec2(id.xy) - vec2(halfN)) * 2.0f * 3.14159265359f / lengthScales[i];
        float kMag = length(K);
        float kMagRcp = 1.0f / kMag;

        if (kMag < 0.0001f) {
            kMagRcp = 1.0f;
        }

        float w_0 = 2.0f * 3.14159265359f / u_FFTData.RepeatTime;
        float dispersion = floor(sqrt(u_FFTData.Gravity * kMag) / w_0) * w_0 * u_FrameData.AppTimeSeconds;

        vec2 exponent = EulerFormula(dispersion);

        vec2 htilde = ComplexMult(h0, exponent) + ComplexMult(h0conj, vec2(exponent.x, -exponent.y));
        vec2 ih = vec2(-htilde.y, htilde.x);

        vec2 displacementX = ih * K.x * kMagRcp;
        vec2 displacementY = htilde;
        vec2 displacementZ = ih * K.y * kMagRcp;

        vec2 displacementX_dx = -htilde * K.x * K.x * kMagRcp;
        vec2 displacementY_dx = ih * K.x;
        vec2 displacementZ_dx = -htilde * K.x * K.y * kMagRcp;

        vec2 displacementY_dz = ih * K.y;
        vec2 displacementZ_dz = -htilde * K.y * K.y * kMagRcp;

        vec2 htildeDisplacementX = vec2(displacementX.x - displacementZ.y, displacementX.y + displacementZ.x);
        vec2 htildeDisplacementZ = vec2(displacementY.x - displacementZ_dx.y, displacementY.y + displacementZ_dx.x);
        
        vec2 htildeSlopeX = vec2(displacementY_dx.x - displacementY_dz.y, displacementY_dx.y + displacementY_dz.x);
        vec2 htildeSlopeZ = vec2(displacementX_dx.x - displacementZ_dz.y, displacementX_dx.y + displacementZ_dz.x);

        imageStore(u_SpectrumTextures, ivec3(id.xy, i * 2), vec4(htildeDisplacementX, htildeDisplacementZ));
        imageStore(u_SpectrumTextures, ivec3(id.xy, i * 2 + 1), vec4(htildeSlopeX, htildeSlopeZ));
    }
}

layout(local_size_x = 8, local_size_y = 8, local_size_y = 1) in;
void main()
{
     if (u_Uniforms.Mode == MODE_INITIALIZE_SPECTRUM)
    {
        InitializeSpectrum();
    } 
    else if (u_Uniforms.Mode == MODE_PACK_SPECTRUM_CONJUGATE)
    {
        PackSpectrumConjugate();
    }
    else if (u_Uniforms.Mode == MODE_UPDATE_SPECTRUM_FOR_FFT)
    {
        UpdateSectrumForFFT();
    }
}
