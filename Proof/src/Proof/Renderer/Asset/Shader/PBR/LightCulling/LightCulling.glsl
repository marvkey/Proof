#Compute Shader
#version 450 core
#include <Common.glslh>
#include <PBR/LightCulling/LightCulling.glslh>
#include <PBR/Lights.glslh>

//https://www.3dgep.com/forward-plus/#Forward
//https://anteru.net/blog/2016/mapping-between-HLSL-and-GLSL/
//https://github.com/snowzurfer/vulkan-forward-plus/blob/master/assets/shaders/light_culling.comp
//https://github.com/zimengyang/ForwardPlus_Vulkan/blob/master/src/shaders/computeLightList.comp
//layout(push_constant) uniform PushConstants
//{
//    uvec3 NumThreads;
//} u_PushData;
// The depth from the screen space texture.
layout(set = 1, binding = 0) uniform sampler2D u_DepthTexture;

// Precomputed frustums for the grid.
layout(set = 1, binding = 1) buffer Frustrums
{
    Frustum u_Frustrums[];
};
layout(push_constant) uniform PushConstants
{
    uvec2 NumThreads;
} u_PushData;


float ScreenSpaceToViewSpaceDepth(const float screenDepth)
{
	float depthLinearizeMul = -u_Camera.Projection[3][2];
	float depthLinearizeAdd = u_Camera.Projection[2][2];

	return depthLinearizeMul / (depthLinearizeAdd - screenDepth);
}

shared uint minDepthInt;
shared uint maxDepthInt;
shared uint visiblePointLightCount;
shared vec4 frustumPlanes[6];
shared int visiblePointLightIndices[MAX_NUM_LIGHTS_PER_TILE];

layout (local_size_x = TILE_SIZE,local_size_y = TILE_SIZE) in;
void main()
{

    ivec2 location = ivec2(gl_GlobalInvocationID.xy);
    ivec2 itemID = ivec2(gl_LocalInvocationID.xy);
    ivec2 tileID = ivec2(gl_WorkGroupID.xy);
    ivec2 tileNumber = ivec2(gl_NumWorkGroups.xy);
    uint index = tileID.y * tileNumber.x + tileID.x;

    // Initialize shared global values for depth and light count
    if (gl_LocalInvocationIndex == 0)
    {
		minDepthInt = 0xFFFFFFFF;
		maxDepthInt = 0;
		visiblePointLightCount = 0;
		//visibleSpotLightCount = 0;
    }

    barrier();

        // Step 1: Calculate the minimum and maximum depth values (from the depth buffer) for this group's tile
    vec2 tc = vec2(location) / u_ScreenData.FullResolution;
    float linearDepth = ScreenSpaceToViewSpaceDepth(textureLod(u_DepthTexture, tc, 0).r);

    // Convert depth to uint so we can do atomic min and max comparisons between the threads
    uint depthInt = floatBitsToUint(linearDepth);
    atomicMin(minDepthInt, depthInt);
    atomicMax(maxDepthInt, depthInt);

    barrier();
    if (gl_LocalInvocationIndex == 0)
    {
		// Convert the min and max across the entire tile back to float
		float minDepth = uintBitsToFloat(minDepthInt);
		float maxDepth = uintBitsToFloat(maxDepthInt);

		// Steps based on tile sale
		vec2 negativeStep = (2.0 * vec2(tileID)) / vec2(tileNumber);
		vec2 positiveStep = (2.0 * vec2(tileID + ivec2(1, 1))) / vec2(tileNumber);

		// Set up starting values for planes using steps and min and max z values
		frustumPlanes[0] = vec4(1.0, 0.0, 0.0, 1.0 - negativeStep.x); // Left
		frustumPlanes[1] = vec4(-1.0, 0.0, 0.0, -1.0 + positiveStep.x); // Right
		frustumPlanes[2] = vec4(0.0, 1.0, 0.0, 1.0 - negativeStep.y); // Bottom
		frustumPlanes[3] = vec4(0.0, -1.0, 0.0, -1.0 + positiveStep.y); // Top
		frustumPlanes[4] = vec4(0.0, 0.0, -1.0, -minDepth); // Near
		frustumPlanes[5] = vec4(0.0, 0.0, 1.0, maxDepth); // Far

		// Transform the first four planes
		for (uint i = 0; i < 4; i++)
		{
		    frustumPlanes[i] *= u_Camera.ViewProjectionMatrix;
		    frustumPlanes[i] /= length(frustumPlanes[i].xyz);
		}

		// Transform the depth planes
		frustumPlanes[4] *= u_Camera.View;
		frustumPlanes[4] /= length(frustumPlanes[4].xyz);
		frustumPlanes[5] *= u_Camera.View;
		frustumPlanes[5] /= length(frustumPlanes[5].xyz);
    }

	 // Step 3: Cull lights.
    // Parallelize the threads against the lights now.
    // Can handle 256 simultaniously. Anymore lights than that and additional passes are performed
    const uint threadCount = TILE_SIZE * TILE_SIZE;
	uint passCount = (u_LightData.PointLightCount + threadCount - 1) / threadCount;

	for (uint i = 0; i < passCount; i++)
    {
		// Get the lightIndex to test for this thread / pass. If the index is >= light count, then this thread can stop testing lights
		uint lightIndex = i * threadCount + gl_LocalInvocationIndex;
		if (lightIndex >= u_LightData.PointLightCount)
		    break;

		vec4 position = vec4(s_PointLights.Lights[lightIndex].Position, 1.0f);
		float radius = s_PointLights.Lights[lightIndex].Radius;
		//radius += radius * 0.3f;

		// Check if light radius is in frustum
		float distance = 0.0;
		for (uint j = 0; j < 6; j++)
		{
		    distance = dot(position, frustumPlanes[j]) + radius;
		    if (distance <= 0.0) // No intersection
				break;
		}

		// If greater than zero, then it is a visible light
		if (distance <= 0.0)
		{
		    // Add index to the shared array of visible indices
		    uint offset = atomicAdd(visiblePointLightCount, 1);
		    visiblePointLightIndices[offset] = -1;
		}

    }

	barrier();

    // One thread should fill the global light buffer
    if (gl_LocalInvocationIndex == 0)
    {
		
		const uint offset = index * MAX_NUM_LIGHTS_PER_TILE; // Determine position in global buffer

		for (uint i = 0; i < visiblePointLightCount; i++) 
		{
			//s_PointLightIndexList[offset + i] = visiblePointLightIndices[i];
			s_PointLightIndexList.Indices[offset + i] =-1;
		}

		if (visiblePointLightCount != MAX_NUM_LIGHTS_PER_TILE)
		{
		    // Unless we have totally filled the entire array, mark it's end with -1
		    // Final shader step will use this to determine where to stop (without having to pass the light count)
			s_PointLightIndexList.Indices[offset + visiblePointLightCount] = -1;
		}

	}


}
