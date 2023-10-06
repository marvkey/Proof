#pragma once
#include "Proof/Core/Core.h"
#include "CollisonCallback.h"
#include <PxPhysicsAPI.h>

namespace Proof {
	class PhysicsWorld : public RefCounted
	{
	public:
		// begin world and end world is already called
		PhysicsWorld(Count<class World> world);
		virtual ~PhysicsWorld();
		void Simulate(float deltaTime);
		Count<World> GetWorld() 
		{
			return m_World;
		}

		physx::PxScene* GetPhysicsScene() {
			return m_Scene;
		}
		bool HasActor(class Entity entity);
		Count<class PhysicsActor> CreateActor(class Entity entity);
		Count<class PhysicsActor> GetActor(class Entity entity);
		void RemoveActor(class Entity entity);

		bool HasController(Entity entity);
		Count<class PhysicsController> CreateController(Entity entity);
		Count<class PhysicsController> GetController(Entity entity);
		void RemoveController(Entity entity);

		physx::PxControllerManager* GetPhysXControllerManager() const { return m_PhysXControllerManager; }
	private:
		void StartWorld();
		void EndWorld();

		bool Advance(float deltaTime);
		void CreateRegions();
	private:
		float m_Accumulator = 0.0f;
		// needs to be changed have a way to figure this out
		float m_SubStepSize = 1.0f / 60.0f;
		
		class physx::PxScene* m_Scene = nullptr;
		class physx::PxPvdTransport* m_Transport = nullptr;
		class CollisionCallback m_CollisionCallback;
		// inserting an element with this technique makes 
		// creates a copy nd uses desctrocture
		// so we would use Count so it is created once instead of twice
		// performance saved
		std::unordered_map<UUID, Count<class PhysicsActor>> m_Actors;
		std::unordered_map<UUID,Count<class PhysicsController>> m_Controllers;
		Count<class World> m_World;
		physx::PxBounds3* m_RegionBounds = nullptr;
		physx::PxControllerManager* m_PhysXControllerManager;
	};
}
