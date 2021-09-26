#include "Proofprch.h"
#include "MaterialAsset.h"
#include "Proof/Renderer/3DRenderer/Renderer3D.h"
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
		//out << YAML::Key << "Ambient"<<YAML::Value << m_Material.m_Ambient;
		//out << YAML::Key << "Diffuse"<<YAML::Value << m_Material.m_Diuffuse;
		//out << YAML::Key << "Specular"<<YAML::Value << m_Material.m_Specular;
		out << YAML::Key << "AlbedoColour"<<YAML::Value << m_Material.m_Colour;
		out << YAML::Key << "Roughness"<<YAML::Value << m_Material.m_Roughness;
		out << YAML::Key << "Metallness"<<YAML::Value << m_Material.m_Metallness;
		out << YAML::Key << "AO"<<YAML::Value << m_Material.m_AO;
		std::string temp;
		out << YAML::Key << "MetallicTexturePath" << YAML::Value << (m_Material.MetallicTexture !=nullptr?m_Material.MetallicTexture->GetPath(): temp);
		out << YAML::Key << "AlbedoTexturePath" << YAML::Value << (m_Material.AlbedoTexture != nullptr ? m_Material.AlbedoTexture->GetPath() : temp);
		out << YAML::Key << "NormalTexturePath" << YAML::Value << (m_Material.NormalTexture != nullptr ? m_Material.NormalTexture->GetPath() : temp);
		out << YAML::Key << "RoughnessTexturePath" << YAML::Value << (m_Material.RoughnessTexture!= nullptr ? m_Material.RoughnessTexture->GetPath() : temp);

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
		
		//m_Material.m_Ambient= data["Ambient"].as<glm::vec3>();
		//m_Material.m_Diuffuse= data["Diffuse"].as<glm::vec3>();
		//m_Material.m_Specular= data["Specular"].as<glm::vec3>();
		
		m_Material.m_Colour= data["AlbedoColour"].as<glm::vec3>();
		m_Material.m_Metallness = data["Metallness"].as<float>();
		m_Material.m_Roughness = data["Roughness"].as<float>();
		m_Material.m_AO = data["AO"].as<float>();
		
		std::string metallicTexture =data["MetallicTexturePath"].as<std::string>();
		std::string AlbedoTexture =data["AlbedoTexturePath"].as<std::string>();
		std::string NormalTexture =data["NormalTexturePath"].as<std::string>();
		std::string RoughnessTexture =data["RoughnessTexturePath"].as<std::string>();
		if(metallicTexture.empty()==false){
			m_Material.MetallicTexture = Texture2D::Create(metallicTexture);
		}
		if (AlbedoTexture.empty() == false) {
			m_Material.AlbedoTexture = Texture2D::Create(AlbedoTexture);
		}
		if (NormalTexture.empty() == false) {
			m_Material.NormalTexture = Texture2D::Create(NormalTexture);
		}
		if (RoughnessTexture.empty() == false) {
			m_Material.RoughnessTexture = Texture2D::Create(RoughnessTexture);
		}
		
		return true;
	}
	uint32_t MaterialAsset::GetImageID() {
		return InstancedRenderer3D::m_WhiteTexture->GetID();
	}
	Material MaterialAsset::GetMaterial() {
		return m_Material;
	}
}