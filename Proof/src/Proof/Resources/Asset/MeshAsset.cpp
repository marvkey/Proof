#include "Proofprch.h"
#include "MeshAsset.h"
#include "Proof/Scene/Mesh.h"
#include "Proof/Renderer/3DRenderer/Renderer3D.h"
#include <yaml-cpp/yaml.h>

namespace Proof
{

	MeshAsset::MeshAsset(const std::string& FilePath,const std::string& AssetSavePath) {
		m_ID = AssetManager::CreateID();
		m_Mesh = new Mesh(FilePath);
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
		out << YAML::Key << "SubMeshes";
		out << YAML::Flow;
		out << YAML::BeginSeq;
		if(m_Mesh != nullptr){
			int i=0;
			for (SubMesh subMesh: m_Mesh->GetSubMeshes()) {
				if(subMesh.m_Enabled==false){
					out<<i;
				}
				i++;
			}
		}
		out << YAML::EndSeq;
		out<<YAML::Key<<"Enabled"<<m_Mesh->m_Enabled;
		out<<YAML::Key<<"FaceCulling"<<m_Mesh->m_FaceCulling;
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

		if(m_Mesh == nullptr){
			m_Mesh = new Mesh{m_PathOfPointerToFile};
			if (data["SubMeshes"]) {
				for (auto& subMesh : data["SubMeshes"]) {
					uint32_t index = subMesh.as<uint32_t>();
					if (m_Mesh->GetSubMeshes().size() > index) {
						m_Mesh->meshes[index].m_Enabled = false;
					}
				}
			}
			m_Mesh->m_Enabled = data["Enabled"].as<bool>();
			m_Mesh->m_FaceCulling = data["FaceCulling"].as<bool>();
		}else{
			delete m_Mesh;
			m_Mesh = new Mesh{m_PathOfPointerToFile};
			if (data["SubMeshes"]) {
				for (auto& subMesh : data["SubMeshes"]) {
					uint32_t index = subMesh.as<uint32_t>();
					if (m_Mesh->GetSubMeshes().size() > index) {
						m_Mesh->meshes[index].m_Enabled = false;
					}
				}
			}
			m_Mesh->m_Enabled= data["Enabled"].as<bool>();
			m_Mesh->m_FaceCulling= data["FaceCulling"].as<bool>();
		}
		return true;
	}
	void MeshAsset::Reinstate(const std::string& path){
		m_PathOfPointerToFile = path;
		delete m_Mesh;
		m_Mesh = new Mesh(path);
	}
	uint32_t MeshAsset::GetImageID() {
		return InstancedRenderer3D::m_WhiteTexture->GetID();
	}
	
}