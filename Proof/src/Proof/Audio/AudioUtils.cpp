#include "Proofprch.h"
#include "AudioUtils.h"
#include "AudioTools.h"
#include "Proof/Scene/Component.h"
#include "Proof/Asset/AssetManager.h"
#include "Audio.h"
namespace Proof::Utils {

    SoundConfiguration AudioComponentToSoundConfig(const AudioComponent& component)
    {
        SoundConfiguration soundConfig;
        if(AssetManager::HasAsset(component.AudioAsset);
            soundConfig.Aduio =AssetManager::GetAsset<Audio>(component.AudioAsset));

        soundConfig.VolumeMultiplier = component.VolumeMultiplier;
        soundConfig.PitchMultiplier = component.PitchMultiplier;
        soundConfig.Looping = component.Looping;
        soundConfig.MasterReverbSend = component.MasterReverbSend;
        soundConfig.LowPassFilter = component.LowPassFilter;
        soundConfig.HighPassFilterValue = component.HighPassFilter;
        soundConfig.SpatializationEnabled = component.SpatializationEnabled;

        // SPECILIZATION

        soundConfig.AttenuationMod = component.AttenuationModel;
        
        soundConfig.MinGain = component.MinGain;
        soundConfig.MaxGain = component.MaxGain;

        soundConfig.MinDistance = component.MinDistance;
        soundConfig.MaxDistance = component.MaxDistance;
        
        soundConfig.ConeInnerAngleInRadians = component.ConeInnerAngleInRadians;
        soundConfig.ConeOuterAngleInRadians = component.ConeOuterAngleInRadians;
        soundConfig.ConeOuterGain = component.ConeOuterGain;
       
        soundConfig.DopplerFactor = component.DopplerFactor;
        soundConfig.Rolloff = component.Rolloff;
        
        return soundConfig;
    }
    AudioTransform TransformToAudioTransform(const TransformComponent& transform)
    {
        auto rotation = transform.GetRotation();
        return {
            transform.Location,
            rotation * Math::GetFowardVector() /* orientation */,
            rotation * Math::GetUpVector()  /* up */
        };
    }
}

