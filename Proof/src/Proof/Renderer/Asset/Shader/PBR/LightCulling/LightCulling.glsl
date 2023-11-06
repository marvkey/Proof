#Compute Shader
#version 450 core
#include <Common.glslh>
#include <PBR/Lights.glslh>

//https://github.com/InCloudsBelly/X2_RenderingEngine/blob/e7c349b70bd95af3ab673556cdb56cb2cc40b48e/Resources/Shaders/LightCulling.glsl
//https://github.com/bcrusco/Forward-Plus-Renderer/blob/master/Forward-Plus/Forward-Plus/source/shaders/light_culling.comp.glsl
layout(set = 1, binding = 0) uniform sampler2D u_DepthTexture;



//https://www.3dgep.com/forward-plus/#Compute_MinMax_Depth_Values

struct Frustum 
{
    vec4 Planes[6]; // Array to store the six frustum planes
};
struct Sphere
{
    vec3 Center;   // Center point.
    float Radius;   // Radius.
};
bool IsSphereInFrustum(Sphere sphere, Frustum frustum) 
{
	    for (int i = 0; i < 6; i++) 
		{
			float distance = dot(frustum.Planes[i], vec4(sphere.Center, 1.0));
			if (distance < -sphere.Radius) 
			{
				// The sphere is completely outside this frustum plane.
				return false;
			}
        }
    return true;
}
bool IsSphereInsidePlane(Sphere sphere, vec4 plane)
{
    // Calculate the signed distance from the sphere center to the plane.
    float distance = dot(vec4(sphere.Center, 1.0), plane);

    // If the distance is greater than or equal to the negative radius, the sphere is inside the plane.
    return distance >= -sphere.Radius;
}
float ScreenSpaceToViewSpaceDepth(const float screenDepth)
{
	float depthLinearizeMul = -u_Camera.Projection[3][2];
	float depthLinearizeAdd = u_Camera.Projection[2][2];

	return depthLinearizeMul / (screenDepth +depthLinearizeAdd );
}

shared uint minDepthInt;
shared uint maxDepthInt;
shared uint visiblePointLightCount;
shared Frustum frustrumPlane;
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
    uint depthInt = floatBitsToUint(-linearDepth);
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
		frustrumPlane.Planes[0] = vec4(1.0, 0.0, 0.0, 1.0 - negativeStep.x); // Left
		frustrumPlane.Planes[1] = vec4(-1.0, 0.0, 0.0, -1.0 + positiveStep.x); // Right
		frustrumPlane.Planes[2] = vec4(0.0, 1.0, 0.0, 1.0 - negativeStep.y); // Bottom
		frustrumPlane.Planes[3] = vec4(0.0, -1.0, 0.0, -1.0 + positiveStep.y); // Top
		frustrumPlane.Planes[4] = vec4(0.0, 0.0, -1.0, -minDepth); // Near
		frustrumPlane.Planes[5] = vec4(0.0, 0.0, 1.0, maxDepth); // Far

		// Transform the first four planes
		for (uint i = 0; i < 4; i++)
		{
		    frustrumPlane.Planes[i] *= u_Camera.ViewProjectionMatrix;
		    frustrumPlane.Planes[i] /= length(frustrumPlane.Planes[i].xyz);
		}

		// Transform the depth planes
		frustrumPlane.Planes[4] *= u_Camera.View;
		frustrumPlane.Planes[4] /= length(frustrumPlane.Planes[4].xyz);
		frustrumPlane.Planes[5] *= u_Camera.View;
		frustrumPlane.Planes[5] /= length(frustrumPlane.Planes[5].xyz);
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

		Sphere sphere;
		sphere.Center = s_PointLights.Lights[lightIndex].Position;
		sphere.Radius = s_PointLights.Lights[lightIndex].Radius;
		if(IsSphereInFrustum(sphere,frustrumPlane))
		{
			uint offset = atomicAdd(visiblePointLightCount, 1);
			visiblePointLightIndices[offset] = int(lightIndex);
		}
    }

	barrier();

    // One thread should fill the global light buffer
    if (gl_LocalInvocationIndex == 0)
    {
		
		const uint offset = index * MAX_NUM_LIGHTS_PER_TILE; // Determine position in global buffer

		for (uint i = 0; i < visiblePointLightCount; i++) 
		{
			s_PointLightIndexList.Indices[offset + i] = visiblePointLightIndices[i];
		}

		if (visiblePointLightCount != MAX_NUM_LIGHTS_PER_TILE)
		{
		    // Unless we have totally filled the entire array, mark it's end with -1
		    // Final shader step will use this to determine where to stop (without having to pass the light count)
			s_PointLightIndexList.Indices[offset + visiblePointLightCount] = -1;
		}

	}


}
