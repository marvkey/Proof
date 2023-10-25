#include "Proofprch.h"
#include "CollisonCallback.h"
//#include "../CollisionInfo.h"
#include "Proof/Scene/Script.h"
#include "Proof/Scene/Component.h"
#include "Proof/Scene/Entity.h"
#include "Proof/Scene/World.h" 

#include "PhysicsActor.h"
namespace Proof
{
	void CollisionCallback::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) {
		for (uint32_t actorIndex = 0; actorIndex < count; actorIndex++)
		{
			PhysicsActor* triggerActor = (PhysicsActor*)pairs[actorIndex].triggerActor->userData;
			PhysicsActor* overlappTrigger = (PhysicsActor*)pairs[actorIndex].otherActor->userData;
			if (pairs[actorIndex].status & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
			{
				//triggerActor->OnTriggerEnter(overlappTrigger);
				//overlappTrigger->OnOverlapTriggerEnter(triggerActor);
			}
			if (pairs[actorIndex].status & physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
			{
				//triggerActor->OnTriggerLeave(overlappTrigger);
				//overlappTrigger->OnOverlapTriggerLeave(triggerActor);
			}

			if (pairs[actorIndex].status & physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
			{
				//triggerActor->OnTriggerStay(overlappTrigger);
				//overlappTrigger->OnOverlapTriggerStay(triggerActor);
			}
		}
	}
	void CollisionCallback::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) {
		PhysicsActor* actor0 = (PhysicsActor*)pairHeader.actors[0]->userData;
		PhysicsActor* actor1  =(PhysicsActor *) pairHeader.actors[1]->userData;
		
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
			//actor0->OnCollisonStay(actor1);
			//actor1->OnCollisonStay(actor0);
		}
		#if 0
		CollisionInfo collisonInfo1;
		CollisionInfo collisonInfo2;

		collisonInfo1.NumContacts = nbPairs;
		collisonInfo2.NumContacts = nbPairs;

		collisonInfo1.otherEntity = entity2;
		collisonInfo2.otherEntity = entity1;

		ColliderTypes collider1;
		ColliderTypes collider2;
		switch (pairs->shapes[0]->getGeometryType()) {
		case physx::PxGeometryType::eBOX:
			collider1 = ColliderTypes::CubeCollider;
			break;
		case physx::PxGeometryType::eCAPSULE:
			collider1 = ColliderTypes::CapsuleCollider;
			break;
		case physx::PxGeometryType::eSPHERE:
			collider1 = ColliderTypes::SphereCollider;
			break;
		default:
			break;
		}

		switch (pairs->shapes[1]->getGeometryType()) {
		case physx::PxGeometryType::eBOX:
			collider2 = ColliderTypes::CubeCollider;
			break;
		case physx::PxGeometryType::eCAPSULE:
			collider2 = ColliderTypes::CapsuleCollider;
			break;
		case physx::PxGeometryType::eSPHERE:
			collider2 = ColliderTypes::SphereCollider;
			break;
		default:
			break;
		}

		collisonInfo1.OtherColliderType = collider2;
		collisonInfo2.OtherColliderType = collider1;
		physx::PxContactPairPoint* points;
		pairs->extractContacts(points, nbPairs);
		for (int i = 0; i < nbPairs; i++) {
			ContactPointInfo contactInfo;
			physx::PxContactPairPoint& contactPoint = points[i];
			contactInfo.Distance = contactPoint.separation;
			contactInfo.Impulse = Vector{ contactPoint.impulse.x,contactPoint.impulse.y,contactPoint.impulse.z };
			contactInfo.Normal = Vector{ contactPoint.normal.x,contactPoint.normal.y,contactPoint.normal.z };
			contactInfo.Location = Vector{ contactPoint.position.x,contactPoint.position.y,contactPoint.position.z };

			collisonInfo1.ContactPoints.emplace_back(contactInfo);
			collisonInfo2.ContactPoints.emplace_back(contactInfo);
		}
		pdelete points;
		#endif
	}
}
