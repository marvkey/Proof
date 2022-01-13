#pragma once
#include "../Particle.h"
namespace ProofPhysicsEngine {
	/*
	*used to add forces to particles
	*/
	
	class ParticleForceGenerator
	{
	public:
		
		virtual void updateForce(Particle* particle, float deltaTime){}
	};
}