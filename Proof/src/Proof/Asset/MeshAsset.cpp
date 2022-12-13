#include "Proofprch.h"
#include "MeshAsset.h"
#include "Proof/Scene/Mesh.h"
#include "Proof/Renderer/3DRenderer/Renderer3D.h"
#include <yaml-cpp/yaml.h>
#include "AssetManager.h"
#include "Proof/Utils/PlatformUtils.h"

namespace Proof
{
	MeshAsset::MeshAsset(const std::string& meshFilePath,const std::string& savePath):
		Asset(AssetType::Mesh) 
	{
		m_AssetID = AssetManager::CreateID();
		m_Mesh = CreateSpecial<Mesh>(meshFilePath);
		m_SavePath = savePath;
		m_Source = AssetManager::GetAssetSourceID(meshFilePath);
		SaveAsset();
	}
	void MeshAsset::SaveAsset() {
		YAML::Emitter out;
		out<<YAML::BeginMap;
		out<<YAML::Key<<"AssetType"<<YAML::Value<<EnumReflection::EnumString(GetAssetType());
		out << YAML::Key << "ID" << YAML::Value << m_AssetID;
		out << YAML::Key << "AssetSource" << YAML::Value << m_Source;
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
		std::ofstream found(m_SavePath);
		found<<out.c_str();
		found.close();
	}
	bool MeshAsset::LoadAsset(const std::string& FilePath){
		m_SavePath = FilePath;
		YAML::Node data = YAML::LoadFile(FilePath);
		if (!data["AssetType"]) // if there is no scene no
			return false;
		m_AssetID = data["ID"].as<uint64_t>();
		m_Source = data["AssetSource"].as<uint64_t>();

		m_Mesh = CreateSpecial<Mesh>(AssetManager::GetAssetSourcePath(m_Source));
		if (data["SubMeshes"]) {
			for (const auto& subMesh : data["SubMeshes"]) {
				uint32_t index = subMesh.as<uint32_t>();
				if (m_Mesh->GetSubMeshes().size() > index) {
					m_Mesh->meshes[index].m_Enabled = false;
				}
			}
		}
		m_Mesh->m_Enabled = data["Enabled"].as<bool>();
		m_Mesh->m_FaceCulling = data["FaceCulling"].as<bool>();
		return true;
	}
	void MeshAsset::ChangeMesh(const std::string& meshFilepath){
		//m_MeshFilePath = meshFilepath;
		//m_Mesh = CreateSpecial<Mesh>(m_MeshFilePath);
	}
	void* MeshAsset::GetImageID() {
		return InstancedRenderer3D::m_WhiteTexture->GetID();
	}
	
}