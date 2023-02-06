#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Scene/Entity.h"
namespace Proof{

	class PhysicsActor{
	public:
		PhysicsActor(class PhysicsWorld* physicsWorld,Entity entity);
		virtual ~PhysicsActor();
		void OnUpdate(float deltaTime);

		bool IsSleeping();
		void SyncTransform();
	private:
		enum class RigidBodyType m_RigidBodyType;
		class PhysicsWorld* m_PhysicsWorld =nullptr;
		Entity m_Entity;
		void* m_RuntimeBody;
		void AddRigidBody();
		void AddCubeCollider();
		void AddSphereCollider();
	};
}
