//https://github.com/nvpro-samples/gl_ssao/blob/master/hbao_deinterleave.frag.glsl

/*
    In graphics programming, HBAO+ is a fancy way to make shadows look more realistic. 
    The deinterleave pass is like a special step where the computer organizes data to work better and make things faster. 
    It's kind of like rearranging things on a shelf so you can find them quickly. 
    This helps HBAO+ create detailed shadows in a cool and 
    efficient way when you're playing a game or watching something with advanced graphics.
*/
#Vertex Shader
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

void main()
{
    gl_Position = vec4(a_Position.xy, 0.0, 1.0);
}

#Fragment Shader
#version 450 core

#include <Common.glslh>

layout(binding = 0) uniform sampler2D u_DepthMap;
layout(location = 0) out vec4 out_Color[8];

layout(push_constant) uniform Info
{
    int UVOffsetIndex;
} u_PushData;
const vec2 UV_OFFSETS[2] = { vec2(0.5f, 0.5), vec2(0.5f, 2.5f) };

//----------------------------------------------------------------------------------
float LinearizeDepth(const float screenDepth)
{
	return -u_Camera.Projection[3][2] / (screenDepth+ u_Camera.Projection[2][2] );
}

vec4 LinearizeDepth(vec4 deviceZs)
{
    return vec4(LinearizeDepth(deviceZs.x), LinearizeDepth(deviceZs.y), LinearizeDepth(deviceZs.z), LinearizeDepth(deviceZs.w));
}
#if 1
void main() {
    vec2 uv = floor(gl_FragCoord.xy) * 4.0 + UV_OFFSETS[u_PushData.UVOffsetIndex] + 0.5;
    uv *= u_ScreenData.InverseFullResolution;

    vec4 S0 = LinearizeDepth(textureGather(u_DepthMap, uv, 0));
    vec4 S1 = LinearizeDepth(textureGatherOffset(u_DepthMap, uv, ivec2(2, 0), 0));

    out_Color[0] = vec4(S0.w, 0.0, 0.0, 1.0);
    out_Color[1] = vec4(S0.z, 0.0, 0.0, 1.0);
    out_Color[2] = vec4(S1.w, 0.0, 0.0, 1.0);
    out_Color[3] = vec4(S1.z, 0.0, 0.0, 1.0);
    out_Color[4] = vec4(S0.x, 0.0, 0.0, 1.0);
    out_Color[5] = vec4(S0.y, 0.0, 0.0, 1.0);
    out_Color[6] = vec4(S1.x, 0.0, 0.0, 1.0);
    out_Color[7] = vec4(S1.y, 0.0, 0.0, 1.0);
}
#else
void main() {
    vec2 uv = floor(gl_FragCoord.xy) * 4.0 + UV_OFFSETS[u_PushData.UVOffsetIndex];
    ivec2 tc = ivec2(uv);

    out_Color[0] = vec4(LinearizeDepth(texelFetchOffset(u_DepthMap, tc, 0, ivec2(0, 0)).x), 0.0, 0.0, 1.0);
    out_Color[1] = vec4(LinearizeDepth(texelFetchOffset(u_DepthMap, tc, 0, ivec2(1, 0)).x), 0.0, 0.0, 1.0);
    out_Color[2] = vec4(LinearizeDepth(texelFetchOffset(u_DepthMap, tc, 0, ivec2(2, 0)).x), 0.0, 0.0, 1.0);
    out_Color[3] = vec4(LinearizeDepth(texelFetchOffset(u_DepthMap, tc, 0, ivec2(3, 0)).x), 0.0, 0.0, 1.0);
    out_Color[4] = vec4(LinearizeDepth(texelFetchOffset(u_DepthMap, tc, 0, ivec2(0, 1)).x), 0.0, 0.0, 1.0);
    out_Color[5] = vec4(LinearizeDepth(texelFetchOffset(u_DepthMap, tc, 0, ivec2(1, 1)).x), 0.0, 0.0, 1.0);
    out_Color[6] = vec4(LinearizeDepth(texelFetchOffset(u_DepthMap, tc, 0, ivec2(2, 1)).x), 0.0, 0.0, 1.0);
    out_Color[7] = vec4(LinearizeDepth(texelFetchOffset(u_DepthMap, tc, 0, ivec2(3, 1)).x), 0.0, 0.0, 1.0);
}

#endif
