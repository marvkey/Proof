#include "Proofprch.h"
#include "AssetManager.h"
#include "Asset.h"
#include "Proof/Resources/Math/Math.h"
#include <filesystem>
#include <yaml-cpp/yaml.h>
#include "TextureAsset/TextureAsset.h"
#include "MeshAsset.h"
#include "MaterialAsset.h"
namespace Proof
{
	AssetManager* AssetManager::s_AssetManager = new AssetManager();
	void AssetManager::NewAsset(UUID ID,const Count<Asset>& asset) {
		while (ID == 0) {
			PF_CORE_ASSERT(false,"ID cannot be 0");
			ID = AssetManager::CreateID();
		}
		if (HasID(ID) == false) {
			s_AssetManager->m_AllAssets.insert({ID,asset});
			return;
		}
		PF_ASSERT(false,"Asset Maneger Has ID");
	}
	bool AssetManager::HasID(UUID ID) {
		return s_AssetManager->m_AllAssets.find(ID) != s_AssetManager->m_AllAssets.end();
	}
	UUID AssetManager::CreateID() {
		UUID ID = UUID();
		while (HasID(ID) == true || ID ==0) {
			ID = UUID();
		}
		return ID;
	}
	void AssetManager::Remove(UUID ID) {
		if (HasID(ID) == true) {
			s_AssetManager->m_AllAssets.erase(ID);
			return;
		}
		PF_ASSERT(false,"Asset Manager Has ID");
	}
	void AssetManager::SaveAllAsset() {
		for(auto& asset: s_AssetManager->m_AllAssets){
			asset.second->SaveAsset();
		};
	}
	void AssetManager::InitilizeAssets(const std::string& Path) {
		for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(Path)) {
			if (dirEntry.is_directory()) {
				continue;
			}
			if (IsFileValid(dirEntry.path().string())) {

				if (GetAssetType(dirEntry.path().string()) == Texture2DAsset::GetAssetType()) {
					Count<Texture2DAsset> asset = CreateCount<Texture2DAsset>();
					asset->LoadAsset(dirEntry.path().string());
					asset->m_AssetName = dirEntry.path().stem().string();
					AssetManager::NewAsset(asset->GetID(),asset);
					continue;
				}

				if (GetAssetType(dirEntry.path().string()) == MeshAsset::GetAssetType()) {
					Count<MeshAsset> asset = CreateCount<MeshAsset>();
					asset->LoadAsset(dirEntry.path().string());
					AssetManager::NewAsset(asset->GetID(),asset);
					asset->m_AssetName = dirEntry.path().stem().string();
					continue;
				}

				if (GetAssetType(dirEntry.path().string()) == MaterialAsset::GetAssetType()) {
					Count<MaterialAsset> asset = CreateCount<MaterialAsset>();
					asset->LoadAsset(dirEntry.path().string());
					AssetManager::NewAsset(asset->GetID(),asset);
					asset->m_AssetName = dirEntry.path().stem().string();
					continue;
				}
			}
		}
		MakeDirectory("config/AssetManager.ProofAssetManager");
	}
	void AssetManager::MakeDirectory(const std::string& path) {
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Project: " << YAML::Value <<"Proof Engine Test";
		out<<YAML::Key << "Total Asset: "<<YAML::Value<< s_AssetManager->m_AllAssets.size();
		out << YAML::Key << "ProjectAssets" << YAML::Value << YAML::BeginSeq;
		for(auto& asset : s_AssetManager->m_AllAssets){
			out<<YAML::BeginMap;
			out << YAML::Key << "Asset" << YAML::Value << asset.first;
			out<<YAML::Key<<"Type"<<asset.second->GetAssetTypeVirtual();
			out<<YAML::Key<<"Path"<<asset.second->GetPath();
			out<<YAML::EndMap;
		}
		
		std::ofstream found(path);
		found << out.c_str();
		found.close();
	}
	bool AssetManager::IsFileValid(const std::string& Path) {
		YAML::Node data = YAML::LoadFile(Path);
		if (!data["AssetType"]) // if there is no scene no
			return false;
		return true;
	}
	std::string AssetManager::GetAssetType(const std::string& Path) {
		YAML::Node data = YAML::LoadFile(Path);
		return data["AssetType"].as<std::string>();
	}
}