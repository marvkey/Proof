#Compute Shader
#version 450 core
#include <Common.glslh>
#include <PBR/LightCulling/LightCulling.glslh>
//https://www.3dgep.com/forward-plus/#Forward
vec4 ScreenToViewVulkan(vec4 screen)
{
    // Convert to clip space
    vec4 clip = vec4(screen.xy * 2.0 - 1.0, screen.z, screen.w);

    // In Vulkan, the depth range is typically [0.0, 1.0].
    // So, we need to remap it to [-1.0, 1.0] in view space.
    //clip.z = clip.z * 2.0 - 1.0;

    // View space position.
    vec4 view = u_Camera.InverseProjection * clip;
    // Perspective projection.
    view = view / view.w;

    return view;
}
layout(push_constant) uniform PushConstants
{
    uvec3 NumThreads;
} u_PushData;
// ouy frusturm
layout(set = 1, binding = 5) buffer OutFrustums 
{
    Frustum out_Frustrums[];
};
layout(local_size_x = TILE_SIZE, local_size_y = TILE_SIZE, local_size_z = 1) in;
void main()
{
   
    // View space eye position is always at the origin.
    const vec3 eyePos = vec3(0.0, 0.0, 0.0);

    // Compute the 4 corner points on the far clipping plane to use as the 
    // frustum vertices.
    vec4 screenSpace[4];
    // Top left point
    screenSpace[0] = vec4(gl_GlobalInvocationID.xy * TILE_SIZE, 0.0, 1.0);
    // Top right point
    screenSpace[1] = vec4(vec2(gl_GlobalInvocationID.x + 1, gl_GlobalInvocationID.y) * TILE_SIZE, 0.0, 1.0);
    // Bottom left point
    screenSpace[2] = vec4(vec2(gl_GlobalInvocationID.x, gl_GlobalInvocationID.y + 1) * TILE_SIZE, 0.0, 1.0);
    // Bottom right point
    screenSpace[3] = vec4(vec2(gl_GlobalInvocationID.x + 1, gl_GlobalInvocationID.y + 1) * TILE_SIZE, 0.0, 1.0);

    vec3 viewSpace[4];
    // Now convert the screen space points to view space
    for (int i = 0; i < 4; i++)
    {
        viewSpace[i] = ScreenToViewVulkan(screenSpace[i]).xyz;
    }

    // Now build the frustum planes from the view space points
    Frustum frustum;

    // Left plane
    frustum.planes[0] = ComputePlane(eyePos, viewSpace[2], viewSpace[0]);
    // Right plane
    frustum.planes[1] = ComputePlane(eyePos, viewSpace[1], viewSpace[3]);
    // Top plane
    frustum.planes[2] = ComputePlane(eyePos, viewSpace[0], viewSpace[1]);
    // Bottom plane
    frustum.planes[3] = ComputePlane(eyePos, viewSpace[3], viewSpace[2]);

    // Store the computed frustum in global memory (if our thread ID is in bounds of the grid).
    //uint index = gl_GlobalInvocationID.x + (gl_GlobalInvocationID.y * numThreads.x);
    if (gl_GlobalInvocationID.x < u_PushData.NumThreads.x && gl_GlobalInvocationID.y < u_PushData.NumThreads.y) 
    {
        uint index = gl_GlobalInvocationID.x + ( gl_GlobalInvocationID.y * u_PushData.NumThreads.x );
        out_Frustrums[index] = frustum;
    
    }
}