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

struct CustomInputData
{
    float val1;
};

layout(location = CUSTOM_OUTPUT_SLOT_VERTEX_FRAGMENT_POST_PROCESS)
in CustomInputData VertexInputdata;

layout(set = 0, binding = 0) uniform sampler2D u_GlitchNoise;

layout(push_constant) uniform Material
{
    vec3  GlitchAxis;      // direction of glitch
    float GlitchStrength;  // intensity
    float TimeScalee;      // speed of oscillation
} u_MaterialUniform;

void Fragment(inout PostProcessFragmentInput fragmentInput)
{
    vec2 uv = fragmentInput.UV;
    float time = u_FrameData.AppTimeSeconds;

    /* =========================
       Oscillating time signal
       ========================= */
    float osc = sin(time * u_MaterialUniform.TimeScalee);

    /* =========================
       Stable noise sampling
       (NO scrolling)
       ========================= */
    float noise = texture(u_GlitchNoise, uv * 4.0).r;

    // Center and sharpen noise
    noise = (noise - 0.5) * 2.0;
    noise *= osc * u_MaterialUniform.GlitchStrength;

    /* =========================
       Directional offset
       ========================= */
    vec2 dir = normalize(u_MaterialUniform.GlitchAxis.xy + 0.0001);
    vec2 offset = dir * noise;

    /* =========================
       RGB channel split
       ========================= */
    vec2 uvR = fract(uv + offset * 1.2);
    vec2 uvG = fract(uv + offset * 0.8);
    vec2 uvB = fract(uv - offset * 1.0);

    vec3 color;
    color.r = texture(u_InputColor, uvR).r;
    color.g = texture(u_InputColor, uvG).g;
    color.b = texture(u_InputColor, uvB).b;

    /* =========================
       Optional glitch cuts
       ========================= */
    float cut = texture(u_GlitchNoise, uv * 2.5 + osc).r;
    if (cut < 0.12)
    discard;

    fragmentInput.Color = color;
}

void PreEndFragment()
{

}