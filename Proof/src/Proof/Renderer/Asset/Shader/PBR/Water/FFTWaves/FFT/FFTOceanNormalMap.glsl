// This compute shader generates a normal map from the displacement map of an FFT ocean simulation. 
// It calculates normals by taking the differences in displacement values across neighboring texels 
// and averaging them to produce smooth shading. The computed normals are stored in an output texture

#Compute Shader
#version 450 core
//https://github.com/achalpandeyy/OceanFFT/blob/master/Source/Shaders/CS_NormalMap.comp

#define WORK_GROUP_DIM 32

layout (local_size_x = WORK_GROUP_DIM, local_size_y = WORK_GROUP_DIM) in;

// Input displacement map generated from FFT ocean simulation
layout (binding = 0, rgba32f) readonly uniform image2D u_DisplacementMap;

// Output normal map storing surface normals for shading
layout (binding = 1, rgba32f) writeonly uniform image2D o_NormalMap;
// Push constants for resolution and ocean size parameters
layout(push_constant) uniform FFTConstants
{
    int Resolution;  // Grid resolution (number of points per axis)
    int OceanSize;   // Physical size of the ocean grid
} u_PC;

void main()
{
	 // Compute the current pixel coordinates in the output texture
    ivec2 pixelCoord = ivec2(gl_GlobalInvocationID.xy);

    // Compute texel size in world space
    float texel = 1.0 / u_PC.Resolution;
    float texelSize = u_PC.OceanSize * texel;

	  // Sample displacement values from the FFT ocean height map
    vec3 center = imageLoad(u_DisplacementMap, pixelCoord).xyz;
    vec3 right  = vec3(texelSize, 0.0, 0.0) + imageLoad(u_DisplacementMap, ivec2(clamp(pixelCoord.x + 1, 0, u_PC.Resolution - 1), pixelCoord.y)).xyz - center;
    vec3 left   = vec3(-texelSize, 0.0, 0.0) + imageLoad(u_DisplacementMap, ivec2(clamp(pixelCoord.x - 1, 0, u_PC.Resolution - 1), pixelCoord.y)).xyz - center;
    vec3 top    = vec3(0.0, 0.0, -texelSize) + imageLoad(u_DisplacementMap, ivec2(pixelCoord.x, clamp(pixelCoord.y - 1, 0, u_PC.Resolution - 1))).xyz - center;
    vec3 bottom = vec3(0.0, 0.0, texelSize) + imageLoad(u_DisplacementMap, ivec2(pixelCoord.x, clamp(pixelCoord.y + 1, 0, u_PC.Resolution - 1))).xyz - center;

    // Compute cross products to determine surface normal using surrounding height differences
    vec3 topRight    = cross(right, top);
    vec3 topLeft     = cross(top, left);
    vec3 bottomLeft  = cross(left, bottom);
    vec3 bottomRight = cross(bottom, right);

    // Average the four normals for smooth shading
    vec3 normal = normalize(topRight + topLeft + bottomRight + bottomLeft);
    
    // Store computed normal in the output texture
    imageStore(o_NormalMap, pixelCoord, vec4(normal, 1.0));

}