#include "Proofprch.h"
#include "PhysicsSimulationCallback.h"
#include "Proof/Scene/Script.h"
#include "Proof/Scene/Component.h"
#include "Proof/Scene/Entity.h"
#include "Proof/Scene/World.h" 

#include "PhysicsActor.h"
namespace Proof
{
	void PhysicsSimulationCallback::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
	{
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
			}
			if (pairs[actorIndex].status & physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
			{
				TriggersActors.at(triggerEntity.GetUUID()).erase(overlapTriggerEnttity.GetUUID());
				if (TriggersActors.at(triggerEntity.GetUUID()).size() == 0)
					TriggersActors.erase(triggerEntity.GetUUID());
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


		Entity entity0 = actor0->GetEntity();
		Entity entity1 = actor1->GetEntity();

		if (pairs->events & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			//actor0->OnCollisonEnter(actor1);
			//actor1->OnCollisonEnter(actor0);
		}
		if (pairs->events & physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			//actor0->OnCollisonLeave(actor1);
			//actor1->OnCollisonLeave(actor0);
		}
		//if sleeping this is no longer called
		if (pairs->events & physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
		{
			//actor0->OnCollisonStay(actor1);
			//actor1->OnCollisonStay(actor0);
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
