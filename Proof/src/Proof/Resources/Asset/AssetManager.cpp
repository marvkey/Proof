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
	std::vector<std::string> AssetManager::s_PermitableMeshSourceFile = { "obj","fbx","blend"};
	void AssetManager::NewAsset(AssetID ID,const Count<Asset>& asset) {
		while (ID == 0) {
			PF_CORE_ASSERT(false,"ID cannot be 0");
			ID = AssetManager::CreateID();
		}
		if (HasID(ID) == false) {
			s_AssetManager->m_AllAssets.insert({ ID,{AssetInfo(asset->GetPath(), asset->GetAssetType(),ID),asset} });
			if(asset->GetAssetType() == AssetType::PhysicsMaterial)
				s_AssetManager->m_AllPhysicsMaterialAsset.insert({ ID,ForceGetAssetShared<PhysicsMaterialAsset>(ID)});

			return;
		}
		PF_ASSERT(false,"Asset Maneger Has ID");
	}
	bool AssetManager::HasID(AssetID ID) {
		return s_AssetManager->m_AllAssets.find(ID) != s_AssetManager->m_AllAssets.end();
	}
	AssetID AssetManager::CreateID() {
		AssetID ID = AssetID();
		while (HasID(ID) == true || ID ==0) {
			ID = AssetID();
		}
		return ID;
	}
	void AssetManager::Remove(AssetID ID) {
		if (HasID(ID) == true) {

			if (s_AssetManager->m_AllAssets.at(ID).first.Type == AssetType::PhysicsMaterial)
				s_AssetManager->m_AllPhysicsMaterialAsset.erase(ID);

			s_AssetManager->m_AllAssets.erase(ID);
			return;
		}
		PF_ASSERT(false,"Asset Manager does not have ID");
	}
	void AssetManager::NotifyOpenedNewLevel(std::set<AssetID> assetLoadIn){
		AssetManager::GenerateAsset(assetLoadIn);
		//std::thread thread_obj(AssetManager::GenerateAsset, assetLoadIn);
		//thread_obj.join();
	}
	void AssetManager::NotifyOpenedNewAsset(AssetID ID){
		/*
		if (HasID(ID)) {
			Count<Asset> asset = ForceGetAssetShared<Asset>(ID);
			if (asset == nullptr) {
				std::thread thread_obj(AssetManager::GenerateAsset, ID);
			}
		}
		*/
	}
	void AssetManager::SaveAllAsset() {
		for(auto& asset: s_AssetManager->m_AllAssets){
			if (asset.second.second != nullptr) {
				asset.second.second->SaveAsset();
			}
		};
	}
	bool AssetManager::LoadAsset(AssetID ID) {
		auto it = s_AssetManager->m_AllAssets.find(ID);
		if (it == s_AssetManager->m_AllAssets.end())
			return false;

		if (it->second.second != nullptr)
			return true;
		if (it->second.first.Type == AssetType::Mesh) {
			it->second.second = CreateCount<MeshAsset>();
			it->second.second->LoadAsset(it->second.first.Path.string());
			it->second.second->m_AssetName = Utils::FileDialogs::GetFileName(it->second.first.Path);
			return true;
		}
		if (it->second.first.Type == AssetType::Material) {
			it->second.second = CreateCount<MaterialAsset>();
			it->second.second->LoadAsset(it->second.first.Path.string());
			it->second.second->m_AssetName = Utils::FileDialogs::GetFileName(it->second.first.Path);
			return true;
		}
		if (it->second.first.Type == AssetType::PhysicsMaterial) {
			it->second.second = CreateCount<PhysicsMaterialAsset>();
			it->second.second->LoadAsset(it->second.first.Path.string());
			it->second.second->m_AssetName = Utils::FileDialogs::GetFileName(it->second.first.Path);
			return true;
		}
	}
	void AssetManager::InitilizeAssets(const std::string& Path) {
		for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(Path)) {
			if (dirEntry.is_directory())
				continue;
			
			if (IsFileValid(dirEntry.path().string())) {

				if (GetAssetType(dirEntry.path().string()) == AssetType::Material) {
					Count<Texture2DAsset> asset = CreateCount<Texture2DAsset>();
					asset->LoadAsset(dirEntry.path().string());
					asset->m_AssetName = dirEntry.path().stem().string();
					AssetManager::NewAsset(asset->GetAssetID(),asset);
					continue;
				}

				if (GetAssetType(dirEntry.path().string()) == AssetType::Mesh) {
					Count<MeshAsset> asset = CreateCount<MeshAsset>();
					asset->LoadAsset(dirEntry.path().string());
					AssetManager::NewAsset(asset->GetAssetID(),asset);
					asset->m_AssetName = dirEntry.path().stem().string();
					continue;
				}

				if (GetAssetType(dirEntry.path().string()) == AssetType::Material) {
					Count<MaterialAsset> asset = CreateCount<MaterialAsset>();
					asset->LoadAsset(dirEntry.path().string());
					AssetManager::NewAsset(asset->GetAssetID(),asset);
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
			out<<YAML::Key<<"Type"<<EnumReflection::EnumString<AssetType>(asset.second.second->GetAssetType());
			out<<YAML::Key<<"Path"<<asset.second.second->GetPath();
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
	AssetType AssetManager::GetAssetType(const std::string& Path) {
		YAML::Node data = YAML::LoadFile(Path);
		auto type= data["AssetType"].as<std::string>();
		
		return EnumReflection::StringEnum<AssetType>(type);
	}
	void AssetManager::NewInitilizeAssets(const std::string& path) {
		YAML::Node data = YAML::LoadFile(path);

		auto assets = data["ProjectAssets"];

	//	if (!assets)
		//	return;

		for (const auto& asset : assets) {

			AssetID assetID = asset["Asset"].as<uint64_t>();
			std::string path = asset["Path"].as<std::string>();
			auto assetType = EnumReflection::StringEnum<AssetType>(asset["Type"].as<std::string>());
			s_AssetManager->m_AllAssets.insert({ assetID,{AssetInfo(path,assetType,assetID),nullptr} });// setting the asset as null as we will load it in another thread

			if (assetType == AssetType::PhysicsMaterial)
				s_AssetManager->m_AllPhysicsMaterialAsset.insert({ assetID,ForceGetAssetShared<PhysicsMaterialAsset>(assetID) });
		}
	}
	void AssetManager::NewSaveAllAsset(const std::string& Path) {
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Project: " << YAML::Value << "Proof Engine Test";
		out << YAML::Key << "Total Asset: " << YAML::Value << s_AssetManager->m_AllAssets.size();
		out << YAML::Key << "ProjectAssets" << YAML::Value << YAML::BeginSeq;
		for (auto& asset : s_AssetManager->m_AllAssets) {
			if (asset.second.second != nullptr) {
				asset.second.second->SaveAsset();
			}
			out << YAML::BeginMap;
			out << YAML::Key << "Asset" << YAML::Value << asset.first;
			out << YAML::Key << "Type" << EnumReflection::EnumString<AssetType>(asset.second.first.Type);
			out << YAML::Key << "Path" << asset.second.first.Path.string();
			out << YAML::EndMap;
		};

		std::ofstream found(Path);
		found << out.c_str();
		found.close();
	}
	void AssetManager::GenerateAsset(std::set<AssetID> assetLoadIn){
		for (AssetID assetID : assetLoadIn) {
			if (HasID(assetID) == false)continue;

			auto& asset = s_AssetManager->m_AllAssets.at(assetID);

			if (asset.second != nullptr)
				continue;

			if (asset.first.Type == AssetType::Texture) {
				asset.second = CreateCount <Texture2DAsset>();
				asset.second->LoadAsset(asset.first.Path.string());
				asset.second->m_AssetName = asset.first.GetName();

				continue;
			}
			if (asset.first.Type == AssetType::Material) {
				asset.second = CreateCount<MaterialAsset>();
				asset.second->LoadAsset(asset.first.Path.string());
				asset.second->m_AssetName = asset.first.GetName();

				continue;
			}
			if (asset.first.Type == AssetType::Mesh) {
				asset.second = CreateCount <MeshAsset>();
				asset.second->LoadAsset(asset.first.Path.string());
				asset.second->m_AssetName = asset.first.GetName();
				
				continue;
			}
		}
	}
}