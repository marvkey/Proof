#include "Proofprch.h"
#include "PhysxShapesInternal.h"
#include "Proof/Physics/PhysicsActor.h"
#include "Proof/Scene/Entity.h"
#include "Proof/Scene/Component.h"
#include "Proof/Physics/PhysicsEngine.h"
#include "Proof/Physics/PhysicsMaterial.h"
namespace Proof
{

	void PhysxShapesInternal::UpdateOrAddCubeCollider(PhysicsActor& actor)
	{
		Entity actorEntity = actor.m_Entity;
		auto& cubeCollider = actorEntity.GetComponent<BoxColliderComponent>();
		auto& rigidBodyComponent = actorEntity.GetComponent<RigidBodyComponent>();
		physx::PxMaterial* colliderMaterial = (physx::PxMaterial*) (cubeCollider.HasPhysicsMaterial() == false ?
			AssetManager::GetDefaultAsset(DefaultRuntimeAssets::PhysicsMaterial).As<PhysicsMaterial>()->GetRuntimeBody() : (physx::PxMaterial*)cubeCollider.GetPhysicsMaterial()->GetRuntimeBody());

		const glm::vec3 worldScalePositive = glm::abs(actorEntity.GetCurrentWorld()->GetWorldSpaceScale(actorEntity));
		const glm::vec3 colliderScalePositive = glm::abs(cubeCollider.Size);
		glm::vec3 size = worldScalePositive * colliderScalePositive;

		physx::PxShape* body;

		if (actor.m_CubeColliderBody == nullptr)
		{
			body = PhysicsEngine::GetPhysics()->createShape(physx::PxBoxGeometry(PhysxUtils::GlmVectorToPhysxVector(size)), *colliderMaterial, true);
			PF_CORE_ASSERT(body, "Body is not created");

			physx::PxRigidActor* rigidBody = (physx::PxRigidActor*)actor.m_RuntimeBody;
			rigidBody->attachShape(*body);
			actor.m_CubeColliderBody = body;

			auto localtransform = body->getLocalPose();
			localtransform.p += PhysxUtils::GlmVectorToPhysxVector(cubeCollider.Center);
			body->setLocalPose(localtransform);
		}
		else
		{
			body = (physx::PxShape*)actor.m_CubeColliderBody;
			body->setGeometry(physx::PxBoxGeometry(PhysxUtils::GlmVectorToPhysxVector(size)));
		}
	

		body->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, Math::InverseBool(cubeCollider.IsTrigger));
		body->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, cubeCollider.IsTrigger);
	
	}
	void PhysxShapesInternal::UpdateOrAddSphereCollider(PhysicsActor& actor)
	{
		Entity actorEntity = actor.m_Entity;

		SphereColliderComponent& sphereCollider = actorEntity.GetComponent<SphereColliderComponent>();

		const glm::vec3 worldScalePositive = glm::abs(actorEntity.GetCurrentWorld()->GetWorldSpaceScale(actorEntity));
		float radius = glm::abs(sphereCollider.Radius) * glm::max(glm::max(worldScalePositive.x, worldScalePositive.y), worldScalePositive.z);

		physx::PxMaterial* colliderMaterial = (physx::PxMaterial*)(sphereCollider.HasPhysicsMaterial() == false ?
			AssetManager::GetDefaultAsset(DefaultRuntimeAssets::PhysicsMaterial).As<PhysicsMaterial>()->GetRuntimeBody() : (physx::PxMaterial*)sphereCollider.GetPhysicsMaterial()->GetRuntimeBody());
		physx::PxShape* body;

		if (actor.m_SphereColliderBody == nullptr)
		{
			body = PhysicsEngine::GetPhysics()->createShape(physx::PxSphereGeometry(radius), *colliderMaterial, true);
			PF_CORE_ASSERT(body, "Body is not created");

			actor.m_SphereColliderBody = body;
			physx::PxRigidActor* rigidBody = (physx::PxRigidActor*)actor.m_RuntimeBody;
			rigidBody->attachShape(*body);

			physx::PxTransform localtransform = body->getLocalPose();
			localtransform.p += PhysxUtils::GlmVectorToPhysxVector(sphereCollider.Center);
			body->setLocalPose(localtransform);
		}
		else
		{
			body = (physx::PxShape*)actor.m_SphereColliderBody;
			body->setGeometry(physx::PxSphereGeometry(radius));
		}

		

		body->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, Math::InverseBool(sphereCollider.IsTrigger));
		body->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, sphereCollider.IsTrigger);
	}
	void PhysxShapesInternal::UpdateOrAddCapsuleCollider(PhysicsActor& actor)
	{
		Entity actorEntity = actor.m_Entity;

		CapsuleColliderComponent& capsuleCollider = actorEntity.GetComponent<CapsuleColliderComponent>();
		const glm::vec3 worldScalePositive = glm::abs(actorEntity.GetCurrentWorld()->GetWorldSpaceScale(actorEntity));

		physx::PxMaterial* colliderMaterial = (physx::PxMaterial*) (capsuleCollider.HasPhysicsMaterial() == false ? 
			AssetManager::GetDefaultAsset(DefaultRuntimeAssets::PhysicsMaterial).As<PhysicsMaterial>()->GetRuntimeBody() : capsuleCollider.GetPhysicsMaterial()->GetRuntimeBody());
		float radius = capsuleCollider.Radius * glm::max(glm::max(worldScalePositive.x, worldScalePositive.y), worldScalePositive.z);
		float height = glm::abs(capsuleCollider.Height);
		glm::vec3 capsuleRotation = { 0,0,0 };// originial local pos is {0,0,0}
		switch (capsuleCollider.Direction)
		{
			case CapsuleDirection::X:
				{
					height *= worldScalePositive.x;
					capsuleRotation.x += 0;// default is facing X direction
				}
				break;
			case CapsuleDirection::Y:
				{
					height *= worldScalePositive.y;
					capsuleRotation.y += 90; // have to swap ssicne starting is facing X direction
					break;
				}
			case CapsuleDirection::Z:
				{
					height *= worldScalePositive.z;
					capsuleRotation.z += 90; // have to swap ssicne starting is facing X direction
					break;
				}
		}
		physx::PxShape* body;

		if (actor.m_CapsuleColliderBody == nullptr)
		{
			body = PhysicsEngine::GetPhysics()->createShape(physx::PxCapsuleGeometry(radius, height), *colliderMaterial, true);
			PF_CORE_ASSERT(body, "Body is not created");

			actor.m_CapsuleColliderBody = body;
			physx::PxRigidActor* rigidBody = (physx::PxRigidActor*)actor.m_RuntimeBody;
			rigidBody->attachShape(*body);

			physx::PxTransform localtransform = body->getLocalPose();
			localtransform.p += PhysxUtils::GlmVectorToPhysxVector(capsuleCollider.Center);
			localtransform.q += PhysxUtils::QuatTophysxQuat(glm::quat(glm::radians(capsuleRotation)));
			body->setLocalPose(localtransform);

		}
		else
		{
			body = (physx::PxShape*)actor.m_CapsuleColliderBody;
			body->setGeometry(physx::PxCapsuleGeometry(radius, height));
		}

		
		body->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, Math::InverseBool(capsuleCollider.IsTrigger));
		body->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, capsuleCollider.IsTrigger);

		
	}
}

