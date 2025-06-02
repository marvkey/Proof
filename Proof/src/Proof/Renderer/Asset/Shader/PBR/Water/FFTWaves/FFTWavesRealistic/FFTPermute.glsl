//https://github.com/Popov72/OceanDemo/blob/3b9866a1e8ac36e27025f5eb708f1fa5bf21564b/src/assets/ocean/fftInverseFFT3.wgsl

#Compute Shader
#version 450

layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in;

// === Bindings ===
layout(binding = 5) uniform sampler2D InputBuffer;
layout(binding = 6, rg32f) uniform writeonly image2D OutputBuffer;

void main() {
    ivec2 id = ivec2(gl_GlobalInvocationID.xy);
    // Load input value
    vec2 inpute = texelFetch(InputBuffer, id, 0).xy;

    // Multiply by (-1)^((x + y) % 2)
    float sign = 1.0 - 2.0 * float((id.x + id.y) % 2);


    imageStore(OutputBuffer, id, vec4(inpute * sign, 0.0, 0.0));

}
