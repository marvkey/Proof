#Vertex Shader
#version 450
#include <PBR/PBRShaderBases/PBR.Vertex.glsl>

void Vertex(inout PBRVertexInput vertexinput)
{
    
}


#Fragment Shader
#version 450 core
#include <PBR/PBRShaderBases/PBR.Fragment.glsl>

float minHeight;
float maxHeight;

float inverseLerp(float a, float b, float value) 
{
    return clamp((value - a) / (b - a), 0.0, 1.0);
}
void Fragment(inout PBRData pbrData)
{
     float heightPercent = inverseLerp(minHeight,maxHeight,PBR_Input.WorldPosition.Y);
     pbrData.Albedo = vec3(heightPercent); // Green gradient based on heightheightPercent;
     pbrData.Alpha = heightPercent;
    
}


void PreEndFragment()
{
}

        
void LightLateUpdate(inout vec3 lightDir, inout vec3 diffuseBRDF, inout vec3 specularBRDF,DirectionalLight currentLight)
{

}
