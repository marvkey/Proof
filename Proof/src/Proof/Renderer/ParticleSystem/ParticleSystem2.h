#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Math/Math.h"
#include <glm/glm.hpp>

//https://evanvoodoo.github.io/2025-01-24-gpu-particles/

namespace Proof
{
	struct alignas(16) Particle2
	{
		glm::vec3 Position;
		float Rotation;

		glm::vec3 Velocity;
		float Life = 1.0f;

		glm::vec4 Color = glm::vec4{ 1 };

		glm::vec3 Size3D{ 1 };
		uint32_t SystemID;
	};

	class ParticleWorld : public RefCounted
	{
	public:
		ParticleWorld(Count<class World> world);
		void OnUpdate(float update);
		Count<class World> GetWorld();

	private:

		//WeakCount<class World> m_World;
	};
	
	class ParticleEmitter : public RefCounted
	{
	public:
		ParticleEmitter(uint32_t maxParticles = 100);
		void OnUpdate(float dt);

		glm::vec4 minColor = glm::vec4(0, 0, 0, 1), maxColor = glm::vec4(1);
		glm::vec3 minPosition = glm::vec3(0), maxPosition = glm::vec3(20);
		glm::vec3 minVelocity = glm::vec3(-1), maxVelocity = glm::vec3(1);
		glm::vec3 minAccel = glm::vec3(0), maxAccel = glm::vec3(0, 0, -0.5f);
		float minLife = 1, maxLife = 10;
		float spawnInterval = 0.1f, timer = 0;
		uint32_t maxParticles = 100;
		uint32_t drawnParticleCount = 0;
		Count<class StorageBuffer> ParticleBuffer;
		Count<class StorageBuffer> ParticleFreeBufferIndecis;
		Count<class StorageBuffer> ParticleFreeBufferCount;
		Count<class UniformBuffer> ParticleEmitterSettingsBuffer;

		uint32_t GetParticleToSpawn();
	private:
		std::vector<Particle2> m_ParticlePool;
		uint32_t m_ParticlesToSpawn = 0;
		friend class WorldRenderer;
	};

	class ParticleSystem2
	{
		std::vector<ParticleEmitter> m_Emmiters;
	};
}