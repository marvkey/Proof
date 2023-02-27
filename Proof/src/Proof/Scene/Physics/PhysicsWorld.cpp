#include "Proofprch.h"
#include "PhysicsWorld.h"
#include "PhysicsEngine.h"
#include "Proof/Scene/Entity.h"
namespace Proof {
	physx::PxFilterFlags shaderControl(
		physx::PxFilterObjectAttributes attributes0,
		physx::PxFilterData filterData0,
		physx::PxFilterObjectAttributes attributes1,
		physx::PxFilterData filterData1,
		physx::PxPairFlags& pairFlags,
		const void* constantBlock,
		physx::PxU32 constantBlockSize)
	{
		PX_UNUSED(constantBlock);
		PX_UNUSED(constantBlockSize);
		// let triggers through
		if (physx::PxFilterObjectIsTrigger(attributes0) || physx::PxFilterObjectIsTrigger(attributes1))
		{
			pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT;
			return physx::PxFilterFlags();
		}

		pairFlags |= physx::PxPairFlag::eCONTACT_DEFAULT;
		pairFlags |= physx::PxPairFlag::eTRIGGER_DEFAULT;

		return physx::PxFilterFlags();
	}
	PhysicsWorld::PhysicsWorld(World* world, const PhysicsWorldConfig& sceneConfig)
		:
		m_World(world),
		m_Config(sceneConfig)
	{
		physx::PxSceneDesc sceneDesc(PhysicsEngine::GetPhysics()->getTolerancesScale());
		sceneDesc.gravity = physx::PxVec3(m_Config.Gravity.X, m_Config.Gravity.Y, m_Config.Gravity.Z);
		sceneDesc.cpuDispatcher = PhysicsEngine::GetCpuDispatcher();
		sceneDesc.simulationEventCallback = &m_CollisionCallback;
		sceneDesc.filterShader = shaderControl;
		sceneDesc.flags |= physx::PxSceneFlag::eENABLE_CCD | physx::PxSceneFlag::eENABLE_PCM;
		sceneDesc.flags |= physx::PxSceneFlag::eENABLE_ACTIVE_ACTORS;

		m_Scene = PhysicsEngine::GetPhysics()->createScene(sceneDesc);

		if (m_Config.PvdClient)
		{
				
			m_Transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
			PhysicsEngine::GetPVD()->connect(*m_Transport, physx::PxPvdInstrumentationFlag::eALL);
			physx::PxPvdSceneClient* pvdClient = m_Scene->getScenePvdClient();
			if (pvdClient)
			{
				pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
				pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
				pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
			}
		}
		StartWorld();
	}

	PhysicsWorld::~PhysicsWorld()
	{
		EndWorld();
	}

	void PhysicsWorld::OnUpdate(float deltaTime)
	{
		bool advance = Advance(deltaTime);

		if (advance)
		{
			uint32_t numberActors;
			physx::PxActor** activeActors = m_Scene->getActiveActors(numberActors);

			for (uint32_t i = 0; i < numberActors; i++)
			{
				PhysicsActor* actor = (PhysicsActor*)activeActors[i]->userData;
				if (!actor->IsSleeping())
				{
					actor->SyncTransform();
				}
			}
		}
	}
	void PhysicsWorld::StartWorld()
	{
		PF_PROFILE_FUNC();
		
		m_World->ForEachEnitityWith<RigidBodyComponent>([&](Entity entity) {
			auto id =entity.GetEntityID();
			m_PhysicsActor.insert({ id, Count<PhysicsActor>::Create(this, entity) });
		});
	}
	void PhysicsWorld::EndWorld()
	{
		// release all rigid bodies before we release teh scene
		m_PhysicsActor.clear();
		if (m_Config.PvdClient)
		{
			m_Transport->disconnect();
			m_Transport->release();
			//PhysicsEngine::GetPVD()->disconnect();
		}
		m_Scene->release();
	}
	bool PhysicsWorld::Advance(float deltaTime)
	{
		m_Accumulator += deltaTime;
		if (m_Accumulator < m_SubStepSize)
			return false;
		m_Accumulator -= m_SubStepSize;

		m_Scene->simulate(m_SubStepSize);
		m_Scene->fetchResults(true);
		return true;
	}
}
