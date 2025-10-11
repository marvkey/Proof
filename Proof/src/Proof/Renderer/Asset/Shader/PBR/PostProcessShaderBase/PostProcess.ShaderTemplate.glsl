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

void Fragment(inout PostProcessFragmentInput fragmentInput)
{
}

void PreEndFragment()
{

}