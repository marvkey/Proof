#pragma once
#include<vector>
#include "PhysicsObject.h"
namespace ProofPhysicsEngine {
	class PhysicsEngine{
	public:
		void AddObject(const PhysicsObject& object);
		void Simulate(float delta);
		void HandleCollisions();
	private:
		std::vector<PhysicsObject>m_Objects;
	};
}

