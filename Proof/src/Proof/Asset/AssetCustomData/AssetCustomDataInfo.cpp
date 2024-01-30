#include "Proofprch.h"
#include "AssetCustomDataInfo.h"
#include "Proof/Core/Application.h"
#include "Proof/Project/Project.h"
namespace Proof::Utils
{
    std::string GetAssetCustomDataExtensionString(AssetCustomDataType type)
    {
        if (AssetCustomDataTypeMap.contains(type))
            return AssetCustomDataTypeMap.at(type);
        PF_CORE_ASSERT(false, "Unknown assetCustom type");
        return "";
    }
    std::filesystem::path GetAssetCustomDataFileSystemPath(const std::filesystem::path& path)
    {
        auto customDataDir = Application::Get()->GetProject()->GetProjectDirectory() / Application::Get()->GetProject()->GetConfig().AssetCustomDataDirectory;
        return customDataDir / path;
    }
}
