#include "Proofprch.h"
#include "BulletPhysics.h"
namespace Proof
{
	BulletPhysics::BulletPhysics() {
		m_CollisionConfiguration = new btDefaultCollisionConfiguration();
		m_Dispatcher = new btCollisionDispatcher(m_CollisionConfiguration);
		m_Broadphase = new btDbvtBroadphase();
		m_Solver = new btSequentialImpulseConstraintSolver();
		m_World = new btDiscreteDynamicsWorld(m_Dispatcher, m_Broadphase, m_Solver, m_CollisionConfiguration);
	}
	BulletPhysics::~BulletPhysics() {
		delete m_World;
		delete m_Solver;
		delete m_Broadphase;
		delete m_Dispatcher;
		delete m_CollisionConfiguration;
	}
	void BulletPhysics::Simulate(float delta) {
		m_World->stepSimulation(delta);
	}
}
