#Compute Shader
#version 450 core
#include <Common.glslh>
#include <PBR/LightCulling/LightCulling.glslh>
#include <PBR/Lights.glslh>

#define PIXELS_PER_TILE 16


#define BLOCK_SIZE 16
#define MAX_NUM_LIGHTS_PER_TILE 1024

layout(set = 1, binding = 0) uniform sampler2D u_DpethTexture;

// Precomputed frustums for the grid.
layout(set = 1, binding = 1) buffer Frustrums
{
    Frustum u_Frustrums[];
};

layout(push_constant) uniform PushConstants
{
    uvec2 NumThreads;
} u_PushData;



layout (local_size_x = 16, local_size_y = 16) in;
void main()
{
if (gl_GlobalInvocationID.x >= u_PushData.NumThreads.x
	    || gl_GlobalInvocationID.y >= u_PushData.NumThreads.y) {
		return;
	}

	// tile index
	uint index = gl_GlobalInvocationID.y * uint(u_PushData.NumThreads.x)
		+ gl_GlobalInvocationID.x;
	uint lightIndexBegin = index * MAX_NUM_LIGHTS_PER_TILE;
	uint numLightsInTile = 0;
	vec2 texcoordUnit = 1. / u_ScreenData.FullResolution;
	float zNear = -1000000.;
	float zFar = 1000000.;

	for (int i = 0; i < PIXELS_PER_TILE; ++i) {
		for (int j = 0; j < PIXELS_PER_TILE; ++j) {
			vec2 offset = vec2(i + 0.5, j + 0.5);
			vec2 texcoord = (gl_GlobalInvocationID.xy * PIXELS_PER_TILE + offset) * texcoordUnit;
			texcoord.y = 1. - texcoord.y;
			float depth = texture(depthSampler, texcoord).x;
			vec4 screenDepth = vec4(texcoord, depth, 1.);
			vec4 viewDepth = ScreenToView(screenDepth);

			zNear = max(zNear, viewDepth.z);
			zFar = min(zFar, viewDepth.z);
		}
	}

    float diff = zNear - zFar; // distance
    zFar -= diff;
    zNear += diff;

	// lights[index].beginPos = vec4(minDepth, maxDepth, 0., 0.);

	for (int i = 0; i < u_LightData.PointLightCount; ++i) {
		float radius =;

		if (SphereInsideFrustum(pos.xyz, radius, frustums[index], zNear, zFar)) {
			s_PointLightIndexList[lightIndexBegin + numLightsInTile] = i;
			numLightsInTile += 1;
			if(numLightsInTile >= MAX_NUM_LIGHTS_PER_TILE)
				break;
		}
	}

	u_PointlightGrid[index] = int(numLightsInTile);
}

