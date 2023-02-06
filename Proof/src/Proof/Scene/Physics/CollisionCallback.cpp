#include "Proofprch.h"
#include "CollisonCallback.h"
#include "../CollisionInfo.h"
#include "../Script.h"
#include "../Component.h"
#include "../Entity.h"
#include "../World.h"
namespace Proof
{
	void CollisionCallback::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) {
		PF_INFO("Clliosoan trigger");
		return;
		//Entity triggerEntity{ (uint64_t)pairs->triggerShape->getName(),SceneCoreClasses::GetCurrentWorld() };
		//Entity otherEntity{ (uint64_t)pairs->otherShape->getName(),SceneCoreClasses::GetCurrentWorld() };
		//
		//ColliderTypes collider1;
		//ColliderTypes collider2;
		//switch (pairs->triggerShape->getGeometryType()) {
		//	case physx::PxGeometryType::eBOX:
		//		collider1 = ColliderTypes::CubeCollider;
		//		break;
		//	case physx::PxGeometryType::eCAPSULE:
		//		collider1 = ColliderTypes::CapsuleCollider;
		//		break;
		//	case physx::PxGeometryType::eSPHERE:
		//		collider1 = ColliderTypes::SphereCollider;
		//		break;
		//	default:
		//		break;
		//}
		//
		//switch (pairs->otherShape->getGeometryType()) {
		//	case physx::PxGeometryType::eBOX:
		//		collider2 = ColliderTypes::CubeCollider;
		//		break;
		//	case physx::PxGeometryType::eCAPSULE:
		//		collider2 = ColliderTypes::CapsuleCollider;
		//		break;
		//	case physx::PxGeometryType::eSPHERE:
		//		collider2 = ColliderTypes::SphereCollider;
		//		break;
		//	default:
		//		break;
		//}
		//
		//
		//TriggerInfo info;
		//info.otherEntity = otherEntity;
		//info.OtherColliderType = collider2;
		//if (pairs->status & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND) {
		//	if (script1 != nullptr) {
		//		if (script1->Instance != nullptr) {
		//			script1->Instance->OnTriggerEnter(collider1, info);
		//		}
		//	}
		//}
		//
		//if (pairs->status & physx::PxPairFlag::eNOTIFY_TOUCH_LOST) {
		//	if (script1 != nullptr) {
		//		if (script1->Instance != nullptr) {
		//			script1->Instance->OnTriggerLeave(collider1, info);
		//		}
		//	}
		//}
		//
		//if (pairs->status & physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS) {
		//	if (script1 != nullptr) {
		//		if (script1->Instance != nullptr) {
		//			script1->Instance->OnTriggerStay(collider2, info);
		//		}
		//	}
		//}
	}
	void CollisionCallback::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) {
		Entity entity1{ (uint64_t)pairHeader.actors[0]->getName(),SceneCoreClasses::GetCurrentWorld() };
		Entity entity2{ (uint64_t)pairHeader.actors[1]->getName(),SceneCoreClasses::GetCurrentWorld() };
		if (pairs->events & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			PF_INFO("{} begin touch {}", entity1.GetName(), entity2.GetName());
			return;
		}

		if (pairs->events & physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			PF_INFO("{} lost touch{}", entity1.GetName(), entity2.GetName());
			return;
		}

		if (pairs->events & physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
		{
			PF_INFO("{} constant touch {}", entity1.GetName(), entity2.GetName());
			return;
		}
		auto script1 = entity1.GetComponent<ScriptComponent>();
		auto script2 = entity1.GetComponent<ScriptComponent>();
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
			contactInfo.Impulse = Vector{ contactPoint.impulse.x,contactPoint.impulse.y,contactPoint.impulse.z };
			contactInfo.Normal = Vector{ contactPoint.normal.x,contactPoint.normal.y,contactPoint.normal.z };
			contactInfo.Location = Vector{ contactPoint.position.x,contactPoint.position.y,contactPoint.position.z };

			collisonInfo1.ContactPoints.emplace_back(contactInfo);
			collisonInfo2.ContactPoints.emplace_back(contactInfo);
		}
		delete points;
	}
}
