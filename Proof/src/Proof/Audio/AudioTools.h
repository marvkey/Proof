#pragma once
#include "Proof/Core/Core.h"
#include <glm/glm.hpp>
namespace Proof
{
    static constexpr auto STOPPING_FADE_MS = 28;
    static constexpr float SPEED_OF_SOUND = 343.3f;
    enum class SoundState
    {
        Stopped,
        Starting,
        Playing,
        Pausing,
        Paused,
        Stopping,
        FadingOut,
        FadingIn
    };

    enum class AttenuationModel
    {
        None,          // No distance attenuation and no spatialization.
        Inverse,       // Equivalent to OpenAL's AL_INVERSE_DISTANCE_CLAMPED.
        Linear,        // Linear attenuation. Equivalent to OpenAL's AL_LINEAR_DISTANCE_CLAMPED.
        Exponential    // Exponential attenuation. Equivalent to OpenAL's AL_EXPONENT_DISTANCE_CLAMPED.

    };
    struct SoundConfiguration
    {
        Count<class Audio> Aduio =nullptr;  
        float VolumeMultiplier = 1.0f; // min 0, max 1
        float PitchMultiplier = 1.0f;//min 0, max 24
        bool Looping = false;
        //Master Reverb Send is a parameter that controls the amount of an audio signal that is sent to a global reverb effect. 
        //It typically ranges from 0.0 (no signal sent) to 1.0 (full signal sent). 
        //This parameter affects the overall reverb or environmental effect applied to the audio mix but does not involve spatialization.
        // kinda of liek the ever of the sound
        float MasterReverbSend = 0.0f; 

        //n summary, a high-pass filter is an audio processing tool that allows high-frequency components to pass through while attenuating lower frequencies.
        //It has various practical applications in audio engineering, from noise reduction to creative sound shaping and speaker protection. 
        //The choice of cutoff frequency and filter type depends on the specific audio processing goals.
        float LowPassFilter = 1.0f;
        //A low-pass filter (LPF) is like a gate for audio that lets low-frequency sounds pass through while blocking or reducing higher-frequency sounds. 
        //It allows the deep, bassy tones to come through while cutting out or softening the sharp, high-pitched sounds.
        //It's like turning down the treble on your music player to make the music sound smoother and warmer.
        float HighPassFilterValue = 0.0f;

        bool SpatializationEnabled = false;

        // these only work f specialization is enabled
        AttenuationModel AttenuationMod{ AttenuationModel::Inverse };   // Distance attenuation function
        float MinGain{ 0.0f };                                            // Minumum volume muliplier 0-1
        float MaxGain{ 1.0f };                                            // Maximum volume multiplier 0-1
        float MinDistance{ 1.0f };                                        // Distance where to start attenuation  0-maxDisntace
        float MaxDistance{ 1000.0f };                                     // Distance where to end attenuation  minDistance  - infinity
        float ConeInnerAngleInRadians{ 6.283185f };                     // Defines the angle where no directional attenuation occurs 0-6.283185f
        float ConeOuterAngleInRadians{ 6.283185f };                     // Defines the angle where directional attenuation is at max value (lowest multiplier) 0-6.283185f
        float ConeOuterGain{ 0.0f };                                      // Attenuation multiplier when direction of the emmiter falls outside of the ConeOuterAngle min 0- max 1
        float DopplerFactor{ 1.0f };                                      // The amount of doppler effect to apply. Set to 0 to disables doppler effect.  // 0 -1

        float Rolloff{ 0.6f };                                            // Affects steepness of the attenuation curve. At 1.0 Inverse model is the same as Exponential min 0 max 1

        //bool AirAbsorptionEnabled{ true };                            // TODO Enable Air Absorption filter 
        //
        //bool SpreadFromSourceSize{ true };                             // If this option is enabled, spread of a sound source automatically calculated from the source size.
        //float SourceSize{ 1.0f };                                       // Diameter of the sound source in game world.
        //float Spread{ 1.0f };
        //float Focus{ 1.0f };

        bool operator==(const SoundConfiguration& other) const {
            return Aduio == other.Aduio &&
                VolumeMultiplier == other.VolumeMultiplier &&
                PitchMultiplier == other.PitchMultiplier &&
                Looping == other.Looping &&
                MasterReverbSend == other.MasterReverbSend &&
                LowPassFilter == other.LowPassFilter &&
                HighPassFilterValue == other.HighPassFilterValue &&
                SpatializationEnabled == other.SpatializationEnabled &&
                AttenuationMod == other.AttenuationMod &&
                MinGain == other.MinGain &&
                MaxGain == other.MaxGain &&
                MinDistance == other.MinDistance &&
                MaxDistance == other.MaxDistance &&
                ConeInnerAngleInRadians == other.ConeInnerAngleInRadians &&
                ConeOuterAngleInRadians == other.ConeOuterAngleInRadians &&
                ConeOuterGain == other.ConeOuterGain &&
                DopplerFactor == other.DopplerFactor &&
                Rolloff == other.Rolloff;// &&
                //AirAbsorptionEnabled == other.AirAbsorptionEnabled &&
                //SpreadFromSourceSize == other.SpreadFromSourceSize &&
                //SourceSize == other.SourceSize &&
                //Spread == other.Spread &&
                //Focus == other.Focus;
        }

        bool operator!=(const SoundConfiguration& other) const {
            return !(*this == other);
        }
    };

    struct AudioTransform
    {
        glm::vec3 Position{ 0.0f, 0.0f, 0.0f };
        glm::vec3 Orientation{ 0.0f, 0.0f, -1.0f };
        glm::vec3 Up = { 0,1,0 };

        bool operator==(const AudioTransform& other) const
        {
            return Position == other.Position && Orientation == other.Orientation && Up == other.Up;
        }

        bool operator!=(const AudioTransform& other) const
        {
            return !(*this == other);
        }

    };
}