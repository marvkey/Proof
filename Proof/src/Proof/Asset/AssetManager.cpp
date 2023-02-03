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
	#define TEXTURE_EXTENSION "Texture.ProofAsset"
	struct AssetManagerData {
		std::unordered_map<AssetID, AssetContainer> Assets;// path

		//temporay 
		std::unordered_map<std::string, AssetID> AssetPath;// path
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
	std::unordered_map<std::string, AssetID>& AssetManager::GetAssetByPath()
	{
		return s_AssetManagerData->AssetPath;
	}
	AssetInfo AssetManager::GetAssetInfo(AssetID ID) {
		auto& it = GetAssets()[ID];
		return it.Info;
	}
	bool AssetManager::HasID(AssetID ID) {
		return s_AssetManagerData->Assets.contains(ID);
	}
	void AssetManager::NewAsset(const Count<Asset>& asset) {
		if(asset ==nullptr)
			PF_CORE_ASSERT(false, "Asset cannot be nullptr");
		AssetInfo assetInfo;
		assetInfo.ID = asset->GetAssetID();
		assetInfo.Path = std::filesystem::relative(asset->m_SavePath, s_AssetManagerData->AssetDirectory);
		assetInfo.Type = asset->GetAssetType();
		assetInfo.State = AssetState::Unloaded;
		if (asset->GetAssetType() == AssetType::MeshSourceFile)
			PF_CORE_ASSERT(false);
		s_AssetManagerData->Assets.insert({ asset->GetAssetID(),{assetInfo,asset} });
		s_AssetManagerData->AssetPath.insert({ assetInfo.Path.string(),asset->GetAssetID() });
	}

	void AssetManager::AddMemoryAsset(const Count<Asset>& asset)
	{
	
	}
	void AssetManager::NewAsset(AssetID ID, const std::filesystem::path& path) {
		if (ID == 0) {
			PF_CORE_ASSERT(false, "ID cannot be 0");
		}
		PF_ASSERT(HasID(ID) == false, "Asset Manager Has ID");
		AssetInfo assetInfo;
		assetInfo.ID = ID;
		assetInfo.Path = std::filesystem::relative(path, s_AssetManagerData->AssetDirectory);
		auto extension = Utils::FileDialogs::GetFileExtension(path);
		if (MeshHasFormat(extension))
			assetInfo.Type = AssetType::MeshSourceFile;
		else if(TextureHasFormat(extension))
			assetInfo.Type = AssetType::TextureSourceFile;
		else {
			PF_ERROR("Extension .{} not supported ", extension);
			return;
		}
		assetInfo.State = AssetState::Unloaded;
		s_AssetManagerData->Assets.insert({ ID,{assetInfo,nullptr} });
		s_AssetManagerData->AssetPath.insert({ assetInfo.Path.string(),ID});
	}
	void AssetManager::AddWorldAsset(AssetID ID, const std::filesystem::path& path)
	{
		if (ID == 0) {
			PF_CORE_ASSERT(false, "ID cannot be 0");
		}
		PF_ASSERT(HasID(ID) == false, "Asset Manager Has ID");
		std::filesystem::path finalPath = std::filesystem::relative(path, s_AssetManagerData->AssetDirectory);
		if (finalPath.has_parent_path())
		{
			auto parentDir = finalPath.parent_path();
			finalPath = parentDir /= {Utils::FileDialogs::GetFileName(path) + "." + "ProofWorld"};
		}
		else
		{
			finalPath = { Utils::FileDialogs::GetFileName(path) + "." + "ProofWorld" };
		}

		AssetInfo assetInfo;
		assetInfo.ID = ID;
		assetInfo.Path = finalPath;
		assetInfo.Type = AssetType::World;
		s_AssetManagerData->Assets.insert({ ID,{assetInfo,nullptr} });
		s_AssetManagerData->AssetPath.insert({ assetInfo.Path.string(),ID });
	}
	void AssetManager::UnloadAsset(AssetID ID) {
		PF_ASSERT(false, "Funciton not ready");

		PF_ASSERT(HasID(ID) == false, "Asset Manager does not have ID");
		auto& AssetInfo = s_AssetManagerData->Assets[ID].Info;
		auto asset = s_AssetManagerData->Assets[ID].Asset;
	}

	void GenerateAsset(const std::filesystem::path& path) {
		if (s_AssetManagerData->AssetPath.contains(path.string()) == false)
			return;
		AssetManager::NewAsset(AssetManager::CreateID(), path.string());
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
		while (HasID(ID) == true) {
			ID = AssetID();
		}
		return ID;
	}
	void AssetManager::Remove(AssetID ID) {
		auto info = s_AssetManagerData->Assets.at(ID).Info;
		s_AssetManagerData->Assets.erase(ID);
		s_AssetManagerData->AssetPath.erase(info.Path.string());
	}
	AssetType AssetManager::GetAssetTypeFromFilePath(const std::filesystem::path& path){
		const std::string fileFullExtension = Utils::FileDialogs::GetFullFileExtension(path);
		if (fileFullExtension == MESH_EXTENSION)
			return AssetType::Mesh;
		if (fileFullExtension == MATERIAL_EXTENSION)
			return AssetType::Material;
		if (fileFullExtension == TEXTURE_EXTENSION)
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

	AssetInfo AssetManager::GetAssetInfo(const std::filesystem::path& path)
	{
		return s_AssetManagerData->Assets[s_AssetManagerData->AssetPath.at(path.string())].Info;
	}

	bool AssetManager::HasAsset(const std::filesystem::path& path)
	{
		return s_AssetManagerData->AssetPath.contains(path.string());
	}
	

	template <class T>
	void LoadAssetTemplate(AssetID ID) {
		auto it = s_AssetManagerData->Assets.find(ID);
		auto& assetInfo = s_AssetManagerData->Assets[ID].Info;
		it->second.Asset = CreateCount<T>();
		auto path = Application::Get()->GetProject()->GetAssetFileSystemPath(assetInfo.Path);
		s_AssetManagerData->Assets[ID].Asset->LoadAsset(path.string());
		assetInfo.State = AssetState::Ready;
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
	

	void AssetManager::InitilizeAssets() {
		YAML::Node data = YAML::LoadFile(s_AssetManagerData->AssetRegistry.string());
		auto assets = data["Assets"];
		if (!assets)return;

		for (const auto& asset : assets) {

			AssetID assetID = asset["Asset"].as<uint64_t>();
			std::string path = asset["Path"].as<std::string>();
			auto assetType = EnumReflection::StringEnum<AssetType>(asset["Type"].as<std::string>());
			AssetInfo assetInfo;
			assetInfo.ID = assetID;
			assetInfo.Path = path;
			assetInfo.Type = assetType;
			assetInfo.State = AssetState::Unloaded;
			s_AssetManagerData->Assets.insert({ assetID,{assetInfo,nullptr} });// setting the asset as null as we will load it in another thread
			s_AssetManagerData->AssetPath.insert({ path,assetID });
		}
	}
	void AssetManager::SaveAllAssets() {
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Assets" << YAML::Value << YAML::BeginSeq;

		for (auto& [id,assetManager] : s_AssetManagerData->Assets) {
			
			const auto& assetInfo = assetManager.Info;
			const auto& asset = assetManager.Asset;
			if (assetInfo.State == AssetState::Ready) {
				asset->SaveAsset();
			}
			out << YAML::BeginMap;
			out << YAML::Key << "Asset" << YAML::Value << assetInfo.ID;
			out << YAML::Key << "Type" << EnumReflection::EnumString<AssetType>(assetInfo.Type);
			out << YAML::Key << "Path" << assetInfo.Path.string();
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