#include "Proofprch.h"
#include "NVDIAPhysX.h"
#include "ProofPhysxCollisionCallBack.h"
#include "cooking/PxCooking.h"

namespace Proof
{

	NVDIAPhysXEngine::NVDIAPhysXEngine() {
		m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_DefaultAllocatorCallback,
			m_DefaultErrorCallback);
		if (!m_Foundation)
			PF_CORE_ASSERT(false,"PxCreateFoundation failed!");
		
		m_Pvd = physx::PxCreatePvd(*m_Foundation);
		physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
		m_Pvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);
		m_ToleranceScale.length = 100;        // typical length of an object one meter
		m_ToleranceScale.speed = 981;         // typical speed of an object, gravity*1s is a reasonable choice
		m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, m_ToleranceScale, true,m_Pvd);
		if (!m_Physics)
			PF_CORE_ASSERT(false,"PxCreateFoundation failed!");

		physx::PxSceneDesc sceneDesc(m_Physics->getTolerancesScale());
		sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
		m_Dispatcher = physx::PxDefaultCpuDispatcherCreate(2);
		sceneDesc.cpuDispatcher = m_Dispatcher;
		sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
		m_Scene = m_Physics->createScene(sceneDesc);
		
		physx::PxPvdSceneClient* pvdClient = m_Scene->getScenePvdClient();
		if (pvdClient) {
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		}
		
		m_Cooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_Foundation, physx::PxCookingParams(m_Physics->getTolerancesScale()));
		if (!m_Cooking)
			PF_CORE_ASSERT(false,"PxCreateCooking failed!");

		//m_CollisionCallback = new ProofPhysxCollisionCallback();
		//m_Scene->setSimulationEventCallback(m_CollisionCallback);

	}
	NVDIAPhysXEngine::~NVDIAPhysXEngine() {
		m_Pvd->disconnect();
		m_Cooking->release();
		m_Physics->release();
		m_Foundation->release();
		//m_Dispatcher->release();
		//m_Pvd->release();
		//m_Scene->release();
		//delete m_CollisionCallback;
	}
	void NVDIAPhysXEngine::AddRigidBody() {
	}
	void NVDIAPhysXEngine::Simulate(float delta) {
		m_Scene->simulate(delta);
		m_Scene->fetchResults(true);
	}
}
