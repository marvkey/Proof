#include "Proofprch.h"
#include "PhysicsWorld.h"
#include "PhysicsEngine.h"
#include "Proof/Scene/World.h"
#include "Proof/Scene/Entity.h"
#include "PhysicsUtils.h"
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
		pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS;
		pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_CCD;

		return physx::PxFilterFlags();
	
	}
	PhysicsWorld::PhysicsWorld(World* world)
		:
		m_World(world)
	{
		ScopeTimer copeTimer("Initilized physics world ");
		physx::PxSceneDesc sceneDesc(PhysicsEngine::GetPhysics()->getTolerancesScale());
		

		sceneDesc.flags |= physx::PxSceneFlag::eENABLE_CCD | physx::PxSceneFlag::eENABLE_PCM;
		sceneDesc.flags |= physx::PxSceneFlag::eENABLE_ACTIVE_ACTORS;
		sceneDesc.flags |= physx::PxSceneFlag::eENABLE_ENHANCED_DETERMINISM;

		const PhysicsSettings& engineSettings = PhysicsEngine::GetSettings();
		sceneDesc.gravity = PhysXUtils::ToPhysXVector(engineSettings.Gravity);
		sceneDesc.broadPhaseType = PhysXUtils::ProofToPhysXBroadphaseType(engineSettings.BroadPhaseType);
		sceneDesc.frictionType = PhysXUtils::ProofToPhysXFrictionType(engineSettings.FrictionModel);

		sceneDesc.cpuDispatcher = PhysicsEngine::GetCpuDispatcher();
		sceneDesc.simulationEventCallback = &m_CollisionCallback;
		sceneDesc.filterShader = shaderControl;

		PF_CORE_ASSERT(sceneDesc.isValid());

		//sceneDesc.ad
		//if (m_Config.PvdClient)
		//{
		//	m_Transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
		//	PhysicsEngine::GetPVD()->connect(*m_Transport, physx::PxPvdInstrumentationFlag::eALL);
		//}
		m_Scene = PhysicsEngine::GetPhysics()->createScene(sceneDesc);
		physx::PxPvdSceneClient* pvdClient = m_Scene->getScenePvdClient();

		//m_Scene->()
		if (PhysicsEngine::GetPVD()->isConnected())
		{
			PF_ENGINE_INFO("Physics Engine Visual debugger connected");
		}
		else
		{
			PF_ENGINE_INFO("Physics Engine Visual debugger not connected");
		}
		if (pvdClient)
		{
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		}
		CreateRegions();

		StartWorld();
	}

	PhysicsWorld::~PhysicsWorld()
	{
		EndWorld();
	}

	void PhysicsWorld::OnFixedUpdate(float deltaTime)
	{
		PF_PROFILE_FUNC();
		if (m_World->IsPlaying())
		{
			for (auto& [Id, actor] : m_Actors)
			{
				// right now just updating transform
				actor->OnFixedUpdate(deltaTime);
			}
		}
		bool advance = Advance(deltaTime);

		if (advance)
		{
			uint32_t numberActors;
			physx::PxActor** activeActors = m_Scene->getActiveActors(numberActors);

			for (uint32_t i = 0; i < numberActors; i++)
			{
				Count<PhysicsActor> actor = (PhysicsActor*)activeActors[i]->userData;
				if (!actor->IsSleeping())
				{
					actor->SyncTransform();
				}
			}
		}
	}
	bool PhysicsWorld::HasActor(UUID id)
	{
		return m_Actors.contains(id);
	}
	Count<PhysicsActor> PhysicsWorld::NewActor(UUID id)
	{
		Entity ent = m_World->GetEntity(id);;
		if (!ent.HasComponent<RigidBodyComponent>())
			return nullptr;
		auto actor= Count<PhysicsActor>::Create(this,ent);
		m_Actors[id] = actor;
		return actor;
	}
	Count<PhysicsActor> PhysicsWorld::GetActor(UUID id)
	{
		PF_CORE_ASSERT(HasActor(id), " Does not contain actor");
		return m_Actors.at(id);
	}
	Count<PhysicsActor> PhysicsWorld::TryGetActor(UUID id)
	{
		if (HasActor(id))return m_Actors.at(id);
	}
	void PhysicsWorld::RemoveActor(UUID id)
	{
		PF_CORE_ASSERT(HasActor(id), " Does not contain actor");
		m_Actors.erase(id);
	}
	void PhysicsWorld::StartWorld()
	{
		PF_PROFILE_FUNC();
		
		m_World->ForEachEnitityWith<RigidBodyComponent>([&](Entity entity) {
			auto id =entity.GetUUID();
			m_Actors.insert({ id, Count<PhysicsActor>::Create(this, entity) });
		});
	}
	void PhysicsWorld::EndWorld()
	{
		// release all rigid bodies before we release teh scene
		m_Actors.clear();
		//if (m_Config.PvdClient)
		//{
		//	PhysicsEngine::GetPVD()->disconnect();
		//	m_Transport->disconnect();
		//	m_Transport->release();
		//}
		m_Scene->release();
		m_World = nullptr;

		if (m_RegionBounds)
			delete[] m_RegionBounds;
	}
	bool PhysicsWorld::Advance(float deltaTime)
	{
		m_Accumulator += deltaTime;
		if (m_Accumulator < m_SubStepSize)
			return false;
		m_Accumulator -= m_SubStepSize;
		PF_PROFILE_FUNC("PhysicsWorld::Advance");
		for (uint32_t i = 0; i < m_SubStepSize; i++)
		{
			m_Scene->simulate(m_SubStepSize);
			m_Scene->fetchResults(true);
		}
		return true;
	}
	void PhysicsWorld::CreateRegions()
	{
		const PhysicsSettings& settings = PhysicsEngine::GetSettings();

		if (settings.BroadPhaseType == BroadphaseType::AutomaticBoxPrune)
			return;
		if(m_RegionBounds)
			delete[] m_RegionBounds;

		m_RegionBounds = new physx::PxBounds3[settings.WorldBoundsSubdivisions * settings.WorldBoundsSubdivisions];
		physx::PxBounds3 globalBounds(PhysXUtils::ToPhysXVector(settings.WorldBoundsMin), PhysXUtils::ToPhysXVector(settings.WorldBoundsMax));
		uint32_t regionCount = physx::PxBroadPhaseExt::createRegionsFromWorldBounds(m_RegionBounds, globalBounds, settings.WorldBoundsSubdivisions);

		for (uint32_t i = 0; i < regionCount; i++)
		{
			physx::PxBroadPhaseRegion region;
			region.bounds = m_RegionBounds[i];
			m_Scene->addBroadPhaseRegion(region);
		}

	}
}
