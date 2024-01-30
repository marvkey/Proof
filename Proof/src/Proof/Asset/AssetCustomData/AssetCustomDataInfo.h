#pragma once
#include <unordered_map>
#include <string>
#include <filesystem>
namespace Proof
{
	enum class AssetCustomDataType
	{
		None = 0,
		MeshSourceSavedSettings = 1,
	};
	namespace Utils
	{
		static inline const std::unordered_map<AssetCustomDataType, std::string> AssetCustomDataTypeMap =
		{
				{AssetCustomDataType::None, ""},
				{AssetCustomDataType::MeshSourceSavedSettings, ".MeshSourceCustom.ProofCustomAssetData"}
		};

		std::string GetAssetCustomDataExtensionString(AssetCustomDataType type);

		std::filesystem::path GetAssetCustomDataFileSystemPath(const std::filesystem::path& path);
	}
}