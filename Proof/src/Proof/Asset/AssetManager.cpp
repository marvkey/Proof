#include "Proofprch.h"
#include "AssetManager.h"
#include "Asset.h"
#include "Proof/Math/Math.h"
#include <filesystem>
#include <yaml-cpp/yaml.h>
#include "Asset.h"
#include "Proof/Utils/PlatformUtils.h"
#include "AssetSerelizer.h"
#include <thread>
#include  <algorithm>
#include <execution>
#include "Proof/Core/Application.h"
#include "Proof/Project/Project.h"
#include "Proof/Renderer/Texture.h"
#include "Proof/Renderer/MeshWorkShop.h"
#include "Proof/Scene/Mesh.h"
#include "Proof/Scene/Material.h"
#include <future>
namespace Proof
{
	
	struct AssetManagerData {
		std::unordered_map<AssetID, AssetContainer> Assets;// path
		std::unordered_map<AssetType, std::unordered_set<AssetID>> AllAssetTypes;

		//temporay 
		//std::unordered_map< AssetType,std::unordered_map<AssetID, 
		std::unordered_map<std::filesystem::path, AssetID> AssetPath;// path
		std::unordered_map<AssetType, Special<class AssetSerializer>> AssetSerilizer;// path
		std::filesystem::path AssetDirectory;
		std::filesystem::path AssetRegistry;
	};
	static Special< AssetManagerData> s_AssetManagerData;

	void AssetManager::Init(AssetManagerConfiguration& assetManagerConfiguration) {
		Timer time;

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

		{
			EnumReflection::ForEach<AssetType>([](AssetType assetType) {
				if (assetType != AssetType::None)
					s_AssetManagerData->AllAssetTypes[assetType] = {};
			});
		}
		if (std::filesystem::exists(assetManagerConfiguration.AssetDirectory) == false) {
			std::filesystem::create_directory(assetManagerConfiguration.AssetDirectory);
		}
		if (std::filesystem::exists(s_AssetManagerData->AssetRegistry) == false) {
			SaveAllAssets();
		}
		
		{
			YAML::Node data = YAML::LoadFile(s_AssetManagerData->AssetRegistry.string());
			auto assets = data["Assets"];
			if (!assets)return;

			for (const auto& asset : assets)
			{

				AssetID assetID = asset["Asset"].as<uint64_t>();
				std::string path = asset["Path"].as<std::string>();
				auto assetType = EnumReflection::StringEnum<AssetType>(asset["Type"].as<std::string>());
				AssetInfo assetInfo;
				assetInfo.ID = assetID;
				assetInfo.Path = path;
				assetInfo.Type = assetType;

				if (assetType == AssetType::TextureSourceFile)
					assetInfo.State = AssetState::Ready;
				else
					assetInfo.State = AssetState::Unloaded;
				InternalAddAsset(assetInfo, nullptr);
				//s_AssetManagerData->Assets.insert({ assetID,{assetInfo,nullptr} });// setting the asset as null as we will load it in another thread
				//s_AssetManagerData->AssetPath.insert({ path,assetID });
			}
		}
		EnumReflection::ForEach< DefaultRuntimeAssets>([](DefaultRuntimeAssets currentEnum) {

			// not loading every single mesh runtime asset bacause we may not need them at all in startup
			uint64_t ID = (uint64_t)currentEnum;

			switch (currentEnum)
			{
				case DefaultRuntimeAssets::Cube:
					{
						Count<Mesh> mesh = MeshWorkShop::GenerateCube();
						Count<Asset> asset = mesh;
						CreateRuntimeAsset(ID, asset, "Cube");
					}
					break;
				case DefaultRuntimeAssets::Sphere:
					{
						Count<Mesh> mesh = MeshWorkShop::GenerateSphere();
						Count<Asset> asset = mesh;
						CreateRuntimeAsset(ID, asset, "Sphere");
					}
					break;
				case DefaultRuntimeAssets::Capsule:
					{
						Count<Mesh> mesh = MeshWorkShop::GenerateCapsule();
						Count<Asset> asset = mesh;
						CreateRuntimeAsset(ID, asset, "Capsule");
					}
					break;
				case DefaultRuntimeAssets::Cylinder:
					{
						Count<Mesh> mesh = MeshWorkShop::GenerateCylinder();
						Count<Asset> asset = mesh;
						CreateRuntimeAsset(ID, asset, "Cylinder");
					}
					break;
				case DefaultRuntimeAssets::Cone:
					{
						Count<Mesh> mesh = MeshWorkShop::GenerateCone();
						Count<Asset> asset = mesh;
						CreateRuntimeAsset(ID, asset, "Cone");

					}
					break;
				case DefaultRuntimeAssets::Torus:
					{
						Count<Mesh> mesh = MeshWorkShop::GenerateTorus(50, 50);
						Count<Asset> asset = mesh;
						CreateRuntimeAsset(ID, asset, "Torus");
					}
					break;
				case DefaultRuntimeAssets::Plane:
					{
						
						Count<Mesh> mesh = MeshWorkShop::GeneratePlane();
						Count<Asset> asset = mesh;
						CreateRuntimeAsset(ID, asset, "Plane");
					}
					break;
				case DefaultRuntimeAssets::Material:
					{
						Count<Material> material = Count<Material>::Create("DefaultMaterial");
						Count<Asset> asset = material.Get();
						CreateRuntimeAsset(ID, asset,"DefaultMaterial");
					}
					break;
				case DefaultRuntimeAssets::PhysicsMaterial:
					{
						Count<PhysicsMaterial> material = Count<PhysicsMaterial>::Create();
						Count<Asset> asset = material.Get();
						CreateRuntimeAsset(ID, asset, "DefaultPhysicsMaterial");
					}
					break;
			}
		});
		PF_ENGINE_INFO("Asset Manager Initialized {}m/s", time.ElapsedMillis());
	}
	void AssetManager::ShutDown() {
		Timer time;

		//SaveAllAssets();
		s_AssetManagerData->Assets.clear();
		s_AssetManagerData = nullptr;
		PF_ENGINE_INFO("Asset Manager Shutdown {}m/s", time.ElapsedMillis());
	}
	void AssetManager::InternalAddAsset(AssetInfo assetInfo, Count<Asset> asset)
	{
		PF_CORE_ASSERT(assetInfo.Type != AssetType::None, "Cannot add asset with assetType none");
		PF_CORE_ASSERT(!HasAsset(assetInfo.ID), "Already has assetId");
		s_AssetManagerData->Assets.insert({ assetInfo.ID,{assetInfo,asset} });
		s_AssetManagerData->AllAssetTypes[assetInfo.Type].insert({ assetInfo.ID });
		s_AssetManagerData->AssetPath.insert({ assetInfo.Path.string(),assetInfo.ID });
		if (asset)
		{
			asset->m_ID = assetInfo.ID;
		}
		if (asset && assetInfo.RuntimeAsset == false && !assetInfo.IsAssetSource() && assetInfo.Type != AssetType::World)
			SaveAsset(asset->GetID());
	}
	Count<Asset> AssetManager::InternalGetAsset(AssetID ID)
	{
		PF_CORE_ASSERT(HasAsset(ID), "ID does not exist");
		auto& it = s_AssetManagerData->Assets[ID];
		if (it.Info.Type == AssetType::TextureSourceFile || it.Info.Type == AssetType::FontSourceFile)
			return nullptr;
		if (it.Info.State == AssetState::Unloaded)
			LoadAsset(ID);
		if (it.Info.Type == AssetType::World)
			return nullptr;
		return it.Asset;
	}
	Count<Asset> AssetManager::InternalGetAsset(const std::filesystem::path& path)
	{
		PF_CORE_ASSERT(HasAsset(path), "Path does not exist");
		auto info = AssetManager::GetAssetInfo(path);
		
		return InternalGetAsset(info.ID);
	}
	const AssetInfo& AssetManager::GetAssetInfo(AssetID ID) 
	{
		PF_CORE_ASSERT(HasAsset(ID), "Does not contian assetID");
		auto& it = s_AssetManagerData->Assets.at(ID);
		return it.Info;
	}

	const std::unordered_set<AssetID>& AssetManager::GetAllAssetType(AssetType type)
	{
		if (s_AssetManagerData->AllAssetTypes.contains(type))
			return s_AssetManagerData->AllAssetTypes[type];
		return {};
	}
	
	void AssetManager::NewAsset(Count<Asset>& asset, const std::filesystem::path& savePath)
	{
		PF_CORE_ASSERT(asset);
		asset->m_ID = AssetManager::CreateID();
		AssetInfo assetInfo;
		assetInfo.Path = std::filesystem::relative(savePath, AssetManager::GetDirectory());
		assetInfo.State = AssetState::Ready;
		assetInfo.ID = asset->GetID();
		assetInfo.Type = asset->GetAssetType();

		InternalAddAsset(assetInfo, asset);
	}

	Count<Asset> AssetManager::GetDefaultAsset(DefaultRuntimeAssets asset)
	{
		uint64_t ID = (uint64_t)asset;
		return InternalGetAsset((uint64_t)asset);
	}

	void AssetManager::NewAssetSource(const std::filesystem::path& path, AssetType type)
	{
		const std::string extension = FileSystem::GetFileExtension(path);

		// mesh source file actually have a mesh source so we keeping it
		if (type == AssetType::MeshSourceFile && Utils::MeshHasFormat(extension))
		{
			AssetInfo assetInfo;
			assetInfo.Path = std::filesystem::relative(path, AssetManager::GetDirectory());
			assetInfo.State = AssetState::Unloaded;
			assetInfo.ID = AssetManager::CreateID();
			assetInfo.Type = AssetType::MeshSourceFile;
			
			InternalAddAsset(assetInfo, nullptr);
			return;
		}

		if (type == AssetType::TextureSourceFile && Utils::TextureHasFormat(extension))
		{
			AssetInfo assetInfo;
			assetInfo.Path = std::filesystem::relative(path, AssetManager::GetDirectory());
			assetInfo.State = AssetState::Ready;
			assetInfo.ID = AssetManager::CreateID();
			assetInfo.Type = AssetType::TextureSourceFile;

			InternalAddAsset(assetInfo, nullptr);
			return;
		}
	
	}
	bool AssetManager::IsAssetLoaded(AssetID ID)
	{
		PF_CORE_ASSERT(HasAsset(ID), "ID does not exist");
		auto info = GetAssetInfo(ID);
		return info.State == AssetState::Ready;
		return false;
	}
	bool AssetManager::HasAsset(AssetID ID) {
		if (ID == 0)return false;
		return s_AssetManagerData->Assets.contains(ID);
	}
	
	void AssetManager::UnloadAsset(AssetID ID) 
	{
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
			if (Utils::MeshHasFormat(extension)) {
				NewAssetSource(it.path(),AssetType::MeshSourceFile);
				continue;
			}

			if (Utils::TextureHasFormat(extension)) {
				NewAssetSource(it.path(), AssetType::TextureSourceFile);
				std::string path = std::filesystem::relative(it.path().parent_path() /= FileSystem::GetFileName(it.path())).string();
				path += ".Texture.ProofAsset";
				Count<Asset> asset = Texture2D::Create(TextureConfiguration(FileSystem::GetFileName(it.path())), it.path());
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

	const AssetInfo& AssetManager::GetAssetInfo(const std::filesystem::path& path)
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
		 assetInfo.State = AssetState::Loading;

		 if (asset)
		 {
			 s_AssetManagerData->Assets[ID].Asset = asset;
			 assetInfo.State = AssetState::Ready;
		 }
		return asset != nullptr;
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
	const std::unordered_map<AssetID, AssetContainer>& AssetManager::GetAssets()
	{
		return s_AssetManagerData->Assets;
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
		auto& it = s_AssetManagerData->Assets.at(ID);

		if (it.Info.RuntimeAsset)
			return;
		auto path = std::filesystem::relative(newPath, AssetManager::GetDirectory());
		// changing teh old data in assetBypath
		s_AssetManagerData->AssetPath.erase(it.Info.Path);
		// creating the new data
		s_AssetManagerData->AssetPath.insert({ path.string(),it.Info.ID });
		// new assetINfo
		it.Info.Path = path;
	}
	std::filesystem::path AssetManager::GetAssetFileSystemPath(const std::filesystem::path& path) 
	{
		return Application::Get()->GetProject()->GetAssetFileSystemPath(path);
	}
	std::filesystem::path AssetManager::GetAssetFileSystemPathRelative(const std::filesystem::path& path) 
	{
		return std::filesystem::relative(path, AssetManager::GetDirectory());
	}
	std::filesystem::path AssetManager::GetDirectory() 
	{
		return Application::Get()->GetProject()->GetAssetDirectory();
	}
}