#Vertex Shader
#version 450
#include <Common.glslh>
#include <PBR/PBRShaderBases/PBR.Vertex.glsl>

/*
//https://catlikecoding.com/unity/tutorials/flow/waves/#2
#include <Common.glslh>

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;
layout(location = 5) in mat4 aTransform;
layout(location = 9) in mat4 aPrevTransform;
*/

layout(std140, set=0, binding = 0) uniform WaterData
{
	vec4 Color;

	float Speed;
	uint WaveCount;
	float WaveSpread; //0-1
	float WaveDistribution; // 0-1

	vec2 WaveDirection;
	vec2 MinMaxWavelength;
	
	vec2 MinMaxSteepness;
} u_WaterData;


struct WaterWave
{
	vec2 Direction;
	float Steepness; //range (0-1)
	float WaveLength;
};


struct VertexOutput
{
   vec4 Color;
};
layout(location = 21) out VertexOutput Output;


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

void Vertex(inout PBRVertexInput vertexInput)
{
	Output.Color = u_WaterData.Color;

   vec3 gridPoint = vertexInput.VertexPosition;
   vec3 tangent = vec3(1,0,0);
   vec3 binormal = vec3(0,0,1);
   vec3 p = gridPoint;

   	for(uint i =0; i < u_WaterData.WaveCount; i++)
	{
		float t = i / float(u_WaterData.WaveCount - 1);
		float angleOffset = u_WaterData.WaveSpread * (i - (u_WaterData.WaveCount - 1) / 2.0);
		//float angleOffset = u_WaterData.WaveSpread * t;
        mat2 rotation = mat2(cos(angleOffset), -sin(angleOffset), sin(angleOffset), cos(angleOffset));
        vec2 direction = rotation * u_WaterData.WaveDirection;

		float waveLength = mix(u_WaterData.MinMaxWavelength.y,u_WaterData.MinMaxWavelength.x, t * u_WaterData.WaveDistribution);
		float steepness = mix(u_WaterData.MinMaxSteepness.y,u_WaterData.MinMaxSteepness.x, t * u_WaterData.WaveDistribution);

		steepness = clamp(steepness,0,1);

		WaterWave wave;
		wave.Direction = direction;
		wave.Steepness = steepness;
		wave.WaveLength = waveLength;
		
        // Apply diminishing influence based on the wave index
        float influence = 1.0 / (i + 1);
		//influence = 1;

		p += GerstnerWave(wave, gridPoint, tangent, binormal) *influence ;// Normalize displacement
	}

	vec3 normal = normalize(cross(binormal,tangent));
	vertexInput.VertexPosition = p;
	vertexInput.Normal = normal;
	//vertexInput.Tangent = tangent;
	//vertexInput.Bitangent = binormal;

}

#Fragment Shader
#version 450 core

#include <PBR/PBRShaderBases/PBR.Fragment.glsl>

layout(set = 0, binding = 1) uniform sampler2D u_TextureMap;

struct VertexOutput
{
   vec4 Color;
};
layout(location = 21) in VertexOutput Input;

void Fragment(inout PBRData pbrData)
{
    pbrData.Albedo = Input.Color.xyz;
    pbrData.Metalness = 0;
    pbrData.Roughness = 0.7;
    pbrData.Normal = normalize(PBR_Input.Normal);

	//pbrData.Normal = normalize(texture(u_TextureMap, PBR_Input.TexCoords).rgb * 2.0f - 1.0f);
	//pbrData.Normal =transpose(mat3(PBR_Input.Tangent, PBR_Input.Bitangent, PBR_Input.Normal)) * PBR_Input.Normal;
	//pbrData.Normal =transpose(mat3(PBR_Input.Tangent, PBR_Input.Bitangent, PBR_Input.Normal)) * pbrData.Normal;
	//pbrData.Normal = normalize(PBR_Input.WorldNormals * pbrData.Normal);

}


void PreEndFragment()
{
}