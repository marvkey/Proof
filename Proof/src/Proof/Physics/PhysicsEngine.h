#pragma once
#include "Proof/Math/Vector.h"
#include "Proof/Math/Math.h"
#include <glm/glm.hpp>

#define PX_PHYSX_STATIC_LIB
#include <PxPhysicsAPI.h>
#include "CollisonCallback.h"
#include "PhysicsMeshCooker.h"
//solve objexts goign to fast for collisoin
//https://www.youtube.com/watch?v=XvrFQJ3n8Mo
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