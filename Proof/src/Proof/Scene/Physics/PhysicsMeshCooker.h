#pragma once
#include "Proof/Core/Core.h"
#include <PxPhysicsAPI.h>
#include "proof/Asset/AssetManager.h"
namespace Proof{

	class PhysicsMeshCooker {
	public:
		static bool HasMesh(AssetID ID);
		static void CookMesh(AssetID ID);
		static const Count<class Mesh> GetConvexMeshAsMesh(AssetID ID);
		static physx::PxConvexMesh* GetConvexMesh(AssetID ID);
		static void DeleteMesh(AssetID ID);
	private:
		static void Init();
		static void Release();
		friend class PhysicsEngine;
	};
}
