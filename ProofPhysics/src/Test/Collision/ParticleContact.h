#pragma once
#include "../Particle.h"
namespace ProofPhysicsEngine {
	class ParticleContact {
	public:
		/**
		* Holds the particles that are involved in the contact. The
		* second of these can be NULL, for contacts with the scenery.
		*/
		Particle* Particles[2];

		/*
		*hodls resutution force at contact
		*/
		float Resistution = 0;
		
		/**
		*holds the direction of contact
		*/
		Proof::Vector<float> ContactNormal = {};

		/*
		* how deep the contact is
		*/
		float Penetration = 0.0f;
	protected:
		/*
		* resoves the collision
		*/
		void ResolveContact(float deltaTime) {
			ResolveImpulse(deltaTime);
			ResolvePenetration(deltaTime);
		}

		/**
		* Calculates the separating velocity at this contact between the 2 objecs
		*/
		float CalculateSeparatingVelocity() const {
			Proof::Vector<float>  relativeVelocity = Particles[0]->Velocity;
			if (Particles[1]) relativeVelocity -= Particles[1]->Velocity;
			return relativeVelocity.Dot(ContactNormal);
		}
		/**
		* impules calculation
		*/
		void ResolveImpulse(float deltaTime) {
			// Find the velocity in the direction of the contact.
			float separatingVelocity = CalculateSeparatingVelocity();
			// Check whether it needs to be resolved.
			if (separatingVelocity > 0)
			{
				// The contact is either separating or stationary - there’s
				// no impulse required.
				return;
			}
			// Calculate the new separating velocity.
			float newSepVelocity = -separatingVelocity * Resistution;
			float deltaVelocity = newSepVelocity - separatingVelocity;
			// We apply the change in velocity to each object in proportion to
			// its inverse mass (i.e., those with lower inverse mass [higher
			// actual mass] get less change in velocity).
			float totalInverseMass = Particles[0]->GetMass();
			if (Particles[1]) totalInverseMass += Particles[1]->GetMass();
			// If all particles have infinite mass, then impulses have no effect.
			if (totalInverseMass <= 0) return;
			// Calculate the impulse to apply.
			float impulse = deltaVelocity / totalInverseMass;
			// Find the amount of impulse per unit of inverse mass.
			Proof::Vector<float> impulsePerIMass = ContactNormal * impulse;

			// Apply impulses: they are applied in the direction of the contact,
			// and are proportional to the inverse mass.
			Particles[0]->Velocity = Particles[0]->Velocity +
				impulsePerIMass * Particles[0]->GetMass();
			if (Particles[1])
			{
				// Particle 1 goes in the opposite direction.
				Particles[1]->Velocity = Particles[1]->Velocity +
					impulsePerIMass * -Particles[1]->GetMass();
			}
		}

		void ResolvePenetration(float DeltaTime){
			// If we don’t have any penetration, skip this step.
			if (Penetration <= 0) return;
			// The movement of each object is based on its inverse mass, so
			// total that.
			float totalInverseMass = 1/Particles[0]->GetMass();
			if (Particles[1]) totalInverseMass += 1/Particles[1]->GetMass();
			// If all particles have infinite mass, then we do nothing.
			if (totalInverseMass <= 0) return;
			// Find the amount of penetration resolution per unit of inverse mass.
			Proof::Vector<float>  movePerIMass = ContactNormal *
				(-Penetration / totalInverseMass);
			// Apply the penetration resolution.
			Particles[0]->Position = Particles[0]->Position +
				movePerIMass * (1/Particles[0]->GetMass());
			if (Particles[1])
			{
				Particles[1]->Position = Particles[1]->Position +
					movePerIMass * (1/Particles[1]->GetMass());
			}
		}
	};
}