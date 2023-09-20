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
	}
}