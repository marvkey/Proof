
#include <Common.glslh> // your shared camera + utilities
#include <Shader_Material.glslh>
#include <PBR/PostProcessShaderBase/PostProcessCommon.glslh>


//#define MATERIAL_POST_PROCESS

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

#define POST_PROCESS_SHADER_VERTEX_BASE

struct PostProcessVertexInput
{
    vec3 Position;
    vec2 TexCoord;
};

struct OutputBlock
{
    vec2 TexCoord;
    vec2 ScreenUV;       // optional: for NDC space effects
    vec4 ClipPosition;   // optional: for motion vectors, depth linearization, etc.
};

layout(location = 0) out OutputBlock Output;

// Hook for specific post-process shaders
void Vertex(inout PostProcessVertexInput inputt);

void main()
{
    PostProcessVertexInput inputt;
    inputt.Position = a_Position;
    inputt.TexCoord = a_TexCoord;

    Vertex(inputt); // optional override

    vec4 position = vec4(inputt.Position.xy, 0.0, 1.0);
    Output.TexCoord = inputt.TexCoord;
    Output.ScreenUV = inputt.TexCoord; // may want to flip Y in the override
    Output.ClipPosition = position;

    gl_Position = position;
}
