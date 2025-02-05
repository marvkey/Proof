#Vertex Shader
#version 450 core
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
#version 450 core

layout(location = 0) out vec4 o_Color;

struct OutputBlock
{
    vec2 TexCoord;
};

layout(location = 0) in OutputBlock Input;

// Array of textures, one for each player (up to 4)
layout(binding = 0) uniform sampler2D u_Texture[4];

// Push constant to get viewport information
layout(push_constant) uniform PushConstant
{
    vec2 u_ScreenSize;    // The size of the screen in normalized coordinates (1.0, 1.0)
    int u_ViewportCount;  // Number of viewports (1, 2, 3, or 4)
} u_PC;

void main()
{
    vec2 texCoord = Input.TexCoord; // Original full-screen quad UVs
    vec2 adjustedUV = vec2(0.0);   // Adjusted UVs for each viewport
    int index = 0;                 // Index for the texture array

    if (u_PC.u_ViewportCount == 1)
    {
        // Fullscreen rendering: single texture, no adjustments needed
        index = 0;
        adjustedUV = texCoord;
    }
    else if (u_PC.u_ViewportCount == 2)
    {
       // Normalize gl_FragCoord.x to [0, 1] using screen width
        float normalizedX = gl_FragCoord.x / u_PC.u_ScreenSize.x;

        // Split the screen into two horizontal viewports
        if (normalizedX < 0.5) // Left side (Player 1)
        {
            index = 0;
            adjustedUV = vec2(texCoord.x * 2.0, texCoord.y); // Stretch UV to fit left side
        }
        else // Right side (Player 2)
        {
            index = 1;
            adjustedUV = vec2((texCoord.x - 0.5) * 2.0, texCoord.y); // Stretch UV to fit right side
        }
    }
    else if (u_PC.u_ViewportCount == 3)
    {
        // Split the screen into three vertical sections
        float sectionWidth = 1.0 / 3.0; // Width of each section
        if (texCoord.x < sectionWidth) // Left side (Player 1)
        {
            index = 0;
            adjustedUV = vec2(texCoord.x * 3.0, texCoord.y);
        }
        else if (texCoord.x < 2.0 * sectionWidth) // Center (Player 2)
        {
            index = 1;
            adjustedUV = vec2((texCoord.x - sectionWidth) * 3.0, texCoord.y);
        }
        else // Right side (Player 3)
        {
            index = 2;
            adjustedUV = vec2((texCoord.x - 2.0 * sectionWidth) * 3.0, texCoord.y);
        }
    }
    else if (u_PC.u_ViewportCount == 4)
    {
        // Split the screen into four quadrants
        vec2 quadrantSize = vec2(0.5, 0.5); // Each quadrant is half the width and height
        vec2 quadrantPos = texCoord / quadrantSize; // Determine which quadrant we're in

        if (quadrantPos.x < 1.0 && quadrantPos.y < 1.0) // Top-left (Player 1)
        {
            index = 0;
            adjustedUV = texCoord * 2.0;
        }
        else if (quadrantPos.x >= 1.0 && quadrantPos.y < 1.0) // Top-right (Player 2)
        {
            index = 1;
            adjustedUV = vec2((texCoord.x - 0.5) * 2.0, texCoord.y * 2.0);
        }
        else if (quadrantPos.x < 1.0 && quadrantPos.y >= 1.0) // Bottom-left (Player 3)
        {
            index = 2;
            adjustedUV = vec2(texCoord.x * 2.0, (texCoord.y - 0.5) * 2.0);
        }
        else if (quadrantPos.x >= 1.0 && quadrantPos.y >= 1.0) // Bottom-right (Player 4)
        {
            index = 3;
            adjustedUV = vec2((texCoord.x - 0.5) * 2.0, (texCoord.y - 0.5) * 2.0);
        }
    }

    // Fetch the appropriate texture for the current viewport
    o_Color = texture(u_Texture[index], adjustedUV);
	//o_Color = texture(u_Texture[1], Input.TexCoord);

}
