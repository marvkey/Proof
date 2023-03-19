#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Scene/Entity.h"
namespace Proof{

	class PhysicsActor{
	public:
		PhysicsActor(class PhysicsWorld* physicsWorld,Entity entity);
		virtual ~PhysicsActor();

		void AddForce(Vector force, ForceMode mode = ForceMode::Force, bool autoWake = true);
		void AddTorque(Vector force, ForceMode mode = ForceMode::Force, bool autoWake = true);
		void PutToSleep();
		void WakeUp();

		bool IsSleeping();
		void SyncTransform();
		void OnFixedUpdate(float deltaTime);


		void OnCollisonEnter(const PhysicsActor* actor);


		// actor enters a trigger box, current actor gets told actor has overlappe
		void OnTriggerEnter(const PhysicsActor* actor);

		// current acotr overlaps another trigger box
		void OnOverlapTriggerEnter(const PhysicsActor* actor);
	private:
		friend class PhysicsWorld;
		// not sure how we gonna treat this yet
		enum class RigidBodyType m_RigidBodyType;
		class PhysicsWorld* m_PhysicsWorld =nullptr;
		Entity m_Entity;
		void* m_RuntimeBody;
		void AddRigidBody();
		void AddCubeCollider();
		void AddSphereCollider();
		void AddCapsuleCollider();
		void AddMeshCollider();
	};
}
