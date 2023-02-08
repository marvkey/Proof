#include "Proofprch.h"
#include "MeshAsset.h"
#include "Proof/Scene/Mesh.h"
#include "Proof/Renderer/3DRenderer/Renderer3D.h"
#include <yaml-cpp/yaml.h>
#include "AssetManager.h"
#include "Proof/Utils/PlatformUtils.h"

namespace Proof
{
	MeshAsset::MeshAsset(const std::string& meshFilePath,const std::string& savePath, const std::vector<uint32_t>& excludeIndex):
		Asset(AssetType::Mesh) 
	{
		m_AssetID = AssetManager::CreateID();
		m_SavePath = savePath;
		auto parentDir = std::filesystem::relative(m_SavePath.parent_path(), Application::Get()->GetProject()->GetAssetDirectory());
		m_SavePath = parentDir /= {Utils::FileDialogs::GetFileName(m_SavePath) + "."+GetExtension()};
		if (AssetManager::HasAsset(meshFilePath) == false)
			AssetManager::NewAsset(AssetManager::CreateID(), meshFilePath);
		m_Source = AssetManager::GetAssetInfo(meshFilePath).ID;
		m_DiscardMesh = excludeIndex;
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
		for (size_t i = 0; i < m_DiscardMesh.size(); i++) {
			auto& val = m_DiscardMesh[i];
			out << val;
		}
		out << YAML::EndSeq;
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

		if (data["SubMeshes"]) {
			for (const auto& subMesh : data["SubMeshes"]) {
				uint32_t index = subMesh.as<uint32_t>();
				m_DiscardMesh.emplace_back(index);
			}
		}
		return true;
	}
	void MeshAsset::ChangeMesh(const std::string& meshFilepath){
		//m_MeshFilePath = meshFilepath;
		//m_Mesh = CreateSpecial<Mesh>(m_MeshFilePath);
	}
	MeshAsset::MeshAsset() :
		Asset(AssetType::Mesh)
	{
	}
	Count<Mesh> MeshAsset::GetMesh() {
		//we do not want to hold a variable because it means when we want to delte that mesh source file
		/// it wont be deleted 
		/// 
		/// solution we crate a macor for when we running the game as an actual game
		/// when in taht macro we just return without checking cause u cannot delete asset
		/// during that phase
		if (AssetManager::HasID(m_Source) == false)return nullptr;
			auto asset = AssetManager::GetAsset< MeshSourceFileAsset>(m_Source);
		return asset->GetMesh();
	}
	
	MeshSourceFileAsset::MeshSourceFileAsset(const std::string& meshFilePath):
		Asset(AssetType::MeshSourceFile)
	{
		if (AssetManager::HasAsset(meshFilePath) == false)
			AssetManager::NewAsset(AssetManager::CreateID(), meshFilePath);
		m_AssetID = AssetManager::GetAssetInfo(meshFilePath).ID;
		m_SavePath = meshFilePath;
	}
	bool MeshSourceFileAsset::LoadAsset(const std::string& FilePath) {
		m_SavePath = FilePath;
		m_Mesh = CreateCount<Mesh>(FilePath);
		return m_Mesh != nullptr;
	}

	std::string MeshSourceFileAsset::GetExtension()const {
		return Utils::FileDialogs::GetFullFileExtension(m_SavePath);
	}
	Count<Mesh> MeshSourceFileAsset::GetMesh()
	{
		return m_Mesh;
	}
}