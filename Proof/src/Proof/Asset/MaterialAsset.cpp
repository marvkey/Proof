#include "Proofprch.h"
#include "MaterialAsset.h"
#include "Proof/Renderer/3DRenderer/Renderer3D.h"
#include "Proof/Math/Math.h"
#include <yaml-cpp/yaml.h>
#include "Proof/Resources/ExternalCreations.h"
#include "AssetManager.h"
#include "Proof/Renderer/Texture.h"
#include "Proof/Utils/PlatformUtils.h"

namespace Proof{
	MaterialAsset::MaterialAsset(const std::string& assetSavePath):
		Asset(AssetType::Material)
	{
		m_AssetID = AssetManager::CreateID();
		m_SavePath = assetSavePath;
		SaveAsset();
	}
	void MaterialAsset::SaveAsset() {
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "AssetType" << YAML::Value << EnumReflection::EnumString(GetAssetType());
		out << YAML::Key << "ID" << YAML::Value << m_AssetID;

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
		std::ofstream found(m_SavePath);
		found << out.c_str();
		found.close();
	}
	bool MaterialAsset::LoadAsset(const std::string& filePath) {
		m_SavePath = filePath;
		YAML::Node data = YAML::LoadFile(m_SavePath.string());
		if (!data["AssetType"]) // if there is no scene no
			return false;
		m_AssetID = data["ID"].as<uint64_t>();
		
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
	const Material& MaterialAsset::GetMaterial()const {
		return m_Material;
	}
}