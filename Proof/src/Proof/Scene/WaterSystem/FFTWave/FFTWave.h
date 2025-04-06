#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Utils/MultiUse.h"
#include "Proof/Scene/WaterSystem/Wave.h"
#include <array>
#include <glm/glm.hpp>

//https://github.com/gasgiant/Ocean-URP/blob/main/Assets/OceanSystem/Runtime/OceanSimulation.cs
namespace Proof
{
	class Texture2D;
	class Image2D;
	class RenderMaterial;
	class Material;
	class MaterialTable;
	class ComputePass;
	struct TransformComponent;
	class GlobalBufferSet;
	class StorageBufferSet;
	class Entity;
	class ImageView;

	class FFTWaveCascade : RefCounted
	{
	public:

		FFTWaveCascade(uint32_t cascadeIndex,class FFTWave* wave); // because of reference coutnign problems
		

		void Resize(uint32_t mapSize);
		Count<ImageView> SpectrumMap;
		Count<ImageView> DisplacementMap;
		Count<ImageView> NormalMap;
		Count<StorageBufferSet> SBButterflyFactors;
		Count<StorageBufferSet> SBFFTBuffer;

		struct WaveCascadeParameters
		{
			// Denotes the distance the cascade's tile should cover (in meters).
			glm::vec2 TileLength = glm::vec2(50.0f, 50.0f);

			ClampedValue<float, 0.f, 2.0f> DisplacementScale = 1.0f; 
			ClampedValue<float, 0.f, 2.0f> NormalScale = 1.0f; 

			ClampedValue<float, 0.f, 2.0f> TimeScale = 1.0f; // how fast foams build up

			// Denotes the average wind speed above the water (in meters per second). Increasing makes waves steeper and more 'chaotic'.
			ClampedValue<float, 0.0001f, 100.0f> WindSpeed = 20.0f;

			glm::vec2 WindDirection{ 0.5f,0.5f };

			// Denotes the distance from shoreline (in kilometers). Increasing makes waves steeper, but reduces their 'choppiness'.
			ClampedValue<float, 0.0001f, 1000.0f> FetchLength = 550.0f;

			ClampedValue<float, 0.0f, 2.0f> Swell = 0.8;

			// Modifies the attenuation of high frequency waves.
			ClampedValue<float, 0.0f, 1.0f> Detail = 1.0f; // Note: This should be reduced as the number of cascades increases to avoid *too* much detail!

			// Modifies how much wind and swell affect the direction of the waves.
			ClampedValue<float, 0.0f, 1.0f> Spread = 0.2f;

			// Modifies how steep a wave needs to be before foam can accumulate.
			ClampedValue<float, 0.0f, 2.0f> Whitecap = 0.5f; // Note: 'Wispier' foam can be created by increasing 'foamAmount' and decreasing 'whitecap'.

			// Foam amount modifier.
			ClampedValue<float, 0.0f, 10.0f> FoamAmount = 5.0f;

			// Spectrum seed used for wave generation.
			glm::ivec2 SpectrumSeed = glm::ivec2(0);

		}Settings;

		uint32_t GetCascadeIndex() { return m_CascadeIndex; }
	private:

		struct WaveCascadeInternalSettings
		{
			bool ShouldGenerateSpectrum = true;
			float Time = 0.0f;
			float FoamGrowRate = 0.0f;
			float FoamDecayRate = 0.0f;
		}InternalSettings;

		friend class FFTWave;
		uint32_t m_CascadeIndex = 0;
		
	};

	enum class FFTWaveMapSize
	{
		SIZE_128 = 128,
		SIZE_256 = 256,
		SIZE_512 = 512,
		SIZE_1024 = 1024
	};
	class FFTWave : public Wave
	{

	public:
		FFTWave(Count<FFTWave> other);
		FFTWave(Count<class Water> water);

		static inline const float G = 9.81f;// Gravitational constant
		// Source: https://wikiwaves.org/Ocean-Wave_Spectra#JONSWAP_Spectrum
		static inline float JONSWAPAlpha(float windSpeed = 20.0f, float fetchLength = 550e3f) 
		{
			return 0.076f * std::pow((windSpeed * windSpeed) / (fetchLength * G), 0.22f);
		}

		// Source: https://wikiwaves.org/Ocean-Wave_Spectra#JONSWAP_Spectrum
		static inline float JONSWAPPeakAngularFrequency(float windSpeed = 20.0f, float fetchLength = 550e3f) 
		{
			return 22.0f * std::pow((G * G) / (windSpeed * fetchLength), 1.0f / 3.0f);
		}
	public:
		void Update(float deltaTime);
		virtual void Render(Count<class WorldRenderer> renderer);

		struct FFTWaveInfo
		{
			float WindMagnitude = 14.142135f;
			float windAngle = 45.f;
			float Choppiness = 1.5f;
			FFTWaveMapSize OceanSize = FFTWaveMapSize::SIZE_1024;
			uint32_t NumCascades = 1;
		}WaveInfo;

		Count<Image2D> GetSpectrumTexture() { return m_SpectrumTexture; };
		Count<Image2D> GetDisplacementMap() { return m_DisplacementMap; };
		Count<Image2D> GetNormalMap() { return m_NormalMap; };

		const std::vector<Count<FFTWaveCascade>>& GetCascades()const { return m_Cascades; }
		const std::vector<Count<FFTWaveCascade>>& GetCascades(){ return m_Cascades; }
	private:
		void InitPasses();
		void InitTextures();
		Count<class RenderMaterial> GetRenderMaterial();
	private:

		Count<ComputePass> m_SpectrumPass;
		Count<ComputePass> m_ButterflyPass;
		Count<ComputePass> m_SpectrumModulatePass;
		Count<ComputePass> m_FFTPass;
		Count<ComputePass> m_TransposePass;
		Count<ComputePass> m_FFTUnpack;

		Count<class StorageBufferSet> m_SBButterflyFactors;
		Count<class StorageBufferSet> m_FFTBuffer;

		Count<class UniformBufferSet>m_WaterBuffer;
		Count<Image2D> m_SpectrumTexture;
		Count<Image2D> m_DisplacementMap;
		Count<Image2D> m_NormalMap;
		Count<class RenderCommandBuffer> m_CommandBuffer;
		Count<class Mesh> m_Grid;
		Count<class RenderMaterial> m_RenderMaterial;

		std::vector<Count<FFTWaveCascade>> m_Cascades;
	};
}