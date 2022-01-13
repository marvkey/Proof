#include "Particle.h"

namespace ProofPhysicsEngine {
	void Particle::Update(float deltaTime){
		/*
		*using one of the 5 equations of mostion to updat the postion
		* we can use more complexed equations but it would slow down performnce
		*/
		/*
		*update postion
		*/
		Position += Velocity * deltaTime;
		// acceleration from the force
		// using inverse mass we can set objects with 0 mass therfore having infinite masses 
		Proof::Vector<float> resultingAcc = Acceleration;
		resultingAcc += ForceAccum* (1 / Mass);

		//updating linear velocity from acceleration
		Velocity += resultingAcc * deltaTime;
		
		// adding drag we are pasin it to the power of delta time 
		// so we are not adding drag every frame 
		Velocity *= pow(Damping, deltaTime);

		// reseting the force of the ating on the object
		ForceAccum = Proof::Vector<float>{ 0,0,0 };
	}
}