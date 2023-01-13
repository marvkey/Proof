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
	MaterialAsset::MaterialAsset(const std::string& savePath):
		Asset(AssetType::Material)
	{
		m_AssetID = AssetManager::CreateID();
		m_SavePath = savePath;
		auto parentDir = std::filesystem::relative(m_SavePath.parent_path());
		m_SavePath = parentDir /= {Utils::FileDialogs::GetFileName(m_SavePath) + "." + GetExtension()};
		SaveAsset();
	}
	void MaterialAsset::SaveAsset() {
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "AssetType" << YAML::Value << EnumReflection::EnumString(GetAssetType());
		out << YAML::Key << "ID" << YAML::Value << m_AssetID;

		out << YAML::Key << "AlbedoColour"<<YAML::Value << m_Material.Colour;
		out << YAML::Key << "Roughness"<<YAML::Value << m_Material.Roughness;
		out << YAML::Key << "Metallness"<<YAML::Value << m_Material.Metallness;
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
		
		m_Material.Colour= data["AlbedoColour"].as<glm::vec3>();
		m_Material.Metallness = data["Metallness"].as<float>();
		m_Material.Roughness = data["Roughness"].as<float>();
		
		//std::string metallicTexture =data["MetallicTexturePath"].as<std::string>();
		//std::string AlbedoTexture =data["AlbedoTexturePath"].as<std::string>();
		//std::string NormalTexture =data["NormalTexturePath"].as<std::string>();
		//std::string RoughnessTexture =data["RoughnessTexturePath"].as<std::string>();
		return true;
	}
	
	const Material& MaterialAsset::GetMaterial()const {
		return m_Material;
	}
}