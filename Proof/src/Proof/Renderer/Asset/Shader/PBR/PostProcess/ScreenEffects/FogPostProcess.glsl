#Vertex Shader
#version 450

#include <PBR/PostProcessShaderBase/PostProcess.Vertex.glsl>

// can be empty
struct CustomOutPutData
{
    float val1;
};

layout(location = CUSTOM_OUTPUT_SLOT_VERTEX_FRAGMENT_POST_PROCESS) out CustomOutPutData OutPutData;


void Vertex(inout PostProcessVertexInput vertexInput)
{
}


#Fragment Shader
#version 450 core

#include <PBR/PostProcessShaderBase/PostProcess.Fragment.glsl>

struct CustomInputData
{
    float val1;
};

layout(location = CUSTOM_OUTPUT_SLOT_VERTEX_FRAGMENT_POST_PROCESS)
in CustomInputData VertexInputdata;


/*
    IMPORTANT:

    This needs to be the CURRENT scene depth texture from the renderer,
    NOT a normal texture asset.

    The renderer needs to bind the scene depth texture to this post-process
    material before rendering the effect.
*/

 /*
 	FogColor           = vec3(0.52, 0.72, 0.88)
		FogStartDistance   = 10.0
		FogFallOffDistance = 45.0
		FogStrength        = 1.0*/
layout(push_constant) uniform Material
{
    vec3  FogColor;

    float FogStartDistance;
    float FogFallOffDistance;
    float FogStrength;
} u_MaterialUniform;


float LinearizeDepth(const float screenDepth)
{
    float depthLinearizeMul = u_Camera.DepthUnpackConsts.x;
    float depthLinearizeAdd = u_Camera.DepthUnpackConsts.y;

    return depthLinearizeMul / (depthLinearizeAdd - screenDepth);
}


void Fragment(inout PostProcessFragmentInput fragmentInput)
{
    vec2 uv = fragmentInput.UV;

    /*
        Start with whatever the previous post-process gave us.

        DO NOT sample u_InputColor and then accidentally throw away
        previous modifications if your PostProcess base has already
        populated fragmentInput.Color.
    */
    vec3 color = fragmentInput.Color;


    /* =========================
       Scene Depth
       ========================= */
    float screenDepth = texture(u_InputDepth, uv).r;

    float depth = LinearizeDepth(screenDepth);
    depth = abs(depth);


    /* =========================
       Distance Fog

       FogStartDistance:
           Distance where fog begins.

       FogFallOffDistance:
           Distance over which the fog transitions
           from clear to completely fogged.

       Example:
           Start = 10
           Falloff = 45

           Fog begins at 10 units.
           Fog reaches maximum around 55 units.
       ========================= */
    float fogAmount = smoothstep(
    u_MaterialUniform.FogStartDistance,
    u_MaterialUniform.FogStartDistance + u_MaterialUniform.FogFallOffDistance,
    depth
    );


    /* =========================
       Fog Strength

       0 = disabled
       1 = normal/full fog

       Useful for smoothly fading fog on/off.
       ========================= */
    fogAmount *= clamp(u_MaterialUniform.FogStrength, 0.0, 1.0);


    /* =========================
       Skybox

       Kept here because your original shader
       checked for the skybox.

       Right now your original shader did not
       actually change fogAmount in this branch,
       so this intentionally preserves that behavior.
       ========================= */
    bool isSkybox =
    screenDepth <= 0.00001f ||
    screenDepth >= 0.99999f;

    if (isSkybox)
    {
        // Your old shader currently did nothing here.
        //
        // If later you want less fog on the sky:
        //
        // fogAmount *= 0.5f;
    }


    /* =========================
       Apply Fog
       ========================= */
    color = mix(
    color,
    u_MaterialUniform.FogColor,
    fogAmount
    );


    fragmentInput.Color = color;
}


void PreEndFragment()
{

}