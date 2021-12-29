#pragma once
#include "../ProofPhysics/src/PhysicsEngine.h"

namespace Proof {
	class PhysicsEngine {
	public:
		virtual void Update(float delta);
	private:
		ProofPhysicsEngine::PhysicsEngine m_PhysicsEngine;
		friend class World;
	};
}