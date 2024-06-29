#Vertex Shader
#version 450 core

//layout(location = 0) in vec3 a_Position;
//layout(location = 1) in vec2 a_TexCoord;


struct OutputBlock
{
	vec2 TexCoord;
};

layout (location = 0) out OutputBlock Output;
void main()
{
/*
	vec4 position = vec4(a_Position.xy, 0.0, 1.0);
	Output.TexCoord = a_TexCoord;
	gl_Position = position;
	*/

	vec4 pos = vec4((float((gl_VertexIndex >> 1U) & 1U)) * 4.0 - 1.0, (float(gl_VertexIndex & 1U)) * 4.0 - 1.0, 0, 1.0);
  gl_Position = pos;
	//Output.TexCoord = vec2(0,0);

}
#Fragment Shader
#version 450 core
#include <Common.glslh>
struct OutputBlock
{
	vec2 TexCoord;
};
layout (binding = 0) uniform sampler2D u_TransperantAccum;
layout (binding = 1) uniform sampler2D u_TransperantReveal;

layout (location = 0) in OutputBlock Input;

// calculate floating point numbers equality accurately
bool IsApproximatelyEqual(float a, float b)
{
	return abs(a - b) <= (abs(a) < abs(b) ? abs(b) : abs(a)) * Epsilon;
}

// get the max value between three values
float Max3(vec3 v) 
{
	return max(max(v.x, v.y), v.z);
}

layout(location = 0) out vec4 out_FragColor;

void main()
{
	ivec2 coords = ivec2(gl_FragCoord.xy);
	//ivec2 coords = ivec2(Input.TexCoord);
	vec4 accumulation = texelFetch(u_TransperantAccum, coords,0);
	float revealage = texelFetch(u_TransperantReveal, coords,0).r;


	vec4 frag = vec4(0);
	// save the blending and color texture fetch cost if there is not a transparent fragment
	if (IsApproximatelyEqual(revealage, 1.0f)) 
		discard;
		// fragment color

	// suppress overflow
	if (isinf(Max3(abs(accumulation.rgb)))) 
		accumulation.rgb = vec3(accumulation.a);

	// prevent floating point precision bug
	vec3 average_color = accumulation.rgb / max(accumulation.a, Epsilon);

	// blend pixels
	frag = vec4(average_color, 1.0f - revealage);

	out_FragColor = frag;

	//out_FragColor = vec4(accumulation.rgb/max(accumulation.a,1e-5),revealage);

	/*
	ivec2 coords = ivec2(gl_FragCoord.xy);
	vec4 accumulation = texelFetch(u_TransperantAccum, coords,0);
	float revealage = texelFetch(u_TransperantReveal, coords,0).r;

	out_FragColor = vec4(accumulation.rgb/max(accumulation.a,1e-5),revealage);
	*/

}