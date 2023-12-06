#include "Proofprch.h"
#include "PhysicsWorld.h"
#include "PhysicsEngine.h"
#include "Proof/Scene/World.h"
#include "Proof/Scene/Entity.h"
#include "PhysicsActor.h"
#include "PhysicsController.h"
#include "PhysicsUtils.h"
#include "PhysicsLayerManager.h"
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
		/*
		physx::PxFilterData filterData;
		filterData.word0 = layerInfo.CollidesValue;
		filterData.word1 = layerInfo.CollidesWith;
		filterData.word2 = (uint32_t)collisionDetection;
		filterData.word3 = layerInfo.LayerID;
		*/
#if 1
		if (!PhysicsLayerManager::ShouldCollide(filterData0.word3, filterData1.word3))
			return physx::PxFilterFlag::eSUPPRESS;

		if (physx::PxFilterObjectIsTrigger(attributes0) || physx::PxFilterObjectIsTrigger(attributes1))
		{
			pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT;
			pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS;
			return physx::PxFilterFlag::eDEFAULT;
		}

		pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT;
		pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS;

		if (filterData0.word2 == (uint32_t)CollisionDetectionType::Discrete || filterData1.word2 == (uint32_t)CollisionDetectionType::Discrete)
		{
			pairFlags |= physx::PxPairFlag::eDETECT_DISCRETE_CONTACT;
		}

		if (filterData0.word2 == (uint32_t)CollisionDetectionType::Continuous || filterData1.word2 == (uint32_t)CollisionDetectionType::Continuous)
		{
			pairFlags |= physx::PxPairFlag::eDETECT_CCD_CONTACT;
			pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_CCD;
		}

		if ((filterData0.word0 & filterData1.word1) || (filterData1.word0 & filterData0.word1))
		{
			pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
			pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_LOST;
			return physx::PxFilterFlag::eDEFAULT;
		}

		return physx::PxFilterFlag::eSUPPRESS;
#else
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
#endif

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
		sceneDesc.bounceThresholdVelocity = engineSettings.BounceThresholdVelocity;

		PF_CORE_ASSERT(sceneDesc.isValid());

		m_PhysXScene = PhysicsEngine::GetPhysics()->createScene(sceneDesc);
		physx::PxPvdSceneClient* pvdClient = m_PhysXScene->getScenePvdClient();

		m_PhysXControllerManager = PxCreateControllerManager(*m_PhysXScene);
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
			physx::PxActor** activeActors = m_PhysXScene->getActiveActors(numberActors);

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

		// TODO disable in runtime
		m_PhysXScene->getSimulationStatistics(m_SimulationStats);

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
		Count<PhysicsActor> actor = Count<PhysicsActor>::Create(instance, entity);
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
		m_PhysXScene->release();
		m_PhysXScene = nullptr;
		m_World = nullptr;

		if (m_RegionBounds)
			pdelete[] m_RegionBounds;
	}
	bool PhysicsWorld::Advance(float deltaTime)
	{
		SubStepStrategy(deltaTime);

		for (uint32_t i = 0; i < m_NumSubSteps; i++)
		{
			m_PhysXScene->simulate(m_SubStepSize);
			m_PhysXScene->fetchResults(true);
		}

		if (m_NumSubSteps > 0)
		{
			for (auto& [triggerActorID, triggerActorData] : m_CollisionCallback.TriggersActors) 
			{
				for (auto& [otherTriggerID, triggerData] : triggerActorData)
				{
					if (triggerData.ReadyToCallPersist == false)
					{
						triggerData.ReadyToCallPersist = true;
						continue;
					}
					PF_INFO("trigger Actor {} on overlap by {}", triggerData.TriggerActor->GetEntity().GetName(), triggerData.OverlapTrigger->GetEntity().GetName());
				}
			}
		}
		return m_NumSubSteps != 0;

	}
	void PhysicsWorld::SubStepStrategy(float deltaTime)
	{
		if (m_Accumulator > m_SubStepSize)
			m_Accumulator = 0.0f;

		m_Accumulator += deltaTime;
		if (m_Accumulator < m_SubStepSize)
		{
			m_NumSubSteps = 0;
			return;
		}

		m_NumSubSteps = glm::min(static_cast<uint32_t>(m_Accumulator / m_SubStepSize), c_MaxSubSteps);
		m_Accumulator -= (float)m_NumSubSteps * m_SubStepSize;
	}
	void PhysicsWorld::CreateRegions()
	{
		const PhysicsSettings& settings = PhysicsEngine::GetSettings();

		if (settings.BroadPhaseType == BroadphaseType::AutomaticBoxPrune)
			return;
		if (m_RegionBounds)
			pdelete[] m_RegionBounds;

		m_RegionBounds = pnew physx::PxBounds3[settings.WorldBoundsSubdivisions * settings.WorldBoundsSubdivisions];
		physx::PxBounds3 globalBounds(PhysXUtils::ToPhysXVector(settings.WorldBoundsMin), PhysXUtils::ToPhysXVector(settings.WorldBoundsMax));
		uint32_t regionCount = physx::PxBroadPhaseExt::createRegionsFromWorldBounds(m_RegionBounds, globalBounds, settings.WorldBoundsSubdivisions);

		for (uint32_t i = 0; i < regionCount; i++)
		{
			physx::PxBroadPhaseRegion region;
			region.bounds = m_RegionBounds[i];
			m_PhysXScene->addBroadPhaseRegion(region);
		}

	}
}