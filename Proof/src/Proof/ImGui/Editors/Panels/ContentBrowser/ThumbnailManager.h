#pragma once
#include "Proof/Asset/Asset.h"
#include "Proof/Scene/Camera/EditorCamera.h"
#include <queue>
#include <stack>
#include <unordered_map>

#include "Proof/Scene/Entity.h"

namespace  Proof
{
    enum class AssetThumbnailStatus
    {
        Missing,
        Queued,
        Rendering,
        Ready,
        Failed
    };

    struct AssetThumbnail
    {
        Count<class Texture2D> Texture;
        AssetThumbnailStatus Status = AssetThumbnailStatus::Missing;
        AssetID AssetID = 0;
    };

    enum class ThumbnailSize : uint32_t
    {
        Low = 128,
        Medium = 256,
        High = 512,
        Extreme = 1024
    };
    class AssetThumbnailRenderer  : public RefCounted
    {
    public:
        AssetThumbnailRenderer();

        Count<class Texture2D> GenerateThumbnail(AssetID assetID, FrameTime dt);
        void ClearPreview();

    private:
        Entity PreparePreviewEntity();

        void GenerateMeshThumbnail(Count<Mesh> mesh);
        void GenerateDynamicMeshThumbnail(Count<DynamicMesh> mesh);
        void GenerateMaterialThumbnail(Count<class Material> material);
        void GenerateMeshSource(Count<class MeshSource> mesh);
        void FrameCameraToBounds(const struct AABB bounds, float dstMultipler =0.6f);

    private:
        Count<World> m_World;
        Count<WorldRenderer> m_Renderer;
        EditorCamera m_Camera;
        float m_ThumbnailSize = (float)ThumbnailSize::Medium;
        friend class ContentBrowserPanel;
    };

  
   
    class AssetThumbnailManager : public RefCounted
    {

    public:
        AssetThumbnailManager();
        void OnUpdate(FrameTime dt);

        Count<class Texture2D> GetThumbnail(AssetID assetID);


    private:
        void CacheThumbnail(AssetID thumbnailAssetID,Count<Texture2D> thumbnail);
        Count<Texture2D> LoadCachedThumbnail(AssetID assetID, const std::filesystem::path& thumbnailPath);
        std::filesystem::path GetThumbnailCachePath(AssetID assetID) const ;

    private:

        std::unordered_map<AssetID, AssetThumbnail> m_AssetThumbnails;
        std::stack<AssetID> m_ThumbnailQueue;
        Count<AssetThumbnailRenderer> m_ThumbnailRenderer;
        AssetID m_PendingCacheAssetID = 0;
        Count<Texture2D> m_PendingCacheTexture;

        bool m_PreviewCleared = false;
        friend class ContentBrowserPanel;
    };
}
