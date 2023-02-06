#pragma once
#include "../../ProofPhysics/src/PhysicsWorld.h"
#include "../World.h"
#include "../Component.h"

#define PX_PHYSX_STATIC_LIB
#include <PxPhysicsAPI.h>
#include "CollisonCallback.h"
#include "PhysicsMeshCooker.h"

namespace Proof {
	class PhysicsEngine {
	public:
		static physx::PxFoundation* GetFoundation();
		static physx::PxPhysics* GetPhysics();
		static physx::PxPvd* GetPVD();
		static physx::PxDefaultCpuDispatcher* GetCpuDispatcher();
	private:
		static void Init();
		static void Release();
		friend class Application;
	};
}