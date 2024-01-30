#pragma once
#include "Proof/Asset/AssetTypes.h"
#include <unordered_map>
namespace Proof
{
    class MeshSourceSavedSettings
    {
    public:
        struct MeshSourceMetaData
        {
            AssetID ID;//Mesh Source ID
            std::map<uint32_t, AssetID> MaterialList; // Material Index, Material ID
        };
        static const MeshSourceMetaData& GetMeshSourceMetaData(AssetID ID);
        static bool HasMeshSourceMetaData(AssetID ID) { return s_MeshesSourcesMetaData.contains(ID); }
        static const std::unordered_map<AssetID, MeshSourceMetaData>& GetMeshesSourceData() { return s_MeshesSourcesMetaData; }
    private:
        static void DeleteMeshSourceData(AssetID ID);
        static std::unordered_map<AssetID, MeshSourceMetaData> s_MeshesSourcesMetaData;//mesh sourceID, MeshData
        friend class MeshSourceSavedSettingSerializer;
        friend class AssetCustomDataManager;
        friend class MeshImporter;
    };
}