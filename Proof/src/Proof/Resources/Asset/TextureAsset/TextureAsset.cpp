#include "Proofprch.h"
#include "TextureAsset.h"
#include "Proof/Renderer/3DRenderer/Renderer3D.h"
#include <yaml-cpp/yaml.h>
#include "../AssetManager.h"

namespace Proof{
	Texture2DAsset::Texture2DAsset(const std::string& texturePath,const std::string& savePath) {
		m_ID = AssetManager::CreateID();

		m_Texture =Texture2D::Create(texturePath);
		m_SavePath = savePath;
		m_TexturePath = texturePath;
		SaveAsset();
	}
	void Texture2DAsset::SaveAsset() {
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "AssetType" << YAML::Value << GetAssetType();
		out << YAML::Key << "ID" << YAML::Value << m_ID;
		out << YAML::Key << "Texture2D" << YAML::Value << m_TexturePath;
		out << YAML::EndMap;

		std::ofstream found(m_SavePath);
		found << out.c_str();
		found.close();
	}
	
	bool Texture2DAsset::LoadAsset(const std::string& filePath) {
		m_SavePath = filePath;
		YAML::Node data = YAML::LoadFile(m_SavePath);

		if (!data["AssetType"]) // if there is no scene no
			return false;

		m_ID = data["ID"].as<uint64_t>();
		m_TexturePath = data["Texture2D"].as<std::string>();

		m_Texture = Texture2D::Create(m_TexturePath);
		return true;
	}
	uint32_t Texture2DAsset::GetImageID() {
		return m_Texture != nullptr ? m_Texture->GetID() : InstancedRenderer3D::m_WhiteTexture->GetID();
	}
}