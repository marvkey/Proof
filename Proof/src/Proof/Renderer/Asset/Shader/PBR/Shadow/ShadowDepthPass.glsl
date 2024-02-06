#Vertex Shader
#version 450

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;
layout(location = 5) in mat4 aTransform;
layout(location = 9) in mat4 aPrevTransform;

#define SHADOW_MAP_CASCADE_COUNT 4
layout(push_constant) uniform PushConsts {
	uint CascadeIndex;
} u_CascadeInfo;


layout(std140, set = 0, binding = 0) uniform ShadowCascadesProjection {
    mat4 ViewProjection[4];
} u_Cascades;



void main()
{
	gl_Position =  u_Cascades.ViewProjection[u_CascadeInfo.CascadeIndex] *  aTransform * vec4(aPosition, 1.0);
}

#Fragment Shader
#version 450


void main() 
{	
	
}