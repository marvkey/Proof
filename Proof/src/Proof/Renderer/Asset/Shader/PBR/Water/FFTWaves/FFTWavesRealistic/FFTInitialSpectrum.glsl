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
layout(std140, binding = 5) uniform Params {
    uint Size;
    float LengthScale;
    float CutoffHigh;
    float CutoffLow;
    float GravityAcceleration;
    float Depth;
};



// === SpectrumParameter Struct ===
struct SpectrumParameter {
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
layout(std430, binding = 6) readonly buffer SpectrumParams {
    SpectrumParameter elements[];
} spectrums;

// === Functions ===
float frequency(float k, float g, float depth) {
    return sqrt(g * k * tanh(min(k * depth, 20.0)));
}

float frequencyDerivative(float k, float g, float depth) {
    float th = tanh(min(k * depth, 20.0));
    float ch = cosh(k * depth);
    return g * (depth * k / (ch * ch) + th) / frequency(k, g, depth) / 2.0;
}

float normalisationFactor(float s) {
    float s2 = s * s;
    float s3 = s2 * s;
    float s4 = s3 * s;
    if (s < 5.0) {
        return -0.000564 * s4 + 0.00776 * s3 - 0.044 * s2 + 0.192 * s + 0.163;
    }
    return -4.80e-08 * s4 + 1.07e-05 * s3 - 9.53e-04 * s2 + 5.90e-02 * s + 0.393;
}

float cosine2s(float theta, float s) {
    return normalisationFactor(s) * pow(abs(cos(0.5 * theta)), 2.0 * s);
}

float spreadPower(float omega, float peakOmega) {
    if (omega > peakOmega) {
        return 9.77 * pow(abs(omega / peakOmega), -2.5);
    }
    return 6.97 * pow(abs(omega / peakOmega), 5.0);
}

float directionSpectrum(float theta, float omega, SpectrumParameter pars) {
    float s = spreadPower(omega, pars.peakOmega) + 16.0 * tanh(min(omega / pars.peakOmega, 20.0)) * pars.swell * pars.swell;
    return mix(2.0 / PI * cos(theta) * cos(theta), cosine2s(theta - pars.angle, s), pars.spreadBlend);
}

float TMACorrection(float omega, float g, float depth) {
    float omegaH = omega * sqrt(depth / g);
    if (omegaH <= 1.0) return 0.5 * omegaH * omegaH;
    if (omegaH < 2.0) return 1.0 - 0.5 * pow(2.0 - omegaH, 2.0);
    return 1.0;
}

float JONSWAP(float omega, float g, float depth, SpectrumParameter pars) {
    float sigma = omega <= pars.peakOmega ? 0.07 : 0.09;
    float r = exp(-(omega - pars.peakOmega) * (omega - pars.peakOmega) / (2.0 * sigma * sigma * pars.peakOmega * pars.peakOmega));
    float oneOverOmega = 1.0 / omega;
    float peakOmegaOverOmega = pars.peakOmega / omega;

    return pars.scale * TMACorrection(omega, g, depth) * pars.alpha * g * g *
           pow(oneOverOmega, 5.0) *
           exp(-1.25 * pow(peakOmegaOverOmega, 4.0)) *
           pow(abs(pars.gamma), r);
}

float shortWavesFade(float kLength, SpectrumParameter pars) {
    return exp(-pars.shortWavesFade * pars.shortWavesFade * kLength * kLength);
}

// === Main Compute Entry Point ===
void main() {
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);

    float deltaK = 2.0 * PI / LengthScale;
    float nx = float(id.x) - float(Size) / 2.0;
    float nz = float(id.y) - float(Size) / 2.0;
    vec2 k = vec2(nx, nz) * deltaK;
    float kLength = length(k);

    if (kLength <= CutoffHigh && kLength >= CutoffLow) {
        float omega = frequency(kLength, GravityAcceleration, Depth);
        imageStore(WavesData, id, vec4(k.x, 1.0 / kLength, k.y, omega));

        float kAngle = atan(k.y, k.x);
        float dOmegadk = frequencyDerivative(kLength, GravityAcceleration, Depth);

        SpectrumParameter local = spectrums.elements[0];
        SpectrumParameter swell = spectrums.elements[1];

        float spectrum = JONSWAP(omega, GravityAcceleration, Depth, local) *
                         directionSpectrum(kAngle, omega, local) *
                         shortWavesFade(kLength, local);

        if (swell.scale > 0.0) {
            spectrum += JONSWAP(omega, GravityAcceleration, Depth, swell) *
                        directionSpectrum(kAngle, omega, swell) *
                        shortWavesFade(kLength, swell);
        }

        vec2 noise = texelFetch(Noise, id, 0).xy;
        float factor = sqrt(2.0 * spectrum * abs(dOmegadk) / kLength * deltaK * deltaK);
        imageStore(H0K, id, vec4(noise * factor, 0.0, 0.0));
    } else {
        imageStore(H0K, id, vec4(0.0));
        imageStore(WavesData, id, vec4(k.x, 1.0, k.y, 0.0));
    }
}
