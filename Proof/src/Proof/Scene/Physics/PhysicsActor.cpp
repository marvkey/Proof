#include "Proofprch.h"
#include "PhysicsActor.h"
#include "Proof/Scene/Component.h"
#include "PhysicsWorld.h"
#include "PhysicsEngine.h"
#include "Proof/Scene/Component.h"
#include "Proof/Scene/Material.h"

#include "Proof/Scripting/ScriptEngine.h"
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

		static physx::PxForceMode::Enum ToPhysxForce(ForceMode mode) {
			switch (mode)
			{
				case Proof::ForceMode::Force:
					return physx::PxForceMode::eFORCE;
					break;
				case Proof::ForceMode::Impule:
					return physx::PxForceMode::eIMPULSE;
					break;
				case Proof::ForceMode::VelocityChange:
					return physx::PxForceMode::eVELOCITY_CHANGE;
					break;
				case Proof::ForceMode::Acceleration:
					return physx::PxForceMode::eACCELERATION;
					break;
			}

			PF_CORE_ASSERT(false, "Not Valid");
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
		if (m_Entity.HasComponent<CapsuleColliderComponent>()) AddCapsuleCollider();
		if (m_Entity.HasComponent<MeshColliderComponent>()) AddMeshCollider();

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
	void PhysicsActor::AddForce(Vector force, ForceMode mode, bool autoWake)
	{
		if (m_RigidBodyType == RigidBodyType::Static)return;
		physx::PxRigidDynamic* rigidBody = (physx::PxRigidDynamic*)m_RuntimeBody;
		rigidBody->addForce({ force.X,force.Y,force.Z }, Utils::ToPhysxForce(mode), autoWake);
	}
	void PhysicsActor::AddTorque(Vector force, ForceMode mode, bool autoWake)
	{
		if (m_RigidBodyType == RigidBodyType::Static)return;
		physx::PxRigidDynamic* rigidBody = (physx::PxRigidDynamic*)m_RuntimeBody;
		rigidBody->addTorque({ force.X,force.Y,force.Z }, Utils::ToPhysxForce(mode), autoWake);
	}

	void PhysicsActor::PutToSleep()
	{
		if (m_RigidBodyType == RigidBodyType::Static)return;

		physx::PxRigidDynamic* rigidBody = (physx::PxRigidDynamic*)m_RuntimeBody;
		rigidBody->putToSleep();
	}
	void PhysicsActor::WakeUp()
	{
		if (m_RigidBodyType == RigidBodyType::Static)return;

		physx::PxRigidDynamic* rigidBody = (physx::PxRigidDynamic*)m_RuntimeBody;
		rigidBody->wakeUp();
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
	void PhysicsActor::OnFixedUpdate(float deltaTime)
	{
		// posibbly calling fixed update on scripts maybe we add it or maybe not

		{
			physx::PxRigidActor* rigidBody = (physx::PxRigidActor*)m_RuntimeBody;

			TransformComponent transform = m_PhysicsWorld->GetWorld()->GetWorldTransformComponent(m_Entity);
			physx::PxTransform newPos(PhysxUtils::VectorToPhysxVector(transform.Location), PhysxUtils::VectorToPhysxQuat(transform.Rotation));
			rigidBody->setGlobalPose(newPos,false);

			// adjusting the new size	
			if (m_Entity.HasComponent<CubeColliderComponent>())
			{
				physx::PxShape* colliderShape =(physx::PxShape *) m_Entity.GetComponent<CubeColliderComponent>()->m_RuntimeBody;

				const Vector colliderScalePositive = m_Entity.GetComponent<CubeColliderComponent>()->OffsetScale.GetPositive();

				Vector size = m_PhysicsWorld->GetWorld()->GetWorldScale(m_Entity).GetPositive() * colliderScalePositive;
				colliderShape->setGeometry(physx::PxBoxGeometry(PhysxUtils::VectorToPhysxVector(size)));
			}
		}
	}

	void PhysicsActor::OnCollisonEnter(const PhysicsActor* actor)
	{
		if (ScriptEngine::EntityHasScripts(m_Entity))
		{
			ScriptMeathod::OnCollisionEnter(m_Entity, actor->m_Entity);
		}
	}
	void PhysicsActor::OnCollisonLeave(const PhysicsActor* actor)
	{
		if (ScriptEngine::EntityHasScripts(m_Entity))
		{
			ScriptMeathod::OnCollisionLeave(m_Entity, actor->m_Entity);
		}
	}
	void PhysicsActor::ClearForce(ForceMode mode )
	{
		if (m_RigidBodyType == RigidBodyType::Static)return;

		physx::PxRigidDynamic* rigidBody = (physx::PxRigidDynamic*)m_RuntimeBody;
		rigidBody->clearForce(Utils::ToPhysxForce(mode));
	}
	void PhysicsActor::ClearTorque(ForceMode mode)
	{
		if (m_RigidBodyType == RigidBodyType::Static)return;

		physx::PxRigidDynamic* rigidBody = (physx::PxRigidDynamic*)m_RuntimeBody;
		rigidBody->clearTorque(Utils::ToPhysxForce(mode));
	}

	Vector PhysicsActor::GetLinearVelocity()
	{
		if (m_RigidBodyType == RigidBodyType::Static)return  Vector(0);
		physx::PxRigidDynamic* rigidBody = (physx::PxRigidDynamic*)m_RuntimeBody;

		return PhysxUtils::PhysxToVector(rigidBody->getLinearVelocity());
	}

	Vector PhysicsActor::GetAngularVelocity()
	{
		if (m_RigidBodyType == RigidBodyType::Static)return  Vector(0);
		physx::PxRigidDynamic* rigidBody = (physx::PxRigidDynamic*)m_RuntimeBody;

		return PhysxUtils::PhysxToVector(rigidBody->getAngularVelocity());
	}
	void PhysicsActor::SetLinearVelocity(Vector velocity, bool wakeUp )
	{
		if (m_RigidBodyType == RigidBodyType::Static)return;

		physx::PxRigidDynamic* rigidBody = (physx::PxRigidDynamic*)m_RuntimeBody;
		rigidBody->setLinearVelocity(PhysxUtils::VectorToPhysxVector(velocity), wakeUp);
	}
	void PhysicsActor::SetAngularVelocity(Vector velocity, bool wakeUp)
	{
		if (m_RigidBodyType == RigidBodyType::Static)return;
		physx::PxRigidDynamic* rigidBody = (physx::PxRigidDynamic*)m_RuntimeBody;
		rigidBody->setAngularVelocity(PhysxUtils::VectorToPhysxVector(velocity), wakeUp);
	}
	void PhysicsActor::OnTriggerEnter(const PhysicsActor* actor)
	{
		if (ScriptEngine::EntityHasScripts(m_Entity))
		{
			ScriptMeathod::OnTriggerEnter(m_Entity,actor->m_Entity);
		}
	}

	void PhysicsActor::OnOverlapTriggerEnter(const PhysicsActor* actor)
	{
		if (ScriptEngine::EntityHasScripts(m_Entity))
		{
			ScriptMeathod::OnOverlapTriggerEnter(m_Entity, actor->m_Entity);
		}
	}


	void PhysicsActor::AddRigidBody()
	{
		if (defauultMaterial == nullptr)
			defauultMaterial = PhysicsEngine::GetPhysics()->createMaterial(0.6f, 0.6f, 0.6f);

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
		
		Vector size = worldScalePositive * colliderScalePositive;
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
		SphereColliderComponent* sphereCollider = m_Entity.GetComponent<SphereColliderComponent>();
		float size = sphereCollider->Radius * m_Entity.GetCurrentWorld()->GetWorldScale(m_Entity).GetMaxAbsolute();

		physx::PxMaterial* colliderMaterial = sphereCollider->HasPhysicsMaterial() == false ? defauultMaterial : (physx::PxMaterial*)sphereCollider->GetPhysicsMaterial()->m_RuntimeBody;
		physx::PxShape* body = PhysicsEngine::GetPhysics()->createShape(physx::PxSphereGeometry(size), *colliderMaterial, true);

		physx::PxTransform localtransform = body->getLocalPose();
		localtransform.p += PhysxUtils::VectorToPhysxVector(sphereCollider->OffsetLocation);
		body->setLocalPose(localtransform);
		body->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, Math::InverseBool(sphereCollider->IsTrigger));
		body->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, sphereCollider->IsTrigger);

		sphereCollider->m_RuntimeBody = body;
		physx::PxRigidActor* rigidBody = (physx::PxRigidActor*)m_RuntimeBody;
		rigidBody->attachShape(*body);
	}

	void PhysicsActor::AddCapsuleCollider()
	{
		CapsuleColliderComponent* capsuleCollider = m_Entity.GetComponent<CapsuleColliderComponent>();
		const Vector worldScalePositive = m_Entity.GetCurrentWorld()->GetWorldScale(m_Entity).GetPositive();
		physx::PxMaterial* colliderMaterial = capsuleCollider->HasPhysicsMaterial() == false ? defauultMaterial : (physx::PxMaterial*)capsuleCollider->GetPhysicsMaterial()->m_RuntimeBody;
		float radius = capsuleCollider->Radius * worldScalePositive.GetMaxAbsolute()*2.35;
		float height = capsuleCollider->Height;
		Vector capsuleRotation = { 0,0,0 };// originial local pos is {0,0,0}
		switch (capsuleCollider->Direction)
		{
			case CapsuleDirection::X:
				{
					height *= worldScalePositive.X;
					capsuleRotation.X += 0;// default is facing X direction
				}
				break;
			case CapsuleDirection::Y:
				{
					height *= worldScalePositive.Y;
					capsuleRotation.Y += 90; // have to swap ssicne starting is facing X direction
					break;
				}
			case CapsuleDirection::Z:
				{
					height *= worldScalePositive.Z;
					capsuleRotation.Z += 90; // have to swap ssicne starting is facing X direction
					break;
				}
		}
		physx::PxShape* body = PhysicsEngine::GetPhysics()->createShape(physx::PxCapsuleGeometry(radius, height), *colliderMaterial, true);
		body->setName(fmt::to_string(m_Entity.GetEntityID()).c_str()); // we can easily rigidBodyComponent After collsion

		physx::PxTransform localtransform = body->getLocalPose();
		localtransform.p += PhysxUtils::VectorToPhysxVector(capsuleCollider->OffsetLocation);
		//localtransform.q += PhysxUtils::VectorToPhysxQuat(capsuleRotation);
		body->setLocalPose(localtransform);
		body->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, Math::InverseBool(capsuleCollider->IsTrigger));
		body->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, capsuleCollider->IsTrigger);

		capsuleCollider->m_RuntimeBody = body;
		physx::PxRigidActor* rigidBody = (physx::PxRigidActor*)m_RuntimeBody;
		rigidBody->attachShape(*body);
	}

	void PhysicsActor::AddMeshCollider()
	{
		
		MeshColliderComponent* meshCollider = m_Entity.GetComponent<MeshColliderComponent>();
		if (!AssetManager::HasAsset(meshCollider->GetMeshSource()))return;
		if (PhysicsMeshCooker::HasMesh(meshCollider->GetMeshSource()) == false)
		{
			PhysicsMeshCooker::CookMesh(meshCollider->GetMeshSource());
		}
		physx::PxMaterial* colliderMaterial = meshCollider->HasPhysicsMaterial() == false ? defauultMaterial : (physx::PxMaterial*)meshCollider->GetPhysicsMaterial()->m_RuntimeBody;
		physx::PxShape* body = PhysicsEngine::GetPhysics()->createShape(
			physx::PxTriangleMeshGeometry(PhysicsMeshCooker::GetConvexMesh(meshCollider->GetMeshSource())),*colliderMaterial, true);
		//ADD CONVEX MESH TO ASSET
		body->setName(fmt::to_string(m_Entity.GetEntityID()).c_str()); // we can easily rigidBodyComponent After collsion
		meshCollider->m_RuntimeBody = body;
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
