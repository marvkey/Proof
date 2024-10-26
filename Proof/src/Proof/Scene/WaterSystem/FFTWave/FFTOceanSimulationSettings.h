#pragma once
#include "Proof/Core/Core.h"
#include <glm/glm.hpp>
#include <glm/common.hpp>
namespace Proof
{

	enum class FFTResolutionValue
	{
		Low = 64,
		Medium = 128,
		High = 256,
		Extreme = 512
	};

	enum FFTCascadeNumberValue
	{
		Two = 2,
		Three = 3,
		Four = 4
	};

	enum class FFTReadbackCascadeMode
	{
		None = 0,
		Largest = 1,
		TwoLargest = 2
	};

	enum class FFTCascadeDomainsMode
	{
		Auto,
		Manual
	};
	struct OceanSimulationSettings
	{
		FFTResolutionValue Resolution = FFTResolutionValue::Medium;
		FFTCascadeNumberValue CascadeNumber = FFTCascadeNumberValue::Four;

		float AnisoLevel = 6; // 0-9
		bool UpdateSpectrum = true;
		bool SimulateFoam = true;
		FFTReadbackCascadeMode ReadBackCascades = FFTReadbackCascadeMode::None;
		uint32_t SamplingIterations = 3; //1 -5

		FFTCascadeDomainsMode CascadeDomainsMode = FFTCascadeDomainsMode::Auto;

		// Auto
		float SimulationScale = 400;

		// manual Cascade
		bool AllowOverlap = false;
		float MinWavesInCascade = 6; //1 - 10

		float C0Scale;
		float C1Scale;
		float C2Scale;
		float C3Scale;

		const uint32_t SmallestWaveMultiplierAuto = 4;
		const uint32_t MinWavesInCascadeAuto = 6;


        glm::vec4 LengthScales()
        {
            glm::vec4 lengthScales = glm::vec4(0);
            if (CascadeDomainsMode == FFTCascadeDomainsMode::Auto)
            {
                lengthScales[0] = SimulationScale;
                for (int i = 1; i < 4; i++)
                {
                    lengthScales[i] = lengthScales[i - 1] * SmallestWaveMultiplierAuto * MinWavesInCascadeAuto / (float)Resolution;
                }
            }
            else
            {
                lengthScales = glm::vec4(C0Scale, C1Scale, C2Scale, C3Scale);
            }
            return lengthScales;
        }

        void CalculateCascadeDomains(glm::vec4& cutoffsLow, glm::vec4& cutoffsHigh)
        {
            if (CascadeDomainsMode == FFTCascadeDomainsMode::Auto)
            {
                CalculateCascadeDomainsManual(LengthScales(), false, MinWavesInCascadeAuto, cutoffsLow, cutoffsHigh);
            }
            else
            {
                CalculateCascadeDomainsManual(LengthScales(), AllowOverlap, MinWavesInCascade, cutoffsLow, cutoffsHigh);
            }
        }

        void CalculateCascadeDomainsManual(glm::vec4 lengthScales, bool allowOverlap, float minWavesInCascade,
            glm::vec4& cutoffsLow, glm::vec4& cutoffsHigh)
        {
            glm::vec4 lows = glm::vec4();
            for (int i = 0; i < 4; i++)
            {
                lows[i] = 2 * 3.1415926535897932384 / lengthScales[i] * minWavesInCascade;
            }

            glm::vec4 highs = glm::vec4();
            for (int i = 0; i < 4; i++)
            {
                highs[i] = 2 * 3.1415926535897932384 * (float)Resolution / lengthScales[i] / SmallestWaveMultiplierAuto;
            }
            highs[(int)CascadeNumber - 1] *= SmallestWaveMultiplierAuto / 2;

            cutoffsHigh = highs;
            cutoffsLow = allowOverlap ? lows : glm::max(lows, glm::vec4(0, highs[0], highs[1], highs[2]));

            if (CascadeNumber < 4)
            {
                cutoffsLow.w = 0;
                cutoffsHigh.w = 0;
            }

            if (CascadeNumber < 3)
            {
                cutoffsLow.z = 0;
                cutoffsHigh.z = 0;
            }
        }
	};


}