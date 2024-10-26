#Compute Shader
#version 450 core
#include <Common.glslh>
#include <Water/FFTWaves/FFT/GPURandom.glslh>
#include <Water/FFTWaves/FFT/Oceanography.glslh>

#define MODE_INITIALIZE_SPECTRUM 0
#define MODE_CONJUGATE_SPECTRUM 1

layout(push_constant) uniform PushData
{
     int Mode;
}u_Uniforms;


layout(set = 0, binding = 0, rgba16f) uniform image2DArray u_H0;
// wave vector x, chop, wave vector z, frequency
layout(set = 0, binding = 1, rgba16f) uniform image2DArray u_WavesData;
layout(set = 0, binding = 2, rg16f) uniform image2DArray u_H0K;

layout(std430, set = 0, binding = 3) buffer SpectrumBuffer 
{
    SpectrumParams Spectrums[];
}s_Spectrum;

layout(set = 0, binding = 4) uniform sampler2D u_EqualizerRamp0;
layout(set = 0, binding = 5) uniform sampler2D u_EqualizerRamp1;

// Function to generate a normal random number
float NormalRandom(uvec3 id)
{
    vec2 random01 = Hash23(id); // Assuming hash23 function is defined elsewhere
    return cos(2.0 * 3.1415926 * random01.x) * sqrt(-2.0 * log(random01.y));
}

float CascadeContribution(float k, int cascade) 
{
    if (k > u_WaveParams.CutoffsHigh[cascade] || k < u_WaveParams.CutoffsLow[cascade]) 
    {
        return 0.0;
    } 
    else 
    {
        float total = 0.0;
        for (uint i = 0; i < u_WaveParams.CascadesCount; i++) 
        {
            total += float(k <= u_WaveParams.CutoffsHigh[i] && k >= u_WaveParams.CutoffsLow[i]);
        }
        return 1.0 / total;
    }
}

void CalculateInitialsForCascade(uvec3 id) 
{
    vec2 noise = vec2(NormalRandom(id), NormalRandom(uvec3(id.xy, id.z + u_WaveParams.CascadesCount)));
    float deltaK = 2.0 * PI / u_WaveParams.LengthScales[id.z];
    int nx = int(id.x) - int(u_WaveParams.Size) / 2;
    int nz = int(id.y) - int(u_WaveParams.Size) / 2;
    vec2 k = vec2(nx, nz) * deltaK;
    float kLength = length(k);
    
    float contribution = CascadeContribution(kLength, int(id.z));
    
    if (contribution > 0.0) {
        float theta = atan(k.y, k.x);
        float omega = Frequency(kLength, Depth);
        float dOmegadk = FrequencyDerivative(kLength, Depth);

        float spectrum = FullSpectrum(omega, theta - u_WaveParams.LocalWindDirection / 180.0 * PI, s_Spectrum.Spectrums[0]) * s_Spectrum.Spectrums[0].scale
                         * ShortWavesFade(kLength, s_Spectrum.Spectrums[0].shortWavesFade);
                         
        if (s_Spectrum.Spectrums[1].scale > 0.0) {
            spectrum += FullSpectrum(omega, theta - u_WaveParams.SwellDirection / 180.0 * PI, s_Spectrum.Spectrums[1]) * s_Spectrum.Spectrums[1].scale
                        * ShortWavesFade(kLength, s_Spectrum.Spectrums[1].shortWavesFade);
        }
        
        float rampUV = clamp((-u_WaveParams.RampsXLimits.x + Log10(2.0 * PI / kLength)) / (u_WaveParams.RampsXLimits.y - u_WaveParams.RampsXLimits.x), 0.0, 1.0);
        vec2 eq0 = textureLod(u_EqualizerRamp0, vec2(rampUV,0),0).xy;
        vec2 eq1 = textureLod(u_EqualizerRamp1, vec2(rampUV,0),0).xy;
        vec2 eq = mix(eq0, eq1, u_WaveParams.EqualizerLerpValue);
        float scaleRamp = eq.r;
        float lambda = u_WaveParams.Chop * eq.g;
        
        imageStore(u_WavesData, ivec3(id), vec4(k.x, lambda, k.y, omega));
        imageStore(u_H0K, ivec3(id), vec4(contribution * scaleRamp * vec2(noise.x, noise.y) * sqrt(2.0 * spectrum * abs(dOmegadk) / kLength * deltaK * deltaK), 0.0, 0.0));
    } else {
        imageStore(u_WavesData, ivec3(id), vec4(k.x, 0.0, k.y, 0.0));
        imageStore(u_H0K, ivec3(id), vec4(0.0, 0.0, 0.0, 0.0));
    }
}


void CalculateInitialSpectrum()
{
	for (uint i = 0; i < u_WaveParams.CascadesCount; i++)
	{
		CalculateInitialsForCascade(uvec3(gl_GlobalInvocationID.xy, i));
	}
}

void CalculateConjugatedSpectrum() 
{
    ivec3 id = ivec3(gl_GlobalInvocationID.xyz);

    for (uint i = 0; i < u_WaveParams.CascadesCount; i++) 
    {
        // Compute indices
        ivec3 index = ivec3(id.xy, int(i));
        ivec3 minusIndex = ivec3((u_WaveParams.Size - id.x) % u_WaveParams.Size,
                                 (u_WaveParams.Size - id.y) % u_WaveParams.Size, int(i));

        // Fetch values from the H0K image array
        vec2 h0K = imageLoad(u_H0K, index).xy;
        vec2 h0MinusK = imageLoad(u_H0K, minusIndex).xy;

        // Store computed values into the H0 image array
        imageStore(u_H0, index, vec4(h0K.x, h0K.y, h0MinusK.x, -h0MinusK.y));
    }
}




layout(local_size_x = 8, local_size_y = 8, local_size_y = 1) in;
void main()
{
     if (u_Uniforms.Mode == MODE_INITIALIZE_SPECTRUM)
    {
        CalculateInitialSpectrum();
    } 
    else if (u_Uniforms.Mode == MODE_CONJUGATE_SPECTRUM)
    {
        CalculateConjugatedSpectrum();
    }
}