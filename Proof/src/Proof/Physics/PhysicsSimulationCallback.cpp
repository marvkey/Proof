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
			PhysicsActor* triggerActor = (PhysicsActor*)pairs[actorIndex].triggerActor->userData;
			PhysicsActor* overlappTrigger = (PhysicsActor*)pairs[actorIndex].otherActor->userData;
			if (pairs[actorIndex].status & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
			{
				//triggerActor->OnTriggerEnter(overlappTrigger);
				//overlappTrigger->OnOverlapTriggerEnter(triggerActor);
			}
			else if (pairs[actorIndex].status & physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
			{
				//triggerActor->OnTriggerLeave(overlappTrigger);
				//overlappTrigger->OnOverlapTriggerLeave(triggerActor);
			}
			else if (pairs[actorIndex].status & physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
			{
				//triggerActor->OnTriggerStay(overlappTrigger);
				//overlappTrigger->OnOverlapTriggerStay(triggerActor);
			}
		}
	}
	void PhysicsSimulationCallback::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs)
	{

		auto removedActorA = pairHeader.flags & physx::PxContactPairHeaderFlag::eREMOVED_ACTOR_0;
		auto removedActorB = pairHeader.flags & physx::PxContactPairHeaderFlag::eREMOVED_ACTOR_1;

		if (removedActorA || removedActorB)
			return;

		PhysicsActor* actor0 = (PhysicsActor*)pairHeader.actors[0]->userData;
		PhysicsActor* actor1 = (PhysicsActor*)pairHeader.actors[1]->userData;
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

		if (pairs->events & physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
		{
			PF_TRACE("PersistCollisionbetween {} and {}", entity0.GetName(), entity1.GetName());
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
