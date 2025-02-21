// This compute shader calculates the Fourier spectrum of ocean waves by applying
// the initial wave spectrum, the phase data, and the dispersion relation.
// It also introduces wave choppiness for more realistic wave displacement.
#Compute Shader
#version 450 core
//https://github.com/achalpandeyy/OceanFFT/blob/master/Source/Shaders/CS_Spectrum.comp

#define COMPUTE_WORK_GROUP_DIM 32
#define RESOLUTION 512

const float PI = 3.14159265359;
const float GRAVITY = 9.81;
const float KM = 370.0; // Phillips spectrum constant for large-scale ocean waves

layout (local_size_x = COMPUTE_WORK_GROUP_DIM, local_size_y = COMPUTE_WORK_GROUP_DIM) in;

layout (binding = 0, r32f) readonly uniform image2D u_Phases;
layout (binding = 1, r32f) readonly uniform image2D u_InitialSpectrum;
layout (binding = 2, rgba32f) writeonly uniform image2D o_Spectrum;

layout(push_constant) uniform SpectrumConstants
{
    int OceanSize;    // Physical size of the ocean grid
    float Choppiness; // Controls wave steepness and horizontal displacement
} u_PC;

// Performs complex multiplication: (a + bi) * (c + di)
vec2 MultiplyComplex(vec2 a, vec2 b)
{
    return vec2(a.x * b.x - a.y * b.y, a.y * b.x + a.x * b.y);
}

// Multiplies a complex number by 'i' (imaginary unit)
vec2 MultiplyByI(vec2 z)
{
    return vec2(-z.y, z.x);
}

// Computes angular frequency using the dispersion relation
float ComputeOmega(float k)
{
    return sqrt(GRAVITY * k * (1.0 + (k * k) / (KM * KM)));
}

void main()
{
   // Get pixel coordinates in the texture
    ivec2 pixelCoord = ivec2(gl_GlobalInvocationID.xy);

    // Compute wave vector components using FFT indexing
    float n = (pixelCoord.x < 0.5 * RESOLUTION) ? pixelCoord.x : pixelCoord.x - RESOLUTION;
    float m = (pixelCoord.y < 0.5 * RESOLUTION) ? pixelCoord.y : pixelCoord.y - RESOLUTION;
    vec2 waveVector = (2.0 * PI * vec2(n, m)) / u_PC.OceanSize;

    // Load the current phase value and convert it into a complex exponential form
    float phase = imageLoad(u_Phases, pixelCoord).r;
    vec2 phaseVector = vec2(cos(phase), sin(phase)); // e^(i*phase)

    // Load the initial wave spectrum values
    vec2 h0 = vec2(imageLoad(u_InitialSpectrum, pixelCoord).r, 0.0);
    vec2 h0Star = vec2(imageLoad(u_InitialSpectrum, (RESOLUTION - pixelCoord) % (RESOLUTION - 1)).r, 0.0);
    h0Star.y *= -1.0; // Complex conjugate

    // Compute the complex wave height h(k, t)
    vec2 h = MultiplyComplex(h0, phaseVector) + MultiplyComplex(h0Star, vec2(phaseVector.x, -phaseVector.y));

    // Compute horizontal displacements using choppiness factor
    vec2 hX = -MultiplyByI(h * (waveVector.x / length(waveVector))) * u_PC.Choppiness;
    vec2 hZ = -MultiplyByI(h * (waveVector.y / length(waveVector))) * u_PC.Choppiness;

    // Remove DC term at zero frequency to prevent unrealistic drift
    if (waveVector.x == 0.0 && waveVector.y == 0.0)
    {
        h = vec2(0.0);
        hX = vec2(0.0);
        hZ = vec2(0.0);
    }

    // Store the computed spectrum components in the output texture
    imageStore(o_Spectrum, pixelCoord, vec4(hX + MultiplyByI(h), hZ));
}

