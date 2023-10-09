#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Core/Buffer.h"
#include "PhysicsUtils.h"
#include "Proof/Asset/AssetTypes.h"
namespace Proof
{
	enum class MeshColliderType : uint8_t { None = 0, Triangle = 1, Convex = 2 };
	struct SubmeshColliderData
	{
		Buffer ColliderData;
		glm::mat4 Transform;
	};

	struct MeshColliderData
	{
		std::vector<SubmeshColliderData> Submeshes;
		MeshColliderType Type;
	};

	class PhysicsMeshCooker
	{
	public:
		static bool HasMesh(AssetID ID);
		static void CookMesh(AssetID ID);
		static const Count<class Mesh> GetConvexMeshAsMesh(AssetID ID);
		static physx::PxTriangleMesh* GetConvexMesh(AssetID ID);
		static void DeleteMesh(AssetID ID);

		static Count<class Mesh> GetBoxColliderMesh();
		static Count<class Mesh> GetCapsuleColliderMesh();
		static Count<class Mesh> GetSphereColliderMesh();
	private:
		static void Init();
		static void Release();

		static CookingResult CookConvexMesh(const Count<class MeshCollider>& collider, const Count<class MeshSource>& meshSource, const std::vector<uint32_t>& submeshIndices, MeshColliderData& outData);
		static CookingResult CookTriangleMesh(const Count<MeshCollider>& colliderAsset, const Count<class MeshSource>& meshSource, const std::vector<uint32_t>& submeshIndices, MeshColliderData& outData);
		friend class PhysicsEngine;
	};
}
