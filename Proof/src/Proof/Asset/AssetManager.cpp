#include "Proofprch.h"
#include "AssetManager.h"
#include "Asset.h"
#include "Proof/Math/Math.h"
#include <filesystem>
#include <yaml-cpp/yaml.h>
#include "Asset.h"
#include "AssetSupport.h"
#include "Proof/Utils/PlatformUtils.h"
#include "AssetSerelizer.h"
#include <thread>
#include  <algorithm>
#include <execution>
#include "Proof/Renderer/Texture.h"
#include <future>
namespace Proof
{
	#define MESH_EXTENSION "Mesh.ProofAsset"
	#define MATERIAL_EXTENSION "Material.ProofAsset"
	#define TEXTURE_EXTENSION "Texture.ProofAsset"
	#define WORLD_EXTENSION "ProofWorld"
	#define PHYSICS_MATERIAL "PhysicsMaterial.ProofAsset"

	static Special< AssetManagerData> s_AssetManagerData;

	void AssetManager::Init(AssetManagerConfiguration& assetManagerConfiguration) {
		s_AssetManagerData =CreateSpecial< AssetManagerData>();

		s_AssetManagerData->AssetDirectory = assetManagerConfiguration.AssetDirectory;
		s_AssetManagerData->AssetRegistry = assetManagerConfiguration.AssetManager;
		{
			s_AssetManagerData->AssetSerilizer[AssetType::Material] = CreateSpecial<MaterialAssetSerializer>();
			s_AssetManagerData->AssetSerilizer[AssetType::PhysicsMaterial] = CreateSpecial<PhysicsMaterialAssetSerializer>();
			s_AssetManagerData->AssetSerilizer[AssetType::Mesh] = CreateSpecial<MeshAssetSerializer>();
			s_AssetManagerData->AssetSerilizer[AssetType::Texture] = CreateSpecial<TextureAssetSerializer>();
			s_AssetManagerData->AssetSerilizer[AssetType::MeshSourceFile] = CreateSpecial<MeshSourceAssetSerializer>();
			s_AssetManagerData->AssetSerilizer[AssetType::Prefab] = CreateSpecial<PrefabAssetSerilizer>();
			s_AssetManagerData->AssetSerilizer[AssetType::UIPanel] = CreateSpecial<UIPanelAssetSerilizer>();
			s_AssetManagerData->AssetSerilizer[AssetType::ParticleSystem] = CreateSpecial<ParticleSystemSerilizer>();
		}
		if (std::filesystem::exists(assetManagerConfiguration.AssetDirectory) == false) {
			std::filesystem::create_directory(assetManagerConfiguration.AssetDirectory);
		}
		if (std::filesystem::exists(s_AssetManagerData->AssetRegistry) == false) {
			SaveAllAssets();
		}
		AssetManager::InitilizeAssets();
	}
	void AssetManager::UnInizilize() {
		//SaveAllAssets();
		s_AssetManagerData->Assets.clear();
		s_AssetManagerData = nullptr;
	}
	void AssetManager::AddAsset(AssetInfo assetInfo, Count<Asset> asset)
	{
		//GetAssets().insert({ assetInfo.ID,{assetInfo,asset} });
		//GetAssetByPath().insert({ assetInfo.Path.string(),assetInfo.ID });
	}
	std::unordered_map<AssetID, AssetContainer>& AssetManager::GetAssets() {
		return s_AssetManagerData->Assets;
	}
	std::unordered_map<std::filesystem::path, AssetID>& AssetManager::GetAssetByPath()
	{
		return s_AssetManagerData->AssetPath;
	}
	AssetInfo AssetManager::GetAssetInfo(AssetID ID) {
		PF_CORE_ASSERT(HasAsset(ID), "Does not contian assetID");
		auto& it = GetAssets().at(ID);
		return it.Info;
	}
	std::string AssetManager::GetExtension(AssetType type)
	{
		switch (type)
		{
			case Proof::AssetType::None:
				return "";
			case Proof::AssetType::Mesh:
				return "Mesh.ProofAsset";
			case Proof::AssetType::Texture:
				return "Texture.ProofAsset";
			case Proof::AssetType::Material:
				return "Material.ProofAsset";
			case Proof::AssetType::World:
				return "ProofWorld";
			case Proof::AssetType::MeshSourceFile:
				return "";
			case Proof::AssetType::PhysicsMaterial:
				return "PhysicsMaterial.ProofAsset";
			case Proof::AssetType::Prefab:
				return "Prefab.ProofAsset";
			case Proof::AssetType::TextureSourceFile:
				return "";
			case Proof::AssetType::UIPanel:
				return "UIPanel.ProofAsset";
			case Proof::AssetType::ParticleSystem:
				return "ParticleSystem.ProofAsset";
			default:
				break;
		}
		PF_CORE_ASSERT(false,fmt::format("Asset Type {} does not have extension", EnumReflection::EnumString(type)).c_str());
	}
	void AssetManager::NewAssetSource(const std::filesystem::path& path, AssetType type)
	{
		if (type == AssetType::MeshSourceFile && MeshHasFormat(Utils::FileDialogs::GetFileExtension(path)))
		{
			AssetInfo assetInfo;
			assetInfo.Path = std::filesystem::relative(path, AssetManager::GetDirectory());
			assetInfo.State = AssetState::Unloaded;
			assetInfo.ID = AssetManager::CreateID();
			assetInfo.Type = AssetType::MeshSourceFile;

			GetAssets().insert({ assetInfo.ID,{assetInfo,nullptr} });
			GetAssetByPath().insert({ assetInfo.Path.string(),assetInfo.ID });
			return;
		}

		if (type == AssetType::TextureSourceFile && TextureHasFormat(Utils::FileDialogs::GetFileExtension(path)))
		{
			AssetInfo assetInfo;
			assetInfo.Path = std::filesystem::relative(path, AssetManager::GetDirectory());
			assetInfo.State = AssetState::Ready;
			assetInfo.ID = AssetManager::CreateID();
			assetInfo.Type = AssetType::TextureSourceFile;

			GetAssets().insert({ assetInfo.ID,{assetInfo,nullptr}});
			GetAssetByPath().insert({ assetInfo.Path.string(),assetInfo.ID });
			return;
		}
	}
	bool AssetManager::HasAsset(AssetID ID) {
		if (ID == 0)return false;
		return s_AssetManagerData->Assets.contains(ID);
	}
	void AssetManager::AddWorldAsset(AssetID ID, const std::filesystem::path& path)
	{
		if (ID == 0) {
			PF_CORE_ASSERT(false, "ID cannot be 0");
		}
		PF_ASSERT(HasAsset(ID) == false, "Asset Manager Has ID");
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
		//s_AssetManagerData->Assets[ID].Asset = nullptr;
		PF_ASSERT(false, "Funciton not ready");

		PF_ASSERT(HasAsset(ID) == false, "Asset Manager does not have ID");
		auto& AssetInfo = s_AssetManagerData->Assets[ID].Info;
		auto asset = s_AssetManagerData->Assets[ID].Asset;
	}

	
	void AssetManager::GenerateAllSourceAssets() {
		for (auto& it : std::filesystem::recursive_directory_iterator(s_AssetManagerData->AssetDirectory)) {
			if (AssetManager::HasAsset(it.path()))continue;
			std::string extension = it.path().extension().string();
			extension.erase(extension.begin());// remove the "."
			if (MeshHasFormat(extension)) {
				NewAssetSource(it.path(),AssetType::MeshSourceFile);
				continue;
			}

			if (TextureHasFormat(extension)) {
				NewAssetSource(it.path(), AssetType::TextureSourceFile);
				std::string path = std::filesystem::relative(it.path().parent_path() /= Utils::FileDialogs::GetFileName(it.path())).string();
				path += ".Texture.ProofAsset";
				Count<Asset> asset = Texture2D::Create(TextureConfiguration(Utils::FileDialogs::GetFileName(it.path())), it.path());
				AssetManager::NewAsset(asset, path);
				continue;
			}
		}
	}
	AssetID AssetManager::CreateID() {
		AssetID ID = AssetID();
		while (HasAsset(ID) == true) {
			ID = AssetID();
		}
		return ID;
	}
	void AssetManager::Remove(AssetID ID) {
		PF_CORE_ASSERT(HasAsset(ID), "Asset does not exist");
		auto info = s_AssetManagerData->Assets.at(ID).Info;
		s_AssetManagerData->Assets.erase(ID);
		s_AssetManagerData->AssetPath.erase(info.Path);
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
		PF_CORE_ASSERT(HasAsset(path), "Does not conatin Asset");
		auto changepath = AssetManager::GetAssetFileSystemPathRelative(path);
		return s_AssetManagerData->Assets[s_AssetManagerData->AssetPath.at(changepath.string())].Info;
	}

	bool AssetManager::HasAsset(const std::filesystem::path& path)
	{
		auto changepath  = AssetManager::GetAssetFileSystemPathRelative(path);
		return s_AssetManagerData->AssetPath.contains(changepath.string());
	}
	
	bool AssetManager::LoadAsset(AssetID ID) {
		PF_ASSERT(HasAsset(ID) == true, "Asset Manager does not have ID");
		
		auto& assetInfo = s_AssetManagerData->Assets[ID].Info;
		if (!s_AssetManagerData->AssetSerilizer.contains(assetInfo.Type))
			return false;

		 auto asset= s_AssetManagerData->AssetSerilizer.at(assetInfo.Type)->TryLoadAsset(assetInfo);
		 if (asset)
		 {
			 assetInfo.State = AssetState::Ready;
			 s_AssetManagerData->Assets[ID].Asset = asset;
		 }
		return asset != nullptr;
	}

	AssetManagerData const* AssetManager::GetAssetManagerData()
	{
		return s_AssetManagerData.get();
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

			if(assetType == AssetType::TextureSourceFile)
				assetInfo.State = AssetState::Ready;
			else
				assetInfo.State = AssetState::Unloaded;

			s_AssetManagerData->Assets.insert({ assetID,{assetInfo,nullptr} });// setting the asset as null as we will load it in another thread
			s_AssetManagerData->AssetPath.insert({ path,assetID });
		}
	}
	void AssetManager::SaveAsset(AssetID Id) {

		PF_CORE_ASSERT(AssetManager::HasAsset(Id), "trying to save asset that does not exist");
		const auto& assetInfo = GetAssetInfo(Id);
		if (assetInfo.RuntimeAsset)
			return;
		if (!assetInfo.IsAssetSource() && assetInfo.State == AssetState::Ready)
		{
			PF_CORE_ASSERT(s_AssetManagerData->AssetSerilizer.contains(assetInfo.Type), "AssetManager Save Does not contain speicfied type");
			s_AssetManagerData->AssetSerilizer.at(assetInfo.Type)->Save(assetInfo, AssetManager::GetAsset<Asset>(Id));
		}

	}
	void AssetManager::SaveAllAssets() {
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Assets" << YAML::Value << YAML::BeginSeq;

		for (auto& [id,assetManager] : s_AssetManagerData->Assets) {
			
			const auto& assetInfo = assetManager.Info;
			const auto& asset = assetManager.Asset;
			if (assetInfo.RuntimeAsset)
				continue;
			if (assetInfo.State == AssetState::Ready && assetInfo.IsAssetSource() ==false) {
				if (s_AssetManagerData->AssetSerilizer.contains(assetInfo.Type))
					s_AssetManagerData->AssetSerilizer.at(assetInfo.Type)->Save(assetInfo,asset);
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
	void AssetManager::SaveAssetManager() 
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Assets" << YAML::Value << YAML::BeginSeq;

		for (auto& [id, assetManager] : s_AssetManagerData->Assets)
		{

			const auto& assetInfo = assetManager.Info;
			if (assetInfo.RuntimeAsset)
				continue;
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
		Timer time;
		#if 0
		auto particles_task = std::async(std::launch::async,
			[&]() {
			std::for_each(std::execution::par,
				assetLoadIn.begin(),
				assetLoadIn.end(),
				[](AssetID Part) {
				AssetManager::LoadAsset(Part);
			});
		});
		particles_task.wait();
		#else
		for (auto& id : assetLoadIn)
			LoadAsset(id);
		#endif
		
		PF_ENGINE_INFO("Time Load Asset {}ms ",time.ElapsedMillis());
	}

	void AssetManager::ChangeAssetPath(AssetID ID, const std::filesystem::path& newPath) 
	{
		PF_CORE_ASSERT(AssetManager::HasAsset(ID), "Does not contain asset");
		auto& it = GetAssets().at(ID);

		auto path = std::filesystem::relative(newPath, AssetManager::GetDirectory());
		// changing teh old data in assetBypath
		GetAssetByPath().erase(it.Info.Path);
		// creating the new data
		GetAssetByPath().insert({ path.string(),it.Info.ID });
		// new assetINfo
		it.Info.Path = path;
	}
	std::filesystem::path AssetManager::GetAssetFileSystemPath(const std::filesystem::path& path) {
		return Application::Get()->GetProject()->GetAssetFileSystemPath(path);
	}
	std::filesystem::path AssetManager::GetAssetFileSystemPathRelative(const std::filesystem::path& path) {
		return std::filesystem::relative(path, AssetManager::GetDirectory());
	}
	std::filesystem::path AssetManager::GetDirectory() {
		return Application::Get()->GetProject()->GetAssetDirectory();
	}
}