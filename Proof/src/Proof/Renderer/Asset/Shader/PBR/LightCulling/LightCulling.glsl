#Compute Shader
#version 450 core
#include <Common.glslh>
#include <PBR/LightCulling/LightCulling.glslh>
#include <PBR/Lights.glslh>

//https://www.3dgep.com/forward-plus/#Forward
//https://anteru.net/blog/2016/mapping-between-HLSL-and-GLSL/
//layout(push_constant) uniform PushConstants
//{
//    uvec3 NumThreads;
//} u_PushData;
// The depth from the screen space texture.
layout(set = 1, binding = 0) uniform sampler2D u_DpethTexture;

// Precomputed frustums for the grid.
layout(set = 1, binding = 1) buffer Frustrums
{
    Frustum u_Frustrums[];
};
layout(push_constant) uniform PushConstants
{
    uvec3 NumThreads;
} u_PushData;
layout(set = 1, binding = 2) coherent buffer PointLightIndexCounterBuffer {
    uint o_PointLightIndexCounter[];
};

layout(set = 1, binding = 3) coherent buffer SpotLightIndexCounterBuffer {
    uint o_SpotLightIndexCounter[];
};


layout(set = 1, binding = 4,rg32ui) uniform uimage2D u_ImagePointLightGrid;
layout(set = 1, binding = 5,rg32ui) uniform uimage2D u_ImageSpotLightGrid;
shared uint uMinDepth;
shared uint uMaxDepth;
shared Frustum GroupFrustum;

// o is opaque geo and t is transpaerant
shared uint o_PointLightCount; // for certaion tile
shared uint o_PointLightList[MAX_LIGHT_COUNT]; // visible lights
shared uint o_PointLightIndexStartOffset;

shared uint o_SpotLightCount; // for tile
shared uint o_SpotLightList[MAX_LIGHT_COUNT]; // visible lights
shared uint o_SpotLightIndexStartOffset;

//shared uint t_PointLightCount;
//shared uint t_PointLightIndexStartOffset;
//shared uint t_PointLightList[MAX_LIGHT_COUNT];
//
//shared uint t_SpotLightCount;
//shared uint t_SpotLightIndexStartOffset;
//shared uint t_SpotLightList[MAX_LIGHT_COUNT];

// Add the light to the visible light list for opaque geometry.
void o_AppendPointLight(uint lightIndex)
{
    uint index = atomicAdd(o_PointLightCount, 1);
    if (index < MAX_LIGHT_COUNT)
    {
        o_PointLightList[index] = lightIndex;
    }
}

void o_AppendSpotLight(uint lightIndex)
{
    uint index = atomicAdd(o_SpotLightCount, 1);
    if (index < MAX_LIGHT_COUNT)
    {
        o_SpotLightList[index] = lightIndex;
    }
}
vec4 ScreenToViewVulkan(vec4 screen)
{
    // Convert to clip space
    vec4 clip = vec4(screen.xy * 2.0 - 1.0, screen.z, screen.w);

    // In Vulkan, the depth range is typically [0.0, 1.0].
    // So, we need to remap it to [-1.0, 1.0] in view space.
    clip.z = clip.z * 2.0 - 1.0;

    // View space position.
    vec4 view = u_Camera.InverseProjection * clip;
    // Perspective projection.
    view = view / view.w;

    return view;
}
// Add the light to the visible light list for transparent geometry.
//void t_AppendLight(uint lightIndex)
//{
//    uint index = atomicAdd(t_LightCount, 1);
//    if (index < MAX_LIGHT_COUNT)
//    {
//        t_LightList[index] = lightIndex;
//    }
//}
//
//void t_AppendLight(uint lightIndex)
//{
//    uint index = atomicAdd(t_LightCount, 1);
//    if (index < MAX_LIGHT_COUNT)
//    {
//        t_LightList[index] = lightIndex;
//    }
//}

layout (local_size_x = TILE_SIZE,local_size_y = TILE_SIZE) in;
void main()
{
	
    uvec2 texCoord = gl_GlobalInvocationID.xy;
    float floatDepth = textureLod(u_DpethTexture, texCoord,0).r;
    //vec2 tc = vec2(texCoord) / u_ScreenData.FullResolution;
    //float floatDepth = ScreenSpaceToViewSpaceDepth(textureLod(u_DpethTexture, tc,0).r);
    uint unsignedDepth = floatBitsToUint(-floatDepth );

    if (gl_LocalInvocationIndex == 0)
    {
        uMinDepth = 0xffffffff;
        uMaxDepth = 0;
        o_PointLightCount = 0;
        o_SpotLightCount = 0;
       // t_LightCount = 0;
        GroupFrustum = u_Frustrums[gl_WorkGroupID.x + (gl_WorkGroupID.y * gl_NumWorkGroups.x)];
    }

    barrier(); 

    atomicMin(uMinDepth, unsignedDepth);
    atomicMax(uMaxDepth, unsignedDepth);
    
    barrier(); 
    /*
        The view space depth values are computed using the ScreenToView function and extracting the z component of the position in view space. We only need these values to compute the near and far clipping planes in view space so we only need to know the distance from the viewer.

        When culling lights for transparent geometry, we don’t want to use the minimum depth value from the depth map. Instead we will clip the lights using the camera’s near clipping plane. In this case, we will use the nearClipVS value which is the distance to the camera’s near clipping plane in view space.

        Since I’m using a right-handed coordinate system with the camera pointing towards the negative z axis in view space, the minimum depth clipping plane is computed with a normal n
         pointing in the direction of the negative z axis and the distance to the origin d
         is -minDepth. We can verify that this is correct by using the constant-normal form of a plane:

        (n⋅X)−d=0
        By substituting n=(0,0,−1)
        , X=(x,y,z)
         and d=−zmin
         we get:

        ((0,0,−1)⋅(x,y,z))−(−zmin)0x+0y+(−1)z−(−zmin)(−1)z−(−zmin)−zz=====000−zminzmin
        Which implies that (0,0,zmin)
         is a point on the minimum depth clipping plane.
    */
    float fMinDepth = uintBitsToFloat(uMinDepth);
    float fMaxDepth = uintBitsToFloat(uMaxDepth);

    // Convert depth values to view space.
    float minDepthVS = ScreenToViewVulkan(vec4(0.0, 0.0, fMinDepth,1 )).z;
    float maxDepthVS = ScreenToViewVulkan(vec4(0.0, 0.0, fMaxDepth,1 )).z;
    float nearClipVS = ScreenToViewVulkan(vec4(0.0, 0.0, 0.0, 1.0)).z;

    // Clipping plane for minimum depth value 
    // (used for testing lights within the bounds of opaque geometry).
    Plane minPlane = Plane(vec3(0.0, 0.0, -1.0), -minDepthVS);

     //cull point lights
     for ( uint i = gl_LocalInvocationIndex; i < u_LightData.PointLightCount; i += TILE_SIZE * TILE_SIZE )
     {
        PointLight pointLight = s_PointLights.Lights[i];
        vec3 viewPos =( vec4(pointLight.Position,1) * u_Camera.View).xyz;
        Sphere sphere = { viewPos, pointLight.Radius };

        if ( SphereInsideFrustum( sphere, GroupFrustum, nearClipVS, maxDepthVS ) )
        {
            //Add light to light list for transparent geometry.
            //t_AppendLight( i );
                o_AppendPointLight( i );

            if ( !SphereInsidePlane( sphere, minPlane ) )
            {
                // Add light to light list for opaque geometry.
                //o_AppendPointLight( i );
           }
        }
     }

     //cul spot lights
     for ( uint i = gl_LocalInvocationIndex; i < u_LightData.SpotLightCount; i += TILE_SIZE * TILE_SIZE )
     {
        SpotLight spotLight = s_SpotLights.Lights[i];

        vec4 viewPos = vec4(spotLight.Position,1) * u_Camera.View;
        vec4 viewDirection = vec4(spotLight.Direction,0)* u_Camera.View;
        viewDirection = normalize(viewDirection);
        float coneRadius = tan( radians( spotLight.Angle ) ) * spotLight.Range;
            
        Cone cone = { viewPos.xyz, spotLight.Range, viewDirection.xyz, coneRadius };
        if ( ConeInsideFrustum( cone, GroupFrustum, nearClipVS, maxDepthVS ) )
        {
            // Add light to light list for transparent geometry.
            //t_AppendLight( i );
            if (!ConeInsidePlane( cone, minPlane ))
            {
                // Add light to light list for opaque geometry.
                o_AppendSpotLight( i );
            }
        }
     }
     barrier();

  //  if (gl_LocalInvocationIndex == 0)
  //  {
        // Update light grid for opaque geometry.
        o_PointLightIndexStartOffset = atomicAdd(o_PointLightIndexCounter[0], o_PointLightCount);
        imageStore(u_ImagePointLightGrid, ivec2(gl_WorkGroupID.xy), uvec4(o_PointLightIndexStartOffset, o_PointLightCount,0,0));

        o_SpotLightIndexStartOffset = atomicAdd(o_SpotLightIndexCounter[0], o_SpotLightCount);
        imageStore(u_ImageSpotLightGrid, ivec2(gl_WorkGroupID.xy), uvec4(o_SpotLightIndexStartOffset, o_SpotLightCount,0,0));
  //  }

    barrier(); 

    // Update the light index list (all threads).
    // For opaque geometry.
    for (uint i = gl_LocalInvocationIndex; i < o_PointLightCount; i += TILE_SIZE * TILE_SIZE)
    {
        s_PointLightIndexList[o_PointLightIndexStartOffset + i] = o_PointLightList[i];
    }

    for (uint i = gl_LocalInvocationIndex; i < o_SpotLightCount; i += TILE_SIZE * TILE_SIZE)
    {
        s_SpotLightIndexList[o_SpotLightIndexStartOffset + i] = o_SpotLightList[i];
    }

    // For transparent geometry.
    //for (uint i = gl_LocalInvocationIndex; i < t_LightCount; i += TILE_SIZE * TILE_SIZE)
    //{
    //    t_LightIndexList[t_LightIndexStartOffset + i] = t_LightList[i];
    //}

}
