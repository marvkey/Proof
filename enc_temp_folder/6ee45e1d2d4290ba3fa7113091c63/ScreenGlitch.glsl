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

layout(set = 0, binding = 0) uniform sampler2D u_GlitchNoise;

layout(push_constant) uniform Material
{

    vec3 GlitchAxis;
    float GlitchScale ;
    float TimeScalee; // shoudl animate this not leave static has cool effect so maybe lerp betwewn 1 and 0

} u_MaterialUniform;


void oldGlitch(inout PostProcessFragmentInput fragmentInput)
{
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
//https://www.youtube.com/watch?v=0NbOI8qG3Yg
void Fragment(inout PostProcessFragmentInput fragmentInput)
{
      vec2 uv = fragmentInput.UV;

    // Scroll the noise texture over time
    vec2 noiseUV = uv + vec2(u_FrameData.AppTimeSeconds * u_MaterialUniform.TimeScalee);
    float noiseR = texture(u_GlitchNoise, fract(noiseUV)).r;


    // adding more distortion
    // this adds a lot more distorion
    {
        noiseR = noiseR -0.5;
        noiseR *=2;
    }
 
    // Offset each channel using wrapped UVs
    vec2 offsetR = fract(uv + vec2(noiseR * u_MaterialUniform.GlitchAxis.x, 0.0));
    vec2 offsetG = fract(uv + vec2(noiseR * u_MaterialUniform.GlitchAxis.y, 0.0));
    vec2 offsetB = fract(uv + vec2(0.0, noiseR * u_MaterialUniform.GlitchAxis.z));

    float r = texture(u_InputColor, offsetR).r;
    float g = texture(u_InputColor, offsetG).g;
    float b = texture(u_InputColor, offsetB).b;

    fragmentInput.Color = vec3(r, g, b);


}

void PreEndFragment()
{

}