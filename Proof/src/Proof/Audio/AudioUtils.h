#pragma once
namespace Proof
{
	struct SoundConfiguration;
	struct AudioComponent;
	struct AudioTransform;
	struct TransformComponent;
	namespace Utils 
	{

		SoundConfiguration AudioComponentToSoundConfig(const AudioComponent& component);

		AudioTransform TransformToAudioTransform(const TransformComponent& transform);

		static inline float DecibelsToVolume(float decibels)
		{
			return std::pow(10.0f, decibels / 20.0f);
		}

		static inline float VolumeToDecibels(float volume)
		{
			if (volume <= 0.0f)
				return -80.0f;

			return 20.0f * std::log10(volume);
		}
	}
}