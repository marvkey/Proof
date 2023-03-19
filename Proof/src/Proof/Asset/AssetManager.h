#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Core/UUID.h"
#include <set>
#include <fstream>
#include <string>
#include <filesystem>
#include "Proof/Utils/PlatformUtils.h"
#include "Asset.h"
namespace Proof
{

	struct AssetInfo {
		std::filesystem::path Path;
		AssetType Type = AssetType::None;
		AssetState State =  AssetState::None;
		AssetID ID{ 0 };

		bool RuntimeAsset = false;
		std::string GetName()const {
			return Utils::FileDialogs::GetFileName(Path);
		}
		bool IsAssetSource()const  {
			return Type == AssetType::MeshSourceFile || Type == AssetType::TextureSourceFile;
		}
		friend class AssetManager;
	};

	struct AssetContainer {
		AssetInfo Info;
		Count<Asset> Asset = nullptr;
	};

	struct AssetManagerConfiguration {
		std::filesystem::path AssetDirectory;
		std::filesystem::path AssetManager;
	};
	struct AssetManagerData {
		std::unordered_map<AssetID, AssetContainer> Assets;// path

		//temporay 
		std::unordered_map<std::filesystem::path, AssetID> AssetPath;// path
		std::unordered_map<AssetType, Special<class AssetSerializer>> AssetSerilizer;// path
		std::filesystem::path AssetDirectory;
		std::filesystem::path AssetRegistry;
	};
	class World;
	class Proof_API AssetManager {
	public:
		static std::string GetExtension(AssetType type);

		// PATH (PASS THE FULL PATH)
		static void NewAsset(Count<Asset>& asset, const std::filesystem::path& savePath) {
			PF_CORE_ASSERT(asset);
			asset->m_ID = AssetManager::CreateID();
			AssetInfo assetInfo;
			assetInfo.Path = std::filesystem::relative(savePath, AssetManager::GetDirectory());
			assetInfo.State = AssetState::Ready;
			assetInfo.ID = asset->GetID();
			assetInfo.Type = asset->GetAssetType();

			GetAssets().insert({ assetInfo.ID,{assetInfo,asset} });
			GetAssetByPath().insert({ assetInfo.Path.string(),assetInfo.ID });
			AssetManager::SaveAsset(assetInfo.ID);
		}

		/**
		 * .
		 * 
		 * @param savePath  Pass the Full path
		 */
		template <class AssetType, class... Args,
			std::enable_if_t<std::is_constructible<AssetType, Args...>::value&& Is_Compatible<AssetType, Asset>::value, int> = 0>
		static Count<AssetType> NewAsset(const std::filesystem::path& savePath, Args&&... args) {
			static_assert(!std::is_same<AssetType, class World>::value, "Cannot craet  a world like this");
			Count<Asset> asset = Count<AssetType>::Create(std::forward<Args>(args)...);
			asset->m_ID = AssetManager::CreateID();
			AssetInfo assetInfo;
			assetInfo.Path = std::filesystem::relative(savePath, AssetManager::GetDirectory());
			assetInfo.State = AssetState::Ready;
			assetInfo.ID = asset->GetID();
			assetInfo.Type = asset->GetAssetType();
			GetAssets().insert({ assetInfo.ID,{assetInfo,asset} });
			GetAssetByPath().insert({ assetInfo.Path.string(),assetInfo.ID });
			AssetManager::SaveAsset(assetInfo.ID);
			return asset.As<AssetType>();
		}
		// cannot access this asset by path only by its id
		template<class AssetType, typename ... Args, std::enable_if_t<Is_Compatible<AssetType, Asset>::value, int> = 0>
		static Count<AssetType> CreateRuntimeAsset(Args&&... args) {
			static_assert(!std::is_same<AssetType, class World>::value, "Cannot craet  a world like this");
			Count<Asset> asset = Count<AssetType>::Create(std::forward<Args>(args)...);
			asset->m_ID = AssetManager::CreateID();
			AssetInfo assetInfo;
			//assetInfo.Path = std::filesystem::relative(savePath, AssetManager::GetDirectory());
			assetInfo.State = AssetState::Ready;
			assetInfo.ID = asset->GetID();
			assetInfo.Type = asset->GetAssetType();
			assetInfo.RuntimeAsset = true;
			GetAssets().insert({ assetInfo.ID,{assetInfo,asset} });
			//GetAssetByPath().insert({ assetInfo.Path.string(),assetInfo.ID });
			return asset.As<AssetType>();
		}
/*
		template <class Type, std::enable_if_t<Is_Compatible<Type, Asset>::value, int> = 0>
		static void NewAssetNoLoad(const std::filesystem::path& savePath) {
			AssetInfo assetInfo;
			assetInfo.Path = std::filesystem::relative(savePath, AssetManager::GetDirectory());
			assetInfo.State = AssetState::Unloaded;
			assetInfo.ID = CreateID();
			assetInfo.Type = Type::GetStaticType();

			GetAssets().insert({ assetInfo.ID,{assetInfo,nullptr} });
			GetAssetByPath().insert({ assetInfo.Path.string(),assetInfo.ID });
		}
		*/


		/*
		*path Pass the full path
		*/
		static void NewAssetSource(const std::filesystem::path& path, AssetType type);
		static bool IsAssetLoaded(AssetID ID) {
			PF_CORE_ASSERT(HasAsset(ID), "ID does not exist");
			auto info = GetAssets()[ID].Info;
			return info.State == AssetState::Ready;
		}
		template<class T>
		static Count<T>GetAsset(AssetID ID) {
			PF_CORE_ASSERT(HasAsset(ID), "ID does not exist");
			auto& it = GetAssets().at(ID);
			if (it.Info.Type == AssetType::TextureSourceFile)
				return nullptr;
			if (it.Info.State == AssetState::Unloaded)
				LoadAsset(ID);
			if (it.Info.Type == AssetType::World)
				return nullptr;
			return it.Asset.As<T>();
		}
		/**
		 * .
		 * 
		 * @param path Pass the full path
		 * \return 
		 */
		template<class T>
		static Count<T>GetAsset(const std::filesystem::path& path) {
			PF_CORE_ASSERT(AssetManager::HasAsset(path));
			auto info = AssetManager::GetAssetInfo(path);
			return GetAsset<T>(info.ID);
		}
		static bool HasAsset(AssetID ID);
		/**
		 * does not check if path exist so may crash
		 * pass the FULL PATH not relative to asset path
		 * @param path we are going to check
		 * @return true if an asset does have that path
		*/
		static bool HasAsset(const std::filesystem::path& path);

		static bool HasAsset(const Count< Asset>& asset) {
			if (asset)
				return HasAsset(asset->GetID());
			return false;
		}
		static AssetInfo GetAssetInfo(AssetID ID);
		/**
		 * removes an asset
		 * it does not check if the asset id exist
		 * so it may crash if you do not check
		 * @param ID assset handle we are removeing
		 */
		static void Remove(AssetID ID);
		/**
		 * gets the asset info by saved path
		 * @param path PASS THE FULLL PATH
		 * @return teh asset info
		 */
		static AssetInfo GetAssetInfo(const std::filesystem::path& path);
		static AssetInfo GetAssetInfo(const Count< Asset>& asset) {
			return GetAssetInfo(asset->GetID());
		}

		static AssetID CreateID();
		/**
		 * basically tells us what type of asset it is from it extension
		 * it does not care if this asset is stored in proof
		 * just checks the extension of the file
		 * @param path the fiel path of the asset
		 * @return the type of asset it would be in proof
		 */
		static AssetType GetAssetTypeFromFilePath(const std::filesystem::path& path);

		static void LoadMultipleAsset(std::set<AssetID> assetLoadIn);
		// reutns the fule file path
		static std::filesystem::path GetAssetFileSystemPath(const std::filesystem::path& path);
		// returns relative to the asset manager folder
		static std::filesystem::path GetAssetFileSystemPathRelative(const std::filesystem::path& path);

		static std::filesystem::path GetDirectory();
		// pass full path
		static void AddWorldAsset(AssetID ID, const std::filesystem::path& path);
		static void GenerateAllSourceAssets();
		static bool LoadAsset(AssetID ID);
		static const AssetManagerData const* GetAssetManagerData();


		static void UnloadAsset(AssetID ID);

		static void SaveAsset(AssetID Id);
		// change assetPath
		// pass the full path
		static void ChangeAssetPath(AssetID ID, const std::filesystem::path& newPath);

		// loops trhough all assets and saves them
		static void SaveAllAssets();
		// just saves the assetanager inot the assetManager file
		static void SaveAssetManager();
	private:
		static std::unordered_map<AssetID, AssetContainer>& GetAssets();
		static std::unordered_map<std::filesystem::path, AssetID>& GetAssetByPath();
		static void Init(AssetManagerConfiguration& assetManagerConfiguration);
		static void UnInizilize();
		

		static std::vector<std::string> s_PermitableMeshSourceFile;
	
		static void InitilizeAssets();

		friend class Application;
		friend class AssetManagerPanel;
		friend class ContentBrowserPanel;
		friend class NewContentBrowserPanel;
		friend class PhysicsEngine;
	};
}	