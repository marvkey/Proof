#include "Proofprch.h"
#include "ThumbnailManager.h"

#include "Proof/Asset/AssetManager.h"
#include "Proof/ImGui/Editors/EditorResources.h"
#include "Proof/Renderer/Image.h"
#include "Proof/Renderer/Renderer.h"
#include "Proof/Renderer/Texture.h"
#include "Proof/Renderer/WorldRenderer.h"
#include "Proof/Scene/World.h"
#include "Proof/Scene/Entity.h"
#include "Proof/Scene/Material.h"
#include "Proof/Scene/Mesh.h"
#include "Proof/Utils/ContainerUtils.h"

namespace Proof
{
    static const std::vector<AssetType> SupportedThumbnailTypes = { AssetType::Mesh, AssetType::Material, AssetType::DynamicMesh, AssetType::Texture };

    AssetThumbnailRenderer::AssetThumbnailRenderer()
    {
        m_World = Count<World>::Create();
        m_Renderer = Count<WorldRenderer>::Create();

        m_Renderer->SetContext(m_World);

        Entity light = m_World->CreateEntity("Thumbnail Light");
        light.AddComponent<SkyLightComponent>();
        light.AddComponent<DirectionalLightComponent>();
        light.GetComponent<TransformComponent>().SetRotationEuler(glm::vec3(80.0f, 10.0f, 0.0f));

        Entity mesh = m_World->CreateEntity("Mesh");
        m_Camera.SetFOV(45.0f);
    }

    void AssetThumbnailRenderer::OnUpdate(FrameTime dt)
    {
        PF_PROFILE_FUNC();

        uint32_t size = (uint32_t) m_ThumbnailSize;
        m_Camera.SetViewportSize(size, size);
        m_Camera.SetActive(false);
        m_Camera.Recalculate();

		m_Renderer->SetViewportSize(size, size);

        m_World->OnUpdateEditor(dt);
        m_World->OnRenderEditor(m_Renderer, dt, m_Camera);

        // The render submitted this frame belongs to this asset.
        m_LastThumbnailID = m_CurrentThumbnailID;
    }

    void AssetThumbnailRenderer::GenerateThumbnail(AssetID assetID)
    {
        PF_PROFILE_FUNC();
        PF_CORE_ASSERT(AssetManager::HasAsset(assetID));

        m_CurrentThumbnailID = assetID;

        auto type = AssetManager::GetAssetInfo(assetID).Type;
        if (type == AssetType::Material)
            GenerateMaterialThumbnail(AssetManager::GetAsset<Material>(assetID)) ;

        // Set up or replace the mesh/material/asset preview entity here.
        // Do not call OnRenderEditor here.
    }

    Count<Texture2D> AssetThumbnailRenderer::GetLastThumbnail()
    {
        PF_PROFILE_FUNC();

        if (m_LastThumbnailID == 0 || !AssetManager::HasAsset(m_LastThumbnailID))
            return EditorResources::FileIcon;

        TextureConfiguration configuration;
        configuration.DebugName = AssetManager::GetAssetInfo(m_LastThumbnailID).GetName() + " Thumbnail";
        configuration.Format = ImageFormat::RGBA32F;
        configuration.Width = (uint32_t)m_ThumbnailSize;
        configuration.Height = (uint32_t)m_ThumbnailSize;

        return Texture2D::Create(configuration, m_Renderer->GetFinalPassImage());
    }

    AssetID AssetThumbnailRenderer::GetLastThumbnailID() const
    {
        return m_LastThumbnailID;
    }

    void AssetThumbnailRenderer::GenerateMeshThumbnail(Count<class Mesh> mesh)
    {
        Entity ent = m_World->TryGetEntityByTag("Mesh");

        ent.RemoveComponent<MeshComponent>();
        ent.RemoveComponent<DynamicMeshComponent>();
    }

    void AssetThumbnailRenderer::GenerateDynamicMeshThumbnail(Count<class DynamicMesh> mesh)
    {
        Entity ent = m_World->TryGetEntityByTag("Mesh");
        ent.RemoveComponent<MeshComponent>();
        ent.RemoveComponent<DynamicMeshComponent>();
    }

    void AssetThumbnailRenderer::GenerateMaterialThumbnail(Count<class Material> material)
    {
        Entity ent = m_World->TryGetEntityByTag("Mesh");
        ent.RemoveComponent<MeshComponent>();
        ent.RemoveComponent<DynamicMeshComponent>();

        ent.AddComponent<MeshComponent>().SetMesh((uint64_t)DefaultRuntimeAssets::Sphere);

        auto mesh = ent.GetComponent<MeshComponent>().GetMesh();
        ent.GetComponent<MeshComponent>().MaterialTable->SetMaterial(0, material);

        AABB boudingBox =  mesh->GetMeshSource()->GetBoundingBox();

        FrameCameraToBounds(boudingBox);


    }

    void AssetThumbnailRenderer::FrameCameraToBounds(const struct AABB bounds)
    {
        glm::vec3 center = (bounds.Min + bounds.Max) * 0.5f;

        std::array<glm::vec3, 8> corners =
        {
            glm::vec3(bounds.Min.x, bounds.Min.y, bounds.Min.z),
            glm::vec3(bounds.Max.x, bounds.Min.y, bounds.Min.z),
            glm::vec3(bounds.Min.x, bounds.Max.y, bounds.Min.z),
            glm::vec3(bounds.Max.x, bounds.Max.y, bounds.Min.z),
            glm::vec3(bounds.Min.x, bounds.Min.y, bounds.Max.z),
            glm::vec3(bounds.Max.x, bounds.Min.y, bounds.Max.z),
            glm::vec3(bounds.Min.x, bounds.Max.y, bounds.Max.z),
            glm::vec3(bounds.Max.x, bounds.Max.y, bounds.Max.z)
        };

        glm::vec3 forward = glm::normalize(glm::vec3(-1.0f, -0.45f, -1.0f));
        glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));
        glm::vec3 up = glm::normalize(glm::cross(right, forward));

        float verticalFOV = glm::radians(m_Camera.GetFOV());
        float aspectRatio = 1.0f;
        float tanVertical = glm::tan(verticalFOV * 0.5f);
        float tanHorizontal = tanVertical * aspectRatio;
        float distance = 0.0f;

        for (const glm::vec3& corner : corners)
        {
            glm::vec3 relativeCorner = corner - center;

            float x = glm::dot(relativeCorner, right);
            float y = glm::dot(relativeCorner, up);
            float z = glm::dot(relativeCorner, forward);

            float horizontalDistance = glm::abs(x) / tanHorizontal - z;
            float verticalDistance = glm::abs(y) / tanVertical - z;

            distance = glm::max(distance, glm::max(horizontalDistance, verticalDistance));
        }

        distance *= 0.60f;

        glm::vec3 cameraPosition = center - forward * distance;
        glm::vec3 cameraDirection = glm::normalize(center - cameraPosition);

        m_Camera.SetPosition(cameraPosition);
        m_Camera.SetDirection(cameraDirection);
        m_Camera.Recalculate();
    
    }

    AssetThumbnailManager::AssetThumbnailManager()
    {
        m_ThumbnailRenderer = Count<AssetThumbnailRenderer>::Create();
    }

    void AssetThumbnailManager::OnUpdate(FrameTime dt)
    {
        PF_PROFILE_FUNC();

        // Save the image rendered at the end of the previous frame.
        if (m_CurrentThumbnail.AssetID != 0)
        {
            AssetID completedAssetID = m_CurrentThumbnail.AssetID;

            if (!AssetManager::HasAsset(completedAssetID))
            {
                m_AssetThumbnails.erase(completedAssetID);
                m_CurrentThumbnail = {};
            }
            else if (m_ThumbnailRenderer->GetLastThumbnailID() == completedAssetID)
            {
                Count<Texture2D> texture = m_ThumbnailRenderer->GetLastThumbnail();
                AssetThumbnail& cachedThumbnail = m_AssetThumbnails[completedAssetID];

                cachedThumbnail.AssetID = completedAssetID;
                cachedThumbnail.Texture = texture;

                // If it was changed while rendering, keep it queued for another render.
                if (cachedThumbnail.Status != AssetThumbnailStatus::Queued)
                    cachedThumbnail.Status = AssetThumbnailStatus::Ready;

                m_CurrentThumbnail = {};
            }
        }

        // Select exactly one new asset before this frame's render submission.
        if (m_CurrentThumbnail.AssetID == 0 && !m_ThumbnailQueue.empty())
        {
            AssetID assetID = m_ThumbnailQueue.front();
            m_ThumbnailQueue.pop();

            if (!AssetManager::HasAsset(assetID))
            {
                m_AssetThumbnails.erase(assetID);
            }
            else
            {
                AssetThumbnail& thumbnail = m_AssetThumbnails[assetID];
                thumbnail.AssetID = assetID;
                thumbnail.Status = AssetThumbnailStatus::Rendering;

                m_CurrentThumbnail = thumbnail;
                m_ThumbnailRenderer->GenerateThumbnail(assetID);
            }
        }

        // This is the only thumbnail-world render submission this frame.
        m_ThumbnailRenderer->OnUpdate(dt);
    }

    void AssetThumbnailManager::RegenerateThumbnail(AssetID assetID)
    {
        PF_PROFILE_FUNC();
        PF_CORE_ASSERT(AssetManager::HasAsset(assetID));

        AssetThumbnail& thumbnail = m_AssetThumbnails[assetID];
        thumbnail.AssetID = assetID;

        if (thumbnail.Status == AssetThumbnailStatus::Queued)
            return;

        thumbnail.Status = AssetThumbnailStatus::Queued;
        m_ThumbnailQueue.push(assetID);
    }

    Count<Texture2D> AssetThumbnailManager::GetThumbnail(AssetID assetID)
    {
        PF_PROFILE_FUNC();
        PF_CORE_ASSERT(AssetManager::HasAsset(assetID));

        const auto& assetInfo = AssetManager::GetAssetInfo(assetID);

        if (!Utils::Contains(SupportedThumbnailTypes, assetInfo.Type))
            return EditorResources::FileIcon;

        if (assetInfo.Type == AssetType::Texture && AssetManager::IsAssetLoaded(assetID))
            return AssetManager::GetAsset<Texture2D>(assetID);

        auto iterator = m_AssetThumbnails.find(assetID);

        if (iterator == m_AssetThumbnails.end())
        {
            AssetThumbnail thumbnail;
            thumbnail.AssetID = assetID;
            thumbnail.Status = AssetThumbnailStatus::Queued;

            m_AssetThumbnails.emplace(assetID, thumbnail);
            m_ThumbnailQueue.push(assetID);

            return EditorResources::FileIcon;
        }

        AssetThumbnail& thumbnail = iterator->second;

        if (thumbnail.Texture)
            return thumbnail.Texture;

        return EditorResources::FileIcon;
    }
}