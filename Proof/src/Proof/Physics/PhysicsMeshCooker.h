#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Core/Buffer.h"
#include "PhysicsUtils.h"
#include "Proof/Asset/AssetTypes.h"
namespace Proof
{
	enum class MeshColliderType : uint8_t { None = 0, Triangle = 1, Convex = 2 };
	struct SubMeshColliderData
	{
		Buffer ColliderData;
		glm::mat4 Transform;
	};

	struct MeshColliderData
	{
		std::vector<SubMeshColliderData> SubMeshes;
		MeshColliderType Type;
	};

	class PhysicsMeshCooker
	{
	public:
		//invalidate old if its enabeld even if collider exist in cache it will be regenerated
		static std::pair<CookingResult, CookingResult> CookMesh(AssetID colliderHandle, bool invalidateOld = false);
		//invalidate old if its enabeld even if collider exist in cache it will be regenerated
		static std::pair<CookingResult, CookingResult> CookMesh(Count<class MeshCollider> colliderAsset, bool invalidateOld = false);
	private:
		static void Init();
		static void ShutDown();

		static bool SerializeMeshCollider(const std::filesystem::path& filepath, MeshColliderData& meshData);
		static MeshColliderData DeserializeMeshCollider(const std::filesystem::path& filepath);

		static CookingResult CookConvexMesh(const Count<class MeshCollider>& collider, const Count<class MeshSource>& meshSource, const std::vector<uint32_t>& submeshIndices, MeshColliderData& outData);
		static CookingResult CookTriangleMesh(const Count<MeshCollider>& colliderAsset, const Count<class MeshSource>& meshSource, const std::vector<uint32_t>& submeshIndices, MeshColliderData& outData);

		static void GenerateDebugMesh(const Count<MeshCollider>& colliderAsset, const MeshColliderData& colliderData);
		friend class PhysicsEngine;
	};
}
