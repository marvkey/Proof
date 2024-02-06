

#define GOLDEN_RATIO                       1.61803398875f


layout(set = 0, binding = 0) buffer SobolBuffer {
    uint sobolBuffer[];
};

layout(set = 0, binding = 1) buffer ScramblingTileBuffer {
    uint scramblingTileBuffer[];
};

layout(set = 0, binding = 2) buffer RankingTileBuffer {
    uint rankingTileBuffer[];
};

layout(set = 0, binding = 3, rgba8) writeonly uniform image2D blueNoiseTexture;

};

// Blue Noise Sampler by Eric Heitz. Returns a value in the range [0, 1].
float sampleRandomNumber(uint pixel_i, uint pixel_j, uint sample_index, uint sample_dimension, uint samples_per_pixel) {
    // Wrap arguments
    pixel_i = pixel_i & 127u;
    pixel_j = pixel_j & 127u;
    sample_index = (sample_index % samples_per_pixel) & 255u;
    sample_dimension = sample_dimension & 255u;

    #ifndef SPP
    #    define SPP 256
    #endif

    #if SPP == 1
        uint ranked_sample_index = sample_index ^ 0;
    #else
        // xor index based on optimized ranking
        uint ranked_sample_index = sample_index ^ rankingTileBuffer[sample_dimension + (pixel_i + pixel_j * 128u) * 8u];
    #endif

    // Fetch value in sequence
    uint value = sobolBuffer[sample_dimension + ranked_sample_index * 256u];

    // If the dimension is optimized, xor sequence value based on optimized scrambling
    value = value ^ scramblingTileBuffer[(sample_dimension % 8u) + (pixel_i + pixel_j * 128u) * 8u];

    // Convert to float and return
    return (float(value) + 0.5) / 256.0;
}

void main() {
    ivec2 dispatch_thread_id = ivec2(gl_GlobalInvocationID.xy);
    if (all(lessThan(dispatch_thread_id.xy, ivec2(128)))) {
        float2 xi = float2(
            sampleRandomNumber(uint(dispatch_thread_id.x), uint(dispatch_thread_id.y), g_frame_info_cb[0].frameIndex, 0u, g_frame_info_cb[0].randomSamplesPerPixel),
            sampleRandomNumber(uint(dispatch_thread_id.x), uint(dispatch_thread_id.y), g_frame_info_cb[0].frameIndex, 1u, g_frame_info_cb[0].randomSamplesPerPixel)
        );
        imageStore(blueNoiseTexture, dispatch_thread_id, vec4(xi, 0.0, 0.0));
    }
}