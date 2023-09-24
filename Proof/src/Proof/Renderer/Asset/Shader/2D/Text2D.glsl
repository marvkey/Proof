#Vertex Shader
#version 450 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_FontIndex;
layout(location = 4) in float a_BackgroundTexture;

layout(location = 0) out vec4 out_Color;
layout(location = 1) out vec2 out_TexCoord;
layout(location = 2) out float out_FontIndex;
layout(set = 0, binding = 0) uniform CameraData
{
    mat4 ProjectionMatrix;
    mat4 ViewMatrix;
    vec3 Position;
}Camera;


void main()
{
	out_Color = a_Color;
	out_TexCoord = a_TexCoord;
	out_FontIndex = a_FontIndex;

	gl_Position = Camera.ProjectionMatrix * Camera.ViewMatrix * vec4(a_Position, 1.0);
}
#Fragment Shader
#version 450
layout(location = 0) out vec4 outFragColor;
layout(location = 0) in vec4 in_Color;
layout(location = 1) in vec2 in_TexCoord;
layout(location = 2) in float in_FontIndex;

layout(set = 0, binding = 1) uniform sampler2D u_FontAtlas[32];

float screenPxRange() {
    const float pxRange = 2.0; // set to distance field's pixel range( Marve TODO)
    vec2 unitRange = vec2(pxRange) / vec2(textureSize(u_FontAtlas[int(in_FontIndex)], 0));
    vec2 screenTexSize = vec2(1.0) / fwidth(in_TexCoord);
    return max(0.5 * dot(unitRange, screenTexSize), 1.0);
}

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

void main()
{
	vec4 texColor = in_Color * texture(u_FontAtlas[int(in_FontIndex)], in_TexCoord);

	vec3 msd = texture(u_FontAtlas[int(in_FontIndex)], in_TexCoord).rgb;
	float sd = median(msd.r, msd.g, msd.b);
	float screenPxDistance = screenPxRange() * (sd - 0.5);
	float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);
	if (opacity == 0.0)
		discard;

	vec4 bgColor = vec4(0.0);
	outFragColor = mix(bgColor, in_Color, opacity);
	if (outFragColor.a == 0.0)
		discard;

}
