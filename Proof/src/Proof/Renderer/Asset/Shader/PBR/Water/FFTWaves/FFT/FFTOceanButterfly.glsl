//https://github.com/2Retr0/GodotOceanWaves/blob/4e1e219bc8f55c38092ed34e6fee568b900d95b5/assets/shaders/compute/fft_butterfly.glsl
#Compute Shader

#version 460
/** Precomputes the ButterflyFactors for a Stockham FFT kernel */

#define PI (3.141592653589793)

layout(local_size_x = 64, local_size_y = 1, local_size_z = 1) in;

layout(std430, set = 0, binding = 0) restrict writeonly buffer FFTBuffer 
{
	vec4 Butterfly[]; // log2(MapSize) x MapSize
};

/** Returns exp(j*x) assuming x >= 0. */
vec2 ExpComplex(in float x) 
{
	return vec2(cos(x), sin(x));
}

#define BUTTERFLY(col, stage) (Butterfly[(stage)*MapSize + (col)])
void main() {
	const uint MapSize = gl_NumWorkGroups.x * gl_WorkGroupSize.x * 2;
	const uint col = gl_GlobalInvocationID.x;   // Column in row
	const uint stage = gl_GlobalInvocationID.y; // Stage of FFT

	uint stride = 1 << stage, mid = MapSize >> (stage + 1);
	uint i = col >> stage, j = col % stride;

	vec2 twiddleFactor = ExpComplex(PI / float(stride) * float(j));
	uint r0 = stride*(i +   0) + j, r1 = stride*(i + mid) + j;
	uint w0 = stride*(2*i + 0) + j, w1 = stride*(2*i + 1) + j;

	vec2 readIndices = vec2(uintBitsToFloat(r0), uintBitsToFloat(r1));

	BUTTERFLY(w0, stage) = vec4(readIndices,  twiddleFactor);
	BUTTERFLY(w1, stage) = vec4(readIndices, -twiddleFactor);
}
