#include "Proofprch.h"
#include "TextureAsset.h"
#include "Proof/Renderer/3DRenderer/Renderer3D.h"
#include <yaml-cpp/yaml.h>

namespace Proof{
	Texture2DAsset::Texture2DAsset(const std::string& FilePath,const std::string& AssetSavePath) {
		m_ID = AssetManager::CreateID();

		m_Texture =Texture2D::Create(FilePath);
		m_AssetType = AssetType::Texture2DAsset;
		m_Path = AssetSavePath;
		m_PathOfPointerToFile = FilePath;
		SaveAsset();
	}
	void Texture2DAsset::SaveAsset() {
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "AssetTypeString" << YAML::Value << "AssetType::Texture2DAsset";
		out << YAML::Key << "AssetType" << YAML::Value << (int)m_AssetType;
		out << YAML::Key << "ID" << YAML::Value << m_ID;
		out << YAML::Key << "Texture2D" << YAML::Value << m_PathOfPointerToFile;
		out << YAML::EndMap;

		std::ofstream found(m_Path);
		found << out.c_str();
		found.close();
	}
	
	bool Texture2DAsset::LoadAsset(const std::string& FilePath) {
		m_Path = FilePath;
		YAML::Node data = YAML::LoadFile(FilePath);

		if (!data["AssetTypeString"]) // if there is no scene no
			return false;

		m_AssetType = (AssetType)data["AssetType"].as<uint32_t>();
		m_ID = data["ID"].as<uint32_t>();
		m_PathOfPointerToFile = data["Texture2D"].as<std::string>();

		m_Texture = Texture2D::Create(m_PathOfPointerToFile);
		return true;
	}
	uint32_t Texture2DAsset::GetImageID() {
		return m_Texture != nullptr ? m_Texture->GetID() : InstancedRenderer3D::m_WhiteTexture->GetID();
	}
}