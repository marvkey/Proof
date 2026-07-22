#include "Proofprch.h"
#include "ThumbnailManager.h"

#include "Proof/Asset/AssetManager.h"
#include "Proof/ImGui/Editors/EditorResources.h"
#include "Proof/Project/Project.h"
#include "Proof/Renderer/Image.h"
#include "Proof/Renderer/Renderer.h"
#include "Proof/Renderer/Texture.h"
#include "Proof/Renderer/WorldRenderer.h"
#include "Proof/Scene/World.h"
#include "Proof/Scene/Entity.h"
#include "Proof/Scene/Material.h"
#include "Proof/Scene/Mesh.h"
#include "Proof/Utils/ContainerUtils.h"
#include"../vendor/stb_image_write.h"

namespace Proof
{
    static const std::vector<AssetType> SupportedThumbnailTypes = { AssetType::Mesh, AssetType::Material, AssetType::DynamicMesh, AssetType::Texture };

    bool IsThumbnailOutdated(const std::filesystem::path& assetPath, const std::filesystem::path& thumbnailPath) 
    {
        std::error_code error;

        if (!std::filesystem::exists(assetPath, error) || error)
            return true;

        if (!std::filesystem::exists(thumbnailPath, error) || error)
            return true;

        auto assetWriteTime = std::filesystem::last_write_time(assetPath, error);

        if (error)
            return true;

        auto thumbnailWriteTime = std::filesystem::last_write_time(thumbnailPath, error);

        if (error)
            return true;

        return assetWriteTime > thumbnailWriteTime;
    }


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

        AssetType type = AssetManager::GetAssetInfo(assetID).Type;

        switch (type)
        {
            case AssetType::Mesh:
                GenerateMeshThumbnail(AssetManager::GetAsset<Mesh>(assetID));
            break;

            case AssetType::Material:
                GenerateMaterialThumbnail(AssetManager::GetAsset<Material>(assetID));
            break;

            case AssetType::DynamicMesh:
                GenerateDynamicMeshThumbnail(AssetManager::GetAsset<DynamicMesh>(assetID));
            break;

            case AssetType::Texture:
            // skikp

                //GenerateTextureThumbnail(AssetManager::GetAsset<Texture2D>(assetID));
            break;

            default:
                PF_CORE_ASSERT(false);
            break;

        }
        // Set up or replace the mesh/material/asset preview entity here.
        // Do not call OnRenderEditor here.
    }

    Count<Texture2D> AssetThumbnailRenderer::GetLastThumbnail()
    {
        PF_PROFILE_FUNC();

        if (m_LastThumbnailID == 0 || !AssetManager::HasAsset(m_LastThumbnailID))
            return EditorResources::FileIcon;

        if (AssetManager::GetAssetInfo(m_LastThumbnailID).Type == AssetType::Texture)
        {
            TextureConfiguration configuration;
            configuration.DebugName = "Texture Thumbnail";
            configuration.Format = ImageFormat::RGBA;
            configuration.Width = (uint32_t)m_ThumbnailSize;
            configuration.Height = (uint32_t)m_ThumbnailSize;

            return Texture2D::Create(configuration);
        }


        ImageConfiguration imageConfiguration;
        imageConfiguration.DebugName = AssetManager::GetAssetInfo(m_LastThumbnailID).GetName() + " Thumbnail Image";
        imageConfiguration.Transfer = true;
        imageConfiguration.Format = ImageFormat::RGBA;
        imageConfiguration.Usage = ImageUsage::Texture;
        imageConfiguration.Width = (uint32_t)m_ThumbnailSize;
        imageConfiguration.Height = (uint32_t)m_ThumbnailSize;

        Count<Image2D> thumbnailImage = Image2D::Create(imageConfiguration);

        TextureConfiguration configuration;
        configuration.DebugName = AssetManager::GetAssetInfo(m_LastThumbnailID).GetName() + " Thumbnail Image";
        configuration.Format = ImageFormat::RGBA;
        configuration.Width = (uint32_t)m_ThumbnailSize;
        configuration.Height = (uint32_t)m_ThumbnailSize;
        Count<Texture2D> thumbnail = Texture2D::Create(configuration,thumbnailImage,false);

        Renderer::BlitImage(Renderer::GetRendererCommandBuffer(), m_Renderer->GetFinalPassImage(), thumbnail->GetImage());

      //  Renderer::BlitImage(Renderer::GetRendererCommandBuffer(), m_Renderer->GetFinalPassImage(), thumbnail->GetImage());


        return thumbnail;
    }

    AssetID AssetThumbnailRenderer::GetLastThumbnailID() const
    {
        return m_LastThumbnailID;
    }

    void AssetThumbnailRenderer::GenerateMeshThumbnail(Count<class Mesh> mesh)
    {
        Entity ent = m_World->TryGetEntityByTag("Mesh");
        m_World->DeleteEntity(ent);
        ent = m_World->CreateEntity("Mesh");

        ent.AddComponent<MeshComponent>().SetMesh(mesh->GetID());

        AABB boudingBox = mesh->GetBoundingBox();
        FrameCameraToBounds(boudingBox);
                                                  
    }

    void AssetThumbnailRenderer::GenerateDynamicMeshThumbnail(Count<class DynamicMesh> mesh)
    {
        Entity ent = m_World->TryGetEntityByTag("Mesh");
        m_World->DeleteEntity(ent);
        ent = m_World->CreateEntity("Mesh");

        ent.AddComponent<DynamicMeshComponent>().SetMesh(mesh->GetID());
        AABB boudingBox = mesh->GetBoundingBox();

        FrameCameraToBounds(boudingBox);
    }

    void AssetThumbnailRenderer::GenerateMaterialThumbnail(Count<class Material> material)
    {
        Entity ent = m_World->TryGetEntityByTag("Mesh");
        m_World->DeleteEntity(ent);
        ent = m_World->CreateEntity("Mesh");

        ent.AddComponent<MeshComponent>().SetMesh((uint64_t)DefaultRuntimeAssets::Sphere);

        auto mesh = ent.GetComponent<MeshComponent>().GetMesh();
        ent.GetComponent<MeshComponent>().MaterialTable->SetMaterial(0, material);

        AABB boudingBox = mesh->GetBoundingBox();
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

        // The blit was submitted during the previous frame.
        // It is now safe to read the RGBA8 thumbnail and save it.
        if (m_PendingCacheAssetID != 0)
        {
            AssetID assetID = m_PendingCacheAssetID;

            if (AssetManager::HasAsset(assetID) && m_PendingCacheTexture)
            {
                AssetThumbnail& thumbnail = m_AssetThumbnails[assetID];
                thumbnail.AssetID = assetID;
                thumbnail.Texture = m_PendingCacheTexture;

                if (thumbnail.Status != AssetThumbnailStatus::Queued)
                    thumbnail.Status = AssetThumbnailStatus::Ready;

               CacheThumbnail (assetID, m_PendingCacheTexture);
            }
            else
            {
                m_AssetThumbnails.erase(assetID);
            }

            m_PendingCacheAssetID = 0;
            m_PendingCacheTexture = nullptr;
        }

        // The scene render for this asset completed during the previous frame.
        // Submit the RGBA32F -> RGBA8 blit, but do not read it yet.
        if (m_CurrentThumbnail.AssetID != 0 && m_ThumbnailRenderer->GetLastThumbnailID() == m_CurrentThumbnail.AssetID)
        {
            AssetID completedAssetID = m_CurrentThumbnail.AssetID;

            if (AssetManager::HasAsset(completedAssetID))
            {
                Count<Texture2D> texture = m_ThumbnailRenderer->GetLastThumbnail();

                if (texture)
                {
                    m_PendingCacheAssetID = completedAssetID;
                    m_PendingCacheTexture = texture;
                }
            }
            else
            {
                m_AssetThumbnails.erase(completedAssetID);
            }

            m_CurrentThumbnail = {};
        }

        // Configure exactly one new asset for this frame's thumbnail render.
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

    void AssetThumbnailManager::CacheThumbnail(AssetID thumbnailAssetID, Count<Texture2D> thumbnail)
    {
        PF_PROFILE_FUNC();

        if (!thumbnail || !AssetManager::HasAsset(thumbnailAssetID))
            return;

        const AssetInfo& assetInfo = AssetManager::GetAssetInfo(thumbnailAssetID);
        std::filesystem::path directory = Project::GetActive()->GetCacheDirectory() / "AssetThumbnails" / EnumReflection::EnumString(assetInfo.Type);

        std::error_code error;
        std::filesystem::create_directories(directory, error);

        if (error)
        {
            PF_ENGINE_ERROR("Failed to create thumbnail cache directory: {}", directory.string());
            return;
        }

        std::filesystem::path thumbnailPath = directory / (std::to_string((uint64_t)thumbnailAssetID) + ".png");
        const TextureConfiguration& configuration = thumbnail->GetSpecification();

        PF_CORE_ASSERT(configuration.Format == ImageFormat::RGBA, "Thumbnail cache only supports RGBA8");

        uint32_t width = configuration.Width;
        uint32_t height = configuration.Height;
        Buffer pixels = thumbnail->GetImage()->GetStoredDataAsBuffer();
        size_t expectedSize = (size_t)width * (size_t)height * 4;

        if (!pixels.Data || pixels.Size < expectedSize)
        {
            PF_ENGINE_ERROR("Unable to read thumbnail pixels for asset: {}", thumbnailAssetID);
            return;
        }

        stbi_flip_vertically_on_write(1);

        int result = stbi_write_png(thumbnailPath.string().c_str(), (int)width, (int)height, 4, pixels.Data, (int)width * 4);

        if (result == 0)
        {
            PF_ENGINE_ERROR("Failed to cache thumbnail: {}", thumbnailPath.string());
            return;
        }

        PF_ENGINE_TRACE("Cached thumbnail: {}", thumbnailPath.string());

        pixels.Release();
    }

    Count<Texture2D> AssetThumbnailManager::LoadCachedThumbnail(AssetID assetID,
        const std::filesystem::path& thumbnailPath)
    {
        PF_PROFILE_FUNC();

        if (!std::filesystem::exists(thumbnailPath))
            return nullptr;

        TextureConfiguration configuration;
        configuration.DebugName = AssetManager::GetAssetInfo(assetID).GetName() + " Cached Thumbnail";
        configuration.Format = ImageFormat::RGBA;

        Count<Texture2D> thumbnail = Texture2D::Create(configuration,thumbnailPath);

        if (!thumbnail)
            PF_ENGINE_ERROR("Failed to load cached thumbnail: {}", thumbnailPath.string());

        return thumbnail;
    }

    std::filesystem::path AssetThumbnailManager::GetThumbnailCachePath(AssetID assetID) const
    {
        PF_CORE_ASSERT(AssetManager::HasAsset(assetID));

        const AssetInfo& assetInfo = AssetManager::GetAssetInfo(assetID);

        return Project::GetActive()->GetCacheDirectory() /
            "AssetThumbnails" /
            EnumReflection::EnumString(assetInfo.Type) /
            (std::to_string((uint64_t)assetID) + ".png");
    }

    Count<Texture2D> AssetThumbnailManager::GetThumbnail(AssetID assetID)
    {
       
        PF_PROFILE_FUNC();
        PF_CORE_ASSERT(AssetManager::HasAsset(assetID));

        const AssetInfo& assetInfo = AssetManager::GetAssetInfo(assetID);

        if (!Utils::Contains(SupportedThumbnailTypes, assetInfo.Type))
            return EditorResources::FileIcon;

        if (assetInfo.Type == AssetType::Texture)
        {
            if (AssetManager::IsAssetLoaded(assetID))
                return AssetManager::GetAsset<Texture2D>(assetID);

            return EditorResources::FileIcon;
        }

        std::filesystem::path assetPath = AssetManager::GetAssetFileSystemPath(assetInfo.Path);
        std::filesystem::path thumbnailPath = GetThumbnailCachePath(assetID);
        bool outdated = IsThumbnailOutdated(assetPath, thumbnailPath);

        auto iterator = m_AssetThumbnails.find(assetID);

        if (iterator != m_AssetThumbnails.end())
        {
            AssetThumbnail& thumbnail = iterator->second;

            if (outdated &&
                thumbnail.Status != AssetThumbnailStatus::Queued &&
                thumbnail.Status != AssetThumbnailStatus::Rendering)
            {
                thumbnail.Status = AssetThumbnailStatus::Queued;
                m_ThumbnailQueue.push(assetID);
            }

            if (thumbnail.Texture)
                return thumbnail.Texture;

            return EditorResources::FileIcon;
        }

        AssetThumbnail thumbnail;
        thumbnail.AssetID = assetID;

        if (!outdated)
        {
            Count<Texture2D> cachedThumbnail = LoadCachedThumbnail(assetID, thumbnailPath);

            if (cachedThumbnail)
            {
                thumbnail.Texture = cachedThumbnail;
                thumbnail.Status = AssetThumbnailStatus::Ready;

                m_AssetThumbnails.emplace(assetID, thumbnail);

                return cachedThumbnail;
            }
        }

        thumbnail.Status = AssetThumbnailStatus::Queued;

        m_AssetThumbnails.emplace(assetID, thumbnail);
        m_ThumbnailQueue.push(assetID);

        return EditorResources::FileIcon;
    }
}