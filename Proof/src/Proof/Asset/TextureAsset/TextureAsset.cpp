#include "Proofprch.h"
#include "TextureAsset.h"
#include "Proof/Renderer/3DRenderer/Renderer3D.h"
#include <yaml-cpp/yaml.h>
#include "../AssetManager.h"

namespace Proof{
	Texture2DAsset::Texture2DAsset(const std::string& texturePath,const std::string& savePath):
		Asset(AssetType::Texture) 
	{
		m_AssetID = AssetManager::CreateID();

		m_Texture = Texture2D::Create(texturePath);
		m_SavePath = savePath;
		auto parentDir = std::filesystem::relative(m_SavePath.parent_path());
		m_SavePath = parentDir /= {Utils::FileDialogs::GetFileName(m_SavePath) + "." + GetExtension()};
		if (AssetManager::HasAsset(texturePath) == false)
			AssetManager::NewAsset(AssetManager::CreateID(), texturePath);
		m_Source = AssetManager::GetAssetInfo(texturePath).ID;
		SaveAsset();
	}
	void Texture2DAsset::SaveAsset() {
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "AssetType" << YAML::Value << EnumReflection::EnumString(GetAssetType());
		out << YAML::Key << "ID" << YAML::Value << m_AssetID;
		out << YAML::Key << "AssetSource" << YAML::Value << m_Source;
		out << YAML::EndMap;
		std::ofstream found(m_SavePath);
		found << out.c_str();
		found.close();
	}
	
	bool Texture2DAsset::LoadAsset(const std::string& filePath) {
		m_SavePath = filePath;
		YAML::Node data = YAML::LoadFile(m_SavePath.string());

		if (!data["AssetType"]) // if there is no scene no
			return false;

		m_AssetID = data["ID"].as<uint64_t>();
		m_Source = data["AssetSource"].as<uint64_t>();

		m_Texture = Texture2D::Create(AssetManager::GetAssetInfo(m_Source).Path.string());
		return true;
	}
	Image Texture2DAsset::GetImage() {
		return m_Texture != nullptr ? m_Texture->GetImage() : InstancedRenderer3D::m_WhiteTexture->GetImage();
	}
}