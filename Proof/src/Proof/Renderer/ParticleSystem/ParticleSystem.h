#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Asset/Asset.h"
#include "Proof/Math/Math.h"
#include <glm/glm.hpp>
#include "ParticleBuffers.h"
#include "Proof/Scene/SceneUtils.h"
#include "Proof/Scene/World.h" //temporary cause waeak Count

//https://github.com/unrealsid/OpenGL-GPU-Particles-2
//https://www.reddit.com/r/GraphicsProgramming/comments/1jn7w3v/i_finally_got_around_to_building_a_gpu/
namespace Proof
{
	class ComputePass;
	class RenderCommandBuffer;

	struct alignas(16) Particle
	{
		glm::vec3 Position = glm::vec3(0,0,0);
		float Rotation = 0.0f;

		glm::vec3 Velocity = glm::vec3(0,0,0);
		float Life = 0.0f; // start at 0 so its immedialtey respawnable

		glm::vec4 Color = glm::vec4{ 1 };

		glm::vec3 Size3D{ 1 };
		int bActive = 0; // if its spawned already
	};
	struct ParticleBurst 
	{
		float StartTime;     // seconds, emitter-local time
		uint32_t   Count = 50;         // how many to spawn per burst
		uint32_t   Cycles = 1;    // how many times to repeat (1 = once)
		float Interval = 0;  // seconds between repeats (0 = all at startTime)
		float Probability = 1.0f; // optional: 0..1 to randomly trigger
	};
	class ParticleWorld : public RefCounted
	{
	public:
		ParticleWorld(class World* world);
		void OnUpdate(float update);
		Count<class World> GetWorld();
	private:
		WeakCount<class World> m_World;
		Count<ComputePass> m_ParticleUpdateComputePass;
	};

	class ParticleEmitter : public RefCounted
	{
	public:
		ParticleEmitter(uint32_t maxParticles = 100);
		ParticleEmitter(Count<ParticleEmitter> emitter);
		void OnUpdate(float dt, const Transform& transform, Count<ComputePass> cmdPass);

		SBParticleInitalState ParticleInitialState;
		SBParticleEmitterSettings ParticleEmitterSettings;

		uint32_t GetParticleCount();
		void ResetMaxParticles(uint32_t size);

		SBParticleTrackableData GetTrackableData();
		AssetKey<AssetType::Texture> Texture;

		bool ShouldRender();

		std::vector< ParticleBurst> Bursts;
	private:
		float m_ParticleAccumulator = 0.0f;
		uint32_t m_MaxParticles = 0;
		glm::vec3 m_CurrentPos = glm::vec3(0.0f), m_PrevPos = glm::vec3(0.0f);

		float m_Time = 0.0f;
		float m_PrevTime = 0.0f;
		Count<class StorageBuffer> m_SBParticlesBuffer;
		Count<class StorageBuffer> m_SBParticleParticleInitalState; // storage cause of aling
		Count<class StorageBuffer> m_SBParticleEmitterSettingsBuffer; // storage cause of align 
		Count<class StorageBuffer> m_SBTrackableData; // storage cause of align comptue shader will edit this
		Count<class StorageBuffer> m_SBPerDrawData; // storage cause of align comptue shader will edit this
		friend class WorldRenderer;
	};

	enum class ParticleSystemState
	{
		None,
		Play,
		Pause,
		End
	};

	class ParticleSystem : public Asset
	{
	public:
		ParticleSystem() {};
		ASSET_CLASS_TYPE(ParticleSystem);
		// Create a new emitter and return a reference to it
		Count<ParticleEmitter> CreateEmitter()
		{
			auto emitter = Count<ParticleEmitter>::Create(); // assuming Count<T>::New() or equivalent constructor
			m_Emmiters.push_back(emitter);
			return emitter;
		}

		void AddEmitter(Count<ParticleEmitter> emitter)
		{
			m_Emmiters.push_back(emitter);
		}
		// Get emitter by index
		Count<ParticleEmitter> GetEmitter(size_t index) const
		{
			if (index < m_Emmiters.size())
				return m_Emmiters[index];
			return nullptr;
		}

		// Remove emitter by index
		void RemoveEmitter(size_t index)
		{
			if (index < m_Emmiters.size())
				m_Emmiters.erase(m_Emmiters.begin() + index);
		}

		// Get number of emitters
		size_t GetEmitterCount() const
		{
			return m_Emmiters.size();
		}
		// State control
		void Play()
		{
			m_State = ParticleSystemState::Play;
		}

		void Pause()
		{
			m_State = ParticleSystemState::Pause;
		}

		void Stop()
		{
			m_State = ParticleSystemState::End;
		}

		ParticleSystemState GetState() const
		{
			return m_State;
		}

	private:
		std::vector<Count<ParticleEmitter>> m_Emmiters;
		ParticleSystemState m_State;
	};

	class ParticleSystemInstance : public RefCounted
	{
	public:
		ParticleSystemInstance(Count<ParticleSystem> system)
			: m_ParticleSystem(system)
		{
			SyncWithParicleSystem();
		}
		ParticleSystemInstance();
		ParticleSystemInstance(Count< ParticleSystemInstance> instnace);

		Count<ParticleSystem> GetParticleSystem() const { return m_ParticleSystem; }
		void SyncWithParicleSystem();

		void OnUpdate(float dt, const Transform& transform, Count<ComputePass> cmdPass);

		const std::vector<Count<ParticleEmitter>>& GetEmitters()
		{
			return m_Emmiters;
		};

		void SetParticleSystem(Count<ParticleSystem> system);
	private:
		Count<ParticleSystem> m_ParticleSystem;
		std::vector<Count<ParticleEmitter>> m_Emmiters;
		ParticleSystemState m_State;

		friend class ParticleSystemEditorPanel;
	};
}