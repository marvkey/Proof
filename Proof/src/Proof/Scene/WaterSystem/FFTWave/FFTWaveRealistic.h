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
        float WindSpeed = 1.5f;
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

            void Generate(Count<class FFTWaveRealistic> waveRealistic, uint32_t lengthScale, float cutOfflow, float CutOffHigh, Count<class RenderCommandBuffer> renderCommandBuffer);
        private:

            Count<ComputePass> m_InitialSpectrumPass;
            Count<ComputePass> m_ConjuagatedSpectrumPass;

            Count<Image2D> m_InitialSpectrumMap;
            //precomputed data
            Count<Image2D> m_WavesData;
            Count<Image2D> m_BufferMap;

            Count<class StorageBufferSet> m_SpectrumParameters;
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
            ClampedValue<float, 0.0f, 1.0f> lambda = 1.0f;
            float g = 9.81f;
            float depth = 3.0f;

            DisplaySpectrumSettings Local;
            DisplaySpectrumSettings Swell;

            glm::vec4 Color = glm::vec4(0.011126082368383245, 0.05637409755197975, 0.09868919754109445,1.0f);                    // _Color
            ClampedValue<float, 0.0f, 1.0f> MaxGloss = 0.91f;      // _MaxGloss
            ClampedValue<float, 0.0f, 0.01f> RoughnessScale = 0.0044f; // _RoughnessScale
            ClampedValue<float, 1.0f, 10.0f> LODScale = 7.13f;   // Controls LOD fade, higher = more aggressive LOD

            glm::vec4 FoamColor = glm::vec4(1.0f); // _FoamColor
            ClampedValue<float, 0.0f, 20.0f> FoamScale = 2.4f;    // _FoamScale
            ClampedValue<float, 0.0f, 1.0f> ContactFoam = 1.0f;   // _ContactFoam
            ClampedValue<float, 0.0f, 7.0f> FoamBiasLOD0 = 2.72f;  // _FoamBiasLOD0
            ClampedValue<float, 0.0f, 7.0f> FoamBiasLOD1 = 2.72f;  // _FoamBiasLOD1
            ClampedValue<float, 0.0f, 7.0f> FoamBiasLOD2 = 2.72f;  // _FoamBiasLOD2

            glm::vec4 SSSColor = glm::vec4(0.1541919, 0.8857628, 0.990566,1.0f);                 // _SSSColor
            ClampedValue<float, -5.0f, 1.0f> SSSBase = -0.261f;  // Base depth of subsurface, negative for realism
            ClampedValue<float, 0.1f, 50.0f> SSSScale = 4.7f;   // Spread/falloff of SSS, higher = softer fade
            ClampedValue<float, 0.0f, 2.0f> SSSStrength = 0.15f;   // _SSSStrength


            ClampedValue<float, 0.0f, 1.0f> Roughness = 0.311f;     // _Roughness

            auto operator<=>(const OceanSettings&) const = default;
        }Settings;
        Count<class RenderMaterial> GetRenderMaterial(int lodLevel);

    private:

        void Init();
    private:
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