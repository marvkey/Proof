#pragma once
#include "Proof/Core/Core.h"
#include <glm/glm.hpp>
#include "FFTOceanSimulationSettings.h"
#include "FFTOceanSimulationInput.h"
//https://github.com/gasgiant/Ocean-URP/blob/main/Assets/OceanSystem/Runtime/OceanSimulation.cs
namespace Proof
{
	class ComputePass;
	class StorageBufferSet;

	class FFTSimulation : public RefCounted
	{
	public:
		FFTSimulation(Count<class FFTWave> wave);
		struct FFTTextures
		{
			Count<class Texture2D> InitialSpectrum;
			Count<class Texture2D> WavesData;
			Count<class Texture2D> InitializationBuffer;
			Count<class Texture2D> Turbulence;
			Count<class Texture2D> FFTInOut;
		} Textures;

		OceanSimulationSettings SimulationSettings;
		float LocalWindDirection = 300; // 0 -360
		float SwellDirection = 300; // 0-360
		float WindForce = 0.5; // 0-1

		void Update(float deltaTime);
	private:
		bool SetUp();
		void InitializeRenderTextures(uint32_t size, uint32_t cascadeNumber, uint32_t anisoLevel);
        void CalculateInitialCascade();
	private:
		Count<class FFTWave> m_Wave;

		Count<ComputePass> m_InitialSpectrumPass;
		Count<ComputePass> m_ConjugateSpectrumPass;
		Count<ComputePass> m_FoamSimulationPass;
		glm::ivec2 m_CurrentTextureParams;
		const glm::uvec3 m_LocalWorkGroups = glm::uvec3{ 8u, 8u, 1u };
		Count<StorageBufferSet> m_SBSpectrumBuffer;
        std::array<UBSpectrumParams, 2> m_Spectrums;
		OceanSimulationInputsProvider m_InputProvider;
		OceanSimulationInputs m_Inputs;
        bool m_IsSpectrumInitialized;
        uint32_t m_Size;
		void Init();
		friend class FFTWave;
	};
}