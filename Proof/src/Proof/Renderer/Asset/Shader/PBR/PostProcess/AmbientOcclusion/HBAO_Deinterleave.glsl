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
layout(location = 0) out float out_Color[8];

layout(push_constant) uniform Info
{
    vec2 UvOffset;
} u_PushData;

//----------------------------------------------------------------------------------

#if 1
void main() {
  vec2 uv = floor(gl_FragCoord.xy) * 4.0 + u_PushData.UvOffset + 0.5;
  uv *= u_ScreenData.InverseFullResolution;  
  
  vec4 S0 = textureGather(u_DepthMap, uv, 0);
  vec4 S1 = textureGatherOffset(u_DepthMap, uv, ivec2(2,0), 0);
 
  out_Color[0] = S0.w;
  out_Color[1] = S0.z;
  out_Color[2] = S1.w;
  out_Color[3] = S1.z;
  out_Color[4] = S0.x;
  out_Color[5] = S0.y;
  out_Color[6] = S1.x;
  out_Color[7] = S1.y;
}
#else
void main() {
  vec2 uv = floor(gl_FragCoord.xy) * 4.0 + u_PushData.UvOffset;
  ivec2 tc = ivec2(uv);

  out_Color[0] = texelFetchOffset(u_DepthMap, tc, 0, ivec2(0,0)).x;
  out_Color[1] = texelFetchOffset(u_DepthMap, tc, 0, ivec2(1,0)).x;
  out_Color[2] = texelFetchOffset(u_DepthMap, tc, 0, ivec2(2,0)).x;
  out_Color[3] = texelFetchOffset(u_DepthMap, tc, 0, ivec2(3,0)).x;
  out_Color[4] = texelFetchOffset(u_DepthMap, tc, 0, ivec2(0,1)).x;
  out_Color[5] = texelFetchOffset(u_DepthMap, tc, 0, ivec2(1,1)).x;
  out_Color[6] = texelFetchOffset(u_DepthMap, tc, 0, ivec2(2,1)).x;
  out_Color[7] = texelFetchOffset(u_DepthMap, tc, 0, ivec2(3,1)).x;
}

#endif
