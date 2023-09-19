#pragma once
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
        float VolumeMultiplier = 1.0f; // min 0, max 1
        float PitchMultiplier = 1.0f;//min 0, max 24
        bool Looping = false;

        bool SpatializationEnabled = false;

        float MasterReverbSend = 0.0f;
        float LowPassFilter = 1.0f;
        float HighPassFilterValue = 0.0f;

    };

    struct SoundSpecialization
    {

        AttenuationModel AttenuationMod{ AttenuationModel::Inverse };   // Distance attenuation function
        float MinGain{ 0.0f };                                            // Minumum volume muliplier
        float MaxGain{ 1.0f };                                            // Maximum volume multiplier
        float MinDistance{ 1.0f };                                        // Distance where to start attenuation
        float MaxDistance{ 1000.0f };                                     // Distance where to end attenuation
        float ConeInnerAngleInRadians{ 6.283185f };                     // Defines the angle where no directional attenuation occurs 
        float ConeOuterAngleInRadians{ 6.283185f };                     // Defines the angle where directional attenuation is at max value (lowest multiplier)
        float ConeOuterGain{ 0.0f };                                      // Attenuation multiplier when direction of the emmiter falls outside of the ConeOuterAngle
        float DopplerFactor{ 1.0f };                                      // The amount of doppler effect to apply. Set to 0 to disables doppler effect. 
        float Rolloff{ 0.6f };                                            // Affects steepness of the attenuation curve. At 1.0 Inverse model is the same as Exponential

        bool AirAbsorptionEnabled{ true };                            // TODO Enable Air Absorption filter 

        bool SpreadFromSourceSize{ true };                             // If this option is enabled, spread of a sound source automatically calculated from the source size.
        float SourceSize{ 1.0f };                                       // Diameter of the sound source in game world.
        float Spread{ 1.0f };
        float Focus{ 1.0f };
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