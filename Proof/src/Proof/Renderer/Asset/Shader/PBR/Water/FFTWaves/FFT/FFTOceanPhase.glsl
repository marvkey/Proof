// This compute shader updates the phase values of ocean waves using the dispersion relation. 
// It calculates the new phase for each wave component based on wave frequency and time step, 
// ensuring accurate wave motion in the FFT-based ocean simulation.
#Compute Shader
#version 450 core
//https://github.com/achalpandeyy/OceanFFT/blob/master/Source/Shaders/CS_Phase.comp

#define WORK_GROUP_DIM 32

const float PI = 3.14159265359;
const float GRAVITY = 9.81;
const float KM = 370.0; // Phillips spectrum constant

layout (local_size_x = WORK_GROUP_DIM, local_size_y = WORK_GROUP_DIM) in;

// Input: Current phase values for ocean waves
layout (binding = 0, r32f) readonly uniform image2D u_Phases;

// Output: Updated phase values after applying dispersion relation
layout (binding = 1, r32f) writeonly uniform image2D o_DeltaPhases;

// Push constants storing simulation parameters
layout(push_constant) uniform FFTConstants
{
    float DeltaTime;  // Time step for phase update
    int OceanSize;    // Physical size of ocean grid
    int Resolution;   // Grid resolution (number of points per axis)
} u_PC;

// Computes angular frequency using the dispersion relation
float ComputeOmega(float k)
{
    return sqrt(GRAVITY * k * (1.0 + (k * k) / (KM * KM)));
}

void main()
{
    // Compute pixel coordinates in the phase texture
    ivec2 pixelCoord = ivec2(gl_GlobalInvocationID.xy);

    // Compute wave vector components using FFT indexing
    float n = (pixelCoord.x < 0.5 * u_PC.Resolution) ? pixelCoord.x : pixelCoord.x - u_PC.Resolution;
    float m = (pixelCoord.y < 0.5 * u_PC.Resolution) ? pixelCoord.y : pixelCoord.y - u_PC.Resolution;

    // Compute wave vector magnitude
    vec2 waveVector = (2.0 * PI * vec2(n, m)) / float(u_PC.OceanSize);
    float k = length(waveVector);

    // Apply dispersion relation to update phase
    float deltaPhase = ComputeOmega(k) * u_PC.DeltaTime;
    float phase = imageLoad(u_Phases, pixelCoord).r;
    phase = mod(phase + deltaPhase, 2.0 * PI);

    // Store updated phase value in the output texture
    imageStore(o_DeltaPhases, pixelCoord, vec4(phase, 0.0, 0.0, 0.0));
}