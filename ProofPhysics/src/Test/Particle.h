#pragma once
#include "../../../Proof/src/Proof/Resources/Math/Vector.h"

namespace ProofPhysicsEngine {
	class Particle {
	public:
		/*
		* @param deltaTime: interval of each iteration
		*/
		void Update(float deltaTime);
		/**
		* Holds the linear position of the particle in
		* world space.
		*/
		Proof::Vector<float> Position{ 0,0,0 };
		/**
		* Holds the linear velocity of the particle in
		* world space.
		*/
		Proof::Vector<float> Velocity{ 0,0,0 };

		/*
		* holds acceleration of the particle world spac
		*/
		Proof::Vector<float> Acceleration{0,0,0};
		/*
		*amount aof damping applied to linear motiion
		*
		*/
		float Damping;
		/*
		* sets the mass we are using setter 
		* because we do not want the mass to be equal or less than 0
		* @param mass: Mass variable to assing no less than 0
		*/
		void SetMass(float mass) {
			if (mass < 0)
				Mass = mass;
		}
		/*
		*returns the mass variable sores
		*/
		float GetMass() {
			return Mass;
		}

		void AddForce(const Proof::Vector<float> force) {
			ForceAccum += force;
		}
	private:
		/**
		* Holds the inverse of the mass of the particle. It
		* is more useful to hold the inverse mass because
		* * integration is simpler and because in real-time
		* simulation it is more useful to have objects with
		* infinite mass (immovable) than zero mass
		* (completely unstable in numerical simulation).
		* so basiccaly the mass is = 10
		* we are gonna store 1/10
		* we would convert for u 
		*/
		float Mass=1.0f;

		/*
		*Vector accumulator
		* a vector that combines all the forces acting on an object to get its resultant force
		*/
		Proof::Vector<float> ForceAccum = { 0,0,0 };
		/*
		*Sets the Vector Acumm to be 0 at teh end of each frame
		*/
		void ClearAccum() {
			ForceAccum = Proof::Vector<float>{ 0,0,0 };
		}
	};
}