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

#include "../vendor/stb_image_write.h"
#include "Proof/Renderer/AssetThumbnailGenerator.h"

namespace Proof
{
    static const std::vector<AssetType> SupportedThumbnailTypes =
    {
        AssetType::Mesh,
        AssetType::Material,
        AssetType::DynamicMesh,
        AssetType::Texture,
        AssetType::MeshSourceFile,
    };

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

        m_World->CreateEntity("Mesh");
        m_Camera.SetFOV(45.0f);
    }

    Count<Texture2D> AssetThumbnailRenderer::GenerateThumbnail(AssetID assetID, FrameTime dt)
    {
        PF_PROFILE_FUNC();
        PF_CORE_ASSERT(AssetManager::HasAsset(assetID));
        uint32_t size = (uint32_t)m_ThumbnailSize;

        m_Camera.SetViewportSize(size, size);
        m_Camera.SetActive(false);
        m_Camera.Recalculate();

        m_Renderer->SetViewportSize(size, size);


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
                 {
                     TextureConfiguration textureConfiguration;
                     textureConfiguration.DebugName = AssetManager::GetAssetInfo(assetID).GetName() + " Thumbnail";
                     textureConfiguration.Format = ImageFormat::RGBA;
                     textureConfiguration.Width = size;
                     textureConfiguration.Height = size;

                    Count<Texture2D> thumbnail = Texture2D::Create(textureConfiguration, AssetManager::GetAssetFileSystemPath(AssetManager::GetAssetInfo(assetID).Path));

                    return  thumbnail;
                 }

            case AssetType::MeshSourceFile:
                 {
                    GenerateMeshSource(AssetManager::GetAsset<MeshSource>(assetID));
                 }
            break;
            default:
                PF_CORE_ASSERT(false, "Unsupported thumbnail asset type");
                return nullptr;
        }
        m_World->OnUpdateEditor(dt);
        m_World->OnRenderEditor(m_Renderer, dt, m_Camera);
        

        ImageConfiguration imageConfiguration;
        imageConfiguration.DebugName = AssetManager::GetAssetInfo(assetID).GetName() + " Thumbnail Image";
        imageConfiguration.Transfer = true;
        imageConfiguration.Format = ImageFormat::RGBA;
        imageConfiguration.Usage = ImageUsage::Texture;
        imageConfiguration.Width = size;
        imageConfiguration.Height = size;

        Count<Image2D> thumbnailImage = Image2D::Create(imageConfiguration);

        TextureConfiguration textureConfiguration;
        textureConfiguration.DebugName = AssetManager::GetAssetInfo(assetID).GetName() + " Thumbnail";
        textureConfiguration.Format = ImageFormat::RGBA;
        textureConfiguration.Width = size;
        textureConfiguration.Height = size;

        Count<Texture2D> thumbnail = Texture2D::Create(textureConfiguration, thumbnailImage, false);

        Renderer::BlitImage(Renderer::GetRendererCommandBuffer(), m_Renderer->GetFinalPassImage(), thumbnailImage,true);

        return thumbnail;
    }

    Entity AssetThumbnailRenderer::PreparePreviewEntity()
    {
        PF_PROFILE_FUNC();

        Entity entity = m_World->TryGetEntityByTag("Mesh");

        if (entity)
            m_World->DeleteEntity(entity);

        return m_World->CreateEntity("Mesh");
    }

    void AssetThumbnailRenderer::ClearPreview()
    {
        PF_PROFILE_FUNC();

        Entity entity = m_World->TryGetEntityByTag("Mesh");

        if (entity)
            m_World->DeleteEntity(entity);

        m_World->CreateEntity("Mesh");
    }

    void AssetThumbnailRenderer::GenerateMeshThumbnail(Count<Mesh> mesh)
    {
        PF_PROFILE_FUNC();
        PF_CORE_ASSERT(mesh);

        Entity entity = PreparePreviewEntity();

        auto& meshComponent = entity.AddComponent<MeshComponent>();
        meshComponent.SetMesh(mesh->GetID());


        FrameCameraToBounds(mesh->GetBoundingBox(),0.9f);
    }

    void AssetThumbnailRenderer::GenerateDynamicMeshThumbnail(Count<DynamicMesh> mesh)
    {
        PF_PROFILE_FUNC();
        PF_CORE_ASSERT(mesh);

        Entity entity = PreparePreviewEntity();

        auto& meshComponent = entity.AddComponent<DynamicMeshComponent>();
        meshComponent.SetMesh(mesh->GetID());

        FrameCameraToBounds(mesh->GetBoundingBox(),0.9f);
    }

    void AssetThumbnailRenderer::GenerateMaterialThumbnail(Count<Material> material)
    {
        PF_PROFILE_FUNC();
        PF_CORE_ASSERT(material);

        Entity entity = PreparePreviewEntity();

        auto& meshComponent = entity.AddComponent<MeshComponent>();
        meshComponent.SetMesh((uint64_t)DefaultRuntimeAssets::Sphere);

        Count<Mesh> sphere = meshComponent.GetMesh();

        PF_CORE_ASSERT(sphere);

        meshComponent.MaterialTable->SetMaterial(0, material);

        FrameCameraToBounds(sphere->GetBoundingBox());
    }

    void AssetThumbnailRenderer::GenerateMeshSource(Count<class MeshSource> meshSource)
    {
        PF_PROFILE_FUNC();


        Count<Mesh> mesh = Count<Mesh>::Create(meshSource);
        AssetManager::CreateRuntimeAsset(mesh);
        PF_CORE_ASSERT(mesh);

        Entity entity = PreparePreviewEntity();

        auto& meshComponent = entity.AddComponent<MeshComponent>();
        meshComponent.SetMesh(mesh->GetID());

        FrameCameraToBounds(mesh->GetBoundingBox(),0.9f);
    }

    void AssetThumbnailRenderer::FrameCameraToBounds(const AABB bounds, float distnaceMultipler )
    {
        PF_PROFILE_FUNC();

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

        distance *= distnaceMultipler ;

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

        // Finish disk caching for the thumbnail generated last frame.
        if (m_PendingCacheAssetID != 0)
        {
            AssetID assetID = m_PendingCacheAssetID;
            Count<Texture2D> texture = m_PendingCacheTexture;

            m_PendingCacheAssetID = 0;
            m_PendingCacheTexture = nullptr;

            auto iterator = m_AssetThumbnails.find(assetID);

            if (!AssetManager::HasAsset(assetID) || iterator == m_AssetThumbnails.end() || !texture)
            {
                m_AssetThumbnails.erase(assetID);
            }
            else
            {
                AssetThumbnail& thumbnail = iterator->second;

                // Do not overwrite a newer regeneration request.
                if (thumbnail.Status != AssetThumbnailStatus::Queued)
                {
                    // The texture was already assigned when it was generated.
                    thumbnail.Status = AssetThumbnailStatus::Ready;
                    CacheThumbnail(assetID, texture);

                     AssetManager::TryUnloadAsset(assetID);
                }
            }
        }

        // Generate no more than one thumbnail this frame.
        if (!m_ThumbnailQueue.empty())
        {
            AssetID assetID = m_ThumbnailQueue.top();
            m_ThumbnailQueue.pop();

            if (!AssetManager::HasAsset(assetID))
            {
                m_AssetThumbnails.erase(assetID);
                return;
            }

            auto iterator = m_AssetThumbnails.find(assetID);

            if (iterator == m_AssetThumbnails.end())
                return;

            AssetThumbnail& thumbnail = iterator->second;

            // Ignore stale duplicate entries.
            if (thumbnail.Status != AssetThumbnailStatus::Queued)
                return;

            thumbnail.AssetID = assetID;

            const AssetInfo& assetInfo = AssetManager::GetAssetInfo(assetID);
            std::filesystem::path assetPath = AssetManager::GetAssetFileSystemPath(assetInfo.Path);
            std::filesystem::path thumbnailPath = GetThumbnailCachePath(assetID);

            // Load from disk when the cache exists and is still valid.
            if (!IsThumbnailOutdated(assetPath, thumbnailPath))
            {
                Count<Texture2D> cachedThumbnail = LoadCachedThumbnail(assetID, thumbnailPath);

                if (cachedThumbnail)
                {
                    thumbnail.Texture = cachedThumbnail;
                    thumbnail.Status = AssetThumbnailStatus::Ready;
                    return;
                }
            }

            // Cache is missing, outdated or failed to load.
            thumbnail.Status = AssetThumbnailStatus::Rendering;

            Count<Texture2D> generatedThumbnail =
                m_ThumbnailRenderer->GenerateThumbnail(assetID, dt);

            if (!generatedThumbnail)
            {
                thumbnail.Status = AssetThumbnailStatus::Queued;
                m_ThumbnailQueue.push(assetID);
                return;
            }

            // Make the generated texture visible immediately.
            thumbnail.Texture = generatedThumbnail;
            thumbnail.Status = AssetThumbnailStatus::Rendering;

            // Cache it on the next update.
            m_PendingCacheAssetID = assetID;
            m_PendingCacheTexture = generatedThumbnail;

            m_PreviewCleared = false;
            return;
        }
        if (m_PendingCacheAssetID == 0 && !m_PreviewCleared)
        {
            m_ThumbnailRenderer->ClearPreview();
            m_PreviewCleared = true;
        }
    }


    void AssetThumbnailManager::CacheThumbnail(AssetID thumbnailAssetID, Count<Texture2D> thumbnail)
    {
        PF_PROFILE_FUNC();

        if (!thumbnail || !AssetManager::HasAsset(thumbnailAssetID))
            return;

        const AssetInfo& assetInfo = AssetManager::GetAssetInfo(thumbnailAssetID);

        std::filesystem::path directory =
            Project::GetActive()->GetCacheDirectory() /
            "AssetThumbnails" /
            EnumReflection::EnumString(assetInfo.Type);

        std::error_code error;
        std::filesystem::create_directories(directory, error);

        if (error)
        {
            PF_ENGINE_ERROR("Failed to create thumbnail cache directory: {}", directory.string());
            return;
        }

        std::filesystem::path thumbnailPath =
            directory / (std::to_string((uint64_t)thumbnailAssetID) + ".png");

        const TextureConfiguration& configuration = thumbnail->GetSpecification();


        uint32_t width = configuration.Width;
        uint32_t height = configuration.Height;
        size_t expectedSize = (size_t)width * (size_t)height * 4;

        Buffer pixels = thumbnail->GetImage()->GetStoredDataAsBuffer();

        if (!pixels.Data || pixels.Size < expectedSize)
        {
            PF_ENGINE_ERROR("Unable to read thumbnail pixels for asset: {}", thumbnailAssetID);
            pixels.Release();
            return;
        }

        stbi_flip_vertically_on_write(0);

        int result = stbi_write_png(
            thumbnailPath.string().c_str(),
            (int)width,
            (int)height,
            4,
            pixels.Data,
            (int)width * 4
        );

        pixels.Release();

        if (result == 0)
        {
            PF_ENGINE_ERROR("Failed to cache thumbnail: {}", thumbnailPath.string());
            return;
        }

        PF_ENGINE_TRACE("Cached thumbnail: {}", thumbnailPath.string());
    }

    Count<Texture2D> AssetThumbnailManager::LoadCachedThumbnail(AssetID assetID, const std::filesystem::path& thumbnailPath)
    {
        PF_PROFILE_FUNC();

        std::error_code error;

        if (!std::filesystem::exists(thumbnailPath, error) || error)
            return nullptr;

        TextureConfiguration configuration;
        configuration.DebugName = AssetManager::GetAssetInfo(assetID).GetName() + " Cached Thumbnail";
        configuration.Format = ImageFormat::RGBA;

        Count<Texture2D> thumbnail = Texture2D::Create(configuration, thumbnailPath);

        if (!thumbnail)
            PF_ENGINE_ERROR("Failed to load cached thumbnail: {}", thumbnailPath.string());


        PF_ENGINE_TRACE("Success  load cached thumbnail: {}", thumbnailPath.string());

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

    Count<Texture2D> GetTextureFile(AssetType type)
    {
        switch (type)
        {
            case AssetType::World: return EditorResources::WorldThumbnailIcon;
            case AssetType::PhysicsMaterial: return EditorResources::PMatThumbnailIcon;
            case AssetType::Font: return EditorResources::FontThumbnailIcon;
            case AssetType::Prefab: return EditorResources::PrefabThumbnailIcon;
            case AssetType::UIPanel: return EditorResources::UIThumbnailIcon;
            case AssetType::ParticleSystem: return EditorResources::ParticleSystemThumbnailIcon;
            case AssetType::ParticleEmitter: return EditorResources::ParticleEmitterThumbnailIcon;
            case AssetType::Audio: return EditorResources::AudioThumbnailIcon;
            case AssetType::AudioController: return EditorResources::AudioControllerThumbnailIcon;
            case AssetType::MeshCollider: return EditorResources::MeshColliderThumbnailIcon;
            case AssetType::ScriptFile: return EditorResources::ScriptFileThumbnailIcon;
            case AssetType::InputAction: return EditorResources::InputActionThumbnailIcon;
            case AssetType::InputBindingContext: return EditorResources::InputBindingContextThumbnailIcon;
            case AssetType::Skeleton: return EditorResources::SkeletonThumbnailIcon;
            case AssetType::Animation: return EditorResources::AnimationThumbnailIcon;
            case AssetType::AnimationController: return EditorResources::AnimationControllerThumbnailIcon;
            default:
                break;
        }

        PF_ENGINE_WARN("Thumbnails AssetImage not implemented: {}", EnumReflection::EnumString(type));
        return EditorResources::FileIcon;

    }
    Count<Texture2D> AssetThumbnailManager::GetThumbnail(AssetID assetID)
    {
        PF_PROFILE_FUNC();
        PF_CORE_ASSERT(AssetManager::HasAsset(assetID));

        const AssetInfo& assetInfo = AssetManager::GetAssetInfo(assetID);

        if (!Utils::Contains(SupportedThumbnailTypes, assetInfo.Type))
            return GetTextureFile(assetInfo.Type);

        // Returning an already loaded texture is safe.
        if (assetInfo.Type == AssetType::Texture && AssetManager::IsAssetLoaded(assetID))
            return AssetManager::GetAsset<Texture2D>(assetID);

        auto iterator = m_AssetThumbnails.find(assetID);

        if (iterator != m_AssetThumbnails.end())
        {
            AssetThumbnail& thumbnail = iterator->second;

            // Timestamp checking is okay here because it does not submit renderer work.
            if (thumbnail.Status == AssetThumbnailStatus::Ready)
            {
                std::filesystem::path assetPath = AssetManager::GetAssetFileSystemPath(assetInfo.Path);
                std::filesystem::path thumbnailPath = GetThumbnailCachePath(assetID);

                if (IsThumbnailOutdated(assetPath, thumbnailPath))
                {
                    thumbnail.Status = AssetThumbnailStatus::Queued;
                    m_ThumbnailQueue.push(assetID);
                    m_PreviewCleared = false;
                }
            }

            return thumbnail.Texture ? thumbnail.Texture : EditorResources::FileIcon;
        }

        AssetThumbnail thumbnail;
        thumbnail.AssetID = assetID;
        thumbnail.Status = AssetThumbnailStatus::Queued;

        m_AssetThumbnails.emplace(assetID, thumbnail);
        m_ThumbnailQueue.push(assetID);
        m_PreviewCleared = false;

        return EditorResources::FileIcon;
    }
}
