#Vertex Shader
#version 450
#include <PBR/PBRShaderBases/PBR.Vertex.glsl>

struct CustomOutPutData
{
    
};

// starts at 20 because the pbr takes the first 19 positon for now
layout(location = 23) out CustomOutPutData OutPutData;

void Vertex(inout PBRVertexInput vertexInput)
{
    
}

#Fragment Shader
#version 450 core
#include <PBR/PBRShaderBases/PBR.Fragment.glsl>


struct CustomOutPutData
{
    
};
// starts at 20 because the pbr takes the first 19 positon for now
layout(location = 23) in CustomOutPutData Input;

void Fragment(inout PBRData pbrData)
{
    
}

