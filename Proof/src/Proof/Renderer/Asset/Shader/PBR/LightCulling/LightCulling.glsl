#Compute Shader
#version 450 core

#include <Common.glslh>
#include <PBR/Lights.glslh>

layout(set = 1, binding = 0) uniform sampler2D u_DepthTexture;

struct Frustum
{
    vec4 Planes[6];
};

float ScreenSpaceToViewSpaceDepth(float screenDepth)
{
    float depthLinearizeMul = -u_Camera.Projection[3][2];
    float depthLinearizeAdd =  u_Camera.Projection[2][2];

    return depthLinearizeMul /
    (screenDepth + depthLinearizeAdd);
}

shared uint minDepthInt;
shared uint maxDepthInt;

shared uint visiblePointLightCount;
shared uint visibleSpotLightCount;

shared Frustum frustumPlanes;

shared int visiblePointLightIndices[MAX_NUM_LIGHTS_PER_TILE];
shared int visibleSpotLightIndices[MAX_NUM_LIGHTS_PER_TILE];

layout(
local_size_x = TILE_SIZE,
local_size_y = TILE_SIZE,
local_size_z = 1
) in;

void main()
{
    const uint maxLightsPerTile =
    uint(MAX_NUM_LIGHTS_PER_TILE);

    /*
        Reserve the final position for -1.

        For example, with 1024 entries:
        0–1022 = light indices
        1023   = -1 terminator
    */
    const uint maxStoredLightsPerTile =
    maxLightsPerTile - 1u;

    ivec2 pixelLocation = ivec2(gl_GlobalInvocationID.xy);
    ivec2 tileID        = ivec2(gl_WorkGroupID.xy);
    ivec2 tileCount     = ivec2(gl_NumWorkGroups.xy);

    uint tileIndex =
    uint(tileID.y * tileCount.x + tileID.x);

    /*
        Initialize values shared by this workgroup.
    */
    if (gl_LocalInvocationIndex == 0u)
    {
        minDepthInt = 0xFFFFFFFFu;
        maxDepthInt = 0u;

        visiblePointLightCount = 0u;
        visibleSpotLightCount  = 0u;
    }

    barrier();

    /*
        Clamp the pixel coordinate because the final workgroup can extend
        beyond the actual screen resolution.
    */
    ivec2 screenResolution =
    ivec2(u_ScreenData.FullResolution);

    ivec2 safePixelLocation = clamp(
    pixelLocation,
    ivec2(0),
    screenResolution - ivec2(1)
    );

    vec2 textureCoordinate =
    (vec2(safePixelLocation) + vec2(0.5)) /
    vec2(screenResolution);

    /*
        Find the minimum and maximum depth for this tile.
    */
    float sampledDepth = textureLod(
    u_DepthTexture,
    textureCoordinate,
    0.0
    ).r;

    float linearDepth =
    ScreenSpaceToViewSpaceDepth(sampledDepth);

    uint depthInt = floatBitsToUint(-linearDepth);

    atomicMin(minDepthInt, depthInt);
    atomicMax(maxDepthInt, depthInt);

    barrier();

    /*
        One invocation constructs the tile frustum.
    */
    if (gl_LocalInvocationIndex == 0u)
    {
        float minDepth = uintBitsToFloat(minDepthInt);
        float maxDepth = uintBitsToFloat(maxDepthInt);

        vec2 negativeStep =
        (2.0 * vec2(tileID)) /
        vec2(tileCount);

        vec2 positiveStep =
        (2.0 * vec2(tileID + ivec2(1))) /
        vec2(tileCount);

        frustumPlanes.Planes[0] =
        vec4(
        1.0,
        0.0,
        0.0,
        1.0 - negativeStep.x
        );

        frustumPlanes.Planes[1] =
        vec4(
        -1.0,
        0.0,
        0.0,
        -1.0 + positiveStep.x
        );

        frustumPlanes.Planes[2] =
        vec4(
        0.0,
        1.0,
        0.0,
        1.0 - negativeStep.y
        );

        frustumPlanes.Planes[3] =
        vec4(
        0.0,
        -1.0,
        0.0,
        -1.0 + positiveStep.y
        );

        frustumPlanes.Planes[4] =
        vec4(
        0.0,
        0.0,
        -1.0,
        -minDepth
        );

        frustumPlanes.Planes[5] =
        vec4(
        0.0,
        0.0,
        1.0,
        maxDepth
        );

        /*
            Transform the side planes.
        */
        for (uint i = 0u; i < 4u; ++i)
        {
            frustumPlanes.Planes[i] *=
            u_Camera.ViewProjectionMatrix;

            float planeLength =
            length(frustumPlanes.Planes[i].xyz);

            if (planeLength > 0.0)
            {
                frustumPlanes.Planes[i] /=
                planeLength;
            }
        }

        /*
            Transform the near and far planes.
        */
        frustumPlanes.Planes[4] *= u_Camera.View;
        frustumPlanes.Planes[5] *= u_Camera.View;

        float nearPlaneLength =
        length(frustumPlanes.Planes[4].xyz);

        float farPlaneLength =
        length(frustumPlanes.Planes[5].xyz);

        if (nearPlaneLength > 0.0)
        {
            frustumPlanes.Planes[4] /=
            nearPlaneLength;
        }

        if (farPlaneLength > 0.0)
        {
            frustumPlanes.Planes[5] /=
            farPlaneLength;
        }
    }

    /*
     

        Every invocation must wait until invocation zero has finished
        writing all six frustum planes.
    */
    barrier();

    const uint threadCount =
    uint(TILE_SIZE * TILE_SIZE);

    /*
        Cull all point lights in the scene.
    */
    uint pointLightCount =
    uint(max(u_LightData.PointLightCount, 0));

    uint pointLightPassCount =
    (pointLightCount + threadCount - 1u) /
    threadCount;

    for (uint pass = 0u;
    pass < pointLightPassCount;
    ++pass)
    {
        uint lightIndex =
        pass * threadCount +
        gl_LocalInvocationIndex;

        if (lightIndex >= pointLightCount)
        break;

        PointLight light =
        s_PointLights.Lights[lightIndex];

        vec4 lightPosition =
        vec4(light.Position, 1.0);

        float radius =
        max(light.Radius, 0.0);

        radius *= 1.3;

        bool intersectsTile = true;

        for (uint planeIndex = 0u;
        planeIndex < 6u;
        ++planeIndex)
        {
            float distance =
            dot(
            lightPosition,
            frustumPlanes.Planes[planeIndex]
            ) + radius;

            if (distance <= 0.0)
            {
                intersectsTile = false;
                break;
            }
        }

        if (intersectsTile)
        {
            /*
                The atomic counter may become larger than the array,
                but the array write is protected.
            */
            uint destination = atomicAdd(
            visiblePointLightCount,
            1u
            );

            if (destination < maxStoredLightsPerTile)
            {
                visiblePointLightIndices[destination] =
                int(lightIndex);
            }
        }
    }

    /*
        Cull all spotlights in the scene.
    */
    uint spotLightCount =
    uint(max(u_LightData.SpotLightCount, 0));

    uint spotLightPassCount =
    (spotLightCount + threadCount - 1u) /
    threadCount;

    for (uint pass = 0u;
    pass < spotLightPassCount;
    ++pass)
    {
        uint lightIndex =
        pass * threadCount +
        gl_LocalInvocationIndex;

        if (lightIndex >= spotLightCount)
        break;

        SpotLight light =
        s_SpotLights.Lights[lightIndex];

        float radius =
        max(light.Range, 0.0);

        vec3 sphereCenter =
        light.Position -
        light.Direction * light.Range;

        bool intersectsTile = true;

        for (uint planeIndex = 0u;
        planeIndex < 6u;
        ++planeIndex)
        {
            float distance =
            dot(
            vec4(sphereCenter, 1.0),
            frustumPlanes.Planes[planeIndex]
            ) + radius;

            if (distance <= 0.0)
            {
                intersectsTile = false;
                break;
            }
        }

        if (intersectsTile)
        {
            uint destination = atomicAdd(
            visibleSpotLightCount,
            1u
            );

            if (destination < maxStoredLightsPerTile)
            {
                visibleSpotLightIndices[destination] =
                int(lightIndex);
            }
        }
    }

    /*
        Wait until every invocation has finished adding indices.
    */
    barrier();

    /*
        One invocation copies this tile's lists into the global buffers.
    */
    if (gl_LocalInvocationIndex == 0u)
    {
        uint globalTileOffset =
        tileIndex * maxLightsPerTile;

        /*
            Clamp the counters because atomicAdd continues increasing
            even after the shared array becomes full.
        */
        uint validPointLightCount = min(
        visiblePointLightCount,
        maxStoredLightsPerTile
        );

        uint validSpotLightCount = min(
        visibleSpotLightCount,
        maxStoredLightsPerTile
        );

        /*
            Copy point-light indices.
        */
        for (uint i = 0u;
        i < validPointLightCount;
        ++i)
        {
            s_PointLightIndexList.Indices[
            globalTileOffset + i
            ] = visiblePointLightIndices[i];
        }

        /*
            Always write a valid terminator inside this tile's section.
        */
        s_PointLightIndexList.Indices[
        globalTileOffset + validPointLightCount
        ] = -1;

        /*
            Copy spotlight indices.
        */
        for (uint i = 0u;
        i < validSpotLightCount;
        ++i)
        {
            s_SpotLightIndexList.Indices[
            globalTileOffset + i
            ] = visibleSpotLightIndices[i];
        }

        s_SpotLightIndexList.Indices[
        globalTileOffset + validSpotLightCount
        ] = -1;
    }
}