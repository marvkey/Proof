#pragma once
#include "../../ProofPhysics/src/PhysicsWorld.h"
#include "../World.h"
#include "../Component.h"
#define PX_PHYSX_STATIC_LIB
#include <PxPhysicsAPI.h>
#include "CollisonCallback.h"
namespace Proof {

	class PhysicsEngine {
	public:
		PhysicsEngine(World* world):
			m_World(world){
			SetUpBase();
		}
		virtual ~PhysicsEngine();
		void Start();
		void Simulate(float delta);
		void End();
	private:
		void SetUpBase();
		class physx::PxFoundation* m_Foundation = nullptr;
		class physx::PxPhysics* m_Physics = nullptr;
		class physx::PxDefaultCpuDispatcher* m_Dispatcher = NULL;
		class physx::PxPvd* m_Pvd = nullptr;
		class physx::PxScene* m_Scene = nullptr;
		class physx::PxDefaultAllocator      m_DefaultAllocatorCallback;
		class physx::PxDefaultErrorCallback  m_DefaultErrorCallback;
		class physx::PxCooking* m_Cooking = nullptr;
		class physx::PxTolerancesScale m_ToleranceScale;
		class CollisionCallback m_CollisionCallback;
		World* m_World = nullptr;
		friend class World;
	};
}