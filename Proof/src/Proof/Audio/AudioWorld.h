#pragma once

#include "Proof/Core/Core.h"
#include "Proof/Core/UUID.h"
#include "Proof/Scene/WorldSubSystem.h"

#include "AudioTools.h"
#include "Proof/Scene/World.h"

#include <array>
#include <unordered_map>

namespace Proof
{
	class AudioWorld : public WorldSubSystem
	{
	public:
		AudioWorld(class World* world);
		virtual ~AudioWorld() override;

		void SetContext(class World* world) override;

		void OnUpdate(float deltaTime) override;

		void StartRuntime() override;
		void EndRuntime() override;

		void TransferToWorld(class World* world);
		void Shutdown() override;

		void UpdateListenerPosition(const AudioTransform& transform);
		void UpdateListenerConeAttenuation(float innerAngleRadians = 6.283185f, float outerAngleRadians = 6.283185f, float outerGrain = 0.0f);
		void UpdateAudioListenerVelocity(const glm::vec3& velocity = {});

		void UpdateAudio(UUID soundId, const SoundConfiguration& soundConfiguration);
		void UpdateAudioTransform(UUID soundId, const AudioTransform& transform);
		void UpdateAudioVelocity(UUID soundId, const glm::vec3& velocity);

		bool HasSoundID(UUID hasSoundId);

		void InstantiateAudioSource(class Entity entity);
		void DeleteAudioSource(class Entity entity);

	private:
		Count<class Sound> AddAudio(class Entity entity);

	private:
		WeakCount<class World> m_World;

		struct RuntimeData
		{
			std::unordered_map<UUID, Count<class Sound>> WorldSounds;
			std::array<Count<class AudioListenerActor>, 4> AudioListeners;
			uint32_t AudioListenersCount = 0;
		} m_Runtime;

		std::unordered_map<UUID, Count<class Sound>> m_PersistentSounds;
	};
}