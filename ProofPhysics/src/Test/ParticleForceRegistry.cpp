#include "ParticleForceRegistry.h"
#include "Particle.h"
#include "ForceGenerator.h"

namespace ProofPhysicsEngine {
	void ParticleForceRegistry::UpdateForces(float deltaTime){
		for (auto i = m_Registry.begin(); i != m_Registry.end(); i++)
		{
			i->fg->updateForce(i->particle, deltaTime);
		}
	}
}