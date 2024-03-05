#include "Proofprch.h"
#include "PhysicsSimulationCallback.h"
#include "Proof/Scene/Script.h"
#include "Proof/Scene/Component.h"
#include "Proof/Scene/Entity.h"
#include "Proof/Scene/World.h" 
#include "Proof/Scripting/ScriptWorld.h"
#include "Proof/Scripting/ScriptEngine.h"
#include "PhysicsActor.h"
namespace Proof
{
	void PhysicsSimulationCallback::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
	{
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
		for (uint32_t actorIndex = 0; actorIndex < count; actorIndex++)
		{
			if (pairs[actorIndex].flags & (physx::PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER | physx::PxTriggerPairFlag::eREMOVED_SHAPE_OTHER))
				continue;
			PhysicsActorBase* triggerActor = (PhysicsActorBase*)pairs[actorIndex].triggerActor->userData;
			PhysicsActorBase* overlappTrigger = (PhysicsActorBase*)pairs[actorIndex].otherActor->userData;

			Entity triggerEntity = triggerActor->GetEntity();
			Entity overlapTriggerEnttity = overlappTrigger->GetEntity();
			if (pairs[actorIndex].status & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
			{
				TriggersActors[triggerEntity.GetUUID()][overlapTriggerEnttity.GetUUID()] = { triggerActor,overlappTrigger,false };

				callTriggerMethod("OnTriggerEnterInternal", triggerEntity, overlapTriggerEnttity);
				callTriggerMethod("OnTriggerEnterInternal", overlapTriggerEnttity, triggerEntity);
			}
			if (pairs[actorIndex].status & physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
			{
				TriggersActors.at(triggerEntity.GetUUID()).erase(overlapTriggerEnttity.GetUUID());
				if (TriggersActors.at(triggerEntity.GetUUID()).size() == 0)
					TriggersActors.erase(triggerEntity.GetUUID());


				callTriggerMethod("OnTriggerLeaveInternal", triggerEntity, overlapTriggerEnttity);
				callTriggerMethod("OnTriggerLeaveInternal", overlapTriggerEnttity, triggerEntity);
			}
		}
	}
	void PhysicsSimulationCallback::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs)
	{

		auto removedActorA = pairHeader.flags & physx::PxContactPairHeaderFlag::eREMOVED_ACTOR_0;
		auto removedActorB = pairHeader.flags & physx::PxContactPairHeaderFlag::eREMOVED_ACTOR_1;

		if (removedActorA || removedActorB)
			return;

		PhysicsActorBase* actor0 = (PhysicsActorBase*)pairHeader.actors[0]->userData;
		PhysicsActorBase* actor1 = (PhysicsActorBase*)pairHeader.actors[1]->userData;
		if (!actor0 || !actor1)
			return;


		static auto callCollisionMethod = [](const char* methodName, Entity mainEntity, Entity b)
		{
			if (!mainEntity.HasComponent<ScriptComponent>())
				return;

			const auto& sc = mainEntity.GetComponent<ScriptComponent>();

			Count<ScriptWorld> scriptWorld = mainEntity.GetCurrentWorld()->GetScriptWorld();
			if (!scriptWorld)
				return;

			if (!scriptWorld->IsEntityScriptInstantiated(mainEntity))
				return;

			for (const ScriptComponentsClassesData& scriptMetaData : sc.GetScriptMetadates())
			{
				if (ScriptEngine::IsModuleValid(scriptMetaData.ClassName))
				{
					ScriptEngine::CallMethod(scriptMetaData.Instance, methodName, b.GetUUID().Get());
				}
			}
		};

		Entity entityA= actor0->GetEntity();
		Entity entityB = actor1->GetEntity();

		if (pairs->events & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			callCollisionMethod("OnCollisionEnterInternal", entityA, entityB);
			callCollisionMethod("OnCollisionEnterInternal", entityB, entityA);
		}
		if (pairs->events & physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			callCollisionMethod("OnCollisionLeaveInternal", entityA, entityB);
			callCollisionMethod("OnCollisionLeaveInternal", entityB, entityA);
		}
		//if sleeping this is no longer called
		if (pairs->events & physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
		{
			callCollisionMethod("OnCollisionStayInternal", entityA, entityB);
			callCollisionMethod("OnCollisionStayInternal", entityB, entityA);
		}

		//TODO Look at unity on collider hit https://docs.unity3d.com/ScriptReference/ControllerColliderHit.html
	}
	void PhysicsSimulationCallback::onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count)
	{
		PX_UNUSED(bodyBuffer);
		PX_UNUSED(poseBuffer);
		PX_UNUSED(count);
	}
}
