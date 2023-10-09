#pragma once
#include "Proof/Math/Vector.h"
#include "Proof/Math/Math.h"
#include <glm/glm.hpp>

#include <PxPhysicsAPI.h>
#include "CollisonCallback.h"
#include "PhysicsMeshCooker.h"
#include "PhysicsTypes.h"
//solve objexts goign to fast for collisoin
//https://www.youtube.com/watch?v=XvrFQJ3n8Mo
namespace Proof {
	struct PhysicsSettings;
	class PhysicsEngine {
	public:
		static physx::PxFoundation* GetFoundation();
		static physx::PxPhysics* GetPhysics();
		static physx::PxDefaultCpuDispatcher* GetCpuDispatcher();
		static PhysicsSettings& GetSettings() { return s_Settings; }
	private:
		static inline PhysicsSettings s_Settings;
		static void Init(PhysicsSettings settings = PhysicsSettings());
		static void Release();
		friend class Application;
	};

	
}