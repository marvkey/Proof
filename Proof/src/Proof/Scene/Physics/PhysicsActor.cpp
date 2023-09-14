#include "Proofprch.h"
#include "PhysicsActor.h"
#include "Proof/Scene/Component.h"
#include "PhysicsWorld.h"
#include "../Entity.h"
#include "PhysicsEngine.h"
#include "Proof/Scene/Material.h"
#include "Proof/Scene/Entity.h"

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
	PhysicsActor::PhysicsActor(PhysicsWorld* physicsWorld, UUID entityId)
		:
		m_PhysicsWorld(physicsWorld)
	{
		
		m_Entity = m_PhysicsWorld->GetWorld()->GetEntity(entityId);

		if (!m_Entity.HasComponent<RigidBodyComponent>())
			PF_CORE_ASSERT(false, "Needs rigid body to be a physics Actor");
		
		Build();
	}
	PhysicsActor::~PhysicsActor()
	{
		Release();
	}
	void PhysicsActor::Build()
	{
		AddRigidBody();

		if (m_Entity.HasComponent<CubeColliderComponent>()) AddCubeCollider();
		if (m_Entity.HasComponent<SphereColliderComponent>())AddSphereCollider();
		if (m_Entity.HasComponent<CapsuleColliderComponent>()) AddCapsuleCollider();
		if (m_Entity.HasComponent<MeshColliderComponent>()) AddMeshCollider();
	}
	void PhysicsActor::Release()
	{
		physx::PxRigidActor* rigidBody = (physx::PxRigidActor*)m_RuntimeBody;

		if (m_Entity.HasComponent<CubeColliderComponent>() )
		{
			physx::PxShape* shape = (physx::PxShape*)m_CapsuleColliderBody;
			rigidBody->detachShape(*shape);
			///shape->release();
		}
		if (m_Entity.HasComponent<SphereColliderComponent>())
		{
			physx::PxShape* shape = (physx::PxShape*)m_SphereColliderBody;
			rigidBody->detachShape(*shape);
			//shape->release();
		}
		if (m_Entity.HasComponent<CapsuleColliderComponent>())
		{
			physx::PxShape* shape = (physx::PxShape*)m_CapsuleColliderBody;
			rigidBody->detachShape(*shape);
			//shape->release();
		}
		if (m_Entity.HasComponent<MeshColliderComponent>())
		{
			physx::PxShape* shape = (physx::PxShape*)m_MeshColliderBody;
			rigidBody->detachShape(*shape);
			//shape->release();
		}
		// shoudl the next paremter be false
		m_PhysicsWorld->GetPhysicsScene()->removeActor(*rigidBody);

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

			
		UpdateRigidBody(deltaTime);
		// adjusting the new size	
		if (m_Entity.HasComponent<CubeColliderComponent>())
		{
			if (m_CubeColliderBody == nullptr)
				AddCubeCollider();
			UpdateCubeCollider(deltaTime);
		}
		if (m_Entity.HasComponent<SphereColliderComponent>())
		{
			if (m_CubeColliderBody == nullptr)
				AddSphereCollider();
			UpdateSphereCollider(deltaTime);
		}
		if (m_Entity.HasComponent<CapsuleColliderComponent>())
		{
			if (m_CubeColliderBody == nullptr)
				AddCapsuleCollider();
			UpdateCapsuleCollider(deltaTime);
		}
		if (m_Entity.HasComponent<MeshColliderComponent>())
		{
			if (m_CubeColliderBody == nullptr)
				AddMeshCollider();
			UpdateMeshCollider(deltaTime);
		}
	}

	void PhysicsActor::OnCollisonEnter(const PhysicsActor* actor)
	{
		if (ScriptEngine::EntityHasScripts(m_Entity))
		{
			ScriptMeathod::OnCollisionEnter(m_Entity, actor->m_Entity);
		}
	}
	void PhysicsActor::OnCollisonStay(const PhysicsActor* actor)
	{
		//PF_CORE_ASSERT(false);
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

	void PhysicsActor::OnTriggerStay(const PhysicsActor* actor)
	{
	}

	void PhysicsActor::OnTriggerLeave(const PhysicsActor* actor)
	{
	}

	void PhysicsActor::OnOverlapTriggerEnter(const PhysicsActor* actor)
	{
		if (ScriptEngine::EntityHasScripts(m_Entity))
		{
			ScriptMeathod::OnOverlapTriggerEnter(m_Entity, actor->m_Entity);
		}
	}

	void PhysicsActor::OnOverlapTriggerStay(const PhysicsActor* actor)
	{

	}

	void PhysicsActor::OnOverlapTriggerLeave(const PhysicsActor* actor)
	{

	}


	void PhysicsActor::AddRigidBody()
	{
		if (defauultMaterial == nullptr)
			defauultMaterial = PhysicsEngine::GetPhysics()->createMaterial(0.6f, 0.6f, 0.6f);

		const auto& transformComponent = m_PhysicsWorld->GetWorld()->GetWorldSpaceTransformComponent(m_Entity);

		auto& rigidBodyComponent = m_Entity.GetComponent<RigidBodyComponent>();
		
		
		physx::PxTransform physxTransform{ PhysxUtils::GlmVectorToPhysxVector(transformComponent.Location),
				PhysxUtils::QuatTophysxQuat(transformComponent.GetRotation())};
		physx::PxActor* rigidBodyBase = nullptr;
		if (rigidBodyComponent.m_RigidBodyType == RigidBodyType::Dynamic)
		{

			physx::PxRigidDynamic* body = PhysicsEngine::GetPhysics()->createRigidDynamic(physxTransform);
			body->setName(fmt::to_string(m_Entity.GetUUID()).c_str()); // we can easily rigidBodyComponent After collsion
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
		auto& cubeCollider = m_Entity.GetComponent<CubeColliderComponent>();
		auto& rigidBodyComponent = m_Entity.GetComponent<RigidBodyComponent>();
		physx::PxMaterial* colliderMaterial = cubeCollider.HasPhysicsMaterial() == false ? defauultMaterial : (physx::PxMaterial*)cubeCollider.GetPhysicsMaterial()->m_RuntimeBody;
		
		const glm::vec3 worldScalePositive =glm::abs( m_Entity.GetCurrentWorld()->GetWorldSpaceScale(m_Entity));
		const glm::vec3 colliderScalePositive = glm::abs(cubeCollider.OffsetScale);
		
		glm::vec3 size = worldScalePositive * colliderScalePositive;
		physx::PxShape* body = PhysicsEngine::GetPhysics()->createShape(physx::PxBoxGeometry(PhysxUtils::GlmVectorToPhysxVector(size)), *colliderMaterial, true);
		PF_CORE_ASSERT(body, "Body is not created");
		//body->setName(fmt::to_string(m_Entity.GetUUID()).c_str()); // we can easily rigidBodyComponent After collsion
		auto localtransform = body->getLocalPose();
		localtransform.p += PhysxUtils::GlmVectorToPhysxVector(cubeCollider.OffsetLocation);
		body->setLocalPose(localtransform);
		body->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, Math::InverseBool(cubeCollider.IsTrigger));
		body->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, cubeCollider.IsTrigger);

		m_CubeColliderBody = body;
		physx::PxRigidActor* rigidBody = (physx::PxRigidActor*)m_RuntimeBody;
		rigidBody->attachShape(*body);
	}

	void PhysicsActor::AddSphereCollider()
	{
		SphereColliderComponent& sphereCollider = m_Entity.GetComponent<SphereColliderComponent>();
		const glm::vec3 worldScalePositive = glm::abs(m_Entity.GetCurrentWorld()->GetWorldSpaceScale(m_Entity));

		float size = sphereCollider.Radius * glm::max(glm::max(worldScalePositive.x, worldScalePositive.y), worldScalePositive.z);
		physx::PxMaterial* colliderMaterial = sphereCollider.HasPhysicsMaterial() == false ? defauultMaterial : (physx::PxMaterial*)sphereCollider.GetPhysicsMaterial()->m_RuntimeBody;
		physx::PxShape* body = PhysicsEngine::GetPhysics()->createShape(physx::PxSphereGeometry(size), *colliderMaterial, true);
		PF_CORE_ASSERT(body, "Body is not created");

		physx::PxTransform localtransform = body->getLocalPose();
		localtransform.p += PhysxUtils::VectorToPhysxVector(sphereCollider.OffsetLocation);
		body->setLocalPose(localtransform);
		body->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, Math::InverseBool(sphereCollider.IsTrigger));
		body->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, sphereCollider.IsTrigger);

		m_SphereColliderBody = body;
		physx::PxRigidActor* rigidBody = (physx::PxRigidActor*)m_RuntimeBody;
		rigidBody->attachShape(*body);
	}

	void PhysicsActor::AddCapsuleCollider()
	{
		CapsuleColliderComponent& capsuleCollider = m_Entity.GetComponent<CapsuleColliderComponent>();
		const glm::vec3 worldScalePositive = glm::abs(m_Entity.GetCurrentWorld()->GetWorldSpaceScale(m_Entity));

		physx::PxMaterial* colliderMaterial = capsuleCollider.HasPhysicsMaterial() == false ? defauultMaterial : (physx::PxMaterial*)capsuleCollider.GetPhysicsMaterial()->m_RuntimeBody;
		float radius = capsuleCollider.Radius * glm::max(glm::max(worldScalePositive.x, worldScalePositive.y), worldScalePositive.z);
		float height = capsuleCollider.Height;
		Vector capsuleRotation = { 0,0,0 };// originial local pos is {0,0,0}
		switch (capsuleCollider.Direction)
		{
			case CapsuleDirection::X:
				{
					height *= worldScalePositive.x;
					capsuleRotation.X += 0;// default is facing X direction
				}
				break;
			case CapsuleDirection::Y:
				{
					height *= worldScalePositive.y;
					capsuleRotation.Y += 90; // have to swap ssicne starting is facing X direction
					break;
				}
			case CapsuleDirection::Z:
				{
					height *= worldScalePositive.z;
					capsuleRotation.Z += 90; // have to swap ssicne starting is facing X direction
					break;
				}
		}
		physx::PxShape* body = PhysicsEngine::GetPhysics()->createShape(physx::PxCapsuleGeometry(radius, height), *colliderMaterial, true);
		PF_CORE_ASSERT(body, "Body is not created");

		body->setName(fmt::to_string(m_Entity.GetUUID()).c_str()); // we can easily rigidBodyComponent After collsion

		physx::PxTransform localtransform = body->getLocalPose();
		localtransform.p += PhysxUtils::VectorToPhysxVector(capsuleCollider.OffsetLocation);
		//localtransform.q += PhysxUtils::VectorToPhysxQuat(capsuleRotation);
		body->setLocalPose(localtransform);
		body->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, Math::InverseBool(capsuleCollider.IsTrigger));
		body->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, capsuleCollider.IsTrigger);

		m_CapsuleColliderBody = body;
		physx::PxRigidActor* rigidBody = (physx::PxRigidActor*)m_RuntimeBody;
		rigidBody->attachShape(*body);
	}

	void PhysicsActor::AddMeshCollider()
	{
		MeshColliderComponent& meshCollider = m_Entity.GetComponent<MeshColliderComponent>();
		if (!AssetManager::HasAsset(meshCollider.GetMeshSource()))return;
		if (PhysicsMeshCooker::HasMesh(meshCollider.GetMeshSource()) == false)
		{
			PhysicsMeshCooker::CookMesh(meshCollider.GetMeshSource());	
		}
		physx::PxMaterial* colliderMaterial = meshCollider.HasPhysicsMaterial() == false ? defauultMaterial : (physx::PxMaterial*)meshCollider.GetPhysicsMaterial()->m_RuntimeBody;
		//physx::PxShape* body = PhysicsEngine::GetPhysics()->createShape(
		//	physx::PxTriangleMeshGeometry(PhysicsMeshCooker::GetConvexMesh(meshCollider.GetMeshSource()),PhysxUtils::GlmVectorToPhysxVector(glm::abs(m_PhysicsWorld->GetWorld()->GetWorldSpaceScale(m_Entity))),*colliderMaterial, true);
				//physx::PxShape* body = PhysicsEngine::GetPhysics()->createShape(
		//physx::PxConvexMeshGeometry(PhysicsMeshCooker::GetConvexMesh(meshCollider->GetMeshSource())), *colliderMaterial, true);
		//ADD CONVEX MESH TO ASSET
	//	PF_CORE_ASSERT(body, "Body is not created");
	//
	//	m_MeshColliderBody = body;
	//	body->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, Math::InverseBool(meshCollider.IsTrigger));
	//	body->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, meshCollider.IsTrigger);
	//	physx::PxRigidActor* rigidBody = (physx::PxRigidActor*)m_RuntimeBody;
	//	rigidBody->attachShape(*body);
	}

	void PhysicsActor::UpdateRigidBody(float deltatime)
	{

		auto& rigidBodyComponent = m_Entity.GetComponent<RigidBodyComponent>();
		physx::PxRigidActor* defaultRigidBody = (physx::PxRigidActor*)m_RuntimeBody;

		bool updateBodyType = false;
		if (rigidBodyComponent.m_RigidBodyType == RigidBodyType::Dynamic)
		{
			if (!defaultRigidBody->is<physx::PxRigidDynamic>())
				updateBodyType = true;
		}

		if (rigidBodyComponent.m_RigidBodyType == RigidBodyType::Static)
		{
			if (!defaultRigidBody->is<physx::PxRigidStatic>())
				updateBodyType = true;
		}

		if (updateBodyType)
		{
			Release();
			Build();
		}

		TransformComponent transform = m_PhysicsWorld->GetWorld()->GetWorldSpaceTransformComponent(m_Entity);
		physx::PxTransform newPos(PhysxUtils::GlmVectorToPhysxVector(transform.Location), PhysxUtils::QuatTophysxQuat(transform.GetRotation()));

		defaultRigidBody->setGlobalPose(newPos, false);
		defaultRigidBody->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, Math::InverseBool(m_Entity.GetComponent<RigidBodyComponent>().Gravity));

		if (rigidBodyComponent.m_RigidBodyType == RigidBodyType::Dynamic)
		{
			physx::PxRigidDynamic* rigidBody = (physx::PxRigidDynamic*)m_RuntimeBody;
			rigidBody->setMass(rigidBodyComponent.Mass);
			rigidBody->setAngularDamping(rigidBodyComponent.AngularDrag);
			rigidBody->setLinearDamping(rigidBodyComponent.LinearDrag);
			rigidBody->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, Math::InverseBool(rigidBodyComponent.Gravity));
			rigidBody->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, rigidBodyComponent.Kinimatic);
			rigidBody->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_X, rigidBodyComponent.FreezeLocation.X);
			rigidBody->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Y, rigidBodyComponent.FreezeLocation.Y);
			rigidBody->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, rigidBodyComponent.FreezeLocation.Z);

			rigidBody->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, rigidBodyComponent.FreezeRotation.X);
			rigidBody->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, rigidBodyComponent.FreezeRotation.Y);
			rigidBody->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, rigidBodyComponent.FreezeRotation.Z);
		}

	}

	void PhysicsActor::UpdateCubeCollider(float deltatime)
	{
		auto& cubeCollider = m_Entity.GetComponent<CubeColliderComponent>();
		physx::PxShape* colliderShape = (physx::PxShape*)m_CubeColliderBody;

		const glm::vec3 colliderScalePositive = glm::abs(cubeCollider.OffsetScale);

		glm::vec3 size = glm::abs(m_PhysicsWorld->GetWorld()->GetWorldSpaceScale(m_Entity)) * colliderScalePositive;
		colliderShape->setGeometry(physx::PxBoxGeometry(PhysxUtils::GlmVectorToPhysxVector(size)));
		colliderShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, Math::InverseBool(cubeCollider.IsTrigger));
		colliderShape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, cubeCollider.IsTrigger);
	}

	void PhysicsActor::UpdateSphereCollider(float deltatime)
	{
		auto& sphereCollider = m_Entity.GetComponent<SphereColliderComponent>();
		physx::PxShape* colliderShape = (physx::PxShape*)m_SphereColliderBody;

		glm::vec3 worldScale = glm::abs(m_Entity.GetCurrentWorld()->GetWorldSpaceScale(m_Entity));
		float radius = sphereCollider.Radius * glm::max(glm::max(worldScale.x, worldScale.y), worldScale.z); ;
		colliderShape->setGeometry(physx::PxSphereGeometry(radius));
		colliderShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, Math::InverseBool(sphereCollider.IsTrigger));
		colliderShape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, sphereCollider.IsTrigger);
	}

	void PhysicsActor::UpdateCapsuleCollider(float deltatime)
	{
		auto& capsuleCollider = m_Entity.GetComponent<CapsuleColliderComponent>();
		physx::PxShape* colliderShape = (physx::PxShape*)m_CapsuleColliderBody;
		const glm::vec3 worldScalePositive = glm::abs(m_Entity.GetCurrentWorld()->GetWorldSpaceScale(m_Entity));

		float radius = capsuleCollider.Radius * glm::max(glm::max(worldScalePositive.x, worldScalePositive.y), worldScalePositive.z);;
		float height =glm::abs( capsuleCollider.Height);
		Vector capsuleRotation = { 0,0,0 };// originial local pos is {0,0,0}
		switch (capsuleCollider.Direction)
		{
			case CapsuleDirection::X:
				{
					height *= worldScalePositive.x;
					capsuleRotation.X += 0;// default is facing X direction
				}
				break;
			case CapsuleDirection::Y:
				{
					height *= worldScalePositive.y;
					capsuleRotation.Y += 90; // have to swap ssicne starting is facing X direction
					break;
				}
			case CapsuleDirection::Z:
				{
					height *= worldScalePositive.z;
					capsuleRotation.Z += 90; // have to swap ssicne starting is facing X direction
					break;
				}
		}

		colliderShape->setGeometry(physx::PxCapsuleGeometry(radius,height));
		colliderShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, Math::InverseBool(capsuleCollider.IsTrigger));
		colliderShape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, capsuleCollider.IsTrigger);
	}

	void PhysicsActor::UpdateMeshCollider(float deltatime)
	{
		MeshColliderComponent& meshCollider = m_Entity.GetComponent<MeshColliderComponent>();
		if (!AssetManager::HasAsset(meshCollider.GetMeshSource()))return;
		if (PhysicsMeshCooker::HasMesh(meshCollider.GetMeshSource()) == false)
		{
			PhysicsMeshCooker::CookMesh(meshCollider.GetMeshSource());
		}
		physx::PxShape* colliderShape = (physx::PxShape*)m_MeshColliderBody;
		colliderShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, Math::InverseBool(meshCollider.IsTrigger));
		colliderShape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, meshCollider.IsTrigger);
	}
	void PhysicsActor::SyncTransform()
	{
		physx::PxRigidActor* rigidBody = (physx::PxRigidActor*)m_RuntimeBody;

		TransformComponent& transform = m_Entity.GetComponent<TransformComponent>();
		auto actorPos = rigidBody->getGlobalPose();
		transform.Location = PhysxUtils::PhysxVectorToGlmVector(actorPos.p);
		transform.SetRotation(PhysxUtils::PhysxQuatToQuat(actorPos.q));
	}
}
