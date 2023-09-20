#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Scene/Entity.h"
namespace Proof{
	class Entity;
	class PhysicsActor : public RefCounted {
	public:
		PhysicsActor(class PhysicsWorld* physicsWorld,UUID entity);
		virtual ~PhysicsActor();

		bool IsDynamic();
		void AddForce(glm::vec3 force, ForceMode mode = ForceMode::Force, bool autoWake = true);
		void AddTorque(glm::vec3 force, ForceMode mode = ForceMode::Force, bool autoWake = true);
		void PutToSleep();
		void WakeUp();

		bool IsSleeping();
		void SyncTransform();
		void OnFixedUpdate(float deltaTime);

		void OnCollisonEnter(const PhysicsActor* actor);
		void OnCollisonStay(const PhysicsActor* actor);
		void OnCollisonLeave(const PhysicsActor* actor);

		void ClearForce(ForceMode mode = ForceMode::Force);
		void ClearTorque(ForceMode mode = ForceMode::Force);
		// actor enters a trigger box, current actor gets told actor has overlappe
		void OnTriggerEnter(const PhysicsActor* actor);
		void OnTriggerStay(const PhysicsActor* actor);
		void OnTriggerLeave(const PhysicsActor* actor);

		// current acotr overlaps another trigger box
		void OnOverlapTriggerEnter(const PhysicsActor* actor);
		void OnOverlapTriggerStay(const PhysicsActor* actor);
		void OnOverlapTriggerLeave(const PhysicsActor* actor);

		// veclocity in a straight line
		glm::vec3 GetLinearVelocity();
		// veclocity in an angle
		glm::vec3 GetAngularVelocity();

		void SetLinearVelocity(glm::vec3 velocity, bool wakeUp = true);
		void SetAngularVelocity(glm::vec3 velocity, bool wakeUp = true);

		
	private:
		// not sure how we gonna treat this yet
		enum class RigidBodyType m_RigidBodyType;
		class PhysicsWorld* m_PhysicsWorld =nullptr;
		class Entity m_Entity;
		void* m_RuntimeBody = nullptr;
		void* m_CubeColliderBody = nullptr;
		void* m_SphereColliderBody = nullptr;
		void* m_CapsuleColliderBody = nullptr;
		void* m_MeshColliderBody = nullptr;

		friend class PhysicsWorld;
		friend class PhysxShapesInternal;
	private:
		void AddRigidBody();
		void UpdateRigidBody(float deltatime);
		

		void Release();
		void Build();
	};
}
