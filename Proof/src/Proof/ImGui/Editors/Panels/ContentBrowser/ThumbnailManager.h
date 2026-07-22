#pragma once
#include "Proof/Asset/Asset.h"
#include "Proof/Scene/Camera/EditorCamera.h"
#include <queue>
#include <unordered_map>

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
    class AssetThumbnailRenderer : public RefCounted
    {
    public:
        AssetThumbnailRenderer();
        void GenerateThumbnail(AssetID assetID);
        void OnUpdate(FrameTime dt);

        Count<Texture2D> GetLastThumbnail();
        AssetID GetLastThumbnailID() const;

    private:
        void GenerateMeshThumbnail(Count<class Mesh>  mesh);
        void GenerateDynamicMeshThumbnail(Count<class DynamicMesh>  mesh);
        void GenerateMaterialThumbnail(Count<class Material> material);

        void FrameCameraToBounds(const struct AABB bounds);
    private:

        ThumbnailSize m_ThumbnailSize = ThumbnailSize::Medium;
        Count<class WorldRenderer> m_Renderer;
        Count<class World> m_World;
		EditorCamera m_Camera;

        AssetID m_CurrentThumbnailID;
        AssetID m_LastThumbnailID;
        friend class ContentBrowserPanel;
        friend class AssetThumbnailManager;
    };

   
    class AssetThumbnailManager : public RefCounted
    {

    public:
        AssetThumbnailManager();
        void OnUpdate(FrameTime dt);

        Count<class Texture2D> GetThumbnail(AssetID assetID);

        void RegenerateThumbnail(AssetID assetID); // only for saved asset liek material or mesh that needs to be regeneritead

    private:

        std::unordered_map<AssetID, AssetThumbnail> m_AssetThumbnails;
        std::queue<AssetID> m_ThumbnailQueue;
        Count<AssetThumbnailRenderer> m_ThumbnailRenderer;

        AssetThumbnail m_CurrentThumbnail;
        friend class ContentBrowserPanel;
    };
}
