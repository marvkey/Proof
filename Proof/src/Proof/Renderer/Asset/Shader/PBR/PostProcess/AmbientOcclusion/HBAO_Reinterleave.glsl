//https://github.com/nvpro-samples/gl_ssao/blob/master/hbao_reinterleave.frag.glsl

/*
    In HBAO+ (Horizon-Based Ambient Occlusion Plus), the reinterleave step is a process where 
    previously deinterleaved data is organized back into a more efficient structure for subsequent processing. 
    The deinterleave pass, which initially rearranged data for better performance, is followed by the reinterleave step to 
    restore the data to a format that facilitates efficient computations in the later stages of the HBAO+ algorithm. 
    This back-and-forth operation optimizes memory access patterns, 
    helping achieve better parallelism and cache coherency during the ambient occlusion calculation, 
    ultimately contributing to the algorithm's high-quality and real-time performance in rendering realistic shadows and 
    shading effects in computer graphics.
*/

#Vertex Shader
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_TexCoord;


void main()
{
	gl_Position = vec4(a_Position, 1.0);
}


#Fragment Shader
#version 450 core

layout(binding = 0)  uniform sampler2DArray u_TexResultsArray;
layout(location = 0) out vec4 out_Color;

void main() 
{
	ivec2 fullResPos = ivec2(gl_FragCoord.xy);
	ivec2 offset = fullResPos & 3;
	int sliceId = offset.y * 4 + offset.x;
	ivec2 quarterResPos = fullResPos >> 2;

	out_Color = vec4(texelFetch(u_TexResultsArray, ivec3(quarterResPos, sliceId), 0).xy, 0, 1.0);

}
