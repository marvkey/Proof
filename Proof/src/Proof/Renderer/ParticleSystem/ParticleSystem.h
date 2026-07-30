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

		SBParticleInitialSettings ParticleInitialState;
		SBParticleEmitterSettings ParticleEmitterSettings;


	private:
		uint32_t m_MaxParticles = 1000;
		friend class ParticleEmitterInstance;
		friend class ParticleEmitterPanel;
		friend class ParticleEmitterSerilizer;

	};

	class ParticleEmitterInstance : public RefCounted
	{
	public:
		ParticleEmitterInstance(Count<ParticleEmitter> emitter);
		void OnUpdate(float dt, const Transform& transform, Count<ComputePass> cmdPass);
		SBParticleTrackableData GetTrackableData();
		bool ShouldRender();

		void SyncWithParticleEmitter();

		void Play();
		void Pause();
		void Stop(bool waitUntilFinish = false);
		void Restart();

		uint32_t GetParticleCount()
		{
			return m_MaxParticles;
		}

		SBParticleInstanceState ParticleInstanceState;

		void Reset()
		{
			Reset(m_MaxParticles);
		}
		
	private:
		void Reset(uint32_t maxParticles);
		friend class ParticleEmitterPanel;
		friend class ParticleEffect;
	private:
		ParticleSystemState m_State = ParticleSystemState::None;
		bool m_WaitUntilFinish = false;

		uint32_t m_MaxParticles = 0;
		float m_ParticleAccumulator = 0.0f;
		glm::vec3 m_CurrentPos = glm::vec3(0.0f), m_PrevPos = glm::vec3(0.0f);

		float m_Time = 0.0f;
		float m_PrevTime = 0.0f;
		Count<class StorageBuffer> m_SBParticlesBuffer;
		Count<class StorageBuffer> m_SBParticleParticleInitialState; // storage cause of aling
		Count<class StorageBuffer> m_SBParticleParticleInstanceState;
		Count<class StorageBuffer> m_SBParticleEmitterSettingsBuffer; // storage cause of align 
		Count<class StorageBuffer> m_SBTrackableData; // storage cause of align comptue shader will edit this
		Count<class StorageBuffer> m_SBPerDrawData; // storage cause of align comptue shader will edit this

		Count<ParticleEmitter> m_Emitter;
		friend class WorldRenderer;
		friend class ParticleEmitterSerilizer;
		friend class WorldRenderer;
	};

	using ParticleClipID = uint64_t;

	struct ParticleSystemEmitterClip 
	{
		ParticleSystemEmitterClip(AssetKey<AssetType::ParticleEmitter> emitter)
			:Emitter(emitter)
		{
			
		}

		ParticleSystemEmitterClip()
		{
			
		}

		UUID ID = UUID();

		AssetKey<AssetType::ParticleEmitter> Emitter;
		float StartTime = 0.0f;
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

				EmitterClips.push_back(std::move(instnace));
			}
			
		}

		void SyncAllClipInstances()
		{
			for (const auto& emitterCLips: EmitterClips)
			{
				emitterCLips.Emitter->SyncWithParticleEmitter();
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
			bool Started = false;

		};

		std::vector<EmitterClipInstance> EmitterClips;
	};


	class ParticleSystemTimeline : public RefCounted
	{
	public:
		ParticleSystemTimeline() = default;

		ParticleSystemTimeline(Count<ParticleSystemTimeline> timeline)
		{
			if (!timeline)
				return;

			m_Tracks = timeline->m_Tracks;
		}

		bool AddTrack(const ParticleSystemTrack& track)
		{
			if (!track.Emitter.IsValid())
				return false;

			AssetID id = track.Emitter.GetAssetID();

			if (m_Tracks.contains(id))
				return false;

			m_Tracks.emplace(id, std::move(track));

		return true;
		}



		bool AddTrack(AssetKey<AssetType::ParticleEmitter> emitter)
		{
			if (!emitter.IsValid())
				return false;

			AssetID id = emitter.GetAssetID();

			if (m_Tracks.contains(id))
				return false;

			ParticleSystemTrack track;
			track.Emitter = emitter;

			m_Tracks.emplace(id, std::move(track));
			return true;
		}

		bool RemoveTrack(AssetID id)
		{
			return m_Tracks.erase(id) > 0;
		}

		bool RemoveTrack(AssetKey<AssetType::ParticleEmitter> emitter)
		{
			return RemoveTrack(emitter.GetAssetID());
		}

		bool HasTrack(AssetID id) const
		{
			return m_Tracks.contains(id);
		}

		bool HasTrack(AssetKey<AssetType::ParticleEmitter> emitter) const
		{
			return HasTrack(emitter.GetAssetID());
		}

		ParticleSystemTrack* GetTrack(AssetID id)
		{
			auto it = m_Tracks.find(id);

			if (it == m_Tracks.end())
				return nullptr;

			return &it->second;
		}

		const ParticleSystemTrack* GetTrack(AssetID id) const
		{
			auto it = m_Tracks.find(id);

			if (it == m_Tracks.end())
				return nullptr;

			return &it->second;
		}

		void ClearTracks()
		{
			m_Tracks.clear();
		}

		uint32_t GetTrackCount() const
		{
			return static_cast<uint32_t>(m_Tracks.size());
		}

		std::unordered_map<AssetID, ParticleSystemTrack>& GetTracks()
		{
			return m_Tracks;
		}

		const std::unordered_map<AssetID, ParticleSystemTrack>& GetTracks() const
		{
			return m_Tracks;
		}

	private:
		std::unordered_map<AssetID, ParticleSystemTrack> m_Tracks;

		friend class ParticleSystemTimelineInstance;
		friend class ParticleSystem;
	};


	class ParticleSystemTimelineInstance : public RefCounted
	{
	public:
		ParticleSystemTimelineInstance(Count<ParticleSystemTimeline> timeline)
			: m_Timeline(timeline)
		{
			SyncWithParticleTimeline();
		}

		ParticleSystemTimelineInstance(Count<ParticleSystemTimelineInstance> timeline)
			: m_Timeline(timeline ? timeline->m_Timeline : nullptr)
		{
			SyncWithParticleTimeline();
		}

		void SyncWithParticleTimeline()
		{
			if (!m_Timeline)
				return;

			m_Tracks.clear();

			for (const auto& [id, track] : m_Timeline->m_Tracks)
			{
				
				m_Tracks.emplace(id, ParticleSystemTrackInstance(track));
			}
		}


		const ParticleSystemTrackInstance* GetTrack(AssetID id) const
		{
			auto it = m_Tracks.find(id);
			return it != m_Tracks.end() ? &it->second : nullptr;
		}

		const std::unordered_map<AssetID, ParticleSystemTrackInstance>& GetTracks() const
		{
			return m_Tracks;
		}

	private:
		Count<ParticleSystemTimeline> m_Timeline;
		std::unordered_map<AssetID, ParticleSystemTrackInstance> m_Tracks;

		friend class ParticleEffect;
		friend class ParticleEmitterPanel;
		friend class ParticleSystem;
	};


	class ParticleSystem : public Asset
	{
	public:
		ParticleSystem()
		{
			m_Timeline = Count<ParticleSystemTimeline>::Create();
		}

		ParticleSystem(Count<ParticleSystem> timeline)
		{
			m_Timeline = Count<ParticleSystemTimeline>::Create(timeline->m_Timeline);
		}
		ParticleSystem(AssetKey<AssetType::ParticleEmitter> emmiter )
		{
			m_Timeline = Count<ParticleSystemTimeline>::Create();
			if (emmiter.IsValid())
			{
				
				m_Timeline->m_Tracks[emmiter.GetAssetID()] = {}; 
				m_Timeline->m_Tracks[emmiter.GetAssetID()].Emitter = emmiter;; 
				m_Timeline->m_Tracks[emmiter.GetAssetID()].Clips.push_back(ParticleSystemEmitterClip(emmiter)); 
			}
		}

		ASSET_CLASS_TYPE(ParticleSystem);

		Count<ParticleSystemTimeline> GetTimeline(){return m_Timeline;};

	private:
		Count<ParticleSystemTimeline> m_Timeline;
		friend class ParticleSystemSerilizer;
	};

	class ParticleEffect : public RefCounted
	{
	public:
		ParticleEffect(Count<ParticleSystem> system)
			: m_ParticleSystem(system)
		{
			SyncWithParticleSystem();
		}

		ParticleEffect(Count<ParticleEffect> instnace);

		ParticleEffect()
		{
		
		}

		Count<ParticleSystem> GetParticleSystem() const { return m_ParticleSystem; }
		void SyncWithParticleSystem();

		void OnUpdate(float dt, const Transform& transform, Count<ComputePass> cmdPass);

		void SetParticleSystem(Count<ParticleSystem> system);

		// State control
		void Play();
		void Pause();
		void Stop(bool waitUntilFinish = false);
		void Restart();
		void Reset();

		ParticleSystemState GetState() const
		{
			return m_State;
		}

		Count<ParticleSystemTimelineInstance> GetTimeline()
		{
			return m_Timeline;
		}

		bool SetLooping(bool loop);

		bool GetLooping() const
		{
			return m_Looping;
		}

		bool UpdateOffscreen = true;
		float SimulationSpeed = 1.0f;

	private:
		float m_Time = 0.0f;
		float m_PrevTime = 0.0f;
		bool m_Looping = true;
		bool m_StopRequested = false;

		Count<ParticleSystem> m_ParticleSystem;
		Count<ParticleSystemTimelineInstance> m_Timeline;
		ParticleSystemState m_State = ParticleSystemState::None;

		friend class ParticleSystemEditorPanel;
	};
}