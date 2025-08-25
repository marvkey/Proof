#include "Proofprch.h"

#include "ParticleSystem.h"
#include "Proof/Scene/Component.h"
#include "Proof/Scene/Entity.h"
#include "Proof/Scene/World.h"
#include "Proof/Math/Random.h"
#include "Proof/Renderer/ComputePass.h"
#include "Proof/Renderer/Renderer.h"
#include "Proof/Renderer/CommandBuffer.h"

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

        Renderer::BeginComputePass(Renderer::GetRendererCommandBuffer(), m_ParticleUpdateComputePass);

        world->ForEachEnitityWith<ParticleSystemComponent>([&](Entity entity)
        {
                auto instance = entity.GetComponent<ParticleSystemComponent>().ParticleSytemInstance;
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

    
	ParticleEmitter::ParticleEmitter(uint32_t maxParticles) :
        m_ParticleAccumulator(0)
	{
        ResetMaxParticles(maxParticles);
	}
    ParticleEmitter::ParticleEmitter(Count<ParticleEmitter> otherEmitter)
        :
        m_ParticleAccumulator(0)
    {
        ParticleInitialState = otherEmitter->ParticleInitialState;
        ParticleEmitterSettings = otherEmitter->ParticleEmitterSettings;
        Texture = otherEmitter->Texture;
        Bursts = otherEmitter->Bursts;

        ResetMaxParticles(otherEmitter->m_MaxParticles);
    }
    void ParticleEmitter::OnUpdate(float dt, const Transform& transform, Count<ComputePass> cmdPass)
    {
        uint32_t burstSpawn = 0;
#if 1
        m_PrevTime = m_Time;
        m_Time += dt;

        for (const ParticleBurst& b : Bursts) {
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
            ParticleInitialState.EmitterPosition = transform.Location;
            ParticleInitialState.EmitterPrevPosition = transform.Location;
        }

        m_CurrentPos = transform.Location;
        ParticleInitialState.EmitterPosition = transform.Location;

        m_SBParticleEmitterSettingsBuffer->SetData(Buffer(&ParticleEmitterSettings, sizeof(SBParticleEmitterSettings)));
        m_SBParticleParticleInitalState->SetData(Buffer(&ParticleInitialState, sizeof(SBParticleInitalState)));

        m_ParticleAccumulator +=float(ParticleEmitterSettings.Emission.ParticlesPerSecond) * dt;

        int toSpawn = static_cast<int>(floor(m_ParticleAccumulator));
        m_ParticleAccumulator -= toSpawn;
        if (burstSpawn > 0) {
            PF_ENGINE_INFO("Burst spawn this frame = {}", burstSpawn);
        }

        SBParticlePerDrawState drawState;
        drawState.SpawnNewParticles = toSpawn + burstSpawn; // 
        drawState.DeltaTime = FrameTime::GetWorldDeltaTime();

        m_SBPerDrawData->SetData(Buffer(&drawState, sizeof(SBParticlePerDrawState)));

        cmdPass->SetInput("s_Particles", m_SBParticlesBuffer);
        cmdPass->SetInput("ParticleInitialState", m_SBParticleParticleInitalState);
        cmdPass->SetInput("EmitterSettings",m_SBParticleEmitterSettingsBuffer);
        cmdPass->SetInput("TrackableData", m_SBTrackableData);
        cmdPass->SetInput("PerDrawData", m_SBPerDrawData);

        int workGroupSize = 512;
        int numGroups = (GetParticleCount() + workGroupSize - 1) / workGroupSize;
        cmdPass->Dispatch(numGroups, 1, 1);


        m_PrevPos = transform.Location;
        ParticleInitialState.EmitterPrevPosition = transform.Location;
    }

    uint32_t ParticleEmitter::GetParticleCount()
    {
        return m_MaxParticles;
    }

    void ParticleEmitter::ResetMaxParticles(uint32_t size)
    {
        m_MaxParticles = size;

        // inital setigns for trackable data
        SBParticleTrackableData trackableData;

        trackableData.TimeElapsed = 0.0f;
        trackableData.ActiveParticles = 0;
        trackableData.DeadParticles = 0;
        trackableData.MaxParticles = m_MaxParticles;

        m_Time = 0.0f;
        m_PrevTime = 0.0f;
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
        m_SBParticleEmitterSettingsBuffer = StorageBuffer::Create(Buffer(&ParticleEmitterSettings, sizeof(SBParticleEmitterSettings)));
        m_SBParticleParticleInitalState = StorageBuffer::Create(Buffer(&ParticleInitialState, sizeof(SBParticleInitalState)));
        m_SBTrackableData = StorageBuffer::Create(Buffer(&trackableData, sizeof(SBParticleTrackableData)));
        m_SBPerDrawData = StorageBuffer::Create(sizeof(SBParticlePerDrawState));
    }

    SBParticleTrackableData ParticleEmitter::GetTrackableData()
    {
        PF_PROFILE_FUNC();
        Buffer buffer = m_SBTrackableData->GetDataRaw();
        SBParticleTrackableData data = *buffer.As< SBParticleTrackableData>();

        buffer.Release();

        return data;
    }

    bool ParticleEmitter::ShouldRender()
    {
        return true;
    }
  

    ParticleSystemInstance::ParticleSystemInstance()
    {
    }

    ParticleSystemInstance::ParticleSystemInstance(Count< ParticleSystemInstance> instnace)
    {
        m_ParticleSystem = instnace->m_ParticleSystem;

		SyncWithParicleSystem();
    }

    void ParticleSystemInstance::SyncWithParicleSystem()
    {
        if (m_ParticleSystem == nullptr)
            return;

        m_Emmiters.clear();
        m_State = ParticleSystemState::None;

        for (int i = 0; i < m_ParticleSystem->GetEmitterCount(); i++)
        {
			m_Emmiters.push_back(Count<ParticleEmitter>::Create(m_ParticleSystem->GetEmitter(i)));
        }
    }

    void ParticleSystemInstance::OnUpdate(float dt, const Transform& transform, Count<ComputePass> cmdPass)
    {
        for(auto e : m_Emmiters)
			e->OnUpdate(dt, transform, cmdPass);
    }

    void ParticleSystemInstance::SetParticleSystem(Count<ParticleSystem> system)
    {
        if (system == nullptr)
        {
            m_ParticleSystem = nullptr;
            m_Emmiters.clear();
			m_State = ParticleSystemState::None;
        }

        m_ParticleSystem = system;

		SyncWithParicleSystem();
    }

}