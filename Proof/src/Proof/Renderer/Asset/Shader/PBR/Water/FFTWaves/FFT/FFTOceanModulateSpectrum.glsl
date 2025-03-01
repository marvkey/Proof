//https://github.com/2Retr0/GodotOceanWaves/blob/4e1e219bc8f55c38092ed34e6fee568b900d95b5/assets/shaders/compute/spectrum_modulate.glsl
#Compute Shader
#version 460
/**
 * Modulates the JONSWAP wave spectra texture in time and calculates
 * its gradients. Since the outputs are all real-valued, they are packed
 * in pairs.
 *
 * Sources: Jerry Tessendorf - Simulating Ocean Water
 *          Robert Matusiak - Implementing Fast Fourier Transform Algorithms of Real-Valued Sequences With the TMS320 DSP Platform
 */

#define PI          (3.141592653589793)
#define GRAVITY     (9.81)
#define NUM_SPECTRA (4U)

layout(local_size_x = 16, local_size_y = 16, local_size_z = 1) in;

layout(rgba16f, set = 0, binding = 0) restrict readonly uniform image2DArray u_WaveSpectrumTexture;

layout(std430, set = 1, binding = 0) restrict writeonly buffer FFTBuffer {
	vec2 waveData[];
};

layout(push_constant) restrict readonly uniform PushConstants {
	vec2 tileSize;
	float oceanDepth;
	float simulationTime;

	uint cascadeLayerIndex;
}u_PC;

/** Computes exp(j*x) assuming x >= 0. */
vec2 ComputeExponentialComplex(in float angle) {
	return vec2(cos(angle), sin(angle));
}

/** Multiplies two complex numbers (a0 + j*a1) * (b0 + j*b1) */
vec2 MultiplyComplexNumbers(in vec2 firstComplex, in vec2 secondComplex) {
	return vec2(
		firstComplex.x * secondComplex.x - firstComplex.y * secondComplex.y, 
		firstComplex.x * secondComplex.y + firstComplex.y * secondComplex.x
	);
}

/** Returns the complex conjugate of a given complex number */
vec2 ComputeComplexConjugate(in vec2 complexValue) {
	return vec2(complexValue.x, -complexValue.y);
}

// Dispersion relation based on Jerry Tessendorf's ocean simulation model
float ComputeWaveDispersion(in float waveNumber) {
	return sqrt(GRAVITY * waveNumber * tanh(waveNumber * u_PC.oceanDepth));
}

#define FFT_WAVE_DATA(index, layer) (waveData[(index.z) * gridSize * gridSize * NUM_SPECTRA * 2 + (layer) * gridSize * gridSize + (index.y) * gridSize + (index.x)])

void main() {
	const uint gridSize = gl_NumWorkGroups.x * gl_WorkGroupSize.x;
	const uint numFFTStages = findMSB(gridSize); // log2(gridSize), assuming gridSize is a power of 2
	const ivec2 spectrumTextureDimensions = imageSize(u_WaveSpectrumTexture).xy;
	const ivec3 threadIndex = ivec3(gl_GlobalInvocationID.xy, u_PC.cascadeLayerIndex);

	vec2 waveVector = (threadIndex.xy - spectrumTextureDimensions * 0.5) * 2.0 * PI / u_PC.tileSize;
	float waveNumber = length(waveVector) + 1e-6;
	vec2 waveUnitVector = waveVector / waveNumber;

	// --- WAVE SPECTRUM MODULATION ---
	vec4 spectrumData = imageLoad(u_WaveSpectrumTexture, threadIndex);
	float wavePhaseShift = ComputeWaveDispersion(waveNumber) * u_PC.simulationTime;
	vec2 phaseModulation = ComputeExponentialComplex(wavePhaseShift);

	vec2 complexWaveAmplitude = MultiplyComplexNumbers(spectrumData.xy, phaseModulation) 
		+ MultiplyComplexNumbers(spectrumData.zw, ComputeComplexConjugate(phaseModulation));
	vec2 complexWaveAmplitudeInverted = vec2(-complexWaveAmplitude.y, complexWaveAmplitude.x);

	// --- WAVE DISPLACEMENT CALCULATION ---
	vec2 displacementX = complexWaveAmplitudeInverted * waveUnitVector.y;
	vec2 displacementY = complexWaveAmplitude;
	vec2 displacementZ = complexWaveAmplitudeInverted * waveUnitVector.x;

	// --- WAVE GRADIENT CALCULATION ---
	vec2 gradientY_X = complexWaveAmplitudeInverted * waveVector.y;
	vec2 gradientY_Z = complexWaveAmplitudeInverted * waveVector.x;
	vec2 gradientX_X = -complexWaveAmplitude * waveVector.y * waveUnitVector.y;
	vec2 gradientZ_Z = -complexWaveAmplitude * waveVector.x * waveUnitVector.x;
	vec2 gradientZ_X = -complexWaveAmplitude * waveVector.y * waveUnitVector.x;

	// Packing into FFT buffer
	FFT_WAVE_DATA(threadIndex, 0) = vec2(displacementX.x - displacementY.y, displacementX.y + displacementY.x);
	FFT_WAVE_DATA(threadIndex, 1) = vec2(displacementZ.x - gradientY_X.y, displacementZ.y + gradientY_X.x);
	FFT_WAVE_DATA(threadIndex, 2) = vec2(gradientY_Z.x - gradientX_X.y, gradientY_Z.y + gradientX_X.x);
	FFT_WAVE_DATA(threadIndex, 3) = vec2(gradientZ_Z.x - gradientZ_X.y, gradientZ_Z.y + gradientZ_X.x);
}
