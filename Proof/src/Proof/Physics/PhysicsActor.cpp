#include "Proofprch.h"
#include "PhysicsActor.h"
#include "Proof/Scene/Component.h"
#include "PhysicsWorld.h"
#include "PhysicsEngine.h"
#include "Proof/Scene/Material.h"
#include "Proof/Scene/Entity.h"
#include "Proof/Scripting/ScriptEngine.h"
#include "PhysicsShapes.h"
namespace Proof {

	
	namespace Utils {

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
	PhysicsActor::PhysicsActor(Count<class PhysicsWorld> world, Entity entity)
		:
		PhysicsActorBase(world, PhysicsControllerType::Actor,entity)
	{
		
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

		if (m_Entity.HasComponent<BoxColliderComponent>())	AddCollider(m_Entity.GetComponent<BoxColliderComponent>());
		if (m_Entity.HasComponent<CapsuleColliderComponent>()) AddCollider(m_Entity.GetComponent<CapsuleColliderComponent>());
		if (m_Entity.HasComponent<SphereColliderComponent>()) AddCollider(m_Entity.GetComponent<SphereColliderComponent>());
		//if (m_Entity.HasComponent<MeshColliderComponent>()) AddMeshCollider();
	}
	Entity PhysicsActor::GetEntity()
	{
		return m_Entity;
	}
	void PhysicsActor::Release()
	{
		for (auto& collider : m_Colliders)
			collider->DetachFromActor(m_RigidActor);
		m_Colliders.clear();
		m_RigidActor->getScene()->removeActor(*m_RigidActor);
		m_RigidActor->release();
	}

	bool PhysicsActor::IsDynamic()const
	{
		
		return m_Entity.GetComponent<RigidBodyComponent>().m_RigidBodyType == RigidBodyType::Dynamic;
	}

	void PhysicsActor::AddForce(glm::vec3 force, ForceMode mode, bool autoWake)
	{
		if (!IsDynamic())return;
		physx::PxRigidDynamic* rigidBody = (physx::PxRigidDynamic*)m_RigidActor;
		rigidBody->addForce({ force.x,force.y,force.z }, Utils::ToPhysxForce(mode), autoWake);
	}
	void PhysicsActor::AddTorque(glm::vec3 force, ForceMode mode, bool autoWake)
	{
		if (!IsDynamic())return;
		physx::PxRigidDynamic* rigidBody = (physx::PxRigidDynamic*)m_RigidActor;
		rigidBody->addTorque({ force.x,force.y,force.z }, Utils::ToPhysxForce(mode), autoWake);
	}

	void PhysicsActor::PutToSleep()
	{
		if (!IsDynamic())return;

		physx::PxRigidDynamic* rigidBody = (physx::PxRigidDynamic*)m_RigidActor;
		rigidBody->putToSleep();
	}
	void PhysicsActor::WakeUp()
	{
		if (!IsDynamic())return;

		physx::PxRigidDynamic* rigidBody = (physx::PxRigidDynamic*)m_RigidActor;
		rigidBody->wakeUp();
	}
	void PhysicsActor::SetRigidType(RigidBodyType type)
	{
		RigidBodyComponent& rigidBody = m_Entity.GetComponent<RigidBodyComponent>();
		if (rigidBody.m_RigidBodyType == type)
			return;
		Release();
		rigidBody.m_RigidBodyType = type;
		Build();
	}
	bool PhysicsActor::IsSleeping()
	{

		if (!IsDynamic())return true;
	
		physx::PxRigidDynamic* rigidBody = (physx::PxRigidDynamic*)m_RigidActor;
		return rigidBody->isSleeping();
	}
	void PhysicsActor::OnFixedUpdate(float deltaTime)
	{
		//TransformComponent transform = m_PhysicsWorld->GetWorld()->GetWorldSpaceTransformComponent(m_Entity);
		//physx::PxTransform newPos(PhysXUtils::ToPhysXVector(transform.Location), PhysXUtils::ToPhysXQuat(transform.GetRotation()));
		//m_RigidActor->setGlobalPose(newPos, false);

		//if (!ScriptEngine::IsEntityInstantiated(m_Entity))
		//	return;
		//
		//ScriptEngine::CallMethod(m_Entity.GetComponent<ScriptComponent>().ManagedInstance, "OnPhysicsUpdate", fixedDeltaTime);
	}
	
	void PhysicsActor::SetLocation(const glm::vec3& translation, const bool autowake)
	{
		physx::PxTransform transform = m_RigidActor->getGlobalPose();
		transform.p = PhysXUtils::ToPhysXVector(translation);
		m_RigidActor->setGlobalPose(transform, autowake);

		if (!IsDynamic())
			SyncTransform();
	}

	void PhysicsActor::ClearForce(ForceMode mode )
	{
		if (!IsDynamic())return;

		physx::PxRigidDynamic* rigidBody = (physx::PxRigidDynamic*)m_RigidActor;
		rigidBody->clearForce(Utils::ToPhysxForce(mode));
	}
	void PhysicsActor::ClearTorque(ForceMode mode)
	{
		if (!IsDynamic())return;

		physx::PxRigidDynamic* rigidBody = (physx::PxRigidDynamic*)m_RigidActor;
		rigidBody->clearTorque(Utils::ToPhysxForce(mode));
	}

	glm::vec3 PhysicsActor::GetLinearVelocity()const
	{
		if (!IsDynamic())return  glm::vec3(0);
		physx::PxRigidDynamic* rigidBody = (physx::PxRigidDynamic*)m_RigidActor;

		return PhysXUtils::FromPhysXVector(rigidBody->getLinearVelocity());
	}

	glm::vec3 PhysicsActor::GetAngularVelocity()const
	{
		if (!IsDynamic())return  glm::vec3(0);
		physx::PxRigidDynamic* rigidBody = (physx::PxRigidDynamic*)m_RigidActor;

		return PhysXUtils::FromPhysXVector(rigidBody->getAngularVelocity());
	}
	void PhysicsActor::SetLinearVelocity(const glm::vec3& velocity)
	{
		if (!IsDynamic())return;

		physx::PxRigidDynamic* rigidBody = (physx::PxRigidDynamic*)m_RigidActor;
		rigidBody->setLinearVelocity(PhysXUtils::ToPhysXVector(velocity));
	}
	void PhysicsActor::SetAngularVelocity(const glm::vec3& velocity)
	{
		if (!IsDynamic())return;
		physx::PxRigidDynamic* rigidBody = (physx::PxRigidDynamic*)m_RigidActor;
		rigidBody->setAngularVelocity(PhysXUtils::ToPhysXVector(velocity));
	}

	float PhysicsActor::GetMaxLinearVelocity() const
	{
		if (!IsDynamic())
		{
			PF_ENGINE_WARN("Trying to get max linear velocity of non-dynamic PhysicsActor.");
			return 0.0f;
		}

		physx::PxRigidDynamic* actor = m_RigidActor->is<physx::PxRigidDynamic>();
		PF_CORE_ASSERT(actor);
		return actor->getMaxLinearVelocity();
	}

	void PhysicsActor::SetMaxLinearVelocity(float maxVelocity)
	{
		if (!IsDynamic())
		{
			PF_ENGINE_WARN("Trying to set max linear velocity of non-dynamic PhysicsActor.");
			return;
		}

		physx::PxRigidDynamic* actor = m_RigidActor->is<physx::PxRigidDynamic>();
		PF_CORE_ASSERT(actor);
		actor->setMaxLinearVelocity(maxVelocity);
	}

	float PhysicsActor::GetMaxAngularVelocity() const
	{
		if (!IsDynamic())
		{
			PF_ENGINE_WARN("Trying to get max angular velocity of non-dynamic PhysicsActor.");
			return 0.0f;
		}

		physx::PxRigidDynamic* actor = m_RigidActor->is<physx::PxRigidDynamic>();
		PF_CORE_ASSERT(actor);
		return actor->getMaxAngularVelocity();
	}

	void PhysicsActor::SetMaxAngularVelocity(float maxVelocity)
	{
		if (!IsDynamic())
		{
			PF_ENGINE_WARN("Trying to set max angular velocity of non-dynamic PhysicsActor.");
			return;
		}

		physx::PxRigidDynamic* actor = m_RigidActor->is<physx::PxRigidDynamic>();
		PF_CORE_ASSERT(actor);
		actor->setMaxAngularVelocity(maxVelocity);
	}

	float PhysicsActor::GetLinearDrag() const
	{
		if (!IsDynamic())
		{
			PF_ENGINE_WARN("Trying to get linear drag of non-dynamic PhysicsActor.");
			return 0.0f;
		}

		physx::PxRigidDynamic* actor = m_RigidActor->is<physx::PxRigidDynamic>();
		PF_CORE_ASSERT(actor);
		return actor->getLinearDamping();
	}

	void PhysicsActor::SetLinearDrag(float drag)
	{
		if (!IsDynamic())
		{
			PF_ENGINE_WARN("Trying to set linear drag of non-dynamic PhysicsActor.");
			return;
		}

		physx::PxRigidDynamic* actor = m_RigidActor->is<physx::PxRigidDynamic>();
		PF_CORE_ASSERT(actor);
		actor->setLinearDamping(drag);
		m_Entity.GetComponent<RigidBodyComponent>().LinearDrag = drag;
	}

	float PhysicsActor::GetAngularDrag() const
	{
		if (!IsDynamic())
		{
			PF_ENGINE_WARN("Trying to get angular drag of non-dynamic PhysicsActor.");
			return 0.0f;
		}

		physx::PxRigidDynamic* actor = m_RigidActor->is<physx::PxRigidDynamic>();
		PF_CORE_ASSERT(actor);
		return actor->getAngularDamping();
	}

	void PhysicsActor::SetAngularDrag(float drag)
	{
		if (!IsDynamic())
		{
			PF_ENGINE_WARN("Trying to set angular drag of non-dynamic PhysicsActor.");
			return;
		}

		physx::PxRigidDynamic* actor = m_RigidActor->is<physx::PxRigidDynamic>();
		PF_CORE_ASSERT(actor);
		actor->setAngularDamping(drag);
		m_Entity.GetComponent<RigidBodyComponent>().AngularDrag = drag;
	}

	glm::vec3 PhysicsActor::GetKinematicTargetPosition() const
	{
		if (!IsKinematic())
		{
			PF_ENGINE_WARN("Trying to get kinematic target for a non-kinematic actor.");
			return glm::vec3(0.0f, 0.0f, 0.0f);
		}

		physx::PxRigidDynamic* actor = m_RigidActor->is<physx::PxRigidDynamic>();
		PF_CORE_ASSERT(actor);
		physx::PxTransform target;
		PF_CORE_ASSERT(actor->getKinematicTarget(target), "kinematic target not set");
		return PhysXUtils::FromPhysXVector(target.p);
	}

	glm::quat PhysicsActor::GetKinematicTargetRotation() const
	{
		if (!IsKinematic())
		{
			PF_ENGINE_WARN("Trying to get kinematic target for a non-kinematic actor.");
			return glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		}

		physx::PxRigidDynamic* actor = m_RigidActor->is<physx::PxRigidDynamic>();
		PF_CORE_ASSERT(actor);
		physx::PxTransform target;
		PF_CORE_ASSERT(actor->getKinematicTarget(target), "kinematic target not set");
		return PhysXUtils::FromPhysXQuat(target.q);
	}

	glm::vec3 PhysicsActor::GetKinematicTargetRotationEuler() const
	{
		return glm::eulerAngles(GetKinematicTargetRotation());
	}

	void PhysicsActor::SetKinematicTarget(const glm::vec3& targetPosition, const glm::quat& targetRotation) const
	{
		if (!IsKinematic())
		{
			PF_ENGINE_WARN("Trying to set kinematic target for a non-kinematic actor.");
			return;
		}

		physx::PxRigidDynamic* actor = m_RigidActor->is<physx::PxRigidDynamic>();
		PF_CORE_ASSERT(actor);
		actor->setKinematicTarget(PhysXUtils::ToPhysXTransform(targetPosition, targetRotation));
	}
	void PhysicsActor::SetKinematic(bool isKinematic)
	{
		if (!IsDynamic())
		{
			PF_ENGINE_WARN("Static PhysicsActor can't be kinematic.");
			return;
		}
		RigidBodyComponent& rigidBody = m_Entity.GetComponent<RigidBodyComponent>();

		rigidBody.Kinematic = isKinematic;
		m_RigidActor->is<physx::PxRigidDynamic>()->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, isKinematic);
	}

	void PhysicsActor::SetLockLocation(const VectorTemplate<bool>& location)
	{
		if (!IsDynamic())
			return;

		m_RigidActor->is<physx::PxRigidDynamic>()->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_X, location.X);
		m_RigidActor->is<physx::PxRigidDynamic>()->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Y, location.Y);
		m_RigidActor->is<physx::PxRigidDynamic>()->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, location.Z);

		RigidBodyComponent& rigidBody = m_Entity.GetComponent<RigidBodyComponent>();

		rigidBody.FreezeLocation = location;
	}

	void PhysicsActor::SetLockRotaion(const VectorTemplate<bool>& rotation)
	{
		if (!IsDynamic())
			return;
		m_RigidActor->is<physx::PxRigidDynamic>()->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, rotation.X);
		m_RigidActor->is<physx::PxRigidDynamic>()->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, rotation.Y);
		m_RigidActor->is<physx::PxRigidDynamic>()->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, rotation.Z);

		RigidBodyComponent& rigidBody = m_Entity.GetComponent<RigidBodyComponent>();

		rigidBody.FreezeRotation = rotation;
	}

	void PhysicsActor::SetGravityEnabled(const bool enableGravity)
	{
		m_RigidActor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !enableGravity);
		m_Entity.GetComponent<RigidBodyComponent>().Gravity = enableGravity;
	}

	void PhysicsActor::AddCollider(BoxColliderComponent& collider)
	{
		m_Colliders.push_back(Count<BoxColliderShape>::Create(collider, *this, m_Entity));
	}

	void PhysicsActor::AddCollider(SphereColliderComponent& collider)
	{
		m_Colliders.push_back(Count<SphereColliderShape>::Create(collider, *this, m_Entity));
	}

	void PhysicsActor::AddCollider(CapsuleColliderComponent& collider)
	{
		m_Colliders.push_back(Count<CapsuleColliderShape>::Create(collider, *this, m_Entity));
	}

	void PhysicsActor::AddCollider(MeshColliderComponent& collider)
	{
	}

	void PhysicsActor::SetRotation(const glm::quat& rotation, bool autowake)
	{
		physx::PxTransform transform = m_RigidActor->getGlobalPose();
		transform.q = PhysXUtils::ToPhysXQuat(rotation);
		m_RigidActor->setGlobalPose(transform, autowake);

		if (!IsDynamic())
			SyncTransform();
	}

	void PhysicsActor::Rotate(const glm::quat& rotation, bool autowake)
	{
		physx::PxTransform transform = m_RigidActor->getGlobalPose();
		transform.q *= PhysXUtils::ToPhysXQuat(rotation);
		m_RigidActor->setGlobalPose(transform, autowake);

		if (!IsDynamic())
			SyncTransform();
	}

	float PhysicsActor::GetMass() const
	{
		RigidBodyComponent& rigidBody = m_Entity.GetComponent<RigidBodyComponent>();

		return !IsDynamic() ? rigidBody.Mass : m_RigidActor->is<physx::PxRigidDynamic>()->getMass();
	}

	void PhysicsActor::SetMass(float mass)
	{
		if (!IsDynamic())
			return;

		physx::PxRigidDynamic* actor = m_RigidActor->is<physx::PxRigidDynamic>();
		PF_CORE_ASSERT(actor);
		physx::PxRigidBodyExt::setMassAndUpdateInertia(*actor, mass);
		RigidBodyComponent& rigidBody = m_Entity.GetComponent<RigidBodyComponent>();

		rigidBody.Mass = mass;
	}
	
	void PhysicsActor::AddRigidBody()
	{
		const auto& transformComponent = m_PhysicsWorld->GetWorld()->GetWorldSpaceTransformComponent(m_Entity);
		auto& rigidBodyComponent = m_Entity.GetComponent<RigidBodyComponent>();

		physx::PxTransform physxTransform = PhysXUtils::ToPhysXTransform( m_PhysicsWorld->GetWorld()->GetWorldSpaceTransformComponent(m_Entity));

		if (rigidBodyComponent.m_RigidBodyType == RigidBodyType::Dynamic)
		{

			physx::PxRigidDynamic* body = PhysicsEngine::GetPhysics()->createRigidDynamic(physxTransform);
			body->setName(fmt::to_string(m_Entity.GetUUID()).c_str()); // we can easily rigidBodyComponent After collsion
			body->setMass(rigidBodyComponent.Mass);
			body->setAngularDamping(rigidBodyComponent.AngularDrag);
			body->setLinearDamping(rigidBodyComponent.LinearDrag);
			body->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, Math::InverseBool(rigidBodyComponent.Gravity));
			body->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, rigidBodyComponent.Kinematic);
			body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_X, rigidBodyComponent.FreezeLocation.X);
			body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Y, rigidBodyComponent.FreezeLocation.Y);
			body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, rigidBodyComponent.FreezeLocation.Z);

			body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, rigidBodyComponent.FreezeRotation.X);
			body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, rigidBodyComponent.FreezeRotation.Y);
			body->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, rigidBodyComponent.FreezeRotation.Z);


			//body->setSleepThreshold();
			m_RigidActor = body;

			const PhysicsSettings& settings = PhysicsEngine::GetSettings();
			//m_RigidActor->is<physx::PxRigidDynamic>()->setRigidDynamicLockFlags((physx::PxRigidDynamicLockFlags)m_LockFlags);
			m_RigidActor->is<physx::PxRigidDynamic>()->setSolverIterationCounts(settings.SolverIterations, settings.SolverVelocityIterations);
			m_RigidActor->is<physx::PxRigidDynamic>()->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_CCD, rigidBodyComponent.CollisionDetection == CollisionDetectionType::Continuous);
			m_RigidActor->is<physx::PxRigidDynamic>()->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_SPECULATIVE_CCD, rigidBodyComponent.CollisionDetection == CollisionDetectionType::ContinuousSpeculative);
			m_RigidActor->is<physx::PxRigidDynamic>()->setSleepThreshold(settings.SleepThreshold);
		}
		else
		{
			m_RigidActor = PhysicsEngine::GetPhysics()->createRigidStatic(physxTransform);
		}
		m_RigidActor->userData = this;

		m_PhysicsWorld->GetPhysicsScene()->addActor(*m_RigidActor);
	}

	void PhysicsActor::SyncTransform()
	{
		TransformComponent& transform = m_Entity.Transform();
		glm::vec3 scale = transform.Scale;
		physx::PxTransform actorPose = m_RigidActor->getGlobalPose();
		transform.Location = PhysXUtils::FromPhysXVector(actorPose.p);
		if (!IsAllRotationLocked())
			transform.SetRotation(PhysXUtils::FromPhysXQuat(actorPose.q));

		auto scene = m_Entity.GetCurrentWorld();
		scene->ConvertToLocalSpace(m_Entity);
		transform.Scale = scale;
	}
}
