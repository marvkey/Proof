#pragma once
#include "PhysicsMeshCooker.h"
#include <map>
namespace Proof
{
	struct CachedColliderData
	{
		MeshColliderData SimpleColliderData;
		MeshColliderData ComplexColliderData;
	};
	class PhysicsMeshCache
	{
	public:
		static Count<class Mesh> GetBoxColliderMesh();
		static Count<class Mesh> GetCapsuleColliderMesh();
		static Count<class Mesh> GetSphereColliderMesh();

		const CachedColliderData& GetMeshData(const Count<class MeshCollider>& colliderAsset);
		Count<Mesh> GetDebugMesh(const Count<class MeshCollider>& colliderAsset);
		Count<class DynamicMesh> GetDebugDynamicMesh(const Count<class MeshCollider>& colliderAsset);

		bool Exists(const Count<class MeshCollider>& colliderAsset) const;
		void Rebuild();
		void Clear();
		static const std::map<AssetID, std::map<AssetID, CachedColliderData>>& GetMeshData();
	private:
		static void AddDebugMesh(const Count<class MeshCollider>& colliderAsset, const Count<class Mesh>& debugMesh);
		static void AddDebugMesh(const Count<class MeshCollider>& colliderAsset, const Count<class DynamicMesh>& debugMesh);
		static std::map<AssetID, std::map<AssetID, CachedColliderData>>& GetMeshDataRef();
		static void Init();
		static void ShutDown();

		friend class PhysicsMeshCooker;
		friend class PhysicsEngine;
	};
}