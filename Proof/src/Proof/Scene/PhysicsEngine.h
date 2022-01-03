#pragma once
#include "../ProofPhysics/src/PhysicsWorld.h"

namespace Proof {
	class PhysicsEngine {
	public:
		virtual void Update(float delta);
	private:
		ProofPhysicsEngine::PhysicsWorld m_PhysicsEngine;
		friend class World;
	};
}