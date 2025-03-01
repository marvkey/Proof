// This compute shader generates the initial wave spectrum for FFT-based ocean rendering.
// It uses the Phillips Spectrum to create a frequency-domain representation of the ocean surface.
// The spectrum accounts for wind direction, wave height, and wave dispersion properties.
// The result is stored in a texture that will be used in subsequent FFT passes.


#Compute Shader
#version 450 core
//https://github.com/achalpandeyy/OceanFFT/blob/master/Source/Shaders/CS_InitialSpectrum.comp
#define COMPUTE_WORK_GROUP_DIM 32

// Physical constants
const float PI = 3.14159265359; // Pi value
const float g = 9.81;           // Acceleration due to gravity (m/s^2)
const float KM = 370.0;         // Wave number at which wind stops influencing waves
const float CM = 0.23;          // Minimum wave speed, carnock number

layout (local_size_x = COMPUTE_WORK_GROUP_DIM, local_size_y = COMPUTE_WORK_GROUP_DIM) in;

layout(binding = 0, r32f) writeonly uniform image2D o_InitialSpectrum;


// Push constants defining resolution, ocean size, and wind direction
layout(push_constant) uniform FFTConstants
{
    int Resolution;  // Grid resolution
    int OceanSize;   // Physical size of the simulated ocean
    vec2 Wind;       // Wind direction and magnitude
} u_PC;

// Computes the dispersion relation, which determines the wave frequency
float omega(float k)
{
    return sqrt(g * k * (1.f + ((k * k) / (KM * KM))));
}

// Helper function to compute the square of a number
float square(float x)
{
    return x * x;
}

void main()
{
	// Compute the pixel coordinate in the spectrum grid
    ivec2 pixel_coord = ivec2(gl_GlobalInvocationID.xy);
    
    // Compute the wave vector indices (n, m) based on pixel location
    float n = (pixel_coord.x < 0.5f * u_PC.Resolution) ? pixel_coord.x : pixel_coord.x - u_PC.Resolution;
    float m = (pixel_coord.y < 0.5f * u_PC.Resolution) ? pixel_coord.y : pixel_coord.y - u_PC.Resolution;

	 // Compute the wave vector in spatial coordinates
    vec2 wave_vector = (2.f * PI * vec2(n, m)) / u_PC.OceanSize;
    float k = length(wave_vector); // Compute the wave number

    // Compute wind speed magnitude
    float U10 = length(u_PC.Wind);
    
    // Compute Phillips Spectrum parameters
    float Omega = 0.84f;
    float kp = g * square(Omega / U10); // Peak wave number
    
    float c = omega(k) / k;  // Phase speed of the wave
    float cp = omega(kp) / kp; // Phase speed at peak spectrum
    
    // Compute the wind-wave interaction terms
    float Lpm = exp(-1.25 * square(kp / k));
    float gamma = 1.7;
    float sigma = 0.08 * (1.0 + 4.0 * pow(Omega, -3.0));
    float Gamma = exp(-square(sqrt(k / kp) - 1.0) / (2.0 * square(sigma)));
    float Jp = pow(gamma, Gamma);
    float Fp = Lpm * Jp * exp(-Omega / sqrt(10.0) * (sqrt(k / kp) - 1.0));
    float alphap = 0.006 * sqrt(Omega);
    float Bl = 0.5 * alphap * cp / c * Fp;
    
    // Compute the small-scale wave spectrum
    float z0 = 0.000037 * square(U10) / g * pow(U10 / cp, 0.9);
    float uStar = 0.41 * U10 / log(10.0 / z0);
    float alpham = 0.01 * ((uStar < CM) ? (1.0 + log(uStar / CM)) : (1.0 + 3.0 * log(uStar / CM)));
    float Fm = exp(-0.25 * square(k / KM - 1.0));
    float Bh = 0.5 * alpham * CM / c * Fm * Lpm;
    
    // Compute directional spreading function
    float a0 = log(2.0) / 4.0;
    float am = 0.13 * uStar / CM;
    float Delta = tanh(a0 + 4.0 * pow(c / cp, 2.5) + am * pow(CM / c, 2.5));
    
    // Compute alignment of wave vector with wind direction
    float cosPhi = dot(normalize(u_PC.Wind), normalize(wave_vector));
    
    // Compute the full wave spectrum S(k)
    float S = (1.0 / (2.0 * PI)) * pow(k, -4.0) * (Bl + Bh) * (1.0 + Delta * (2.0 * cosPhi * cosPhi - 1.0));
    
    // Compute the spectrum amplitude in frequency domain
    float dk = 2.0 * PI / u_PC.OceanSize;
    float h = sqrt(S / 2.0) * dk;
    
    // Avoid infinite values for the zero frequency component
    if (wave_vector.x == 0.0 && wave_vector.y == 0.0) 
        h = 0.f;
    
    // Store the computed amplitude in the initial spectrum texture
    imageStore(o_InitialSpectrum, pixel_coord, vec4(h, 0.f, 0.f, 0.f));
}