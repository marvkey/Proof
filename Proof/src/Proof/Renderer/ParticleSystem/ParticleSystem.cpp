 #include "Proofprch.h"

#include "ParticleSystem.h"
#include "Proof/Scene/Component.h"
#include "Proof/Scene/Entity.h"
#include "Proof/Scene/World.h"
#include "Proof/Math/Random.h"
#include "Proof/Renderer/ComputePass.h"
#include "Proof/Renderer/Renderer.h"


#include "Proof/Renderer/UniformBuffer.h"
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/glm.hpp>
#include "Proof/Math/MathResource.h"
#include <glm/gtx/quaternion.hpp>
#include "Proof/Math/Math.h"

namespace Proof
{
    struct alignas(16) UBEmitterSettings
    {
        glm::vec4 minColor;
        glm::vec4 maxColor;
        glm::vec3 minOffset;
        float padding1;
        glm::vec3 maxOffset;
        float padding2;
        glm::vec3 minVelocity;
        float padding3;
        glm::vec3 maxVelocity;
        float padding4;
        glm::vec3 minAccel;
        float padding5;
        glm::vec3 maxAccel;
        float padding6;
        glm::mat4 modelMatrix;
        glm::mat4 rotationMatrix;
        float minLife, maxLife;
    };
    ParticleWorld::ParticleWorld(class World* world)
        :m_World(world)
    {
        m_ParticleUpdateComputePass = ComputePass::Create("Partilce update compute", Renderer::GetShader("ParticleSystemUpdateCompute"));
    }

    void ParticleWorld::OnUpdate(float update)
    {
        Count<World> world = m_World.Lock();

        if (world == nullptr)
            return;
		PF_PROFILE_FUNC();

        Renderer::BeginComputePass(Renderer::GetRendererCommandBuffer(), m_ParticleUpdateComputePass);

        world->ForEachEnitityWith<ParticleEffectComponent>([&](Entity entity)
        {
                auto instance = entity.GetComponent<ParticleEffectComponent>().ParticleEffect;
                if (instance == nullptr)
                    return;
					instance->OnUpdate(update,Transform(world->GetWorldSpaceTransform(entity)), m_ParticleUpdateComputePass);
        });


        Renderer::EndComputePass(m_ParticleUpdateComputePass);

    }

    Count<World> ParticleWorld::GetWorld()
    {
        if (m_World.IsValid())
            return m_World.Lock();
        return nullptr;
    }

    ParticleEmitter::ParticleEmitter()
    {
    }

    ParticleEmitterInstance::ParticleEmitterInstance(Count<ParticleEmitter> otherEmitter)
        :
        m_ParticleAccumulator(0), m_Emitter(otherEmitter)
    {
        SyncWithParticleEmitter();
    }
    void ParticleEmitterInstance::OnUpdate(float dt, const Transform& transform, Count<ComputePass> cmdPass)
    {
        PF_PROFILE_FUNC();

        if (m_State != ParticleSystemState::Play)
            return;

        if (m_WaitUntilFinish)
        {
            const SBParticleTrackableData trackableData = GetTrackableData();

            if (trackableData.State == 2)
            {
                m_State = ParticleSystemState::End;
                m_WaitUntilFinish = false;
                return;
            }
        }

        uint32_t burstSpawn = 0;
#if 1
        m_PrevTime = m_Time;
        m_Time += dt;

        for (const ParticleBurst& b :  m_Emitter->Bursts) {
            // For each cycle, check if its burst time lies in (prevTime, time]
            for (int i = 0; i < b.Cycles; ++i) 
            {
                float t_i = b.StartTime + i * b.Interval;
                if (m_PrevTime <= t_i && t_i <= m_Time)
                {
                    if (b.Probability >= 1.0f || Random::Real<float>(0.0f,1.0f) < b.Probability) {
                        burstSpawn += (uint32_t)b.Count;
                    }
                }
            }
        }
#endif

        if (m_CurrentPos == glm::vec3(0.0f) && m_PrevPos == glm::vec3(0.0f))
        {
            m_CurrentPos = transform.Location;
            m_PrevPos = transform.Location;
            ParticleInstanceState.EmitterPosition = transform.Location;
            ParticleInstanceState.EmitterPrevPosition = transform.Location;
        }

        m_CurrentPos = transform.Location;
        ParticleInstanceState.EmitterPosition = transform.Location;

        m_SBParticleEmitterSettingsBuffer->SetData(Buffer(&m_Emitter->ParticleEmitterSettings, sizeof(SBParticleEmitterSettings)));
        m_SBParticleParticleInitialState->SetData(Buffer(&m_Emitter->ParticleInitialState, sizeof(SBParticleInitialSettings)));
        m_SBParticleParticleInstanceState->SetData(Buffer(&ParticleInstanceState, sizeof(SBParticleInstanceState)));

        m_ParticleAccumulator += float(m_Emitter->ParticleEmitterSettings.Emission.ParticlesPerSecond) * dt;

        int toSpawn = static_cast<int>(floor(m_ParticleAccumulator));
        m_ParticleAccumulator -= toSpawn;
        if (burstSpawn > 0) {
            PF_ENGINE_INFO("Burst spawn this frame = {}", burstSpawn);
        }

        SBParticlePerDrawState drawState;
        drawState.SpawnNewParticles = toSpawn + burstSpawn; // 
        drawState.DeltaTime = dt;

        m_SBPerDrawData->SetData(Buffer(&drawState, sizeof(SBParticlePerDrawState)));

        cmdPass->SetInput("s_Particles", m_SBParticlesBuffer);
        cmdPass->SetInput("ParticleInitialState", m_SBParticleParticleInitialState);
        cmdPass->SetInput("ParticleInstanceState", m_SBParticleParticleInstanceState);
        cmdPass->SetInput("EmitterSettings",m_SBParticleEmitterSettingsBuffer);
        cmdPass->SetInput("TrackableData", m_SBTrackableData);
        cmdPass->SetInput("PerDrawData", m_SBPerDrawData);

        int workGroupSize = 512;
        int numGroups = (m_Emitter->GetParticleCount() + workGroupSize - 1) / workGroupSize;
        cmdPass->Dispatch(numGroups, 1, 1);


        m_PrevPos = transform.Location;
        ParticleInstanceState.EmitterPrevPosition = transform.Location;
    }


    void ParticleEmitterInstance::Reset(uint32_t size)
    {
		PF_PROFILE_FUNC();

        bool sizeChanged = false;
        if (m_MaxParticles != size)
            sizeChanged = true;
        m_MaxParticles = size;

        // inital setigns for trackable data
    	SBParticleTrackableData trackableData{};

    	trackableData.TimeElapsed = 0.0f;
    	trackableData.ActiveParticles = 0;
    	trackableData.DeadParticles = 0;
    	trackableData.MaxParticles = m_MaxParticles;
    	trackableData.State = 1;

        m_Time = 0.0f;
        m_PrevTime = 0.0f;
        m_ParticleAccumulator = 0.0f;
    	m_State = ParticleSystemState::None;
        if (m_SBParticlesBuffer != nullptr) // already initailized 
        {
            // when resize basically reeintialing whoel thing

                std::vector<Particle> pool; pool.resize(m_MaxParticles);
                m_SBParticlesBuffer->Resize(Buffer(pool.data(), m_MaxParticles * sizeof(Particle)));
                m_SBTrackableData->SetData(Buffer(&trackableData, sizeof(SBParticleTrackableData)));
           
            return;
        }

        std::vector<Particle> pool; pool.resize(m_MaxParticles);
        m_SBParticlesBuffer = StorageBuffer::Create(Buffer(pool.data(),m_MaxParticles * sizeof(Particle)));
        m_SBParticleEmitterSettingsBuffer = StorageBuffer::Create(Buffer(&m_Emitter->ParticleEmitterSettings, sizeof(SBParticleEmitterSettings)));
        m_SBParticleParticleInitialState = StorageBuffer::Create(Buffer(&m_Emitter->ParticleInitialState, sizeof(SBParticleInitialSettings)));
        m_SBParticleParticleInstanceState = StorageBuffer::Create(Buffer(&ParticleInstanceState, sizeof(SBParticleInstanceState)));
        m_SBTrackableData = StorageBuffer::Create(Buffer(&trackableData, sizeof(SBParticleTrackableData)));
        m_SBPerDrawData = StorageBuffer::Create(sizeof(SBParticlePerDrawState));


    }

    

    SBParticleTrackableData ParticleEmitterInstance::GetTrackableData()
    {
        PF_PROFILE_FUNC();
        Buffer buffer = m_SBTrackableData->GetDataRaw();

		if (buffer.Data ==nullptr)
			return SBParticleTrackableData();


        SBParticleTrackableData data = *buffer.As< SBParticleTrackableData>();

        buffer.Release();

        return data;
    }

    void ParticleEmitterInstance::SyncWithParticleEmitter()
    {
        Reset(m_Emitter->GetParticleCount());
    }

    void ParticleEmitterInstance::Play()
    {
		bool restart = false;
        if (m_State == ParticleSystemState::End)
            restart = true;

        if (restart)
            Reset(m_MaxParticles);

        m_WaitUntilFinish = false;
        m_State = ParticleSystemState::Play;
    }

    void ParticleEmitterInstance::Pause()
    {
        if (m_State != ParticleSystemState::Play)
            return;

        m_State = ParticleSystemState::Pause;
    }

    void ParticleEmitterInstance::Stop(bool waitUntilFinish)
    {
        if (!waitUntilFinish)
        {
            m_WaitUntilFinish = false;
            m_State = ParticleSystemState::End;
            return;
        }

        ParticleInstanceState.bLooping = 0;
        m_WaitUntilFinish = true;
        m_State = ParticleSystemState::Play;
    }

    void ParticleEmitterInstance::Restart()
    {
		Reset();
		Play();
    }


    ParticleEffect::ParticleEffect(Count<ParticleEffect> instnace)
	{
		if (!instnace)
			return;

		m_ParticleSystem = instnace->m_ParticleSystem;
		m_Looping = instnace->m_Looping;
		UpdateOffscreen = instnace->UpdateOffscreen;
		SimulationSpeed = instnace->SimulationSpeed;

		SyncWithParticleSystem();
	}

	void ParticleEffect::SyncWithParticleSystem()
	{
		if (m_ParticleSystem == nullptr)
		{
			m_Timeline = nullptr;
			m_State = ParticleSystemState::None;
			return;
		}

		m_Timeline = Count<ParticleSystemTimelineInstance>::Create(m_ParticleSystem->GetTimeline());

		m_Time = 0.0f;
		m_PrevTime = 0.0f;
		m_StopRequested = false;
		m_State = ParticleSystemState::None;

		Reset();
	}

	void ParticleEffect::OnUpdate(float dt, const Transform& transform, Count<ComputePass> cmdPass)
	{
		PF_PROFILE_FUNC();

		if (!m_Timeline || m_State != ParticleSystemState::Play)
			return;

		dt *= SimulationSpeed;

		m_PrevTime = m_Time;
		m_Time += dt;

		bool hasStartedEmitter = false;
		bool allEmittersDone = true;

		for (auto& [id, track] : m_Timeline->m_Tracks)
		{
			if (!track.Emitter.IsValid())
				continue;

			for (auto& emitterClip : track.EmitterClips)
			{
				if (!emitterClip.Emitter)
					continue;

				const float startTime = emitterClip.Clip.StartTime;

				if (!emitterClip.Started)
				{
					if (m_StopRequested)
						continue;

					if (m_Time < startTime)
					{
						allEmittersDone = false;
						continue;
					}

					emitterClip.Started = true;
					emitterClip.Emitter->ParticleInstanceState.bLooping = m_Looping ? 1 : 0;
					emitterClip.Emitter->Play();
				}

				hasStartedEmitter = true;

				emitterClip.Emitter->OnUpdate(dt, transform, cmdPass);

				if (!m_Looping || m_StopRequested)
				{
					const SBParticleTrackableData trackableData = emitterClip.Emitter->GetTrackableData();

					if (trackableData.State != 2)
						allEmittersDone = false;
				}
			}
		}

		if (m_StopRequested && !hasStartedEmitter)
		{
			m_StopRequested = false;
			m_State = ParticleSystemState::End;
			return;
		}

		if ((!m_Looping || m_StopRequested) && hasStartedEmitter && allEmittersDone)
		{
			m_StopRequested = false;
			m_State = ParticleSystemState::End;
		}
	}

	void ParticleEffect::SetParticleSystem(Count<ParticleSystem> system)
	{
		if (system == nullptr)
		{
			Stop(false);

			m_ParticleSystem = nullptr;
			m_Timeline = nullptr;
			m_State = ParticleSystemState::None;
			return;
		}

		m_ParticleSystem = system;

		SyncWithParticleSystem();
	}

	void ParticleEffect::Play()
	{

		if (!m_Timeline)
			return;

		bool restart = false;
		if (m_State == ParticleSystemState::End)
			restart = true;

		//if (m_State == ParticleSystemState::Play && !restart)
		//	return;

		if (restart)
		{
			m_Time = 0.0f;
			m_PrevTime = 0.0f;

			for (auto& [trackID, track] : m_Timeline->m_Tracks)
			{
				for (auto& emitterClip : track.EmitterClips)
				{
					emitterClip.Started = false;

					if (!emitterClip.Emitter)
						continue;

					emitterClip.Emitter->ParticleInstanceState.bLooping = m_Looping ? 1 : 0;
					emitterClip.Emitter->Reset();
				}
			}
		}
		else
		{
			for (auto& [trackID, track] : m_Timeline->m_Tracks)
			{
				for (auto& emitterClip : track.EmitterClips)
				{
					if (!emitterClip.Emitter || !emitterClip.Started)
						continue;

					//emitterClip.Emitter->Play();
				}
			}
		}

		m_StopRequested = false;
		m_State = ParticleSystemState::Play;
	}

	void ParticleEffect::Pause()
	{
		if (!m_Timeline || m_State != ParticleSystemState::Play)
			return;

		m_State = ParticleSystemState::Pause;

		for (auto& [trackID, track] : m_Timeline->m_Tracks)
		{
			for (auto& emitterClip : track.EmitterClips)
			{
				if (!emitterClip.Emitter || !emitterClip.Started)
					continue;

				emitterClip.Emitter->Pause();
			}
		}
	}

	void ParticleEffect::Stop(bool waitUntilFinish)
	{
		if (!m_Timeline)
		{
			m_Time = 0.0f;
			m_PrevTime = 0.0f;
			m_StopRequested = false;
			m_State = ParticleSystemState::None;
			return;
		}

		if (!waitUntilFinish)
		{
			for (auto& [trackID, track] : m_Timeline->m_Tracks)
			{
				for (auto& emitterClip : track.EmitterClips)
				{
					emitterClip.Started = false;

					if (!emitterClip.Emitter)
						continue;

					emitterClip.Emitter->Stop(false);
				}
			}

			m_Time = 0.0f;
			m_PrevTime = 0.0f;
			m_StopRequested = false;
			m_State = ParticleSystemState::End;
			return;
		}

		m_StopRequested = true;
		m_State = ParticleSystemState::Play;

		for (auto& [trackID, track] : m_Timeline->m_Tracks)
		{
			for (auto& emitterClip : track.EmitterClips)
			{
				if (!emitterClip.Emitter || !emitterClip.Started)
					continue;

				emitterClip.Emitter->ParticleInstanceState.bLooping = 0;
				emitterClip.Emitter->Stop(true);
			}
		}
	}

	void ParticleEffect::Restart()
	{
	}

	void ParticleEffect::Reset()
	{
    	m_State = ParticleSystemState::None;
		m_Time =0.0f;
		m_PrevTime = 0.0f;
		m_StopRequested = false;
    	

    	for (auto& [trackID, track] : m_Timeline->m_Tracks)
    	{
    		for (auto& emitterClip : track.EmitterClips)
    		{
    			emitterClip.Started = false;
    			if (!emitterClip.Emitter)
    				continue;
    			emitterClip.Emitter->Reset(emitterClip.Emitter->m_MaxParticles);
    			emitterClip.Emitter->ParticleInstanceState.bLooping = m_Looping ? 1 : 0;

    		}
    	}
	}

	bool ParticleEffect::SetLooping(bool loop)
	{
		m_Looping = loop;

		if (!m_Timeline)
			return false;

		for (auto& [trackID, track] : m_Timeline->m_Tracks)
		{
			for (auto& emitterClip : track.EmitterClips)
			{
				if (!emitterClip.Emitter)
					continue;

				emitterClip.Emitter->ParticleInstanceState.bLooping = loop ? 1 : 0;
			}
		}

		return true;
	}
}
