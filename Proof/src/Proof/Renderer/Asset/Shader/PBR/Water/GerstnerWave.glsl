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
/*
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
*/

layout(std140, set=0, binding = 0) uniform GerstnerWaterData
{
	vec4 Color;
	uint WaveCount;
}u_GerstnerWaterData;


struct GerstnerWave
{
	float Wavelength;
	float Amplitude;
	float Steepness;
	float Speed;
	vec2 WaveDirection;
	vec2 WaveVector;
	float WKA;
	float Q;
	float Dispersion;
	int padding;
};

layout(std140, set = 0, binding = 5) buffer GerstnerWaterStorageBuffer
{
	GerstnerWave Waves[];
	
}s_GerstnerWavesData;

struct WaveOutput
{
	vec3 Normal;
	vec3 WavePosition;
};
/*
struct WaterWave
{
	vec2 Direction;
	float Steepness; //range (0-1)
	float WaveLength;
};
*/

struct VertexOutput
{
   vec4 Color;
};
layout(location = 23) out VertexOutput Output;
/*
WaveOutput GetSingleGerstnerWave(GerstnerWave currentWave, vec3 vertexPosition)
{
	WaveOutput outWave;
	outWave.WavePosition = vec3(0);
	outWave.Normal = vec3(0); 

	float waveTime = currentWave.Speed * u_FrameData.AppTimeSeconds;

	float wavePos = dot(vertexPosition.xy,currentWave.WaveVector) - waveTime;

	float waveSin = sin(wavePos);
	float waveCos = cos(wavePos);

	float q =  currentWave.Steepness / currentWave.WKA;

	outWave.Normal.xy = waveSin * currentWave.WKA * currentWave.WaveDirection;

	const float SteepnessThreshold = 50;
	outWave.Normal.z = waveCos * currentWave.Steepness * clamp((currentWave.Amplitude * SteepnessThreshold) / currentWave.Wavelength,0.f,1.f);

	//outWave.WavePosition = vertexPosition;
	outWave.WavePosition.xy = -q * waveSin * currentWave.WaveDirection * currentWave.Amplitude;
	outWave.WavePosition.z = waveCos * currentWave.Amplitude;

	return outWave;
}

WaveOutput GetAllGerstnerWaves(vec3 vertexPosition)
{ 
	WaveOutput totalOutput;
	//totalOutput.WavePosition = vec3(vertexPosition.x,vertexPosition.y,vertexPosition.z);
	totalOutput.WavePosition = vec3(0);
	totalOutput.Normal = vec3(0); 

	// arbitrary start prime numbers
int randx = 10007;
int randy = 802709;
	for(int i = 0; i < u_GerstnerWaterData.WaveCount; i++ )
	{

		float t = i / float(u_GerstnerWaterData.WaveCount - 1);
		float thisIndexFrac = i / float(u_GerstnerWaterData.WaveCount);
		// generate sequence of numbers that *look* random and derive *pseudo* random direction from them
		randx = (randx * 1103515245) + 12345;
		randy = (randy * 1103515245) + 12345;

		const float wavedistribution = 2.0;
		const float thisAlpha = pow(thisIndexFrac, wavedistribution);
	const float wavelength = mix(2, 0.1, thisAlpha);
	const float steepness = mix(0.1, 0.045, thisAlpha);
	const float amplitude = mix(1, 0.1, thisAlpha);
		//GerstnerWave wave = s_GerstnerWavesData.Waves[0];
		GerstnerWave wave;
		wave.Wavelength = wavelength;
		wave.Amplitude =steepness;
		wave.Steepness =amplitude;
		wave.WaveDirection = vec2(cos(float(randx) / 801571.f), sin(float(randy) / 10223.f));
		wave.WaveDirection = normalize(mix(vec2(0.1f,0.9f), (wave.WaveDirection * 2.f) - 1.f, 0.5));

		wave.Dispersion = 2*3.14 / wave.Wavelength;
		wave.Speed = 2;
		wave.WaveVector = wave.WaveDirection * wave.Dispersion;
		wave.WKA =wave.Amplitude * wave.Dispersion;
		//wave.Q
		WaveOutput currentOutput = GetSingleGerstnerWave(wave,vertexPosition);

		totalOutput.WavePosition += currentOutput.WavePosition;
		totalOutput.Normal += currentOutput.Normal;
	}

	totalOutput.Normal = normalize(vec3(totalOutput.Normal.x,totalOutput.Normal.y,1.f - totalOutput.Normal.z));

	return totalOutput;
}
*/
/*
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
*/

vec3 GerstnerWaveCalculateOld(GerstnerWave wave, vec3 vertexPosition,inout vec3 tangent, inout vec3 binormal)
{
	float steepness = wave.Steepness;
	float wavelength = wave.Wavelength;
	float k = 2 * PI / wavelength;
	float c = sqrt(wave.Speed / k);
	vec2 d = normalize(wave.WaveDirection);
	float f = k * (dot(d, vertexPosition.xz) - c * u_FrameData.AppTimeSeconds);
	//float amplitude = wave.Amplitude;
	float amplitude = wave.Amplitude;
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
				a * sin(f) * amplitude ,
				d.y * (a * cos(f))
			);
}

void Vertex(inout PBRVertexInput vertexInput)
{

/*
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
	*/
  
   vec3 gridPoint = vertexInput.VertexPosition;

	vec4 worldPos = aTransform * vec4(gridPoint, 1.0);


	Output.Color = u_GerstnerWaterData.Color;

	vec3 tangent = vec3(1,0,0);
	 vec3 binormal = vec3(0,0,1);
	 vec3 p = gridPoint;

	for(uint i =0; i < u_GerstnerWaterData.WaveCount; i++)
	{
		float minwavelength = 1;
		float maxwavelength =5;

		float minsteepness = 0.1;
		float maxsteepness = 0.4;

		float minamplitude = 0.69;
		float maxamplitude = 7;

		vec2 winddirection =vec2(1,0.5);
		float spread = 0.5;
		float wavedistribution = 0.5;
	

		float t = i / float(u_GerstnerWaterData.WaveCount - 1);
		float angleOffset = spread * (i - (u_GerstnerWaterData.WaveCount - 1) / 2.0);
		//float angleOffset = u_WaterData.WaveSpread * t;
        mat2 rotation = mat2(cos(angleOffset), -sin(angleOffset) , sin(angleOffset), cos(angleOffset));
        vec2 direction = rotation * winddirection;

		float waveLength = mix(maxwavelength,minwavelength, t * wavedistribution);
		float steepness = mix(maxsteepness,minsteepness, t * wavedistribution);

		steepness = clamp(steepness,0,1);
		GerstnerWave wave = s_GerstnerWavesData.Waves[i];

		//GerstnerWave wave;
		//wave.Speed =wave.Wavelength;
		//wave.WaveDirection = direction;
		//wave.Steepness = steepness;
		//wave.Wavelength = waveLength;
        // Apply diminishing influence based on the wave index
        float influence = 1.0 / (i + 1);
		//influence = 1;

		//p += GerstnerWaveCalculateOld(wave, gridPoint, tangent, binormal) * influence;// Normalize displacement
		p += GerstnerWaveCalculateOld(wave, gridPoint, tangent, binormal) ;// Normalize displacement
	}


	vec3 normal = normalize(cross(binormal,tangent));
    vertexInput.VertexPosition = p;
	vertexInput.Normal = normal;

	//WaveOutput wavesOutputs = GetAllGerstnerWaves(gridPoint.xyz);
	//vertexInput.VertexPosition = wavesOutputs.WavePosition + gridPoint;
	//vertexInput.Normal = wavesOutputs.Normal;


	// initiate xyz wpo
	/*
vec2 xy = vec2(0, 0);
float z = 0;

// initiate xyz normal
vec2 nxy = vec2(0.0f, 0.0f);
float nz = 0.0f;

// initiate wave mask
float mz = 0.0f;

// arbitrary start prime numbers
int randx = 10007;
int randy = 802709;

int waves =32;

float minwavelength = 10;
float maxwavelength = 40;

float minsteepness = 0.05;
float maxsteepness = 0.5;

float minamplitude = 0.69;
float maxamplitude = 12;

vec2 winddirection =vec2(0.1,0.1);
float spread = 1.0;
float wavedistribution = 0.25;
// loop through only the maximum allowed amount of waves
for (int i = 0; i < waves; i++)
{
	// normalized wave index alpha based on the *actual* wavecount (not the limit) so that if we have 128 waves
	// but choose to process only 64 waves, the last wave will have a 0.5 index fraction and its alpha value will
	// remain consistent compared to when we process 128 waves. This is so wave sim remains mostly the same if we
	// choose to reduce the amount of waves processed at a distance or something like that
	const float thisIndexFrac = 1 - float(i) / float(waves);

	// generate sequence of numbers that *look* random and derive *pseudo* random direction from them
	randx = (randx * 1103515245) + 12345;
	randy = (randy * 1103515245) + 12345;
	vec2 direction = vec2(cos(float(randx) / 801571.f), sin(float(randy)/ 10223.f));

	// lerp with wind direction & make sure it's normalized
	direction = normalize(mix(winddirection, (direction * 2.f) - 1.f, spread));

	// generate wave alpha
	const float thisAlpha = pow(thisIndexFrac, wavedistribution);

	// compute wave parameters based on wave alpha (big or small wave?)
	const float wavelength = mix(maxwavelength, minwavelength, thisAlpha);
	const float steepness = mix(maxsteepness, minsteepness, thisAlpha);
	const float amplitude = mix(maxamplitude, minamplitude, thisAlpha);

	// Gerstner wave
	float dispersion = 2.f * PI / wavelength;
	//float wavespeed = sqrt(dispersion * 981.f);
	float wavespeed = 2;
	vec2 wavevector = normalize(direction * dispersion);
	float wavetime = wavespeed * u_FrameData.AppTimeSeconds;

	float wavepos = dot(vec2(worldPos.xy), wavevector) - wavetime;

	float wavesin = sin(wavepos);
	float wavecos = cos(wavepos);

	float wKA = amplitude * dispersion;

	float q = steepness / wKA;

	// normal (50 is a limit we hardcode to ensure normal's Z component doesn't exceed a maximum value)
	nxy += wavesin * wKA * direction;
	nz += wavecos * steepness * clamp((amplitude * 50.f) / wavelength,0,1);
	//nz += 0;

	// wpo
	xy += -q * wavesin * direction * amplitude;
	z += wavecos * amplitude;
	
	// mask
	mz += wavecos / waves;
}

vec3 Normal = normalize(vec3(nxy.x, nxy.y, 1));
//Mask = mz;

	//vertexInput.VertexPosition =  vec3(xy.x, xy.y, z)+ gridPoint;
	vertexInput.Normal = Normal;
	vertexInput.WorldPositionOffset = vec3(xy.x, xy.y, z);
*/

}

#Fragment Shader
#version 450 core

//#include <PBR/PBRShaderBases/PBR.Fragment.glsl>
#include <PBR/PBRShaderBases/PBR.FragmentTransperant.glsl>

layout(set = 0, binding = 3) uniform sampler2D u_TextureMap;

struct VertexOutput
{
   vec4 Color;
};
layout(location = 23) in VertexOutput Input;

void Fragment(inout PBRData pbrData)
{
    pbrData.Albedo = Input.Color.xyz;
    pbrData.Metalness = 0;
    pbrData.Roughness = 0.7;
    pbrData.Normal = normalize(PBR_Input.Normal);
    pbrData.Alpha = Input.Color.a;

	//pbrData.Normal = normalize(texture(u_TextureMap, PBR_Input.TexCoords).rgb * 2.0f - 1.0f);
	//pbrData.Normal =transpose(mat3(PBR_Input.Tangent, PBR_Input.Bitangent, PBR_Input.Normal)) * PBR_Input.Normal;
	//pbrData.Normal =transpose(mat3(PBR_Input.Tangent, PBR_Input.Bitangent, PBR_Input.Normal)) * pbrData.Normal;
	//pbrData.Normal = normalize(PBR_Input.WorldNormals * pbrData.Normal);

}


