#include "Proofprch.h"
#include "MeshAsset.h"
#include "Proof/Scene/Model.h"
#include "Proof/Renderer/Renderer3D.h"
#include <yaml-cpp/yaml.h>

namespace Proof
{
	MeshAsset::MeshAsset(const std::string& FilePath,const std::string& AssetSavePath) {
		m_Model = new Model(FilePath);
		m_AssetType = AssetType::MeshAsset;
		m_Path = AssetSavePath;
		m_PathOfPointerToFile = FilePath;
		SaveAsset();
	}
	void MeshAsset::SaveAsset() {
		YAML::Emitter out;
		out<<YAML::BeginMap;
		out<<YAML::Key<<"AssetTypeString"<<YAML::Value<<"AssetType::MeshAsset";
		out << YAML::Key << "AssetType" << YAML::Value << (int)m_AssetType;
		out << YAML::Key << "ID" << YAML::Value << m_ID;
		out << YAML::Key << "Model" << YAML::Value << m_PathOfPointerToFile;
		out << YAML::EndMap;

		std::ofstream found(m_Path);
		found<<out.c_str();
		found.close();
	}
	bool MeshAsset::LoadAsset(const std::string& FilePath){
		m_Path = FilePath;
		YAML::Node data = YAML::LoadFile(FilePath);
		if (!data["AssetTypeString"]) // if there is no scene no
			return false;

		m_AssetType =(AssetType) data["AssetType"].as<uint32_t>();
		m_ID= data["ID"].as<uint32_t>();
		m_PathOfPointerToFile = data["Model"].as<std::string>();

		if(m_Model == nullptr){
			m_Model = new Model{m_PathOfPointerToFile};
		}else{
			delete m_Model;
			m_Model = new Model{m_PathOfPointerToFile};
		}

		return true;
	}
	uint32_t MeshAsset::GetImageID() {
		return InstancedRenderer3D::m_WhiteTexture->GetID();
	}
	Model* MeshAsset::GetModel() {
		return m_Model;
	}
}