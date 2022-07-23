#include "Proofprch.h"
#include "ProofPhysxCollisionCallBack.h"
#include "../CollisionInfo.h"
#include "../Script.h"
#include "../Component.h"
#include "../Entity.h"
#include "../World.h"
namespace Proof
{
	void ProofPhysxCollisionCallback::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) {
		Entity triggerEntity{ (uint64_t)pairs->triggerShape->getName(),SceneCoreClasses::GetCurrentWorld() };
		Entity otherEntity{ (uint64_t)pairs->otherShape->getName(),SceneCoreClasses::GetCurrentWorld() };
		auto script1 = triggerEntity.GetComponent<NativeScriptComponent>();
		//auto script2 = otherEntity.GetComponent<NativeScriptComponent>();
		if (script1 == nullptr)return;

		ColliderTypes collider1;
		ColliderTypes collider2;
		switch (pairs->triggerShape->getGeometryType()) {
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

		switch (pairs->otherShape->getGeometryType()) {
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
		TriggerInfo info;
		info.otherEntity = otherEntity;
		info.OtherColliderType = collider2;
		if (pairs->status & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND) {
			if (script1 != nullptr) {
				if (script1->Instance != nullptr) {
					script1->Instance->OnTriggerEnter(collider1, info);
				}
			}
		}

		if (pairs->status & physx::PxPairFlag::eNOTIFY_TOUCH_LOST) {
			if (script1 != nullptr) {
				if (script1->Instance != nullptr) {
					script1->Instance->OnTriggerLeave(collider1, info);
				}
			}
		}

		if (pairs->status & physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS) {
			if (script1 != nullptr) {
				if (script1->Instance != nullptr) {
					script1->Instance->OnTriggerStay(collider2, info);
				}
			}
		}
	}
	void ProofPhysxCollisionCallback::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) {
		Entity entity1{ (uint64_t)pairHeader.actors[0]->getName(),SceneCoreClasses::GetCurrentWorld() };
		Entity entity2{ (uint64_t)pairHeader.actors[1]->getName(),SceneCoreClasses::GetCurrentWorld() };
		auto script1 = entity1.GetComponent<NativeScriptComponent>();
		auto script2 = entity1.GetComponent<NativeScriptComponent>();
		if (script1 == nullptr && script2 == nullptr)return;

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
			contactInfo.Impulse = Vector<float>{ contactPoint.impulse.x,contactPoint.impulse.y,contactPoint.impulse.z };
			contactInfo.Normal = Vector<float>{ contactPoint.normal.x,contactPoint.normal.y,contactPoint.normal.z };
			contactInfo.Location = Vector<float>{ contactPoint.position.x,contactPoint.position.y,contactPoint.position.z };

			collisonInfo1.ContactPoints.emplace_back(contactInfo);
			collisonInfo2.ContactPoints.emplace_back(contactInfo);
		}

		if (pairs->events & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND) {
			if (script1 != nullptr) {
				if (script1->Instance != nullptr) {
					script1->Instance->OnCollisionEnter(collider1, collisonInfo1);
				}
			}

			if (script2 != nullptr) {
				if (script2->Instance != nullptr) {
					script2->Instance->OnCollisionEnter(collider2, collisonInfo2);
				}
			}
			return;
		}

		if (pairs->events & physx::PxPairFlag::eNOTIFY_TOUCH_LOST) {
			if (script1 != nullptr) {
				if (script1->Instance != nullptr) {
					script1->Instance->OnCollisionLeave(collider1, collisonInfo1);
				}
			}

			if (script2 != nullptr) {
				if (script2->Instance != nullptr) {
					script2->Instance->OnCollisionLeave(collider2, collisonInfo2);
				}
			}
			return;
		}

		if (pairs->events & physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS) {
			if (script1 != nullptr) {
				if (script1->Instance != nullptr) {
					script1->Instance->OnCollisionStay(collider2, collisonInfo2);
				}
			}

			if (script2 != nullptr) {
				if (script2->Instance != nullptr) {
					script2->Instance->OnCollisionStay(collider2, collisonInfo2);
				}
			}
			return;
		}
	}
}
