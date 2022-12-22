#include "Proofprch.h"
#include "AssetManager.h"
#include "Asset.h"
#include "Proof/Math/Math.h"
#include <filesystem>
#include <yaml-cpp/yaml.h>
#include "TextureAsset/TextureAsset.h"
#include "MeshAsset.h"
#include "Asset.h"
#include "MeshAsset.h"
#include "MaterialAsset.h"
#include "PhysicsMaterialAsset.h"
#include "AssetSupport.h"
#include "Proof/Utils/PlatformUtils.h"
namespace Proof
{
	#define MESH_EXTENSION "Mesh.ProofAsset"
	#define MATERIAL_EXTENSION "Material.ProofAsset"
	struct AssetManagerData {
		std::unordered_map<AssetID, AssetContainer> Assets;// path

		//temporay 
		std::unordered_map<std::string, AssetID> AssetSource;// path
		std::filesystem::path AssetDirectory;
		std::filesystem::path AssetRegistry;
	};
	static AssetManagerData* s_AssetManagerData;

	void AssetManager::Init(AssetManagerConfiguration& assetManagerConfiguration) {
		s_AssetManagerData = new AssetManagerData();
		s_AssetManagerData->AssetDirectory = assetManagerConfiguration.AssetDirectory;
		s_AssetManagerData->AssetRegistry = assetManagerConfiguration.AssetManager;
		if (std::filesystem::exists(assetManagerConfiguration.AssetDirectory) == false) {
			std::filesystem::create_directory(assetManagerConfiguration.AssetDirectory);
		}
		if (std::filesystem::exists(s_AssetManagerData->AssetRegistry) == false) {
			SaveAllAssets();
		}
	}
	void AssetManager::UnInizilize() {
		SaveAllAssets();
		delete s_AssetManagerData;
		s_AssetManagerData = nullptr;
	}
	std::unordered_map<AssetID, AssetContainer>& AssetManager::GetAssets() {
		return s_AssetManagerData->Assets;
	}
	AssetInfo AssetManager::GetAssetInfo(AssetID ID) {
		auto& it = GetAssets()[ID];
		return it.Info;
	}
	void AssetManager::NewAsset(AssetID ID, const Count<Asset>& asset) {
		if (ID == 0) {
			PF_CORE_ASSERT(false, "ID cannot be 0");
		}
		PF_ASSERT(HasID(ID) == false, "Asset Manager Has ID");
		AssetInfo assetInfo;
		assetInfo.ID = ID;
		assetInfo.Path = std::filesystem::relative(asset->m_SavePath);
		assetInfo.Loaded = false;
		assetInfo.AssetSource = false;
		assetInfo.Type = asset->GetAssetType();
		s_AssetManagerData->Assets.insert({ ID,{assetInfo,asset} });
	}
	bool AssetManager::HasID(AssetID ID) {
		return s_AssetManagerData->Assets.contains(ID);
	}
	void AssetManager::NewAssetSource(AssetID ID, const std::filesystem::path& path) {
		if (ID == 0) {
			PF_CORE_ASSERT(false, "ID cannot be 0");
		}
		PF_ASSERT(HasID(ID) == false, "Asset Manager Has ID");
		AssetInfo assetInfo;
		assetInfo.ID = ID;
		assetInfo.Path = std::filesystem::relative(path);
		assetInfo.Loaded = false;
		assetInfo.AssetSource = true;
		auto extension = Utils::FileDialogs::GetFileExtension(path);
		if (MeshHasFormat(extension))
			assetInfo.Type = AssetType::MeshSourceFile;
		else if(TextureHasFormat(extension))
			assetInfo.Type = AssetType::TextureSourceFile;

		s_AssetManagerData->Assets.insert({ ID,{assetInfo,nullptr} });
		s_AssetManagerData->AssetSource.insert({ assetInfo.Path.string(),ID});
	}
	void AssetManager::UnloadAsset(AssetID ID) {
		PF_ASSERT(false, "Funciton not ready");

		PF_ASSERT(HasID(ID) == false, "Asset Manager does not have ID");
		auto& AssetInfo = s_AssetManagerData->Assets[ID].Info;
		auto asset = s_AssetManagerData->Assets[ID].Asset;
	}

	void GenerateAsset(const std::filesystem::path& path) {
		if (s_AssetManagerData->AssetSource.contains(path.string()) == true)
			return;
		AssetManager::NewAssetSource(AssetID(), path.string());
	}
	void AssetManager::GenerateAllSourceAssets() {
		for (auto& it : std::filesystem::recursive_directory_iterator(s_AssetManagerData->AssetDirectory)) {
			if (MeshHasFormat(it.path().extension().string())) {
				GenerateAsset(it.path());
				return;
			}

			if (TextureHasFormat(it.path().extension().string())) {
				GenerateAsset(it.path());
				return;
			}
		}
	}
	AssetID AssetManager::CreateID() {
		AssetID ID = AssetID();
		while (HasID(ID) == true || ID ==0) {
			ID = AssetID();
		}
		return ID;
	}
	void AssetManager::Remove(AssetID ID) {
		PF_ASSERT(HasID(ID) != true, "Asset Manager does not have ID");
		auto info = s_AssetManagerData->Assets.at(ID).Info;
		s_AssetManagerData->Assets.erase(ID);

		if(info.AssetSource == true)
			s_AssetManagerData->Assets.erase(ID);
	}
	AssetType AssetManager::GetAssetFromFilePath(const std::filesystem::path& path){
		const std::string fileFullExtension = Utils::FileDialogs::GetFullFileExtension(path);
		if (fileFullExtension == MESH_EXTENSION)
			return AssetType::Mesh;
		if (fileFullExtension == MATERIAL_EXTENSION)
			return AssetType::Material;
		if (fileFullExtension == "Texture.ProofAsset")
			return AssetType::Texture;
		if (fileFullExtension == "ProofWorld")
			return AssetType::World;
		if (fileFullExtension == "PhysicsMaterial.ProofAsset")
			return AssetType::PhysicsMaterial;

		if (MeshHasFormat(Utils::FileDialogs::GetFileExtension(path)))return AssetType::MeshSourceFile;
		if (TextureHasFormat(Utils::FileDialogs::GetFileExtension(path)))return AssetType::TextureSourceFile;
		const std::string fileDirectExtension = Utils::FileDialogs::GetFileExtension(path);
		return AssetType::None;
	}
	AssetID AssetManager::GetAssetSourceID(const std::filesystem::path& path) {
		auto relateivePath = std::filesystem::relative(path);
		if (s_AssetManagerData->AssetSource.contains(relateivePath.string())) {
			return s_AssetManagerData->AssetSource.at(relateivePath.string());
		}

		//if (std::filesystem::exists(path)) {
		//	NewAssetSource(AssetID(), relateivePath);
		//	return s_AssetManagerData->AssetSource.at(relateivePath.string());
		//}
		PF_CORE_ASSERT(false, "Asset does not exist");
		return 0;
	}

	std::string AssetManager::GetAssetSourcePath(AssetID ID) {
		if (s_AssetManagerData->Assets.contains(ID)) {
			return s_AssetManagerData->Assets.at(ID).Info.Path.string();
		}
		return {};
	}
	

	template <class T>
	void LoadAssetTemplate(AssetID ID) {
		auto it = s_AssetManagerData->Assets.find(ID);
		auto& assetInfo = s_AssetManagerData->Assets[ID].Info;
		it->second.Asset = CreateCount<T>();
		s_AssetManagerData->Assets[ID].Asset->LoadAsset(assetInfo.Path.string());
		assetInfo.Loaded = true;
	}
	bool AssetManager::LoadAsset(AssetID ID) {
		PF_ASSERT(HasID(ID) == true, "Asset Manager does not have ID");
		
		auto& asset = s_AssetManagerData->Assets[ID].Asset;
		auto& assetInfo = s_AssetManagerData->Assets[ID].Info;

		switch (assetInfo.Type) {
			case Proof::AssetType::None:
				break;
			case Proof::AssetType::Mesh:
				LoadAssetTemplate<MeshAsset>(ID);
				break;
			case Proof::AssetType::Texture:
				LoadAssetTemplate<Texture2DAsset>(ID);
				break;
			case Proof::AssetType::Material:
				LoadAssetTemplate<MaterialAsset>(ID);
				break;
			case Proof::AssetType::World:
				break;
			case Proof::AssetType::MeshSourceFile:
				LoadAssetTemplate<MeshSourceFileAsset>(ID);
				break;
			case Proof::AssetType::PhysicsMaterial:
				LoadAssetTemplate<PhysicsMaterialAsset>(ID);
				break;
			default:
				break;
		}
		return true;
		
	}
	bool AssetManager::IsFileValid(const std::string& Path) {
		YAML::Node data = YAML::LoadFile(Path);
		if (!data["AssetType"]) // if there is no scene no
			return false;
		return true;
	}

	void AssetManager::InitilizeAssets() {
		YAML::Node data = YAML::LoadFile(s_AssetManagerData->AssetRegistry.string());
		auto assets = data["Assets"];
		if (!assets)return;

		for (const auto& asset : assets) {

			AssetID assetID = asset["Asset"].as<uint64_t>();
			std::string path = asset["Path"].as<std::string>();
			auto assetType = EnumReflection::StringEnum<AssetType>(asset["Type"].as<std::string>());
			bool assetSource = asset["AssetSource"].as<bool>();
			AssetInfo assetInfo;
			assetInfo.ID = assetID;
			assetInfo.Path = path;
			assetInfo.Loaded = false;
			assetInfo.AssetSource = assetSource;
			assetInfo.Type = assetType;
			s_AssetManagerData->Assets.insert({ assetID,{assetInfo,nullptr} });// setting the asset as null as we will load it in another thread
			if (assetSource)
				s_AssetManagerData->AssetSource.insert({ std::filesystem::relative(path).string(),assetID });
		}
	}
	void AssetManager::SaveAllAssets() {
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Assets" << YAML::Value << YAML::BeginSeq;

		for (auto& [id,assetManager] : s_AssetManagerData->Assets) {
			const auto& assetInfo = assetManager.Info;
			const auto& asset = assetManager.Asset;
			if (assetInfo.Loaded) {
				asset->SaveAsset();
			}
			out << YAML::BeginMap;
			out << YAML::Key << "Asset" << YAML::Value << assetInfo.ID;
			out << YAML::Key << "Type" << EnumReflection::EnumString<AssetType>(assetInfo.Type);
			out << YAML::Key << "Path" << assetInfo.Path.string();
			out << YAML::Key << "AssetSource" << assetInfo.AssetSource;
			out << YAML::EndMap;
		};
		out << YAML::EndMap;

		std::ofstream found(s_AssetManagerData->AssetRegistry);
		found << out.c_str();
		found.close();
	}
	void AssetManager::LoadMultipleAsset(std::set<AssetID> assetLoadIn){
		for (AssetID assetID : assetLoadIn) {
			if (HasID(assetID) == false)continue;

			auto asset = s_AssetManagerData->Assets[assetID].Asset;
			auto assetInfo = s_AssetManagerData->Assets[assetID].Info;
			if (asset != nullptr)
				continue;
			switch (assetInfo.Type) {
				case Proof::AssetType::None:
					break;
				case Proof::AssetType::Mesh:
					LoadAssetTemplate<MeshAsset>(assetID);
					break;
				case Proof::AssetType::Texture:
					LoadAssetTemplate<Texture2DAsset>(assetID);
					break;
				case Proof::AssetType::Material:
					LoadAssetTemplate<MaterialAsset>(assetID);
					break;
				case Proof::AssetType::World:
					break;
				case Proof::AssetType::MeshSourceFile:
					break;
				case Proof::AssetType::PhysicsMaterial:
					LoadAssetTemplate<PhysicsMaterialAsset>(assetID);
					break;
				default:
					break;
			}
		}
	}
}