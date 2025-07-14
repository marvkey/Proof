#Vertex Shader
#version 450
#include <PBR/PBRShaderBases/PBR.Vertex.glsl>

struct CustomOutPutData
{
    
};

layout(location = CUSTOM_OUTPUT_SLOT_VERTEX_FRAGMENT_PBR) out CustomOutPutData OutPutData;

void Vertex(inout PBRVertexInput vertexInput)
{
    
}

#Fragment Shader
#version 450 core
#include <PBR/PBRShaderBases/PBR.Fragment.glsl>


struct CustomOutPutData
{
    
};
layout(location = CUSTOM_OUTPUT_SLOT_VERTEX_FRAGMENT_PBR) in CustomOutPutData Input;

void Fragment(inout PBRData pbrData)
{
    
}

