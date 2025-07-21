
//https://github.com/Popov72/OceanDemo/blob/3b9866a1e8ac36e27025f5eb708f1fa5bf21564b/src/assets/ocean/initialSpectrum.wgsl
#Compute Shader

#version 450

#define PI 3.1415926

layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in;

// === Bindings ===
layout(binding = 1, rgba32f) uniform writeonly image2D WavesData;
layout(binding = 2, rg32f)  uniform writeonly image2D H0K;
layout(binding = 4) uniform sampler2D Noise;

// === Uniform Params ===
layout(std140, binding = 5) uniform Params 
{
    uint Size;
    float LengthScale;
    float CutoffHigh;
    float CutoffLow;

    float GravityAcceleration;
    float Depth;
};

// === SpectrumParameter Struct ===
struct SpectrumParameter 
{
    float scale;
    float angle;
    float spreadBlend;
    float swell;

    float alpha;
    float peakOmega;
    float gamma;
    float shortWavesFade;
};

// === SpectrumParameters Buffer ===
layout(std140, binding = 6) buffer SpectrumParams {
    SpectrumParameter elements[];
} spectrums;

// === Functions ===

float Frequency(float k, float g, float depth) {
    return sqrt(g * k * tanh(min(k * depth, 20.0)));
}

float FrequencyDerivative(float k, float g, float depth) {
    float th = tanh(min(k * depth, 20.0));
    float ch = cosh(k * depth);
    return g * (depth * k / (ch * ch) + th) / (2.0 * Frequency(k, g, depth));
}

float NormalisationFactor(float s) {
    float s2 = s * s;
    float s3 = s2 * s;
    float s4 = s3 * s;
    if (s < 5.0)
        return -0.000564 * s4 + 0.00776 * s3 - 0.044 * s2 + 0.192 * s + 0.163;
    else
        return -4.80e-08 * s4 + 1.07e-05 * s3 - 9.53e-04 * s2 + 0.059 * s + 0.393;
}

float Cosine2s(float theta, float s) {
    return NormalisationFactor(s) * pow(abs(cos(0.5 * theta)), 2.0 * s);
}

float SpreadPower(float omega, float peakOmega) {
    if (omega > peakOmega)
        return 9.77 * pow(abs(omega / peakOmega), -2.5);
    else
        return 6.97 * pow(abs(omega / peakOmega), 5.0);
}

float DirectionSpectrum(float theta, float omega, SpectrumParameter pars) {
    float s = SpreadPower(omega, pars.peakOmega)
        + 16.0 * tanh(min(omega / pars.peakOmega, 20.0)) * pars.swell * pars.swell;
    return mix(2.0 / PI * cos(theta) * cos(theta), Cosine2s(theta - pars.angle, s), pars.spreadBlend);
}

float TMACorrection(float omega, float g, float depth) {
    float omegaH = omega * sqrt(depth / g);
    if (omegaH <= 1.0)
        return 0.5 * omegaH * omegaH;
    if (omegaH < 2.0)
        return 1.0 - 0.5 * pow(2.0 - omegaH, 2.0);
    return 1.0;
}

float JONSWAP(float omega, float g, float depth, SpectrumParameter pars) 
{
 float sigma;
	if (omega <= pars.peakOmega)
		sigma = 0.07;
	else
		sigma = 0.09;
	float r = exp(-(omega - pars.peakOmega) * (omega - pars.peakOmega)
		/ 2 / sigma / sigma / pars.peakOmega / pars.peakOmega);
	
	float oneOverOmega = 1 / omega;
	float peakOmegaOverOmega = pars.peakOmega / omega;



	return pars.scale * TMACorrection(omega, g, depth) * pars.alpha * g * g
		* oneOverOmega * oneOverOmega * oneOverOmega * oneOverOmega * oneOverOmega
		* exp(-1.25 * peakOmegaOverOmega * peakOmegaOverOmega * peakOmegaOverOmega * peakOmegaOverOmega)
		* pow(abs(pars.gamma), r);
}

float ShortWavesFade(float kLength, SpectrumParameter pars) {
    return exp(-pars.shortWavesFade * pars.shortWavesFade * kLength * kLength);
}

// === Main Kernel ===
void main() {
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);

    float deltaK = 2.0 * PI / LengthScale;
    int nx = int(id.x) - int(Size) / 2;
    int nz = int(id.y) - int(Size) / 2;
    vec2 k = vec2(float(nx), float(nz)) * deltaK;
    float kLength = length(k);

    if (kLength <= CutoffHigh && kLength >= CutoffLow ) {
        float kAngle = atan(k.y, k.x);
        float omega = Frequency(kLength, GravityAcceleration, Depth);
        imageStore(WavesData, id, vec4(k.x, 1.0 / kLength, k.y, omega));
        float dOmegadk = FrequencyDerivative(kLength, GravityAcceleration, Depth);


        float spectrum = JONSWAP(omega, GravityAcceleration, Depth, spectrums.elements[0])
                       * DirectionSpectrum(kAngle, omega, spectrums.elements[0])
                      * ShortWavesFade(kLength, spectrums.elements[0]);

        if (spectrums.elements[1].scale > 0.0) 
        {
            spectrum += JONSWAP(omega, GravityAcceleration, Depth, spectrums.elements[1])
                      * DirectionSpectrum(kAngle, omega, spectrums.elements[1])
                      * ShortWavesFade(kLength, spectrums.elements[1]);
        }



        vec2 noise = texelFetch(Noise, id, 0).xy;
        float h0 = sqrt(2.0 * (spectrum) * abs(dOmegadk) / kLength * deltaK * deltaK);
        imageStore(H0K, id, vec4(noise * h0 , 0.0, 0.0));

    } 
    else 
    {
        imageStore(H0K, id, vec4(0.0));
        imageStore(WavesData, id, vec4(k.x, 1.0, k.y, 0.0));
    }
}
