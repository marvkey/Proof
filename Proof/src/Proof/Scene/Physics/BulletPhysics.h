#pragma once
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
namespace Proof
{
	class BulletPhysics {
	public:
		BulletPhysics();
		~BulletPhysics();
		void Simulate(float delta);
	private:
		btBroadphaseInterface* m_Broadphase;
		btCollisionConfiguration* m_CollisionConfiguration;
		btCollisionDispatcher* m_Dispatcher;
		btConstraintSolver* m_Solver;
		btDynamicsWorld* m_World;

		friend class PhysicsEngine;
	};
}