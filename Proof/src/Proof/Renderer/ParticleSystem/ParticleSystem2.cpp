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
		m_ParticlePool.resize(maxParticles);
        ParticleBuffer = StorageBuffer::Create(Buffer(m_ParticlePool.data(), m_ParticlePool.size() * sizeof(Particle2)));

        ParticleFreeBufferIndecis = StorageBuffer::Create(Buffer(m_ParticlePool.size() * sizeof(uint32_t)));
        ParticleFreeBufferCount = StorageBuffer::Create(Buffer(sizeof(uint32_t)));

        UBEmitterSettings  settings;
        settings.minColor = minColor;
        settings.maxColor = maxColor;

        settings.minOffset = minPosition;
        settings.maxOffset = maxPosition;

        settings.minVelocity = minVelocity;

        settings.maxVelocity = maxVelocity;

        settings.minAccel = minAccel;
        settings.maxAccel = maxAccel;

        settings.minLife = minLife;
        settings.maxLife = maxLife;

        settings.modelMatrix = glm::mat4(1.0f);
        settings.rotationMatrix = glm::toMat4(glm::quat(1.0f, 0.0f, 0.0f, 0.0f));

        ParticleEmitterSettingsBuffer = UniformBuffer::Create(Buffer(&settings, sizeof(settings)));
	}
    void ParticleEmitter::OnUpdate(float dt)
    {
        timer += (1.0f /60.0f);
        spawnInterval = glm::max(spawnInterval, std::numeric_limits<float>::epsilon());

        // Consume time here
        uint32_t toSpawn = static_cast<uint32_t>(timer / spawnInterval);
        timer -= toSpawn * spawnInterval;
        timer = glm::max(timer, 0.0f);

        // Store how many to spawn this frame
        m_ParticlesToSpawn = toSpawn;
    }
    uint32_t ParticleEmitter::GetParticleToSpawn()
    {
        return m_ParticlesToSpawn;
    }
}