#Vertex Shader
#version 450
#include <PBR/PostProcessShaderBase/PostProcess.Vertex.glsl>

// can be empyt
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

// can be empyt
struct CustomInputData
{
    float val1;
};

layout(location = CUSTOM_OUTPUT_SLOT_VERTEX_FRAGMENT_POST_PROCESS) in CustomInputData VertexInputdata;


layout(push_constant) uniform Material
{

    float GlitchRate; //0.5
    float GlitchScale;//50

} u_MaterialUniform;


void Fragment(inout PostProcessFragmentInput fragmentInput)
{

//float noise = fract(sin(dot(fragmentInput.UV * u_FrameData.AppTimeSeconds, vec2(12.9898, 78.233))) * 43758.5453);
//fragmentInput.Color.rgb += (noise - 0.5) ;
//fragmentInput.Color.rgb *= 1.0 + scanline + flicker;

/*
 vec2 uv = fragmentInput.UV;

    // Pixelation amount (try 240.0 for strong blockiness)
    float pixelSize = 240.0;
    uv = floor(uv * pixelSize) / pixelSize;

    // Small offset based on sin/cos glitch pattern
    float glitchStrength = 0.003;

    vec2 glitchOffsetR = uv + vec2(sin(uv.y * 100.0) * glitchStrength, 0.0);
    vec2 glitchOffsetG = uv + vec2(0.0, cos(uv.x * 100.0) * glitchStrength);
    vec2 glitchOffsetB = uv;

    vec3 col;
    col.r = texture(u_InputColor, glitchOffsetR).r;
    col.g = texture(u_InputColor, glitchOffsetG).g;
    col.b = texture(u_InputColor, glitchOffsetB).b;

    fragmentInput.Color = vec3(col);
    */
}

void PreEndFragment()
{

}