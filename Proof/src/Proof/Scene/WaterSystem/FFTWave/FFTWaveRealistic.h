#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Scene/WaterSystem/Wave.h"

namespace Proof
{
    class Image2D;
    class ComputePass;
    class Texture2D;
    struct alignas(16) UBSpectrumSettings
    {
        float Scale;
        float Angle;
        float SpreadBlend;
        float Swell;
        float Alpha;
        float PeakOmega;
        float Gamma;
        float ShortWavesFade;
    };

    struct DisplaySpectrumSettings
    {
        ClampedValue<float, 0.0f, 1.0f> Scale = 1.0f;
        float WindSpeed = 10.0f;
        ClampedValue<float, -360.0f, 360.0f> WindDirection = 180.0f;
        float Fetch = 100000.0f;
        ClampedValue<float, 0.0f, 1.0f> SpreadBlend = 0.5f;
        ClampedValue<float, 0.0f, 1.0f> Swell = 0.5f;
        float PeakEnhancement = 1.0f;
        float ShortWavesFade = 1.0f;

        auto operator<=>(const DisplaySpectrumSettings&) const = default;
    };

   

	class FFTWaveRealisticCascade : public RefCounted
	{
	public:

		FFTWaveRealisticCascade(uint32_t cascadeIndex, uint32_t size,Count<Texture2D> noiseTexture); // because of reference coutnign problems


        struct Settings
        {
            ClampedValue<uint32_t, 1, 100000> Length = 50;

            ClampedValue<float, 0.0001f, 10000.0f> FetchLength = 200.0f;

            // Minimum wavenumber (2π / max wavelength) that this cascade will simulate.
            //ClampedValue<float, 0.00001f, 1000.0f> CutoffLow = 0.01f;

            // Maximum wavenumber (2π / min wavelength) that this cascade will simulate.
            //ClampedValue<float, 0.00001f, 1000.0f> CutoffHigh = 10.0f;
        }CascadeSettings;

        void CalculateWavesAtTime(Count<class RenderCommandBuffer> cmdBuffer, Count<class FFTWaveRealistic> wave,float time);

        uint32_t GetCascadeIndex()
        {
            return m_CascadeIndex;          
        }
        Count<Image2D> GetCascadeBufferMap() { return m_CascadeBufferMap; }

        Count<Image2D> GetDxDz() { return m_DxDz; }
        Count<Image2D> GetDyDxz() { return m_DyDxz; }
        Count<Image2D> GetDyxDyz() { return m_DyxDyz; }
        Count<Image2D> GetDxxDzz() { return m_DxxDzz; }

        Count<Image2D> GetDisplacementMap() { return m_DisplacementMap; }
        Count<Image2D> GetDerivativesMap() { return m_DerivativesMap; }
        Count<Image2D> GetTurbulenceMap() { return m_TurbulenceMap; }
        Count<Image2D> GetTurbulence2Map() { return m_Turbulence2Map; }

        Count<Image2D> GetActiveTurbulence() { return m_PingPongTurbulence ? m_TurbulenceMap : m_Turbulence2Map; }


        Count<Image2D> GetInitialSpectrumMap() { return m_InitialSpectrumContainer.m_InitialSpectrumMap; }
        Count<Image2D> GetWavesData() { return m_InitialSpectrumContainer.m_WavesData; }
        Count<Image2D> GetInitialSpectrumBufferMap() { return m_InitialSpectrumContainer.m_BufferMap; }
    private:
        static void FillSettingsStruct(const DisplaySpectrumSettings& display, UBSpectrumSettings& settings);

        struct InitialSpectrumContainer
        {
            InitialSpectrumContainer(Count<Texture2D> noiseTexture,uint32_t cascadeIndex);

            void Generate(Count<class FFTWaveRealistic> waveRealistic, uint32_t lengthScale, uint32_t cutOfflow, uint32_t CutOffHigh, Count<class RenderCommandBuffer> renderCommandBuffer);
        private:

            Count<ComputePass> m_InitialSpectrumPass;
            Count<ComputePass> m_ConjuagatedSpectrumPass;

            Count<Image2D> m_InitialSpectrumMap;
            //precomputed data
            Count<Image2D> m_WavesData;
            Count<Image2D> m_BufferMap;

            Count<class UniformBufferSet> m_SpectrumParameters;
            Count<class UniformBufferSet> m_ParamBuffer; // settigns of the cascade
            friend class FFTWaveRealisticCascade;
         };

        Count<ComputePass> m_TimeDependentSpectrum;
        Count<ComputePass> m_TextureMergePass;
        Count<Image2D> m_CascadeBufferMap;

        Count<Image2D> m_DxDz;
        Count<Image2D> m_DyDxz;
        Count<Image2D> m_DyxDyz;
        Count<Image2D> m_DxxDzz;

        Count<Image2D> m_DisplacementMap;
        Count<Image2D> m_DerivativesMap;
        Count<Image2D> m_TurbulenceMap;
        Count<Image2D> m_Turbulence2Map;

        uint32_t m_CascadeIndex = 0;

		bool m_PingPongTurbulence = false; // used to ping pong the turbulence maps
       static float JonswapAlpha(float g, float fetch, float windSpeed) {
            return 0.076f * std::pow(g * fetch / windSpeed / windSpeed, 0.22f);
        }

       static float JonswapPeakFrequency(float g, float fetch, float windSpeed) {
            return 22.0f * std::pow(windSpeed * fetch / g / g, -0.33f);
        }

       InitialSpectrumContainer m_InitialSpectrumContainer;
       friend class FFTWaveRealistic;
	};

	class FFTWaveRealistic : public Wave
	{
	public:
		FFTWaveRealistic(class Water* water);
		FFTWaveRealistic(class Water* water, Count<FFTWaveRealistic> other);
		virtual ~FFTWaveRealistic();

        Count<class Texture2D> GetNoiseTexture() { return m_NoiseTexture; };
        Count<class Image2D> GetPrcomuteData() { return m_PrcomuteData;};
        virtual void Render(Count<class WorldRenderer> renderer);
        void IFFT2D(Count<Image2D> inputImage, Count<Image2D> bufferImage);
        

        const std::vector<Count<FFTWaveRealisticCascade>>& GetCascades() { return m_Cascades; };
        struct OceanSettings
        {
            glm::vec4 Color = glm::vec4(1.0f);                    // _Color
            glm::vec4 FoamColor = glm::vec4(0.73f, 0.67f, 0.62f, 1.0f); // _FoamColor
            glm::vec4 SSSColor = glm::vec4(0,0,0.7,1);                 // _SSSColor

            ClampedValue<float, 0.0f, 2.0f> SSSStrength = 0.2f;   // _SSSStrength
            ClampedValue<float, 0.0f, 1.0f> Roughness = 0.2f;     // _Roughness
            ClampedValue<float, 0.0f, 0.01f> RoughnessScale = 0.005f; // _RoughnessScale
            ClampedValue<float, 0.0f, 1.0f> MaxGloss = 0.9f;      // _MaxGloss

            ClampedValue<float, 0.0f, 7.0f> FoamBiasLOD0 = 1.0f;  // _FoamBiasLOD0
            ClampedValue<float, 0.0f, 7.0f> FoamBiasLOD1 = 1.0f;  // _FoamBiasLOD1
            ClampedValue<float, 0.0f, 7.0f> FoamBiasLOD2 = 1.0f;  // _FoamBiasLOD2
            ClampedValue<float, 0.0f, 20.0f> FoamScale = 1.0f;    // _FoamScale
            ClampedValue<float, 0.0f, 1.0f> ContactFoam = 1.0f;   // _ContactFoam

            ClampedValue<float, 1.0f, 10.0f> LODScale = 7.0f;   // Controls LOD fade, higher = more aggressive LOD
            ClampedValue<float, -5.0f, 1.0f> SSSBase = -1.0f;  // Base depth of subsurface, negative for realism
            ClampedValue<float, 0.1f, 50.0f> SSSScale = 4.0f;   // Spread/falloff of SSS, higher = softer fade


            float g = 9.81f;
            float depth = 100.0f;
            ClampedValue<float, 0.0f, 1.0f> lambda = 0.5f;

            DisplaySpectrumSettings Local;
            DisplaySpectrumSettings Swell;

            auto operator<=>(const OceanSettings&) const = default;
        }Settings;

    private:

        void Init();
        Count<class Texture2D> m_NoiseTexture;
        std::array<Count<ComputePass>,2> m_HorizontalStep;
        std::array<Count<ComputePass>, 2> m_VerticalStep;
        Count<class ComputePass> m_PermuteStep;
        Count<class ComputePass> m_TwiddleFacorsPass;

        Count<class Image2D> m_PrcomuteData;

        Count<class RenderCommandBuffer> m_CommandBuffer;

		std::vector<Count<FFTWaveRealisticCascade>> m_Cascades;
        Count<class RenderMaterial> m_RenderMaterial;
        bool m_InitilizedPrecompute = false;
        Count<class Mesh> m_Grid;

        Count<class UniformBufferSet> m_UBOceanSettingsBuffer;
		Count<class UniformBufferSet> m_UBOceanParamsBuffer;
	};
}