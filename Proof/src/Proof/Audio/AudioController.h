#pragma once

#include "Proof/Asset/Asset.h"
#include "AudioTools.h"

namespace Proof
{
    class AudioController : public Asset
    {
    public:
        AudioController() = default;

        ASSET_CLASS_TYPE(AudioController);


    public:
        AssetKey <AssetType::Audio> AudioSource;

        // Pitch
        float MinPitch = 1.0f;
        float MaxPitch = 1.0f;

        // Effects
        float MasterReverbSend = 0.0f;
        float LowPassFilter = 1.0f;
        float HighPassFilter = 0.0f;

        // Spatialization
        bool SpatializationEnabled = false;

        AttenuationModel AttenuationMod = AttenuationModel::Inverse;

        float MinGain = 0.0f;
        float MaxGain = 1.0f;

        float MinDistance = 1.0f;
        float MaxDistance = 1000.0f;

        float ConeInnerAngleInRadians = 6.283185f;
        float ConeOuterAngleInRadians = 6.283185f;
        float ConeOuterGain = 0.0f;

        float DopplerFactor = 1.0f;
        float Rolloff = 0.6f;
    };
}