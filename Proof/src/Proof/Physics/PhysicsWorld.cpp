#include "Proofprch.h"
#include "PhysicsWorld.h"
#include "PhysicsEngine.h"
#include "Proof/Scene/World.h"
#include "Proof/Scene/Entity.h"
#include "PhysicsActor.h"
#include "PhysicsController.h"
#include "PhysicsUtils.h"
#include "PhysicsLayerManager.h"
#include "PhysicsShapes.h"

#include "Proof/Scripting/ScriptWorld.h"
#include "Proof/Scripting/ScriptEngine.h"
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

		static auto callTriggerMethod = [](const char* methodName, Entity mainEntity, Entity b)
		{
			if (!mainEntity.HasComponent<ScriptComponent>())
				return;

			const auto& sc = mainEntity.GetComponent<ScriptComponent>();

			Count<ScriptWorld> scriptWorld = mainEntity.GetCurrentWorld()->GetScriptWorld();
			if (!scriptWorld)
				return;

			if (!scriptWorld->IsEntityScriptInstantiated(mainEntity))
				return;

			for (const auto& scriptMetaData : sc.GetScriptMetadates())
			{
				if (ScriptEngine::IsModuleValid(scriptMetaData.ClassName))
				{
					ScriptEngine::CallMethod(scriptMetaData.Instance, methodName, b.GetUUID());
				}
			}
		};
		for (uint32_t i = 0; i < m_NumSubSteps; i++)
		{
			m_PhysXScene->simulate(m_SubStepSize);
			m_PhysXScene->fetchResults(true);
		}

		if (m_NumSubSteps > 0)
		{
			PF_PROFILE_FUNC("Physics Trigger check");
			//trigger objects still give persistent event if they are sleeping
			for (auto& [triggerActorID, triggerActorData] : m_CollisionCallback.TriggersActors) 
			{
				for (auto& [otherTriggerID, triggerData] : triggerActorData)
				{
					if (triggerData.ReadyToCallPersist == false)
					{
						triggerData.ReadyToCallPersist = true;
						continue;
					}

					Entity trigerEntity = m_World->TryGetEntityWithUUID(triggerActorID);
					Entity otherTriggerEntity = m_World->TryGetEntityWithUUID(otherTriggerID);

					callTriggerMethod("OnTriggerStayInternal", trigerEntity, otherTriggerEntity);
					callTriggerMethod("OnTriggerStayInternal", trigerEntity, otherTriggerEntity);
					//PF_INFO("trigger Actor {} on overlap by {}", triggerData.TriggerActor->GetEntity().GetName(), triggerData.OverlapTrigger->GetEntity().GetName());
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
	bool PhysicsWorld::Raycast(const glm::vec3& origin, const glm::vec3& direction, float maxDistance, RaycastHit* outHit)
	{
		PF_PROFILE_FUNC();

		physx::PxRaycastBuffer hitInfo;
		bool result = m_PhysXScene->raycast(PhysXUtils::ToPhysXVector(origin), PhysXUtils::ToPhysXVector(glm::normalize(direction)), maxDistance, hitInfo);

		if (result)
		{
			physx::PxRaycastHit& closestHit = hitInfo.block;

			Count<PhysicsActorBase> object = (PhysicsActorBase*)closestHit.actor->userData;
			outHit->HitEntity = object->GetEntity().GetUUID();
			outHit->Position = PhysXUtils::FromPhysXVector(closestHit.position);
			outHit->Normal = PhysXUtils::FromPhysXVector(closestHit.normal);
			outHit->Distance = closestHit.distance;
			outHit->HitCollider = (ColliderShape*)(closestHit.shape->userData);
			return result;
		}

		return result;
	}
	bool PhysicsWorld::SphereCast(const glm::vec3& origin, const glm::vec3& direction, float radius, float maxDistance, RaycastHit* outHit)
	{
		PF_PROFILE_FUNC();

		physx::PxSweepBuffer sweepBuffer;
		bool result = m_PhysXScene->sweep(physx::PxSphereGeometry(radius), physx::PxTransform(PhysXUtils::ToPhysXVector(origin)),
			PhysXUtils::ToPhysXVector(direction), maxDistance, sweepBuffer);

		if (result)
		{
			physx::PxSweepHit& closestHit = sweepBuffer.block;

			Count<PhysicsActorBase> object = (PhysicsActorBase*)closestHit.actor->userData;
			outHit->HitEntity = object->GetEntity().GetUUID();
			outHit->Position = PhysXUtils::FromPhysXVector(closestHit.position);
			outHit->Normal = PhysXUtils::FromPhysXVector(closestHit.normal);
			outHit->Distance = closestHit.distance;
			outHit->HitCollider = (ColliderShape*)(closestHit.shape->userData);
			return result;
		}

		return result;
	}
	bool PhysicsWorld::OverlapBox(const glm::vec3& origin, const glm::vec3& halfSize, std::array<OverlapHit, OVERLAP_MAX_COLLIDERS>& buffer, uint32_t& count)
	{
		return OverlapGeometry(origin, physx::PxBoxGeometry(halfSize.x, halfSize.y, halfSize.z), buffer, count);
	}

	void PhysicsWorld::AddRadialImpulse(const glm::vec3& origin, float radius, float strength, EFalloffMode falloff, bool velocityChange)
	{
		PF_PROFILE_FUNC();
		std::array<OverlapHit, OVERLAP_MAX_COLLIDERS> overlappedColliders;
		memset(overlappedColliders.data(), 0, OVERLAP_MAX_COLLIDERS * sizeof(OverlapHit));

		uint32_t count = 0;
		if (!OverlapSphere(origin, radius, overlappedColliders, count))
			return;

		for (uint32_t i = 0; i < count; i++)
		{
			auto actorBase = overlappedColliders[i].Actor;
			if (actorBase->GetType() == PhysicsControllerType::Actor)
			{
				auto actor = actorBase.As<PhysicsActor>();
				if (actor->IsDynamic() && !actor->IsKinematic())
					actor->AddRadialImpulse(origin, radius, strength, falloff, velocityChange);
			}
		}
	}

	bool PhysicsWorld::OverlapSphere(const glm::vec3& origin, float radius, std::array<OverlapHit, OVERLAP_MAX_COLLIDERS>& buffer, uint32_t& count)
	{
		return OverlapGeometry(origin, physx::PxSphereGeometry(radius), buffer, count);
	}

	bool PhysicsWorld::OverlapGeometry(const glm::vec3& origin, const physx::PxGeometry& geometry, std::array<OverlapHit, OVERLAP_MAX_COLLIDERS>& buffer, uint32_t& count)
	{
		PF_PROFILE_FUNC();

		physx::PxOverlapBuffer buf(m_OverlapBuffer.data(), OVERLAP_MAX_COLLIDERS);
		physx::PxTransform pose = PhysXUtils::ToPhysXTransform(glm::translate(glm::mat4(1.0f), origin));

		bool result = m_PhysXScene->overlap(geometry, pose, buf);
		if (result)
		{
			count = buf.nbTouches > OVERLAP_MAX_COLLIDERS ? OVERLAP_MAX_COLLIDERS : buf.nbTouches;

			for (uint32_t i = 0; i < count; i++)
			{
				buffer[i].Actor = (PhysicsActorBase*)m_OverlapBuffer[i].actor->userData;
				buffer[i].Shape = (ColliderShape*)m_OverlapBuffer[i].shape->userData;
			}
		}

		return result;
	}
}