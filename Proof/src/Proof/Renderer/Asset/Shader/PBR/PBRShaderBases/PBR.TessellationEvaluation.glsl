#Vertex Shader
#version 450
#include <Common.glslh>

layout(triangles, equal_spacing, cw) in;
layout(location = 0) in PBRVertexOutput PBR_Input[];

void TessellationEvaluation(PBRVertexOutput pbrInput[3]);

void main() 
{
    // Basic interpolation of the control points
    vec3 p0 = gl_in[0].gl_Position.xyz;
    vec3 p1 = gl_in[1].gl_Position.xyz;
    vec3 p2 = gl_in[2].gl_Position.xyz;

    // Barycentric coordinates
    vec3 bCoord = gl_TessCoord.x * p0 + gl_TessCoord.y * p1 + gl_TessCoord.z * p2;

    // Set the position for this vertex
    gl_Position = vec4(bCoord, 1.0);
}