#pragma once
#include <filesystem>
#include <MiniAudio.h>
#include "AudioTools.h"
namespace Proof
{
	class AudioEngine 
	{
	public:
		static void PlaySoundByPath(const std::filesystem::path& path); // mainly for assetManager

		static ma_engine& GetEngine();

		static void BeginContext(Count<class World> world);
		static void OnUpdate(float deltaTime);
		static void EndContext();

		static void UpdateListenerPosition(const AudioTransform& transform);
		static void UpdateListenerConeAttenuation(float innerAngleRadians = 6.283185f, float outerAngleRadians = 6.283185f, float outerGrain = 0.0);
		static void UpdateAudioListenerVelocity(const glm::vec3& velocity = {});

		static void UpdateAudio(UUID soundId, const SoundConfiguration& soundCOnfiguration);
		static void UpdateAudioTransform(UUID soundId, const AudioTransform& transform);
		static void UpdateAudioVelocity(UUID soundId,const glm::vec3& velocity);
	private:
		static void Init();
		static void ShutDown();
		friend class Application;
	};
}