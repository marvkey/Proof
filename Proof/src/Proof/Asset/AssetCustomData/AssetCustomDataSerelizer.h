#pragma once
#include <filesystem>
namespace Proof
{
	class AssetCustomDataSerializer
	{
	public:
		virtual void Save(const std::filesystem::path& path) = 0;
		virtual void Load(const std::filesystem::path& path) = 0;
	};

	class MeshSourceSavedSettingSerializer : public AssetCustomDataSerializer
	{
	public:
		virtual void Save(const std::filesystem::path& path);
		virtual void Load(const std::filesystem::path& path);
	};
}