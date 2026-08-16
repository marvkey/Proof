#include "Proofprch.h"
#include "AudioWorld.h"

#include "Sound.h"
#include "AudioUtils.h"
#include "AudioTools.h"
#include "AudioListenerActor.h"

#include "Proof/Scene/World.h"
#include "Proof/Scene/Entity.h"
#include "Proof/Scene/Component.h"

#include "Proof/Asset/AssetManager.h"

#include "Proof/Physics/PhysicsWorld.h"
#include "Proof/Physics/PhysicsActor.h"

namespace Proof
{


	AudioWorld::AudioWorld(World* world)
		: m_World(world)
	{

	}

	Count<Sound> AudioWorld::AddAudio(Entity entity)
	{
		PF_CORE_ASSERT(entity);
		auto& audioComp = entity.GetComponent<AudioComponent>();

		Count<Sound> sound;

		if (audioComp.AudioController.IsValid())
		{
			SoundConfiguration soundConfig = Utils::AudioComponentToSoundConfig(audioComp);
			sound = Count<Sound>::Create(soundConfig);

			if (audioComp.PlayOnAwake)
				sound->Play();
		}
		else
		{
			sound = Count<Sound>::Create();
		}                                         

		m_Runtime.WorldSounds[entity.GetUUID()] = sound;

		return sound;
	}

	void AudioWorld::InstantiateAudioSource(class Entity entity)
	{
		AddAudio(entity);
	}

	void AudioWorld::DeleteAudioSource(class Entity entity)
	{
		PF_CORE_ASSERT(entity);
		auto& audioComp = entity.GetComponent<AudioComponent>();
		if (m_Runtime.WorldSounds.contains(entity.GetUUID()))
			m_Runtime.WorldSounds.erase(entity.GetUUID());
	}

	void AudioWorld::BeginRuntime()
	{
		auto world = m_World.Lock();

		if (!world)
			return;

		auto view = world->GetAllEntitiesWith<AudioComponent>();

		for (auto e : view)
		{
			Entity entity{ e, world.Get() };

			AddAudio(entity)->m_UUID = entity.GetUUID();
		}

		m_Runtime.AudioListeners[0] = Count<AudioListenerActor>::Create();
		m_Runtime.AudioListeners[0]->m_IndexPosition = 0;

		m_Runtime.AudioListenersCount++;
	}

	void AudioWorld::OnUpdate(float deltaTime)
	{
		PF_PROFILE_FUNC();

		auto world = m_World.Lock();

		if (!world)
			return;

		for (auto& [id, sound] : m_Runtime.WorldSounds)
		{
			sound->Update(deltaTime);
		}

		{
			PF_PROFILE_FUNC("AudioWorld::OnUpdate - AudioListener");

			Entity listener;

			auto view = world->GetAllEntitiesWith<AudioListenerComponent>();

			for (auto e : view)
			{
				Entity check = { e, world.Get() };
				auto& listenerComponent = check.GetComponent<AudioListenerComponent>();

				if (listenerComponent.Active)
				{
					listener = check;

					auto transform = Utils::TransformToAudioTransform(world->GetWorldSpaceTransformComponent(listener));

					UpdateListenerPosition(transform);
					UpdateListenerConeAttenuation(listenerComponent.ConeInnerAngleInRadians, listenerComponent.ConeOuterAngleInRadians, listenerComponent.ConeOuterGain);

					auto physicsActor = world->GetPhysicsWorld()->GetActor(listener);

					if (physicsActor)
					{
						if (physicsActor->IsDynamic())
							UpdateAudioListenerVelocity(physicsActor->GetLinearVelocity());
					}
					else
					{
						UpdateAudioListenerVelocity(glm::vec3{ 0 });
					}

					break;
				}
			}
		}

		{

			PF_PROFILE_FUNC("AudioWorld::OnUpdate - Audio");

			auto view = world->GetAllEntitiesWith<AudioComponent>();

			for (auto e : view)
			{
				Entity audioEntity = { e, world.Get() };

				auto& audioComponent = audioEntity.GetComponent<AudioComponent>();
				auto transform = Utils::TransformToAudioTransform(world->GetWorldSpaceTransformComponent(audioEntity));

				Count<Sound> sounds = m_Runtime.WorldSounds[audioEntity.GetUUID()];
				if (!sounds->IsPlaying())   // dont want to update audio soudn while playing because it ranodmize puthc and while playign the ptihc weill be chainging
					UpdateAudio(audioEntity.GetUUID(), Utils::AudioComponentToSoundConfig(audioComponent));
				UpdateAudioTransform(audioEntity.GetUUID(), transform);

				auto physicsActor = world->GetPhysicsWorld()->GetActor(audioEntity);

				if (physicsActor)
				{
					if (physicsActor->IsDynamic())
						UpdateAudioVelocity(audioEntity.GetUUID(), physicsActor->GetLinearVelocity());
				}
				else
				{
					UpdateAudioVelocity(audioEntity.GetUUID(), glm::vec3{ 0 });
				}
			}
		}
	}

	void AudioWorld::EndRuntime()
	{
		m_Runtime = {};
	}

	void AudioWorld::UpdateListenerPosition(const AudioTransform& transform)
	{
		m_Runtime.AudioListeners[0]->UpdateTransform(transform);
	}

	void AudioWorld::UpdateListenerConeAttenuation(float innerAngleRadians, float outerAngleRadians, float outerGrain)
	{
		m_Runtime.AudioListeners[0]->UpdateConeListener(innerAngleRadians, outerAngleRadians, outerGrain);
	}

	void AudioWorld::UpdateAudioListenerVelocity(const glm::vec3& velocity)
	{
		m_Runtime.AudioListeners[0]->UpdateVelocity(velocity);
	}

	void AudioWorld::UpdateAudio(UUID soundId, const SoundConfiguration& soundConfiguration)
	{
		auto world = m_World.Lock();

		if (!world)
			return;

		if (!m_Runtime.WorldSounds.contains(soundId))
		{
			PF_ENGINE_ERROR("Trying to update entity {} doesnt have sound ", world->GetEntity(soundId).GetName());
			return;
		}

		auto sound = m_Runtime.WorldSounds[soundId];
		sound->UpdateDataSource(soundConfiguration);
	}

	void AudioWorld::UpdateAudioTransform(UUID soundId, const AudioTransform& transform)
	{
		auto world = m_World.Lock();

		if (!world)
			return;

		if (!m_Runtime.WorldSounds.contains(soundId))
		{
			PF_ENGINE_ERROR("Trying to update entity {} doesnt have sound ", world->GetEntity(soundId).GetName());
			return;
		}

		auto sound = m_Runtime.WorldSounds[soundId];
		sound->SetTransform(transform);
	}

	void AudioWorld::UpdateAudioVelocity(UUID soundId, const glm::vec3& velocity)
	{
		auto world = m_World.Lock();

		if (!world)
			return;

		if (!m_Runtime.WorldSounds.contains(soundId))
		{
			PF_ENGINE_ERROR("Trying to update entity {} doesnt have sound ", world->GetEntity(soundId).GetName());
			return;
		}

		auto sound = m_Runtime.WorldSounds[soundId];
		sound->SetVelocity(velocity);
	}

	bool AudioWorld::HasSoundID(UUID hasSoundId)
	{
		return m_Runtime.WorldSounds.contains(hasSoundId);
	}


}
