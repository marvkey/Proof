#include "Proofprch.h"
#include "PhysicsWorld.h"
#include "PhysicsEngine.h"
#include "Proof/Scene/World.h"
#include "Proof/Scene/Entity.h"
#include "PhysicsActor.h"
#include "PhysicsController.h"
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
	PhysicsWorld::PhysicsWorld(Count<class World> world)
		:
		m_World(world)
	{
		ScopeTimer copeTimer("Initialized physics world ");
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

		m_Scene = PhysicsEngine::GetPhysics()->createScene(sceneDesc);
		physx::PxPvdSceneClient* pvdClient = m_Scene->getScenePvdClient();

		m_PhysXControllerManager = PxCreateControllerManager(*m_Scene);
		CreateRegions();
	}

	PhysicsWorld::~PhysicsWorld()
	{
	}

	void PhysicsWorld::Simulate(float deltaTime)
	{
		PF_PROFILE_FUNC();
		if (m_World->IsPlaying())
		{
			// not actually on update
			for (auto& [Id, actor] : m_Actors)
				actor->OnFixedUpdate(m_SubStepSize);
		}

		for (auto& [entityID, controller] : m_Controllers)
			controller->OnUpdate(deltaTime);

		bool advance = Advance(deltaTime);

		if (advance)
		{
			uint32_t numberActors = 0;
			physx::PxActor** activeActors = m_Scene->getActiveActors(numberActors);

			for (uint32_t i = 0; i < numberActors; i++)
			{
				// NOTE: controller is in physics scene is this right 
				Count<PhysicsActorBase> actor = (PhysicsActorBase*)activeActors[i]->userData;
				if (actor && !actor->IsSleeping())
				{
					actor->SyncTransform();
				}
			}


		}
	}
	bool PhysicsWorld::HasActor(Entity entity)
	{
		return m_Actors.contains(entity.GetUUID());
	}
	Count<PhysicsActor> PhysicsWorld::CreateActor(Entity entity)
	{
		PF_PROFILE_FUNC();
		auto existingActor = GetActor(entity);
		if (existingActor)
			return existingActor;

		if (!entity.HasComponent<RigidBodyComponent>())
		{
			PF_ENGINE_ERROR("Cannot add Physics Actor without RigidBody");
			return nullptr;
		}
		Count<PhysicsWorld> instance = this;
		Count<PhysicsActor> actor = Count<PhysicsActor>::Create(instance,entity);
		m_Actors[entity.GetUUID()] = actor;
		return actor;
	}
	Count<PhysicsActor> PhysicsWorld::GetActor(Entity entity)
	{
		UUID entityID = entity.GetUUID();
		if (const auto iter = m_Actors.find(entityID); iter != m_Actors.end())
			return iter->second;

		return nullptr;
	}
	
	void PhysicsWorld::RemoveActor(Entity entity)
	{
		PF_CORE_ASSERT(HasActor(entity), " Does not contain actor");
		m_Actors.erase(entity.GetUUID());
	}
	bool PhysicsWorld::HasController(Entity entity)
	{
		return m_Controllers.contains(entity.GetUUID());
	}
	Count<class PhysicsController> PhysicsWorld::CreateController(Entity entity)
	{
		PF_PROFILE_FUNC();
		auto existingController = GetController(entity);
		if (existingController)
			return existingController;

		if (!entity.HasComponent<CharacterControllerComponent>())
		{
			PF_ENGINE_ERROR("Cannot add Physics Actor without CharacterController");
			return nullptr;
		}
		Count<PhysicsWorld> instance = this;
		Count<PhysicsController> controller = Count<PhysicsController>::Create(instance, entity);
		m_Controllers[entity.GetUUID()] = controller;
		return controller;
	}
	Count<class PhysicsController> PhysicsWorld::GetController(Entity entity)
	{
		UUID entityID = entity.GetUUID();
		if (const auto iter = m_Controllers.find(entityID); iter != m_Controllers.end())
			return iter->second;

		return nullptr;
	}
	void PhysicsWorld::RemoveController(Entity entity)
	{
		PF_CORE_ASSERT(HasController(entity), " Does not contain actor");
		m_Controllers.erase(entity.GetUUID());
	}
	void PhysicsWorld::StartWorld()
	{
		PF_PROFILE_FUNC();

		m_World->ForEachEnitityWith<RigidBodyComponent>([&](Entity entity) 
		{
			CreateActor(entity);
		});

		m_World->ForEachEnitityWith<CharacterControllerComponent>([&](Entity entity)
		{
			CreateController(entity);
		});

	}
	void PhysicsWorld::EndWorld()
	{
		// release all rigid bodies before we release teh scene
		m_Actors.clear();
		m_Controllers.clear();
		m_PhysXControllerManager->release();
		m_Scene->release();
		m_Scene = nullptr;
		m_World = nullptr;

		if (m_RegionBounds)
			pdelete[] m_RegionBounds;
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
			pdelete[] m_RegionBounds;

		m_RegionBounds = pnew physx::PxBounds3[settings.WorldBoundsSubdivisions * settings.WorldBoundsSubdivisions];
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
