#pragma once
#include "Proof/Core/Core.h"
#include <array>
#include <vector>
#include <glm/glm.hpp>
namespace Proof
{

    struct alignas(16) UBSpectrumParams
    {
        enum class EnergySpectrumModel
        {
            PiersonMoskowitz,
            JONSWAP,
            TMA
        };

        EnergySpectrumModel EnergySpectrum = EnergySpectrumModel::JONSWAP;
        float WindSpeed = 10; //1 -30

        //not aplicable to PiersonMoskowitz
        float Fetch = 1;
        float Peaking = 2; // 1-10

        float Scale;
        float CutoffWavelength;

        float Alignment = 0.1; // 0-1
        float ExtraAlignment = 0.3; // 0-1

        static UBSpectrumParams Lerp(UBSpectrumParams lhs, UBSpectrumParams rhs, float t)
        {

            return UBSpectrumParams
            {
                t < 0.5 ? lhs.EnergySpectrum : rhs.EnergySpectrum,
                glm::mix(lhs.WindSpeed, rhs.WindSpeed, t),
                glm::mix(lhs.Fetch, rhs.Fetch, t),
                glm::mix(lhs.Peaking, rhs.Peaking, t),
                glm::mix(lhs.Scale, rhs.Scale, t),
                glm::mix(lhs.CutoffWavelength, rhs.CutoffWavelength, t),
                glm::mix(lhs.Alignment, rhs.Alignment, t),
                glm::mix(lhs.ExtraAlignment, rhs.ExtraAlignment, t)
            };
        }

        static UBSpectrumParams GetDefaultLocal()
        {
            return UBSpectrumParams
            {
                EnergySpectrumModel::PiersonMoskowitz,
                5,
                100,
                3.3f,
                1,
                0.01f,
                1,
                0
            };
        }

        static UBSpectrumParams GetDefaultSwell()
        {
            return
            {
                EnergySpectrumModel::JONSWAP,
                10,
                100,
                10,
                0,
                1,
                1,
                0.8f
            };
        }
    };
    struct FFTSwellPreset
    {
        UBSpectrumParams Spectrum = UBSpectrumParams::GetDefaultSwell();
        float ReferenceWaveHeight;
    };

    enum class FFTFilterType
    {
        Bell,
        Highshelf,
        Lowshelf
    };

    struct FFFilter
    {
        FFFilter(const FFFilter&) = default;
        FFFilter& operator=(const FFFilter&) = default;
        static inline const float MinWidth = 0.1f;

        FFTFilterType Type;
        float Center;
        float Value; // -1,2
        float Width; //minwidht -3

        float Evaluate(float x) const 
        {
            float arg;
            float w = glm::max(MinWidth, Width);
            switch (Type)
            {
                case FFTFilterType::Bell:
                    arg = (x - Center);
                    return Value * glm::exp(-arg * arg / w / w);
                case FFTFilterType::Highshelf:
                    arg = glm::min((x - Center), 0.f);
                    return Value * glm::exp(-arg * arg / w / w);
                case FFTFilterType::Lowshelf:
                    arg = glm::max((x - Center), 0.f);
                    return Value * glm::exp(-arg * arg / w / w);
                default:
                    return 0;
            }

        }
    };

    struct FFTEqualizerPreset
    {
        inline static const float XMin = -1.5f;
        inline static const float XMax = 3.5f;
        inline static const int Resolution = 128;

        std::array<glm::vec4, Resolution> Colors;

        std::vector<FFFilter> ScaleFilters;
        std::vector<FFFilter> ChopFilters;

        Count<class Texture2D> Ramp;
        static Count<Texture2D> GetDefaultRamp();

        Count<Texture2D> GetRamp();

    private:
        void BakeRamp();

        static float EvaluateFiltersArray(const std::vector<FFFilter>& filters, float x)
        {
            if (filters.empty()) return 1;
            float v = 1;
            for (int j = 0; j < filters.size(); j++)
            {
                v += filters[j].Evaluate(x);
            }
            return glm::max(0.0f, v);
        }
    private:
        inline static Count<class Texture2D> s_DefaultRamp;

    };

    struct FFTLocalWavePreset
    {
        UBSpectrumParams Spectrum = UBSpectrumParams::GetDefaultSwell();

        float ReferenceWaveHeight;
        float Chop = 1; //0 -2
        FFTEqualizerPreset Equalizer;

        float WindForce;
    };

    struct OceanSimulationInputs
    {
        float TimeScale = 1;
        float Depth = 1000;
        float Chop = 1;
        //c FoamParams foam = FoamParams.GetDefault();
        UBSpectrumParams local = UBSpectrumParams::GetDefaultLocal();
        UBSpectrumParams swell = UBSpectrumParams::GetDefaultSwell();
        Count<Texture2D> EqualizerRamp0;
        Count<Texture2D> equalizerRamp1;
        float EqualizerLerpValue;
        float FoamTrailUpdateTime;
        float ReferenceWaveHeight;
    };

    class OceanSimulationInputsProvider
    {
    public:
        enum class InputsProviderMode { Fixed, Scale };

    private:
        InputsProviderMode Mode;
        float TimeScale = 1.0f;
        float Depth = 1000.0f;

        FFTSwellPreset Swell;
        FFTLocalWavePreset LocalWaves;

        float DisplayWindForce01;
        FFTEqualizerPreset DefaultEqualizer;

        std::vector<FFTLocalWavePreset> LocalWavesArray;
        float MaxWindForce = 0;

    public:
        void OnValidate() 
        {
            MaxWindForce = 0;
            if (!LocalWavesArray.empty()) 
            {
                for (const auto& wavesPreset : LocalWavesArray) 
                {
                    if (wavesPreset.WindForce > MaxWindForce) 
                    {
                        MaxWindForce = wavesPreset.WindForce;
                    }
                }
            }
        }

        void SetDisplayWindForce(float displayWindForce01) {
            DisplayWindForce01 = displayWindForce01;
        }

        void PopulateInputs(OceanSimulationInputs& target) {
            PopulateInputs(target, DisplayWindForce01);
        }

        void PopulateInputs(OceanSimulationInputs& target, float windForce01) {
            windForce01 = DisplayWindForce01;

            target.TimeScale = TimeScale;
            target.Depth = Depth;

            float referenceWaveHeight = 0;
            //if (Swell) 
            {
                target.swell = Swell.Spectrum;
                referenceWaveHeight += Swell.ReferenceWaveHeight;
            }

            if (Mode == InputsProviderMode::Fixed || LocalWavesArray.empty() || LocalWavesArray.size() < 2) 
            {
                target.FoamTrailUpdateTime = 0;
                //if (LocalWaves) 
                {
                    SetValues(target, LocalWaves);
                    referenceWaveHeight += LocalWaves.ReferenceWaveHeight;
                }
            }
            else {
                target.FoamTrailUpdateTime = 1;
                LerpVars lerp = GetLerpVars(windForce01, MaxWindForce, LocalWavesArray);

                //if (lerp.Start && lerp.End) 
                {
                    SetValues(target, lerp.Start, lerp.End, lerp.T);
                    referenceWaveHeight += glm::mix(lerp.Start.ReferenceWaveHeight, lerp.End.ReferenceWaveHeight, lerp.T);
                }
            }
            target.ReferenceWaveHeight = referenceWaveHeight;
        }

    private:
        void SetValues(OceanSimulationInputs& target, FFTLocalWavePreset preset) 
        {
            target.Chop = preset.Chop;
            target.local = preset.Spectrum;
            //target.Foam = preset->GetFoam();
            target.EqualizerLerpValue = 0;
            target.EqualizerRamp0 = GetSafeRamp(preset.Equalizer);
            target.equalizerRamp1 = FFTEqualizerPreset::GetDefaultRamp();
        }

        void SetValues(OceanSimulationInputs& target, FFTLocalWavePreset start, FFTLocalWavePreset end, float t)
        {
            target.Chop = glm::mix(start.Chop, end.Chop, t);
            target.local = UBSpectrumParams::Lerp(start.Spectrum, end.Spectrum, t);
           // target.Foam = FoamParams::Lerp(start->GetFoam(), end->GetFoam(), t);
            target.EqualizerLerpValue = t;
            target.EqualizerRamp0 = GetSafeRamp(start.Equalizer);
            target.equalizerRamp1 = GetSafeRamp(end.Equalizer);
        }

        Count<Texture2D> GetSafeRamp(FFTEqualizerPreset eq) 
        {
            //if (eq) 
            {
                return eq.GetRamp();
            }
            //else if (DefaultEqualizer) 
            {
                return DefaultEqualizer.GetRamp();
            }
            //else 
            {
                return FFTEqualizerPreset::GetDefaultRamp();
            }
        }
        struct LerpVars
        {
            float T = 0;
            FFTLocalWavePreset Start;
            FFTLocalWavePreset End;
        };
        static float InverseLerp(float a, float b, float value) {
            return (value - a) / (b - a);
        }
        static LerpVars GetLerpVars(float windForce01, float maxWindForce, const std::vector<FFTLocalWavePreset>& presets)
        {
            LerpVars result;
            if (presets.size() < 2) return result;

            float windForce = glm::clamp(windForce01,0.0f,1.0f) * maxWindForce;
            size_t i;
            for (i = 0; i < presets.size(); ++i) {
                if (windForce < presets[i].WindForce) break;
            }

            if (i == 0) return result;
            if (i == presets.size()) --i;

            result.Start = presets[i - 1];
            result.End = presets[i];
            result.T = InverseLerp(presets[i - 1].WindForce, presets[i].WindForce, windForce);

            return result;
        }

       
    };

}