#pragma once
#include "Wave.h"
namespace Proof
{
	class Texture2D;
	class RenderMaterial;
	class Material;
	class MaterialTable;
	struct TransformComponent;
	struct alignas(16) UBGerstnerWave
	{
		float Wavelength = 10;
		float Amplitude = 1;
		float Steepness = 0.5;
		float Speed = 3;
		glm::vec2 WaveDirection;
		glm::vec2 WaveVector;
		float WKA = 0.0f;
		float Q = 0.0f;
		float Dispersion;
		int Padding;
	};

	class GerstnerWave : public Wave
	{

	public:
		GerstnerWave(class Water* water, Count<GerstnerWave> other);
		GerstnerWave(class Water* water);

		void Update(float deltaTime);

		void Render(Count<class WorldRenderer> renderer);
		void Render2D(Count<class Renderer2D> renderere2D);

		Count<Texture2D> Texture; //default white texture

		struct alignas(16) WaterDataInfo
		{
			glm::vec4 Color{ 0.36f,0.514f,0.663f,1.0f };

			float Speed = 25; // min 0, 
			uint32_t WaveCount = 5;
			//determining how far the waves fan out from the main direction.
			float WaveSpread = 0.5f; // 0-1
			//affecting the variation of wavelength and steepness across waves.
			float WaveDistribution = 0.5f; //0-1

			glm::vec2 WaveDirection = { 0.5,0.5 };
			glm::vec2 MinMaxWavelength = { 25,30 };

			glm::vec2 MinMaxSteepness = { 0.15,0.3 }; //0-1
		} WaterData;

		struct GerstnerWaveInfo
		{
			static inline const uint32_t MaxGerstnerWavesCount = 128;

			uint32_t WaveCount = 5; //min 1 max 128
			//determining how far the waves fan out from the main direction.
			float WaveSpread = 0.5f; //0-1
			//affecting the variation of wavelength and steepness across waves.
			float WaveDistribution = 0.5f; // min 0 max 10
			glm::vec2 WaveDirection = { 0.5,0.5 };

			glm::vec4 Color{ 0.36f,0.514f,0.663f,1.0f };
			float Speed = 5; // min 0, 

			glm::vec2 MinMaxWavelength{ 10,25 }; // min 0.0001 max 1000.0
			float WavelengthFalloff = 0.5; //min 0 max 1
			glm::vec2 MinMaxAmplitude{ 0.5,1 }; // min =0, max 10
			float AmplitudeFallOff = 0.5f; //min 0 max 1
			glm::vec2 MinMaxSteepness{ 0.15,0.3 }; //0-1
			float SteepnessFallOff = 0.5f; // min 0 max 1

			int Seed = 456;
			bool RandomSeed = false;

			float PlaneSize = 50.0f;

		} GerstnerData;

		bool VisualizeWaveHeight = false;
		//https://github.com/shanlemon/ocean-and-boat-simulation/blob/main/Ocean%20Simulation/Assets/Scripts/Water/WaterController.cs
		float GetWaveHeightAtPosition(glm::vec3 position, float applicationTime);
		float GetWorldWaveHeightAtPosition(glm::vec3 position, float applicationTime);

		void RegeneratePlane();
	private:
		glm::vec3 GetWaveOffset(glm::vec3 position, float applicationTime);
		// has to mathc shader
		glm::vec3 CalculateWave(const UBGerstnerWave& wave, glm::vec3 position, float applicationTime);
		void RecomputeGerstnerWave(UBGerstnerWave& wave);
		void RecomputeWaves();
		void BaseInit();

		void OnUpdatePhysics(float deltaTime, Count<class World> world);
	private:
		std::vector<UBGerstnerWave> m_Waves;
		Count<RenderMaterial> m_RenderMaterial;
		Count<class UniformBufferSet> m_UBWaterDataSet;

		Count<class StorageBufferSet> m_SBGerstnerWavesSet;
		Count<class UniformBufferSet> m_UBGerstnerWaveDataSet;
		Count<class Mesh> m_Plane;

		struct alignas(16) UBGerstnerWaveData
		{
			glm::vec4 Color{ 0.36f,0.514f,0.663f,1.0f };
			uint32_t WaveCount = 1;
		} m_UBGerstnerData;

	};

}