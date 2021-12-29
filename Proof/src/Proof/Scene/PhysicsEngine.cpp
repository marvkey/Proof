#include "Proofprch.h"
#include "PhysicsEngine.h"
namespace Proof {
	void PhysicsEngine::Update(float delta)
	{
		m_PhysicsEngine.Simulate(delta);
		m_PhysicsEngine.HandleCollisions();
	}
}