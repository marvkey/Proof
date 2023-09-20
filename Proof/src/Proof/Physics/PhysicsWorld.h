#pragma once
#include "Proof/Core/Core.h"
#include "PhysicsActor.h"
#include "CollisonCallback.h"
#include <PxPhysicsAPI.h>

namespace Proof {
	struct PhysicsWorldConfig {
		Vector Gravity = { 0.f,-9.81f,0.f };
		bool PvdClient = true;
	};
	class PhysicsWorld {
	public:
		// begin world and end world is already called
		PhysicsWorld(class World* world,const PhysicsWorldConfig& sceneConfig);
		virtual ~PhysicsWorld();
		void OnFixedUpdate(float deltaTime);
		World* GetWorld() {
			return m_World;
		}

		physx::PxScene* GetPhysicsScene() {
			return m_Scene;
		}
		bool HasActor(UUID id);

		Count<PhysicsActor> NewActor(UUID id);
		Count<PhysicsActor> GetActor(UUID id);
		Count<PhysicsActor> TryGetActor(UUID id);
		void RemoveActor(UUID id);
	private:
		void StartWorld();
		void EndWorld();

		bool Advance(float deltaTime);
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
		std::unordered_map<UUID,Count<PhysicsActor>> m_Actors;
		PhysicsWorldConfig m_Config;
		World* m_World;
	};
}
