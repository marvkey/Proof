#pragma once
#include "Proof/Core/Core.h"
#include "PhysicsUtils.h"

#include "Proof/Scene/Entity.h"
namespace Proof{
	class Entity;
	class PhysicsActor : public RefCounted {
	public:
		PhysicsActor(class PhysicsWorld* physicsWorld,Entity entity);
		virtual ~PhysicsActor();

		bool IsDynamic()const;
		void AddForce(glm::vec3 force, ForceMode mode = ForceMode::Force, bool autoWake = true);
		void AddTorque(glm::vec3 force, ForceMode mode = ForceMode::Force, bool autoWake = true);
		void PutToSleep();
		void WakeUp();
		void SetRigidType(RigidBodyType type);

		void ClearForce(ForceMode mode);
		void ClearTorque(ForceMode mode);

		bool IsSleeping();
		void SyncTransform();
		void OnFixedUpdate(float deltaTime);

		Entity GetEntity();

		void AddCollider(BoxColliderComponent& collider);
		void AddCollider(SphereColliderComponent& collider);
		void AddCollider(CapsuleColliderComponent& collider);
		void AddCollider(MeshColliderComponent& collider);
	
		glm::quat GetRotation() const { return PhysXUtils::FromPhysXQuat(m_RigidActor->getGlobalPose().q); }
		glm::vec3 GetRotationEuler() const { return glm::eulerAngles(GetRotation()); }

		void SetRotation(const glm::quat& rotation, bool autowake = true);
		void Rotate(const glm::quat& rotation, bool autowake = true);

		float GetMass() const;
		void SetMass(float mass);

		glm::vec3 GetLinearVelocity() const;
		void SetLinearVelocity(const glm::vec3& velocity);
		glm::vec3 GetAngularVelocity() const;
		void SetAngularVelocity(const glm::vec3& velocity);

		float GetMaxLinearVelocity() const;
		void SetMaxLinearVelocity(float maxVelocity);
		float GetMaxAngularVelocity() const;
		void SetMaxAngularVelocity(float maxVelocity);

		float GetLinearDrag() const;
		void SetLinearDrag(float drag);
		float GetAngularDrag() const;
		void SetAngularDrag(float drag);

		glm::vec3 GetKinematicTargetPosition() const;
		glm::quat GetKinematicTargetRotation() const;
		glm::vec3 GetKinematicTargetRotationEuler() const;
		void SetKinematicTarget(const glm::vec3& targetPosition, const glm::quat& targetRotation) const;

		bool IsKinematic() const { return IsDynamic() && m_RigidBody.Kinematic; }
		void SetKinematic(bool isKinematic);

		void SetLockLocation(const VectorTemplate<bool>& location);
		void SetLockRotaion(const VectorTemplate<bool>& rotation);

		virtual bool IsGravityEnabled() const { return m_RigidBody.Gravity; }
		virtual void SetGravityEnabled(const bool state) ;

		template<typename TShapeType>
		Count<TShapeType> GetCollider()
		{
			for (const auto& collider : m_Colliders)
			{
				if (collider->GetType() == TShapeType::GetStaticType())
					return collider;
			}

			return nullptr;
		}

		physx::PxRigidActor& GetPhysXActor() const { return *m_RigidActor; }
		const std::vector<Count<class ColliderShape>>& GetCollisionShapes() const { return m_Colliders; }
	private:
		class Entity m_Entity;
		physx::PxRigidActor* m_RigidActor = nullptr;
		RigidBodyComponent& m_RigidBody;
		class PhysicsWorld* m_PhysicsWorld =nullptr;

		friend class PhysicsWorld;
		friend class PhysxShapesInternal;

		std::vector<Count<class ColliderShape>> m_Colliders;
	private:

		void AddRigidBody();
		void Release();
		void Build();
	};
}
