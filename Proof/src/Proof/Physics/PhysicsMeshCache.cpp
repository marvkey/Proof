#include "Proofprch.h"
#include "PhysicsMeshCache.h"
#include <map>
#include "Proof/Scene/Mesh.h"
#include "Proof/Renderer/MeshWorkShop.h"
#include "Proof/Asset/AssetManager.h"
#include "MeshCollider.h"
#include "Proof/Core/Application.h"
#include "Proof/Project/Project.h"
namespace Proof
{
    namespace Utils {

        static std::filesystem::path GetCacheDirectory()
        {
            return Application::Get()->GetProject()->GetCacheDirectory() / "PhysicsColliders";
        }

        static void CreateCacheDirectoryIfNeeded()
        {
            std::filesystem::path cacheDirectory = GetCacheDirectory();
            if (!std::filesystem::exists(cacheDirectory))
                std::filesystem::create_directories(cacheDirectory);
        }
    }
    struct PhysicsMeshCacheData
    {
        Count<Mesh> BoxCache = nullptr;
        Count<Mesh> CapsuleCache = nullptr;
        Count<Mesh> SphereCache = nullptr;

        std::map<AssetID, std::map<AssetID, CachedColliderData>> MeshData;// collision mesh, collider ID, collider data

        // Editor-only
        std::map<AssetID, std::map<AssetID, Count<Mesh>>> DebugMeshes;
        std::map<AssetID, std::map<AssetID, Count<DynamicMesh>>> DebugDynamicMeshes;
    };
    PhysicsMeshCacheData* s_PhysicsMeshCacheData = nullptr;
    void PhysicsMeshCache::Init()
    {
        s_PhysicsMeshCacheData = new PhysicsMeshCacheData();
    }
    void PhysicsMeshCache::ShutDown()
    {
        delete s_PhysicsMeshCacheData;
    }
    Count<class Mesh> PhysicsMeshCache::GetBoxColliderMesh()
    {
        if (!s_PhysicsMeshCacheData->BoxCache)
        {
            s_PhysicsMeshCacheData->BoxCache = MeshWorkShop::GenerateCube(glm::vec3{ 0.5 });
            AssetManager::CreateRuntimeAsset(s_PhysicsMeshCacheData->BoxCache, "PhysicsBoxCache");
        }
        return s_PhysicsMeshCacheData->BoxCache;
    }
    Count<class Mesh> PhysicsMeshCache::GetCapsuleColliderMesh()
    {
        if (!s_PhysicsMeshCacheData->CapsuleCache)
        {
            s_PhysicsMeshCacheData->CapsuleCache = MeshWorkShop::GenerateCapsule();
            AssetManager::CreateRuntimeAsset(s_PhysicsMeshCacheData->CapsuleCache, "PhysicsCapsuleCache");
        }
        return s_PhysicsMeshCacheData->CapsuleCache;
    }
    Count<class Mesh> PhysicsMeshCache::GetSphereColliderMesh()
    {
        if (!s_PhysicsMeshCacheData->SphereCache)
        {
            s_PhysicsMeshCacheData->SphereCache = MeshWorkShop::GenerateSphere(0.5f);
            AssetManager::CreateRuntimeAsset(s_PhysicsMeshCacheData->SphereCache, "PhysicsSphereCache");
        }
        return s_PhysicsMeshCacheData->SphereCache;
    }

    const CachedColliderData& PhysicsMeshCache::GetMeshData(const Count<class MeshCollider>& colliderAsset)
    {
        AssetID collisionMesh = colliderAsset->ColliderMesh;

        auto& meshData = s_PhysicsMeshCacheData->MeshData;

        if (meshData.find(collisionMesh) != meshData.end())
        {
            const auto& meshDataMap = meshData.at(collisionMesh);
            if (meshDataMap.find(colliderAsset->GetID()) != meshDataMap.end())
                return meshDataMap.at(colliderAsset->GetID());

            return meshDataMap.at(0);
        }

        // Create/load collision mesh
        auto [simpleMeshResult, complexMeshResult] = PhysicsMeshCooker::CookMesh(colliderAsset);
        PF_CORE_ASSERT(meshData.find(collisionMesh) != meshData.end());
        const auto& meshDataMap = meshData.at(collisionMesh);

        PF_CORE_ASSERT(meshDataMap.find(colliderAsset->GetID()) != meshDataMap.end());
        return meshDataMap.at(colliderAsset->GetID());
    }

    Count<Mesh> PhysicsMeshCache::GetDebugMesh(const Count<class MeshCollider>& colliderAsset)
    {
        AssetID collisionMesh = colliderAsset->ColliderMesh;

        if (s_PhysicsMeshCacheData->DebugMeshes.find(collisionMesh) != s_PhysicsMeshCacheData->DebugMeshes.end())
        {
            const auto& debugMeshes = s_PhysicsMeshCacheData->DebugMeshes.at(collisionMesh);

            if (debugMeshes.find(colliderAsset->GetID()) != debugMeshes.end())
                return debugMeshes.at(colliderAsset->GetID());

            if (debugMeshes.find(0) == debugMeshes.end())
                return nullptr;

            return debugMeshes.at(0);
        }

        return nullptr;
    }

    Count<class DynamicMesh> PhysicsMeshCache::GetDebugDynamicMesh(const Count<class MeshCollider>& colliderAsset)
    {
        AssetID collisionMesh = colliderAsset->ColliderMesh;

        if (s_PhysicsMeshCacheData->DebugDynamicMeshes.find(collisionMesh) != s_PhysicsMeshCacheData->DebugDynamicMeshes.end())
        {
            const auto& debugMeshes = s_PhysicsMeshCacheData->DebugDynamicMeshes.at(collisionMesh);

            if (debugMeshes.find(colliderAsset->GetID()) != debugMeshes.end())
                return debugMeshes.at(colliderAsset->GetID());

            if (debugMeshes.find(0) == debugMeshes.end())
                return nullptr;

            return debugMeshes.at(0);
        }

        return nullptr;
    }

    bool PhysicsMeshCache::Exists(const Count<class MeshCollider>& colliderAsset) 
    {
        AssetID collisionMesh = colliderAsset->ColliderMesh;

        if (s_PhysicsMeshCacheData->MeshData.find(collisionMesh) == s_PhysicsMeshCacheData->MeshData.end())
            return false;

        const auto& meshDataMap = s_PhysicsMeshCacheData->MeshData.at(collisionMesh);
        if (AssetManager::HasAsset(colliderAsset->GetID()) && AssetManager::GetAssetInfo(colliderAsset->GetID()).RuntimeAsset == true)
            return meshDataMap.find(0) != meshDataMap.end();

        return meshDataMap.find(colliderAsset->GetID()) != meshDataMap.end();
    }

    void PhysicsMeshCache::Rebuild()
    {
        PF_ENGINE_INFO("Physics Rebuilding collider cache");

        if (!FileSystem::DeleteFile(Utils::GetCacheDirectory()))
        {
            PF_ENGINE_INFO("Physics", "Failed to delete collider cache!");
            return;
        }

        Clear();

        auto& allMeshColliders = AssetManager::GetAllAssetType(AssetType::MeshCollider);

        for (AssetID id : allMeshColliders)
        {
            auto [simpleMeshResult, complexMeshResult] = PhysicsMeshCooker::CookMesh(id, true);
            if (simpleMeshResult != CookingResult::Success)
                PF_ENGINE_ERROR("Physics Failed to cook simple collider for '{0}'", id);

            if (complexMeshResult != CookingResult::Success)
                PF_ENGINE_ERROR("Physics Failed to cook complex collider for '{0}'", id);
        }

        PF_ENGINE_INFO("Physics Finished rebuilding collider cache");
    }

    void PhysicsMeshCache::Clear()
    {
        s_PhysicsMeshCacheData->MeshData.clear();
        s_PhysicsMeshCacheData->DebugMeshes.clear();
        s_PhysicsMeshCacheData->DebugDynamicMeshes.clear();
    }

    const std::map<AssetID, std::map<AssetID, CachedColliderData>>& PhysicsMeshCache::GetMeshData()
    {
        return s_PhysicsMeshCacheData->MeshData;
    }

    void PhysicsMeshCache::AddDebugMesh(const Count<class MeshCollider>& colliderAsset, const Count<class Mesh>& debugMesh)
    {
        if (AssetManager::HasAsset(colliderAsset->GetID()) && AssetManager::GetAssetInfo(colliderAsset->GetID()).RuntimeAsset == true)
            s_PhysicsMeshCacheData->DebugMeshes[colliderAsset->ColliderMesh][0] = debugMesh;
        else
            s_PhysicsMeshCacheData->DebugMeshes[colliderAsset->ColliderMesh][colliderAsset->GetID()] = debugMesh;
    }

    void PhysicsMeshCache::AddDebugMesh(const Count<class MeshCollider>& colliderAsset, const Count<class DynamicMesh>& debugMesh)
    {
        if (AssetManager::HasAsset(colliderAsset->GetID()) && AssetManager::GetAssetInfo(colliderAsset->GetID()).RuntimeAsset == true)
            s_PhysicsMeshCacheData->DebugDynamicMeshes[colliderAsset->ColliderMesh][0] = debugMesh;
        else
            s_PhysicsMeshCacheData->DebugDynamicMeshes[colliderAsset->ColliderMesh][colliderAsset->GetID()] = debugMesh;
    }
    std::map<AssetID, std::map<AssetID, CachedColliderData>>& PhysicsMeshCache::GetMeshDataRef()
    {
        return s_PhysicsMeshCacheData->MeshData;
    }

}

