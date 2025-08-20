#include "Proofprch.h"

#include "ParticleSystem2.h"
#include "Proof/Scene/World.h"
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

    ParticleWorld::ParticleWorld(Count<World> world)
      //  :m_World(world.Get())
    {
    }

    void ParticleWorld::OnUpdate(float update)
    {
    }

    Count<World> ParticleWorld::GetWorld()
    {
       // if (m_World.IsValid())
      //      return m_World.Lock();
        return nullptr;
    }

    
	ParticleEmitter::ParticleEmitter(uint32_t maxParticles)
	{
        ResetMaxParticles(maxParticles);
	}
    void ParticleEmitter::OnUpdate(float dt)
    {
        m_SBParticleEmitterSettingsBuffer->SetData(Buffer(&ParticleEmitterSettings, sizeof(SBParticleEmitterSettings)));
        m_SBParticleParticleInitalStateBuffer->SetData(Buffer(&ParticleInitialState, sizeof(SBParticleInitalState)));
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


        if (m_SBParticlesBuffer != nullptr) // already initailized 
        {
            // when resize basically reeintialing whoel thing

            std::vector<Particle2> pool; pool.resize(m_MaxParticles);
            m_SBParticlesBuffer->Resize(Buffer(pool.data(), m_MaxParticles * sizeof(Particle2)));
            m_SBTrackableData->SetData(Buffer(&trackableData, sizeof(SBParticleTrackableData)));
            return;
        }

        std::vector<Particle2> pool; pool.resize(m_MaxParticles);
        m_SBParticlesBuffer = StorageBuffer::Create(Buffer(pool.data(),m_MaxParticles * sizeof(Particle2)));
        m_SBParticleParticleInitalStateBuffer = StorageBuffer::Create(Buffer(&ParticleInitialState, sizeof(SBParticleInitalState)));
        m_SBParticleEmitterSettingsBuffer = StorageBuffer::Create(Buffer(&ParticleEmitterSettings, sizeof(SBParticleEmitterSettings)));
        m_SBTrackableData = StorageBuffer::Create(Buffer(&trackableData, sizeof(SBParticleTrackableData)));
    }

    SBParticleTrackableData ParticleEmitter::GetTrackableData()
    {
        PF_PROFILE_FUNC();
        Buffer buffer = m_SBTrackableData->GetDataRaw();
        SBParticleTrackableData data = *buffer.As< SBParticleTrackableData>();

        buffer.Release();

        return data;
    }
  
}