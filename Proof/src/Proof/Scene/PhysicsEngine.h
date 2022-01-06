#pragma once
#include "../ProofPhysics/src/PhysicsWorld.h"
#include "World.h"
namespace Proof {
	class PhysicsEngine {
	public:
		PhysicsEngine(World* world) :
			m_World(world) {

		}
		void Start();
		void Update(float delta);
		void End();
	private:
		ProofPhysicsEngine::PhysicsWorld m_PhysicsEngine;
		World* m_World = nullptr;
		friend class World;
	};
}