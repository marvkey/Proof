#pragma once
#include "Proof/Core/Core.h"
#include "PhysicsActorBase.h"
#include "PhysicsUtils.h"

namespace Proof
{
	class Entity;
	class PhysicsActor : public PhysicsActorBase
	{
	public:
		PhysicsActor(Count<class PhysicsWorld> world, Entity entity);
		virtual ~PhysicsActor();
		float GetInverseMass() const;
		bool IsDynamic()const;
		void AddForce(glm::vec3 force, ForceMode mode = ForceMode::Force, bool autoWake = true);
		void AddTorque(glm::vec3 force, ForceMode mode = ForceMode::Force, bool autoWake = true);
		void PutToSleep();
		void WakeUp();
		void SetRigidType(RigidBodyType type);

		virtual glm::vec3 GetLocation() const { return PhysXUtils::FromPhysXVector(m_RigidActor->getGlobalPose().p); }
		virtual void SetLocation(const glm::vec3& translation, const bool autowake = true);

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

		bool IsKinematic() const 
		{
			RigidBodyComponent& rigidBody = m_Entity.GetComponent<RigidBodyComponent>();
			return IsDynamic() && rigidBody.Kinematic;
		}
		void SetKinematic(bool isKinematic);

		void SetLockLocation(const VectorTemplate<bool>& location);
		void SetLockRotaion(const VectorTemplate<bool>& rotation);

		VectorTemplate<bool> GetLockRotation()const { return m_Entity.GetComponent<RigidBodyComponent>().FreezeRotation; }
		bool IsAllRotationLocked() const 
		{
			auto freezeRotation = GetLockRotation();
			if (freezeRotation.X == true && freezeRotation.Y  == true && freezeRotation.Z == true)return true;
			return false;
		}
		virtual bool IsGravityEnabled() const 
		{ 
			RigidBodyComponent& rigidBody = m_Entity.GetComponent<RigidBodyComponent>();
			return rigidBody.Gravity;
		}
		virtual void SetGravityEnabled(const bool state);

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
		physx::PxRigidActor* m_RigidActor = nullptr;
		friend class PhysicsWorld;
		friend class PhysxShapesInternal;
		std::vector<Count<class ColliderShape>> m_Colliders;
	private:

		void AddRigidBody();
		void Release();
		void Build();
	};
}
