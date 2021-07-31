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
	}
	void MaterialAsset::SaveAsset() {
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "AssetTypeString" << YAML::Value << "AssetType::MaterialAsset";
		out << YAML::Key << "AssetType" << YAML::Value << (int)m_AssetType;
		out << YAML::Key << "ID" << YAML::Value << m_ID;
		out << YAML::Key << "Ambient"<<YAML::Value << m_Material.m_Ambient;
		out << YAML::Key << "Diffuse"<<YAML::Value << m_Material.m_Diuffuse;
		out << YAML::Key << "Specular"<<YAML::Value << m_Material.m_Specular;
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
		m_Material.m_Ambient= data["Ambient"].as<glm::vec3>();
		m_Material.m_Diuffuse= data["Diffuse"].as<glm::vec3>();
		m_Material.m_Specular= data["Specular"].as<glm::vec3>();
		return true;
	}
	uint32_t MaterialAsset::GetImageID() {
		return InstancedRenderer3D::m_WhiteTexture->GetID();
	}
	Material MaterialAsset::GetMaterial() {
		return m_Material;
	}
}