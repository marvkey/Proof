#Vertex Shader
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

layout(location = 0) out vec2 o_TexCoord;
void main()
{
    o_TexCoord = a_TexCoord;
    gl_Position = vec4(a_Position.xy, 0.0, 1.0);
}
#Fragment Shader
#version 450 core


layout(binding = 0) uniform sampler2D u_InputAOTexture;


layout(location = 0) out vec4 o_Occlusion;
layout(location = 0) in vec2 vs_TexCoord;

layout(push_constant) uniform Info
{
    int Type; //0 HBAO,1 SSAO
} u_PushData;

void main()
{
    float occlusion = 1.0f;

    if(u_PushData.Type == 1)
    {
        occlusion *= texture(u_InputAOTexture, vs_TexCoord).x;
    }
    else if (u_PushData.Type == 0) //HBAO
    {
        occlusion *= texture(u_InputAOTexture, vs_TexCoord).x;
    }

    o_Occlusion = occlusion.xxxx;
}
