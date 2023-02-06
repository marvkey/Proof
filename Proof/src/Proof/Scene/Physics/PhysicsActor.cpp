#include "Proofprch.h"
#include "PhysicsActor.h"
#include "Proof/Scene/Component.h"
#include "PhysicsWorld.h"
#include "PhysicsEngine.h"
#include "Proof/Scene/Component.h"
namespace Proof {
	physx::PxMaterial* defauultMaterial;
	namespace Utils {
		static physx::PxMat44 glmMat4ToPhysxMat4(const glm::mat4& mat4) {
			physx::PxMat44 newMat;

			newMat[0][0] = mat4[0][0];
			newMat[0][1] = mat4[0][1];
			newMat[0][2] = mat4[0][2];
			newMat[0][3] = mat4[0][3];

			newMat[1][0] = mat4[1][0];
			newMat[1][1] = mat4[1][1];
			newMat[1][2] = mat4[1][2];
			newMat[1][3] = mat4[1][3];

			newMat[2][0] = mat4[2][0];
			newMat[2][1] = mat4[2][1];
			newMat[2][2] = mat4[2][2];
			newMat[2][3] = mat4[2][3];

			newMat[3][0] = mat4[3][0];
			newMat[3][1] = mat4[3][1];
			newMat[3][2] = mat4[3][2];
			newMat[3][3] = mat4[3][3];


			return newMat;
		}
	}
	PhysicsActor::PhysicsActor(PhysicsWorld* physicsWorld, Entity entity)
		:
		m_PhysicsWorld(physicsWorld),
		m_Entity(entity)
	{
		if (!m_Entity.HasComponent<RigidBodyComponent>())
			PF_CORE_ASSERT(false, "Needs rigid body to be a physics Actor");
		AddRigidBody();

		if (m_Entity.HasComponent<CubeColliderComponent>()) AddCubeCollider();
		if (m_Entity.HasComponent<SphereColliderComponent>())AddSphereCollider();

	}
	PhysicsActor::~PhysicsActor()
	{
		physx::PxRigidActor* rigidBody = (physx::PxRigidActor*)m_RuntimeBody;

		if (m_Entity.HasComponent<CubeColliderComponent>())
		{
			physx::PxShape* shape = (physx::PxShape*)m_Entity.GetComponent<CubeColliderComponent>()->m_RuntimeBody;
			rigidBody->detachShape(*shape);
			shape->release();
		}
		rigidBody->release();
	}
	void PhysicsActor::OnUpdate(float deltaTime)
	{

	}

	bool PhysicsActor::IsSleeping()
	{
		switch (m_RigidBodyType)
		{
			case Proof::RigidBodyType::Static:
				return true;
			case Proof::RigidBodyType::Dynamic:
				physx::PxRigidDynamic* rigidBody = (physx::PxRigidDynamic*)m_RuntimeBody;
				return rigidBody->isSleeping();
		}
		PF_CORE_ASSERT(false);
	}

	void PhysicsActor::AddRigidBody()
	{
		if (defauultMaterial == nullptr)
			defauultMaterial = PhysicsEngine::GetPhysics()->createMaterial(0.6f, 0.6f, 0.f);

		const auto& transformComponent = *m_Entity.GetComponent<TransformComponent>();
		const auto worldLocation = m_PhysicsWorld->GetWorld()->GetWorldLocation(m_Entity);
		const auto worldRotation = m_PhysicsWorld->GetWorld()->GetWorldRotation(m_Entity);

		auto& rigidBodyComponent = *m_Entity.GetComponent<RigidBodyComponent>();

		glm::quat myquaternion = glm::quat(glm::vec3(glm::radians(worldRotation.X), glm::radians(worldRotation.Y), 
			glm::radians( worldRotation.Z)));
		
		
		physx::PxTransform physxTransform{ PhysxUtils::VectorToPhysxVector(worldLocation),
				PhysxUtils::VectorToPhysxQuat(worldRotation)};
		physx::PxActor* rigidBodyBase = nullptr;
		if (rigidBodyComponent.m_RigidBodyType == RigidBodyType::Dynamic)
		{

			physx::PxRigidDynamic* body = PhysicsEngine::GetPhysics()->createRigidDynamic(physxTransform);
			body->setName(fmt::to_string(m_Entity.GetEntityID()).c_str()); // we can easily rigidBodyComponent After collsion
			body->setMass(rigidBodyComponent.Mass);
			body->setAngularDamping(rigidBodyComponent.AngularDrag);
			body->setLinearDamping(rigidBodyComponent.LinearDrag);
			body->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, Math::InverseBool(rigidBodyComponent.Gravity));
			body->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, rigidBodyComponent.Kinimatic);
			body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_X, rigidBodyComponent.FreezeLocation.X);
			body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Y, rigidBodyComponent.FreezeLocation.Y);
			body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, rigidBodyComponent.FreezeLocation.Z);

			body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, rigidBodyComponent.FreezeRotation.X);
			body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, rigidBodyComponent.FreezeRotation.Y);
			body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, rigidBodyComponent.FreezeRotation.Z);

			rigidBodyBase = body;
			m_RigidBodyType = RigidBodyType::Dynamic;
		}

		else
		{
			m_RigidBodyType = RigidBodyType::Static;
			rigidBodyComponent.m_RuntimeBody = PhysicsEngine::GetPhysics()->createRigidStatic(physxTransform);
			physx::PxRigidStatic* body = (physx::PxRigidStatic*)rigidBodyComponent.m_RuntimeBody;
			rigidBodyBase = body;
		}
		m_PhysicsWorld->GetPhysicsScene()->addActor(*rigidBodyBase);
		rigidBodyBase->userData = this;
		rigidBodyComponent.m_RuntimeBody = rigidBodyBase;
		m_RuntimeBody = rigidBodyBase;
	}
	void PhysicsActor::AddCubeCollider()
	{
		//the world positon for this will be held in the rigid body
		auto cubeCollider = m_Entity.GetComponent<CubeColliderComponent>();
		auto& rigidBodyComponent = *m_Entity.GetComponent<RigidBodyComponent>();
		physx::PxMaterial* colliderMaterial = cubeCollider->HasPhysicsMaterial() == false ? defauultMaterial : (physx::PxMaterial*)cubeCollider->GetPhysicsMaterial()->m_RuntimeBody;
		
		const Vector worldScalePositive = m_Entity.GetCurrentWorld()->GetWorldScale(m_Entity).GetPositive();
		const Vector colliderScalePositive = cubeCollider->OffsetScale.GetPositive();
		
		Vector size = worldScalePositive + colliderScalePositive;
		physx::PxShape* body = PhysicsEngine::GetPhysics()->createShape(physx::PxBoxGeometry(PhysxUtils::VectorToPhysxVector(size)), *colliderMaterial, true);
		body->setName(fmt::to_string(m_Entity.GetEntityID()).c_str()); // we can easily rigidBodyComponent After collsion

		auto localtransform = body->getLocalPose();
		localtransform.p += PhysxUtils::VectorToPhysxVector(cubeCollider->OffsetLocation);
		body->setLocalPose(localtransform);
		body->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, Math::InverseBool(cubeCollider->IsTrigger));
		body->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, cubeCollider->IsTrigger);

		cubeCollider->m_RuntimeBody = body;
		physx::PxRigidActor* rigidBody = (physx::PxRigidActor*)m_RuntimeBody;
		rigidBody->attachShape(*body);
	}

	void PhysicsActor::AddSphereCollider()
	{
		auto sphereCollider = m_Entity.GetComponent<SphereColliderComponent>();
		auto& rigidBodyComponent = *m_Entity.GetComponent<RigidBodyComponent>();
		float size = sphereCollider->Radius * m_Entity.GetCurrentWorld()->GetWorldScale(m_Entity).GetMaxAbsolute();

		physx::PxMaterial* colliderMaterial = sphereCollider->HasPhysicsMaterial() == false ? defauultMaterial : (physx::PxMaterial*)sphereCollider->GetPhysicsMaterial()->m_RuntimeBody;
		physx::PxShape* body = PhysicsEngine::GetPhysics()->createShape(physx::PxSphereGeometry(size), *colliderMaterial, true);

		body->setName(fmt::to_string(m_Entity.GetEntityID()).c_str()); // we can easily rigidBodyComponent After collsion
		auto localtransform = body->getLocalPose();
		localtransform.p += PhysxUtils::VectorToPhysxVector(sphereCollider->OffsetLocation);
		body->setLocalPose(localtransform);
		body->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, Math::InverseBool(sphereCollider->IsTrigger));
		body->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, sphereCollider->IsTrigger);

		sphereCollider->m_RuntimeBody = body;
		physx::PxRigidActor* rigidBody = (physx::PxRigidActor*)m_RuntimeBody;
		rigidBody->attachShape(*body);
	}


	void PhysicsActor::SyncTransform()
	{
		physx::PxRigidActor* rigidBody = (physx::PxRigidActor*)m_RuntimeBody;

		TransformComponent& transform = *m_Entity.GetComponent<TransformComponent>();
		auto actorPos = rigidBody->getGlobalPose();
		
		transform.Location = PhysxUtils::PhysxToVector(actorPos.p);
		transform.Rotation = PhysxUtils::PhysxQuatToVector(actorPos.q);
	}
}
