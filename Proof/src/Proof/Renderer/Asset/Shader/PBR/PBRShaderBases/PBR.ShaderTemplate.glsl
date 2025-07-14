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

// Depth drawing modes for PBR shaders
#define PBR_DRAW_DEPTH_NONE      0   // Do not write depth (transparent or UI objects)
#define PBR_DRAW_DEPTH_PREPASS   1   // Write depth in the depth pre-pass
#define PBR_DRAW_DEPTH_OVERRIDE  2   // Write depth during main pass (used when displacement can't be replicated in pre-pass)


#define PBR_DRAW_DEPTH PBR_DRAW_DEPTH_PREPASS  // Options: NONE, PREPASS, OVERRIDE


#include <PBR/PBRShaderBases/PBR.Fragment.glsl>


struct CustomOutPutData
{
    
};
layout(location = CUSTOM_OUTPUT_SLOT_VERTEX_FRAGMENT_PBR) in CustomOutPutData Input;

void Fragment(inout PBRData pbrData)
{
    
}

