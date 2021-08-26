#include "Proofprch.h"
#include "AssetManager.h"
#include "Asset.h"
#include "Proof/Resources/Math/Math.h"
#include <filesystem>
#include <yaml-cpp/yaml.h>
#include "TextureAsset/TextureAsset.h"
#include "MeshAsset.h"
#include "MaterialAsset.h"
namespace Proof{
	AssetManager* AssetManager::s_AssetManager = new AssetManager();
	void AssetManager::NewAsset(uint32_t ID,Asset* asset) {
		if (ID == 0) {
			PF_CORE_ASSERT(false,"ID cannot be 0");
			ID =AssetManager::CreateID();
		}
		if(HasID(ID) ==false){
			s_AssetManager->m_AllAssets.insert({ID,asset});
			return;
		}
		PF_ASSERT(false,"Asset Maneger Has ID");
	}
	bool AssetManager::HasID(uint32_t ID) {
		return s_AssetManager->m_AllAssets.find(ID) != s_AssetManager->m_AllAssets.end();
	}
	Asset* AssetManager::GetAsset(uint32_t ID) {

		if (HasID(ID) == true) {
			return s_AssetManager->m_AllAssets.find(ID)->second;
		}
	}
	uint32_t AssetManager::CreateID() {
		uint32_t ID = Math::RandUINT(1,1000000);
		while (HasID(ID)== true) {
			uint32_t ID = Math::RandUINT(1,1000000);
		}
		return ID;
	}
	void AssetManager::Remove(uint32_t ID) {
		if (HasID(ID) == true) {
			delete s_AssetManager->m_AllAssets.find(ID)->second;

			s_AssetManager->m_AllAssets.erase(ID);
			return;
		}
		PF_ASSERT(false,"Asset Manager Has ID");
	}
	void AssetManager::InitilizeAssets(const std::string& Path) {
		PF_ENGINE_TRACE("starting Loading All Assets %s",Path.c_str());
		for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(Path)){
			if(dirEntry.is_directory()){
				continue;
			}
			if(IsFileValid(dirEntry.path().string())){
				
				if(GetAssetType(dirEntry.path().string()) =="AssetType::Texture2DAsset"){
					Texture2DAsset* asset = new Texture2DAsset;
					asset->LoadAsset(dirEntry.path().string());
					AssetManager::NewAsset(asset->GetID(),asset);
					asset->m_AssetName = dirEntry.path().stem().string();
					continue;
				}

				if(GetAssetType(dirEntry.path().string()) == "AssetType::MeshAsset"){
					MeshAsset* asset = new MeshAsset;
					asset->LoadAsset(dirEntry.path().string());
					AssetManager::NewAsset(asset->GetID(),asset);
					asset->m_AssetName = dirEntry.path().stem().string();
					continue;
				}

				if(GetAssetType(dirEntry.path().string()) == MaterialAsset::GetAssetTypeStaticName()){
					MaterialAsset* asset = new MaterialAsset;
					asset->LoadAsset(dirEntry.path().string());
					AssetManager::NewAsset(asset->GetID(),asset);
					asset->m_AssetName = dirEntry.path().stem().string();
				}
			}
		}

	}
	bool AssetManager::IsFileValid(const std::string& Path) {
		YAML::Node data = YAML::LoadFile(Path);
		if (!data["AssetTypeString"]) // if there is no scene no
			return false;
		return true;
	}
	std::string AssetManager::GetAssetType(const std::string& Path) {
		YAML::Node data = YAML::LoadFile(Path);
		return data["AssetTypeString"].as<std::string>();
	}
}