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
VignetteCenter    = { 0.5f, 0.5f };

VignetteRadius    = 0.35f;
VignetteSoftness  = 0.35f;
VignetteStrength  = 0.90f;

GrayscaleStrength = 0.80f;

CenterBrightness  = 0.80f;

AspectRatio       = 16.0f / 9.0f;
*/

layout(push_constant) uniform Material
{
    vec2  VignetteCenter;     // center of the effect, normally vec2(0.5, 0.5)

    float VignetteRadius;     // how far from the center before the black edges begin
    float VignetteSoftness;   // how soft/smooth the transition into black is

    float VignetteStrength;   // how dark the edges become, 0 = none, 1 = fully black
    float GrayscaleStrength;  // 0 = normal color, 1 = completely grayscale

    float CenterBrightness;   // brightness multiplier for the visible center

bool Enabled;
} u_MaterialUniform;


void Fragment(inout PostProcessFragmentInput fragmentInput)
{
    vec2 uv = fragmentInput.UV;

    /* =========================
       Original scene color
       ========================= */
    vec3 color = texture(u_InputColor, uv).rgb;


    /* =========================
       Grayscale

       0.0 = original color
       1.0 = completely grayscale
       ========================= */
    float grayscale = dot(color, vec3(0.299, 0.587, 0.114));

    color = mix(
    color,
    vec3(grayscale),
    clamp(u_MaterialUniform.GrayscaleStrength, 0.0, 1.0)
    );


    /* =========================
       Center brightness
       ========================= */
    color *= u_MaterialUniform.CenterBrightness;


    /* =========================
       Vignette coordinates

       Correct X for screen aspect ratio so the
       vignette remains roughly circular instead
       of stretching on widescreen displays.
       ========================= */
    vec2 centeredUV = uv - u_MaterialUniform.VignetteCenter;

    centeredUV.x *= u_ScreenData.AspectRatio;

    float distanceFromCenter = length(centeredUV);


    /* =========================
       Black edge vignette

       Radius:
           larger = black begins farther from center
           smaller = black closes in toward center

       Softness:
           larger = smoother transition
           smaller = harder/sharper transition
       ========================= */
    float vignette = smoothstep(
    u_MaterialUniform.VignetteRadius,
    u_MaterialUniform.VignetteRadius + u_MaterialUniform.VignetteSoftness,
    distanceFromCenter
    );


    /* =========================
       Vignette strength

       0.0 = no black edges
       1.0 = edges become fully black
       ========================= */
    vignette *= clamp(u_MaterialUniform.VignetteStrength, 0.0, 1.0);

    color *= 1.0 - vignette;


    fragmentInput.Color =u_MaterialUniform.Enabled == true ? color : fragmentInput.Color;
}


void PreEndFragment()
{

}