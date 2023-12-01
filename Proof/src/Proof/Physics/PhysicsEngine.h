#pragma once
#include "Proof/Math/Vector.h"
#include "Proof/Math/Math.h"
#include <glm/glm.hpp>

#include <Physx/PxPhysicsAPI.h>
#include "CollisonCallback.h"
#include "PhysicsMeshCooker.h"
#include "PhysicsTypes.h"
//solve objexts goign to fast for collisoin
//https://www.youtube.com/watch?v=XvrFQJ3n8Mo
namespace Proof {
	class PhysicsEngine {
	public:
		static physx::PxFoundation* GetFoundation();
		static physx::PxPhysics* GetPhysics();
		static physx::PxDefaultCpuDispatcher* GetCpuDispatcher();
		static PhysicsSettings& GetSettings(); 
		static std::string GetLastErrorMessage(); 

		static Count<class MeshCollider> GetOrCreateColliderAsset(class  Entity entity, struct MeshColliderComponent& component);
	private:
		static void Init();
		static void Release();
		friend class Application;
	};

	
}