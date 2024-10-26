#Compute Shader
#version 450 core
#extension GL_ARB_separate_shader_objects : enable

layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;

layout(set = 0, binding = 0) uniform samplerCube u_CubeMap;
layout(set = 0, binding = 1, rgba16f) uniform writeonly image2D u_EquirectangularMap;

const float PI = 3.1415926535897932384626433832795f;

layout(push_constant) uniform CubeMapSize
{
    uvec2 equirectangularSize;
    uvec2 cubeMapSize;
} u_PushData;


// Function to convert spherical coordinates to cubemap coordinates
vec3 GetCubemapTexCoord(float phi, float theta)
{
    float x = sin(theta) * cos(phi);
    float y = cos(theta);
    float z = sin(theta) * sin(phi);
    return normalize(vec3(x, y, z));
}

void main()
{
    // Get the current pixel coordinates on the equirectangular map
    ivec2 pixelCoords = ivec2(gl_GlobalInvocationID.xy);
    if (pixelCoords.x >= u_PushData.equirectangularSize.x || pixelCoords.y >= u_PushData.equirectangularSize.y) return;

    // Convert pixel coordinates to spherical coordinates
    vec2 uv = vec2(pixelCoords) / vec2(u_PushData.equirectangularSize);
    float phi = (uv.x - 0.5) * 2.0 * PI; // Longitude [-π, π]
    float theta = uv.y * PI;              // Latitude [0, π]

    // Convert spherical coordinates to cubemap coordinates
    vec3 cubeTC = GetCubemapTexCoord(phi, theta);

    // Sample the cubemap texture
    vec4 color = texture(u_CubeMap, cubeTC);

    // Store the resulting color in the equirectangular map
    imageStore(u_EquirectangularMap, pixelCoords, color);
}