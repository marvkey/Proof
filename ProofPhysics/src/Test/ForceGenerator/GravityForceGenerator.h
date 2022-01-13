#pragma once
#include "ForceGenerator.h"
#include "../Particle.h"
namespace ProofPhysicsEngine {
	class GravituForceGenerator : public ParticleForceGenerator {
		Proof::Vector<float> m_Gravity = { 0,0,0 };
	public:
		/**
		* updates all the particles that react with gravtiational force
		*/
		virtual void UpdateForce(Particle* particle, float deltaTime) {
			// Need to do a check to see if the particle has INFINTIE MASS
			particle->AddForce(m_Gravity * particle->GetMass());
		};
	};
}