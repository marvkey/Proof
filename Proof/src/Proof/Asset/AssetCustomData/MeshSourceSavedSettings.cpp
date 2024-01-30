#include "Proofprch.h"
#include "MeshSourceSavedSettings.h"

namespace Proof
{
    std::unordered_map<AssetID, MeshSourceSavedSettings::MeshSourceMetaData> MeshSourceSavedSettings::s_MeshesSourcesMetaData;

    const MeshSourceSavedSettings::MeshSourceMetaData& MeshSourceSavedSettings::GetMeshSourceMetaData(AssetID ID)
    {
        PF_CORE_ASSERT(HasMeshSourceMetaData(ID));
        return s_MeshesSourcesMetaData.at(ID);
    }

    void MeshSourceSavedSettings::DeleteMeshSourceData(AssetID ID)
    {
        if (HasMeshSourceMetaData(ID))
            s_MeshesSourcesMetaData.erase(ID);
    }

}
