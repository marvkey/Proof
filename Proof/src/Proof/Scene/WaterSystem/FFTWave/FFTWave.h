#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Utils/MultiUse.h"
#include "Proof/Scene/WaterSystem/Wave.h"
#include "Proof/Renderer/RenderereCustomTypeSet.h"
#include <array>
#include <glm/glm.hpp>
#include <tuple>
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
			auto operator<=>(const WaveCascadeParameters&) const = default;  // generates all comparison operators

			// Denotes the distance the cascade's tile should cover (in meters).
			glm::vec2 TileLength = glm::vec2(50.0f, 50.0f);

			ClampedValue<float, 0.f, 2.0f> DisplacementScale = 1.0f; 
			ClampedValue<float, 0.f, 2.0f> NormalScale = 1.0f; 

			// Denotes the average wind speed above the water (in meters per second). Increasing makes waves steeper and more 'chaotic'.
			ClampedValue<float, 0.0001f, 100000.0f> WindSpeed = 20.0f;

			ClampedValue<float,-360.0f,360.0f> WindDirection = 180.0f;

			// Denotes the distance from shoreline (in kilometers). Increasing makes waves steeper, but reduces their 'choppiness'.
			ClampedValue<float, 0.0001f, 1000.0f> FetchLength = 550.0f;

			ClampedValue<float, 0.0f, 2.0f> Swell = 0.8;

			// Modifies the attenuation of high frequency waves.
			ClampedValue<float, 0.0f, 1.0f> Detail = 0.25f; // Note: This should be reduced as the number of cascades increases to avoid *too* much detail!

			// Modifies how much wind and swell affect the direction of the waves.
			ClampedValue<float, 0.0f, 1.0f> Spread = 0.2f;

			ClampedValue<float, 0.f, 2.0f> FoamTimeScale = 1.0f; // how fast foams build up

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
		WaveCascadeParameters m_OldSettings;
	};

	enum class FFTWaveMapSize
	{
		SIZE_128 = 128,
		SIZE_256 = 256,
		SIZE_512 = 512,
		SIZE_1024 = 1024
	};

	struct alignas(16) SBWaveHeightQuery
	{
		uint64_t ID;
		float WaveHeight;
		uint32_t pad1;

		glm::vec3 WorldPosition;
		uint32_t pad2;

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

		virtual void Render2D(Count<class Renderer2D> renderer2D);


		struct FFTWaveInfo
		{
			FFTWaveMapSize OceanSize = FFTWaveMapSize::SIZE_1024;
			ClampedValue<int, 1, 8> NumCascades = 3;
			ClampedValue<float, 0.0f, 1.0f> NormalStrength = 0.2f;
			ClampedValue<float, 0.0f, 1.0f> Roughness= 0.2f;
			glm::vec4 WaterColor = glm::vec4(0.000f, 0.560f, 1.000f, 1.000f);
			glm::vec3 FoamColor = glm::vec3(0.73, 0.67, 0.62);


			ClampedValue<float, 0.f, 10.f> DisplacementScale = 1.0f;
			ClampedValue<float, 0.f, 10.f> NormalScale = 1.0f;
			ClampedValue<float, 0.f, 10.f> WindSpeedScale = 1.0f;
			ClampedValue<float, 0.f, 10.f> SwellScale = 1.0f;
			ClampedValue<float, 0.f, 10.f> DetailScale = 1.0f;
			ClampedValue<float, 0.f, 10.f> SpreadScale = 1.0f;

			ClampedValue<float, 0.f, 10.f> FoamTimeScale = 1.0f;
			ClampedValue<float, 0.0f, 10.0f> WhitecapScale = 1.0f; 
			ClampedValue<float, 0.0f, 10.0f> FoamGrowthScale = 1.0f;
			ClampedValue<float, 0.0f, 10.0f> FoamDecayScale = 1.0f;

			bool ViewWaveHeight = false;

			auto operator<=>(const FFTWaveInfo&) const = default;  // generates all comparison operators

		}WaveInfo;

		Count<Image2D> GetSpectrumTexture() { return m_SpectrumTexture; };
		Count<Image2D> GetDisplacementMap() { return m_DisplacementMap; };
		Count<Image2D> GetNormalMap() { return m_NormalMap; };

		const std::vector<Count<FFTWaveCascade>>& GetCascades()const { return m_Cascades; }
		const std::vector<Count<FFTWaveCascade>>& GetCascades(){ return m_Cascades; }

		bool HasWaveHeightQueryID(UUID ID);
		void PushWaveHeightQueryID(UUID ID);
		void UpdateWaveHeightQueryID(UUID ID,glm::vec3 pos);
		void RemoveWaveHeightQueryID(UUID ID);

		bool IsWaveheightQueryReady(UUID ID);
		float GetWaveheight(UUID ID);
	private:
		void InitPasses();
		void InitTextures();
		Count<class RenderMaterial> GetRenderMaterial();
		void UpdateCascade(Count<FFTWaveCascade> cascade);
		void InitialWaveparams();
		void InitBuffer();

	private:

		Count<ComputePass> m_SpectrumPass;
		Count<ComputePass> m_ButterflyPass;
		Count<ComputePass> m_SpectrumModulatePass;
		Count<ComputePass> m_FFTPass;
		Count<ComputePass> m_TransposePass;
		Count<ComputePass> m_FFTUnpack;
		Count<ComputePass> m_QueryWaaveHeightPass;

		Count<class StorageBuffer> m_SBButterflyFactors;
		Count<class StorageBufferSet> m_FFTBuffer;
		Count<class StorageBufferSet> m_FFTSBQueryBuffer;

		Count<class UniformBufferSet> m_WaterScalesBuffer;
		Count<class UniformBufferSet>m_WaterBuffer;
		Count<Image2D> m_SpectrumTexture;
		Count<Image2D> m_DisplacementMap;
		Count<Image2D> m_NormalMap;
		Count<class RenderCommandBuffer> m_CommandBuffer;
		Count<class Mesh> m_Grid;
		Count<class RenderMaterial> m_RenderMaterial;

		std::vector<Count<FFTWaveCascade>> m_Cascades;
		RendererCustomTypeSet<std::vector<SBWaveHeightQuery>> m_FFTQueryBuffer;
		RendererCustomTypeSet<std::vector<SBWaveHeightQuery>> m_FFTQueryBufferResult;
		std::unordered_map<UUID, glm::vec3> m_WaveHeightQueryPositons;
		std::unordered_map<UUID, float> m_WaveHeightQueryResultHeight;

		std::vector<std::tuple<UUID, glm::vec3>> m_SampleWaveheightAtPos;

		FFTWaveInfo m_OldWaveInfo;
		bool m_HasInitialRun = false;
	};
}

