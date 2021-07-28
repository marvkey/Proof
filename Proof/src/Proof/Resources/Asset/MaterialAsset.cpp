#include "Proofprch.h"
#include "MaterialAsset.h"
#include "Proof/Renderer/Renderer3D.h"
#include "Proof/Resources/Math/Math.h"
#include <yaml-cpp/yaml.h>
#include "Proof/Resources/ExternalCreations.h"

namespace Proof{
	MaterialAsset::MaterialAsset(const std::string& FilePath,const std::string& AssetSavePath) {
		m_ID = AssetManager::CreateID();
		m_AssetType = AssetType::MaterialAsset;
		m_Path = AssetSavePath;
		m_PathOfPointerToFile = FilePath;
		SaveAsset();
		m_Material = {{1.0f,1.0f,1.0f,1.0f}};
	}
	void MaterialAsset::SaveAsset() {
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "AssetTypeString" << YAML::Value << "AssetType::MaterialAsset";
		out << YAML::Key << "AssetType" << YAML::Value << (int)m_AssetType;
		out << YAML::Key << "ID" << YAML::Value << m_ID;
		out << YAML::Key << "Colour"<<YAML::Value << m_Material.Colour;
		out << YAML::EndMap;
		std::ofstream found(m_Path);
		found << out.c_str();
		found.close();
	}
	bool MaterialAsset::LoadAsset(const std::string& FilePath) {
		m_Path = FilePath;
		YAML::Node data = YAML::LoadFile(FilePath);
		if (!data["AssetTypeString"]) // if there is no scene no
			return false;

		m_AssetType = (AssetType)data["AssetType"].as<uint32_t>();
		m_ID = data["ID"].as<uint32_t>();
		m_Material.Colour= data["Colour"].as<glm::vec4>();
		return true;
	}
	uint32_t MaterialAsset::GetImageID() {
		return InstancedRenderer3D::m_WhiteTexture->GetID();
	}
	Material MaterialAsset::GetMaterial() {
		return m_Material;
	}
}