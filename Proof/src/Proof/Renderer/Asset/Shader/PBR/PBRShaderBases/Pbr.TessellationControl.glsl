#TessellationControl Shader
#version 450
#include <Common.glslh>
struct PBRVertexOutput
{
    vec3 WorldPosition;
    vec3 Normal;
    mat3 WorldNormals;
    vec2 TexCoords;
    vec3 Tangent;
    vec3 Bitangent;
    vec3 CameraPosition;

    mat3 CameraView; 

    vec3 ShadowMapCoords[4];

    vec3 ViewPosition;

    vec4 NormalizePositionCur; // nomralize device position current
	vec4 NormalizePositionPrev;
    vec3 ViewSpaceValue; // for depth wieghted blending
};

layout(location = 0) in PBRVertexOutput PBR_Input[];
layout(vertices = 3) out;

layout(location = 0) out PBRVertexOutput PBR_Output[];

void TessellationControl(PBRVertexOutput pbrInput[3]);

void main()
{

     
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    PBR_Output[gl_InvocationID] = PBR_Input[gl_InvocationID];

   TessellationControl(PBR_Input);

}