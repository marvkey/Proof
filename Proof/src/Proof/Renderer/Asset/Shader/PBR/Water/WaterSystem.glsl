#Vertex Shader
#version 450

//https://catlikecoding.com/unity/tutorials/flow/waves/#2
#include <Common.glslh>

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;
layout(location = 5) in mat4 aTransform;
layout(location = 9) in mat4 aPrevTransform;


layout(std140, set=0, binding = 10) uniform WaterData
{
	vec4 Color;
	float Speed;
} u_WaterData;


struct WaterWave
{
	vec2 Direction;
	float Steepness; //range (0-1)
	float WaveLength;
};


layout(std140, set=0, binding = 11) uniform Waves
{
	WaterWave Waves[3];
} u_Waves;

struct VertexOutput
{
   vec4 Color;
};
layout(location = 0) out VertexOutput Output;


// returns vertex
vec3 GerstnerWave(WaterWave wave, vec3 vertexPosition,inout vec3 tangent, inout vec3 binormal)
{
	float steepness = wave.Steepness;
	float wavelength = wave.WaveLength;
	float k = 2 * PI / wavelength;
	float c = sqrt(u_WaterData.Speed / k);
	vec2 d = normalize(wave.Direction);
	float f = k * (dot(d, vertexPosition.xz) - c * u_FrameData.AppTimeSeconds);
	float a = steepness / k;


	tangent += vec3(
				-d.x * d.x * (steepness * sin(f)),
				d.x * (steepness * cos(f)),
				-d.x * d.y * (steepness * sin(f))
			);
	binormal += vec3(
				-d.x * d.y * (steepness * sin(f)),
				d.y * (steepness * cos(f)),
				-d.y * d.y * (steepness * sin(f))
			);

	return vec3(
				d.x * (a * cos(f)),
				a * sin(f),
				d.y * (a * cos(f))
			);
}
void main() 
{
	Output.Color = u_WaterData.Color;

	vec3 gridPoint = aPosition;
	vec3 tangent = vec3(0);
	vec3 binormal = vec3(0);
	vec3 p = gridPoint;

	p += GerstnerWave(u_Waves.Waves[0], gridPoint, tangent, binormal);
	p += GerstnerWave(u_Waves.Waves[1], gridPoint, tangent, binormal);
	p += GerstnerWave(u_Waves.Waves[2], gridPoint, tangent, binormal);

	vec3 normal = normalize(cross(binormal, tangent));

    vec4 worldPos = aTransform * vec4(p,1.0);
    gl_Position =  u_Camera.Projection * u_Camera.View * vec4(worldPos.xyz,1.0);
}



#Fragment Shader
#version 450 core
layout(location = 0) out vec4 out_FragColor;

layout(set = 0, binding = 9) uniform sampler2D u_TextureMap;

struct VertexOutput
{
   vec4 Color;
};
layout(location = 0) in VertexOutput Input;

void main()
{
    out_FragColor = Input.Color;
}
