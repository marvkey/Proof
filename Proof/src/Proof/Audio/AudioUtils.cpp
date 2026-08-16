#include "Proofprch.h"
#include "AudioUtils.h"
#include "AudioTools.h"
#include "Proof/Scene/Component.h"
#include "Proof/Asset/AssetManager.h"
#include "Audio.h"
#include "AudioController.h"
#include "Proof/Math/Random.h"

namespace Proof::Utils {

    SoundConfiguration AudioComponentToSoundConfig(const AudioComponent& component)
    {
        SoundConfiguration soundConfig;

        if (!component.AudioController.IsValid())
            return soundConfig;

        Count<AudioController> audioController = component.AudioController.GetAsset<AudioController>();

        if (audioController == nullptr)
            return soundConfig;

        soundConfig.Aduio = audioController->AudioSource.GetAsset<Audio>();

        soundConfig.VolumeMultiplier = component.VolumeMultiplier;
        soundConfig.PitchMultiplier = component.PitchMultiplier * Random::Real(audioController->MinPitch,audioController->MaxPitch);
        soundConfig.Looping = component.Looping;

        soundConfig.MasterReverbSend = audioController->MasterReverbSend;
        soundConfig.LowPassFilter = audioController->LowPassFilter;
        soundConfig.HighPassFilterValue = audioController->HighPassFilter;
        soundConfig.SpatializationEnabled = audioController->SpatializationEnabled;

        // SPECILIZATION

        soundConfig.AttenuationMod = audioController->AttenuationMod;

        soundConfig.MinGain = audioController->MinGain;
        soundConfig.MaxGain = audioController->MaxGain;

        soundConfig.MinDistance = audioController->MinDistance;
        soundConfig.MaxDistance = audioController->MaxDistance;

        soundConfig.ConeInnerAngleInRadians = audioController->ConeInnerAngleInRadians;
        soundConfig.ConeOuterAngleInRadians = audioController->ConeOuterAngleInRadians;
        soundConfig.ConeOuterGain = audioController->ConeOuterGain;

        soundConfig.DopplerFactor = audioController->DopplerFactor;
        soundConfig.Rolloff = audioController->Rolloff;

        return soundConfig;
    }
    AudioTransform TransformToAudioTransform(const TransformComponent& transform)
    {
        auto rotation = transform.GetRotation();
        return {
            transform.Location,
            rotation * Math::GetForwardVector() /* orientation */,
            rotation * Math::GetUpVector()  /* up */
        };
    }
}

