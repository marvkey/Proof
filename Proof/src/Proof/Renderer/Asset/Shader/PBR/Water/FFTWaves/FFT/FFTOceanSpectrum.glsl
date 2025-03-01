//https://github.com/2Retr0/GodotOceanWaves/blob/4e1e219bc8f55c38092ed34e6fee568b900d95b5/assets/shaders/compute/spectrum_compute.glsl
#Compute Shader

#version 450
/**
 * Generates a 2D texture representing the JONSWAP wave spectra
 * w/ Hasselmann directional spreading.
 *
 * Sources: Jerry Tessendorf - Simulating Ocean Water
 *          Christopher J. Horvath - Empirical Directional Wave Spectra for Computer Graphics
 */

#define PI (3.141592653589793)
#define G  (9.81)

layout(local_size_x = 16, local_size_y = 16, local_size_z = 1) in;

layout(rgba16f, set = 0, binding = 0) restrict writeonly uniform image2DArray o_Spectrum;

layout(push_constant) restrict readonly uniform PushConstants {
    ivec2 Seed;
    vec2 TileLength;

    float Alpha;
    float PeakFrequency;
    float WindSpeed;
    float Angle; // radians

    float Depth;
    float Swell;
    float Detail;
    float Spread;

    uint CascadeIndex;
}u_PC;

// --- HELPER FUNCTIONS ---
// Source: https://www.shadertoy.com/view/Xt3cDn
vec2 Hash(in uvec2 x) {
    uint h32 = x.y + 374761393U + x.x * 3266489917U;
    h32 = 2246822519U * (h32 ^ (h32 >> 15));
    h32 = 3266489917U * (h32 ^ (h32 >> 13));
    uint n = h32 ^ (h32 >> 16);
    uvec2 rz = uvec2(n, n * 48271U);
    return vec2((rz.xy >> 1) & uvec2(0x7FFFFFFFU)) / float(0x7FFFFFFF);
}

/** Samples a 2D-bivariate normal distribution */
vec2 Gaussian(in vec2 x) {
    float r = sqrt(-2.0 * log(x.x));
    float theta = 2.0 * PI * x.y;
    return vec2(r * cos(theta), r * sin(theta));
}

/** Returns the complex conjugate of x */
vec2 ConjComplex(in vec2 x) {
    return vec2(x.x, -x.y);
}

// --- SPECTRUM-RELATED FUNCTIONS ---
// Source: Jerry Tessendorf - Simulating Ocean Water
vec2 DispersionRelation(in float k) {
    float a = k * u_PC.Depth;
    float b = tanh(a);
    float dispersionRelation = sqrt(G * k * b);
    float dDispersionRelation = 0.5 * G * (b + a * (1.0 - b * b)) / dispersionRelation;
    return vec2(dispersionRelation, dDispersionRelation);
}

/** Normalization factor approximation for Longuet-Higgins function. */
float LonguetHigginsNormalization(in float s) {
    float a = sqrt(s);
    return (s < 0.4) ? (0.5 / PI) + s * (0.220636 + s * (-0.109 + s * 0.090)) : inversesqrt(PI) * (a * 0.5 + (1.0 / a) * 0.0625);
}

float LonguetHigginsFunction(in float s, in float theta) {
    return LonguetHigginsNormalization(s) * pow(abs(cos(theta * 0.5)), 2.0 * s);
}

float HasselmannDirectionalSpread(in float w, in float wP, in float windSpeed, in float theta) {
    float p = w / wP;
    float s = (w <= wP) ? 6.97 * pow(abs(p), 4.06) : 9.77 * pow(abs(p), -2.33 - 1.45 * (windSpeed * wP / G - 1.17));
    float sXi = 16.0 * tanh(wP / w) * u_PC.Swell * u_PC.Swell;
    return LonguetHigginsFunction(s + sXi, theta - u_PC.Angle);
}

float TMASpectrum(in float w, in float wP, in float alpha) {
    const float beta = 1.25;
    const float gamma = 3.3;
    float sigma = (w <= wP) ? 0.07 : 0.09;
    float r = exp(-(w - wP) * (w - wP) / (2.0 * sigma * sigma * wP * wP));
    float jonswapSpectrum = (alpha * G * G) / pow(w, 5) * exp(-beta * pow(wP / w, 4)) * pow(gamma, r);
    float wH = min(w * sqrt(u_PC.Depth / G), 2.0);
    float kitaigorodskiiDepthAttenuation = (wH <= 1.0) ? 0.5 * wH * wH : 1.0 - 0.5 * (2.0 - wH) * (2.0 - wH);
    return jonswapSpectrum * kitaigorodskiiDepthAttenuation;
}

vec2 GetSpectrumAmplitude(in ivec2 id, in ivec2 mapSize) {
    vec2 dk = 2.0 * PI / u_PC.TileLength;
    vec2 kVec = (id - mapSize * 0.5) * dk;
    float k = length(kVec) + 1e-6;
    float theta = atan(kVec.x, kVec.y);
    vec2 dispersion = DispersionRelation(k);
    float w = dispersion[0];
    float wNorm = dispersion[1] / k * dk.x * dk.y;
    float s = TMASpectrum(w, u_PC.PeakFrequency, u_PC.Alpha);
    float d = mix(0.5 / PI, HasselmannDirectionalSpread(w, u_PC.PeakFrequency, u_PC.WindSpeed, theta), 1.0 - u_PC.Spread) * exp(-(1.0 - u_PC.Detail) * (1.0 - u_PC.Detail) * k * k);
    return Gaussian(Hash(uvec2(id + u_PC.Seed))) * sqrt(2.0 * s * d * wNorm);
}

void main() {
    const ivec2 dims = imageSize(o_Spectrum).xy;
    const ivec3 id = ivec3(gl_GlobalInvocationID.xy, u_PC.CascadeIndex);
    const ivec2 id0 = id.xy;
    const ivec2 id1 = ivec2(mod(-id0, dims));
    imageStore(o_Spectrum, id, vec4(GetSpectrumAmplitude(id0, dims), ConjComplex(GetSpectrumAmplitude(id1, dims))));
}