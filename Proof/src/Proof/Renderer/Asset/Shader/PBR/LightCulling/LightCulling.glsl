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


layout (local_size_x = TILE_SIZE,local_size_y = TILE_SIZE) in;
void main()
{

    if (gl_GlobalInvocationID.x >= u_PushData.NumThreads.x || gl_GlobalInvocationID.y >= u_PushData.NumThreads.y) 
    {
		return;
	}

    uint index = gl_GlobalInvocationID.y * uint(u_PushData.NumThreads.x) + gl_GlobalInvocationID.x;	

    uint lightIndexBegin = index * MAX_NUM_LIGHTS_PER_TILE;
    uint numPointLightsInTile = 0;
    vec2 texcoordUnit = vec2(gl_GlobalInvocationID.xy)/u_ScreenData.FullResolution;

    float zNear = -10000000;
	float zFar = 10000000;

    for (int i = 0; i < TILE_SIZE; ++i) 
	{
		for (int j = 0; j < TILE_SIZE; ++j) 
		{
			vec2 offset = vec2(i + 0.5, j + 0.5);
			vec2 texcoord = (gl_GlobalInvocationID.xy * TILE_SIZE + offset) * texcoordUnit;
			texcoord.y = 1. - texcoord.y;
			float depth = texture(u_DepthTexture, texcoord).x;
			vec4 screenDepth = vec4(texcoord, depth, 1.);
			vec4 viewDepth = ScreenToView(screenDepth);

			zNear = max(zNear, viewDepth.z);
			zFar = min(zFar, viewDepth.z);
		}
	}


	float diff = zNear - zFar; // distance
    zFar -= diff;
    zNear += diff;


	for(uint i = 0; i < u_LightData.PointLightCount; i++)
	{
        PointLight pointLight = s_PointLights.Lights[i];
		vec4 viewPos = vec4(pointLight.Position,1) * u_Camera.View;
        Sphere sphere = { viewPos.xyz, pointLight.Radius };

		if ( SphereInsideFrustum( sphere, u_Frustrums[index], zNear, zFar ) )
		{
				s_PointLightIndexList[lightIndexBegin + numPointLightsInTile] = i;
				numPointLightsInTile++;
				if(numPointLightsInTile >= MAX_NUM_LIGHTS_PER_TILE)
					break;
		}
	}

	s_PointLightGrid[index] = numPointLightsInTile;
}
