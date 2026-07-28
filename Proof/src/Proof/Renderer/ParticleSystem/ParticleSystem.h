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

	enum class ParticleSystemState
	{
		None,
		Play,
		Pause,
		End
	};

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
		uint32_t  Count = 50;         // how many to spawn per burst
		uint32_t  Cycles = 1;    // how many times to repeat (1 = once)
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

	class ParticleEmitter : public Asset
	{
	public:


		ParticleEmitter();
		ASSET_CLASS_TYPE(ParticleEmitter);

		uint32_t GetParticleCount()
		{
			return m_MaxParticles;
		}

		std::vector< ParticleBurst> Bursts;
		AssetKey<AssetType::Texture> Texture;

		SBParticleInitalState ParticleInitialState;
		SBParticleEmitterSettings ParticleEmitterSettings;


	private:
		uint32_t m_MaxParticles = 0;
		friend class ParticleEmitterInstance;

	};

	class ParticleEmitterInstance : public RefCounted
	{
	public:
		ParticleEmitterInstance(Count<ParticleEmitter> emitter);
		void OnUpdate(float dt, const Transform& transform, Count<ComputePass> cmdPass);
		SBParticleTrackableData GetTrackableData();
		bool ShouldRender();

		void SyncWithParticleEmitter();

		void Play(bool restart = true);
		void Pause();
		void Stop();

	private:
		void Reset(uint32_t maxParticles);

	private:

		ParticleSystemState m_State;
		uint32_t m_MaxParticles = 0;  
		float m_ParticleAccumulator = 0.0f;
		glm::vec3 m_CurrentPos = glm::vec3(0.0f), m_PrevPos = glm::vec3(0.0f);

		float m_Time = 0.0f;
		float m_PrevTime = 0.0f;
		Count<class StorageBuffer> m_SBParticlesBuffer;
		Count<class StorageBuffer> m_SBParticleParticleInitalState; // storage cause of aling
		Count<class StorageBuffer> m_SBParticleEmitterSettingsBuffer; // storage cause of align 
		Count<class StorageBuffer> m_SBTrackableData; // storage cause of align comptue shader will edit this
		Count<class StorageBuffer> m_SBPerDrawData; // storage cause of align comptue shader will edit this

		Count<ParticleEmitter> m_Emitter;
		friend class WorldRenderer;
	};


	struct ParticleSystemEmitterClip 
	{
		ParticleSystemEmitterClip(AssetKey<AssetType::ParticleEmitter> emitter)
			:Emitter(emitter)
		{
			
		}

		ParticleSystemEmitterClip()
		{
			
		}
		AssetKey<AssetType::ParticleEmitter> Emitter;
		float StartTime = 0.0f;
		bool Looping = false;
	};
							 
	struct ParticleSystemTrack 
	{
		ParticleSystemTrack()
		{
			
		}

		ParticleSystemTrack(const ParticleSystemTrack& track)
		{
			Emitter = track.Emitter;
			Clips = track.Clips;

		}

		AssetKey<AssetType::ParticleEmitter> Emitter;
		std::vector<ParticleSystemEmitterClip> Clips;
	};

	struct ParticleSystemTrackInstance
	{
		ParticleSystemTrackInstance()
		{
			
		}


		ParticleSystemTrackInstance(const ParticleSystemTrack& track)
		{
			  if (!track.Emitter.IsValid())
				return;

			Emitter = track.Emitter;
			for (auto clip : track.Clips)
			{
				EmitterClipInstance  instnace;
				instnace.Clip  = clip;
				instnace.Emitter = Count<ParticleEmitterInstance>::Create(track.Emitter.GetAsset<ParticleEmitter>());
			}
			
		}

		AssetKey<AssetType::ParticleEmitter> Emitter;
		struct EmitterClipInstance
		{
			EmitterClipInstance()
			{
				
			}
			ParticleSystemEmitterClip Clip;
			Count<ParticleEmitterInstance> Emitter;
		};

		std::vector<EmitterClipInstance> EmitterClips;
	};


	class ParticleSystemTimeline : public RefCounted
	{
		public:

		ParticleSystemTimeline()
		{
			
		}

		ParticleSystemTimeline(Count<ParticleSystemTimeline> timeline)
		{
			for (auto& [id, track] : timeline->m_Tracks)
			{
				AssetKey<AssetType::ParticleEmitter> emitter = id;
				
				if (!emitter.IsValid())
					continue;

				m_Tracks[id] = track; // copy track
				
			}
		}

		const std::unordered_map<AssetID, ParticleSystemTrack>& GetTracks(){return m_Tracks;};
		const std::unordered_map<AssetID, ParticleSystemTrack>& GetTracks()const {return m_Tracks;};

		private:
		std::unordered_map<AssetID, ParticleSystemTrack> m_Tracks;


		friend class ParticleSystemTimelineInstance;
	};


	class ParticleSystemTimelineInstance : public RefCounted
	{
		public:
		ParticleSystemTimelineInstance(Count<ParticleSystemTimeline> timeline)
			:m_Timeline(timeline)
		{
			SyncWithParticleTimeline();
		}

		ParticleSystemTimelineInstance(Count<ParticleSystemTimelineInstance> timeline)
			:m_Timeline(timeline->m_Timeline)
		{
			SyncWithParticleTimeline();
		}


		void SyncWithParticleTimeline()
		{
			if (m_Timeline == nullptr)
			{
				return;
			}

			m_Tracks.clear();
			for (auto& [id, track] : m_Timeline->m_Tracks)
			{
				AssetKey<AssetType::ParticleEmitter> emitter = id;
				if (!emitter.IsValid())
					continue;;

				m_Tracks[id] = ParticleSystemTrackInstance(track);
			}
		}

		const std::unordered_map<AssetID, ParticleSystemTrackInstance>& GetTracks(){return m_Tracks;};

	private:
		Count<ParticleSystemTimeline> m_Timeline;
		std::unordered_map<AssetID, ParticleSystemTrackInstance> m_Tracks;
		friend class ParticleSystemInstance;
	};


	class ParticleSystem : public Asset
	{
	public:
		ParticleSystem() {};
		ASSET_CLASS_TYPE(ParticleSystem);

		Count<ParticleSystemTimeline> GetTimeline(){return m_Timeline;};

	private:
		Count<ParticleSystemTimeline> m_Timeline;
	};

	class ParticleSystemInstance : public RefCounted
	{
	public:
		ParticleSystemInstance(Count<ParticleSystem> system)
			: m_ParticleSystem(system)
		{
			SyncWithParticleSystem();
		}
		ParticleSystemInstance(Count< ParticleSystemInstance> instnace);
		ParticleSystemInstance()
		{
			
		}


		Count<ParticleSystem> GetParticleSystem() const { return m_ParticleSystem; }
		void SyncWithParticleSystem();

		void OnUpdate(float dt, const Transform& transform, Count<ComputePass> cmdPass);

		void SetParticleSystem(Count<ParticleSystem> system);

		// State control
		void Play(bool restart = true);
		void Pause();
		void Stop();

		ParticleSystemState GetState() const
		{
			return m_State;
		}

		Count<ParticleSystemTimelineInstance> GetTimeline(){return m_Timeline;};

	private:

		float m_Time = 0.0f;
		float m_PrevTime = 0.0f;
		Count<ParticleSystem> m_ParticleSystem;
		Count<ParticleSystemTimelineInstance> m_Timeline;
		ParticleSystemState m_State;

		friend class ParticleSystemEditorPanel;
	};
}