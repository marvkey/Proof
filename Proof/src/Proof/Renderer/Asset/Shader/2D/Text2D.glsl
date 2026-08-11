#Vertex Shader
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_FontIndex;
layout(location = 4) in float a_BackgroundTexture;
layout(location = 5) in vec4 a_OutlineColor;
layout(location = 6) in float a_OutlineThickness;

layout(location = 0) out vec4 out_Color;
layout(location = 1) out vec2 out_TexCoord;
layout(location = 2) out float out_FontIndex;
layout(location = 3) out vec4 out_OutlineColor;
layout(location = 4) out float out_OutlineThickness;

layout(set = 0, binding = 0) uniform CameraData
{
    mat4 ProjectionMatrix;
    mat4 ViewMatrix;
    vec3 Position;
} Camera;

void main()
{
    out_Color = a_Color;
    out_TexCoord = a_TexCoord;
    out_FontIndex = a_FontIndex;
    out_OutlineColor = a_OutlineColor;
    out_OutlineThickness = a_OutlineThickness;

    gl_Position = Camera.ProjectionMatrix * Camera.ViewMatrix * vec4(a_Position, 1.0);
}


#Fragment Shader
#version 450

layout(location = 0) out vec4 outFragColor;

layout(location = 0) in vec4 in_Color;
layout(location = 1) in vec2 in_TexCoord;
layout(location = 2) in float in_FontIndex;
layout(location = 3) in vec4 in_OutlineColor;
layout(location = 4) in float in_OutlineThickness;

layout(set = 0, binding = 1) uniform sampler2D u_FontAtlas[32];

const float c_PxRange = 8.0;

float screenPxRange()
{
    vec2 unitRange = vec2(c_PxRange) / vec2(textureSize(u_FontAtlas[int(in_FontIndex)], 0));
    vec2 screenTexSize = vec2(1.0) / fwidth(in_TexCoord);

    return max(0.5 * dot(unitRange, screenTexSize), 1.0);
}

float median(float r, float g, float b)
{
    return max(min(r, g), min(max(r, g), b));
}

void main()
{
    vec3 msd = texture(u_FontAtlas[int(in_FontIndex)], in_TexCoord).rgb;

    float sd = median(msd.r, msd.g, msd.b);

    float screenRange = screenPxRange();

    float textPxDistance = screenRange * (sd - 0.5);
    float textOpacity = clamp(textPxDistance + 0.5, 0.0, 1.0);

    // Outline thickness is in atlas-distance pixels,
    // so it scales together with the glyph.
    float outlineDistance = in_OutlineThickness / c_PxRange;

    float outlinePxDistance = screenRange * ((sd + outlineDistance) - 0.5);
    float outlineOpacity = clamp(outlinePxDistance + 0.5, 0.0, 1.0);

    if (outlineOpacity <= 0.0)
    discard;

    float outlineOnly = max(outlineOpacity - textOpacity, 0.0);

    float textAlpha = textOpacity * in_Color.a;
    float outlineAlpha = outlineOnly * in_OutlineColor.a;

    float finalAlpha = textAlpha + outlineAlpha;

    vec3 finalColor =
    in_Color.rgb * textAlpha +
    in_OutlineColor.rgb * outlineAlpha;

    if (finalAlpha > 0.0)
    finalColor /= finalAlpha;

    outFragColor = vec4(finalColor, finalAlpha);
}