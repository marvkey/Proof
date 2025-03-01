#Vertex Shader
#version 450
#include <Common.glslh>
#include <PBR/PBRShaderBases/PBR.Vertex.glsl>
layout(set = 0, binding = 0) uniform sampler2D u_DisplacementMap;

layout(push_constant) uniform FFTConstants
{
    int OceanSize;   // Physical size of the simulated ocean
} u_PC;


struct VertexOutput
{
   vec3 WorldPos;
   float OceanSize;
   vec2 TexCoord;
};

layout(location = 23) out VertexOutput Output;
void Vertex(inout PBRVertexInput vertexInput)
{

  
   vec3 gridPoint = vertexInput.VertexPosition;

   vec3 finalWorldPosition = gridPoint + texture(u_DisplacementMap, vertexInput.TexCoords).rgb * (512.f/1024);

   vertexInput.VertexPosition = finalWorldPosition;

   Output.WorldPos = finalWorldPosition;
   Output.OceanSize = 1024;
   Output.TexCoord = vertexInput.TexCoords;

}

#Fragment Shader
#version 450 core

//#include <PBR/PBRShaderBases/PBR.Fragment.glsl>
#include <PBR/PBRShaderBases/PBR.FragmentTransperant.glsl>

layout(set = 0, binding = 3) uniform sampler2D u_NormalMap;

struct VertexOutput
{
   vec3 WorldPos;
   float OceanSize;
   vec2 TexCoord;
};

layout(location = 23) flat in VertexOutput Input;

void Fragment(inout PBRData pbrData)
{
    pbrData.Albedo = vec3(0,0,1);
    pbrData.Metalness = 0;
    pbrData.Roughness = 0.7;
    pbrData.Normal = texture(u_NormalMap, PBR_Input.TexCoords).xyz;;
    pbrData.Alpha = 1.0;


}



