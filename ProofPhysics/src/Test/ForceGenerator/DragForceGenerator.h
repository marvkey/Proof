#pragma once
#include "ForceGenerator.h"
#include "../Particle.h"
namespace ProofPhysicsEngine {
	class DragForceGenerator : public ParticleForceGenerator {
		/** Holds the velocity drag coefficient. */
		float k1;
		/** Holds the velocity squared drag coefficient. */
		float k2;
	public:
	
		/** Applies the drag force to the given particle. */
		virtual void update(Particle* particle, float deltaTime) {
			Proof::Vector<float> force;
			
			// Calculate the total drag coefficient.
			float dragCoeff = force.GetLength();
			dragCoeff = k1 * dragCoeff + k2 * dragCoeff * dragCoeff;
			// Calculate the final force and apply it.
			force.Normalize();
			force *= -dragCoeff;
			particle->AddForce(force);
		}
	};
}
