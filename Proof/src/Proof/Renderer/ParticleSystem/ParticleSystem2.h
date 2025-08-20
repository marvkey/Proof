#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Math/Math.h"
#include <glm/glm.hpp>
#include "ParticleBuffers.h"
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

		SBParticleInitalState ParticleInitialState;
		SBParticleEmitterSettings ParticleEmitterSettings;

		uint32_t GetParticleCount();
		void ResetMaxParticles(uint32_t size);

		SBParticleTrackableData GetTrackableData();
	private:
		uint32_t m_MaxParticles = 0;
		Count<class StorageBuffer> m_SBParticlesBuffer;
		Count<class StorageBuffer> m_SBParticleParticleInitalStateBuffer; // storage cause of aling
		Count<class StorageBuffer> m_SBParticleEmitterSettingsBuffer; // storage cause of align 
		Count<class StorageBuffer> m_SBTrackableData; // storage cause of align comptue shader will edit this
		friend class WorldRenderer;
	};

	class ParticleSystem2
	{
		std::vector<ParticleEmitter> m_Emmiters;
	};
}