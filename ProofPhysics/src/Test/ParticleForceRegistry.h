#pragma once
#include<vector>
namespace ProofPhysicsEngine {
	/**
	* holds the forces that act on a particle
	*/
	class Particle;
	class ParticleForceGenerator;
	class ParticleForceRegistry {
	public:
		struct ParticleForceRegistration
		{
			Particle* particle = nullptr;
			ParticleForceGenerator* fg = nullptr;
		};
		void UpdateForces(float deltaTime);
	private:
		std::vector<ParticleForceRegistration> m_Registry;
	};
}