#Vertex Shader
#version 450
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

struct OutputBlock
{
	vec2 TexCoord;
};

layout (location = 0) out OutputBlock Output;

void main()
{
	vec4 position = vec4(a_Position.xy, 0.0, 1.0);
	Output.TexCoord = a_TexCoord;
	gl_Position = position;
}

#Fragment Shader
//https://learnopengl.com/code_viewer_gh.php?code=src/5.advanced_lighting/9.ssao/9.ssao_blur.fs
//https://john-chapman-graphics.blogspot.com/2013/01/ssao-tutorial.html
#version 450 core

struct OutputBlock
{
	vec2 TexCoord;
};
layout (location = 0) in OutputBlock Input;
layout(push_constant) uniform Info
{
    float BlurSize;
} u_Info;
//ouputs
layout(location = 0) out float out_FragColor;
layout(binding = 0) uniform sampler2D u_SSAOTexture;

void main()
{
    vec2 texelSize = 1.0 / vec2(textureSize(u_SSAOTexture, 0));
	float result = 0.0;
    for (int x = -2; x < 2; ++x) 
    {
        for (int y = -2; y < 2; ++y) 
        {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            result += texture(u_SSAOTexture, Input.TexCoord + offset).r;
        }
    }
    out_FragColor = result / (u_Info.BlurSize * u_Info.BlurSize);
}