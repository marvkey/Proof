#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Core/UUID.h"
#include <set>
#include <fstream>
#include <string>
#include <filesystem>
#include "Proof/Utils/FileSystem.h"
#include "Asset.h"
namespace Proof
{
	struct AssetManagerConfiguration
	{
		std::filesystem::path AssetDirectory;
		std::filesystem::path AssetManager;
	};
	struct AssetInfo 
	{
		std::filesystem::path Path;
		AssetType Type = AssetType::None;
		AssetState State =  AssetState::None;
		AssetID ID{ 0 };

		bool RuntimeAsset = false;
		std::string GetName()const {
			return FileSystem::GetFileName(Path);
		}
		bool IsAssetSource()const  {
			return Utils::IsAssetSource(Type);
		}
		friend class AssetManager;
	};

	struct AssetContainer 
	{
		AssetInfo Info;
		Count<Asset> Asset = nullptr;
	};
	enum class DefaultRuntimeAssets // thse are the IDS
	{
		Cube = 50,
		Sphere,
		Capsule,
		Cylinder,
		Cone,
		Torus,
		Plane,
		Material=70,
		PhysicsMaterial,
		//Font=80,
	};
	class World;
	class AssetManager 
	{
	public:

		// PATH (PASS THE FULL PATH)
		static void NewAsset(Count<Asset>& asset, const std::filesystem::path& savePath);
		/**
		 * .
		 *
		 * @param savePath  Pass the Full path
		 */
		template <class AssetType, class... Args,
			std::enable_if_t<std::is_constructible<AssetType, Args...>::value&& Is_Compatible<AssetType, Asset>::value, int> = 0>
		static Count<AssetType> NewAsset(const std::filesystem::path& savePath, Args&&... args)
		{
			static_assert(!std::is_same<AssetType, class World>::value, "Cannot craet  a world like this");
			Count<Asset> asset = Count<AssetType>::Create(std::forward<Args>(args)...);
			AssetInfo assetInfo;
			assetInfo.Path = std::filesystem::relative(savePath, AssetManager::GetDirectory());
			assetInfo.State = AssetState::Ready;
			assetInfo.ID = CreateID();
			assetInfo.Type = asset->GetAssetType();

			InternalAddAsset(assetInfo, asset);
			return asset.As<AssetType>();
		}

		static void CreateRuntimeAsset(Count<Asset> asset, const std::string& name = "")
		{
			CreateRuntimeAsset(CreateID(), asset);
		}

		static void CreateRuntimeAsset(AssetID ID,Count<Asset> asset,const std::string& name = "")
		{
			AssetInfo assetInfo;
			assetInfo.State = AssetState::Ready;
			assetInfo.ID = ID;
			assetInfo.Type = asset->GetAssetType();
			assetInfo.RuntimeAsset = true;
			assetInfo.Path = fmt::format("RuntimeAsset/{}/{}", assetInfo.ID.Get(), name);

			InternalAddAsset(assetInfo, asset);
		}
		// cannot access this asset by path only by its id
		template<class AssetType, typename ... Args, std::enable_if_t<Is_Compatible<AssetType, Asset>::value, int> = 0>
		static Count<AssetType> CreateRuntimeAsset(const std::string& name ,Args&&... args)
		{
			static_assert(!std::is_same<AssetType, class World>::value, "Cannot craet  a world like this");
			Count<Asset> asset = Count<AssetType>::Create(std::forward<Args>(args)...);
			AssetInfo assetInfo;
			assetInfo.State = AssetState::Ready;
			assetInfo.ID = CreateID();
			assetInfo.Type = asset->GetAssetType();
			assetInfo.RuntimeAsset = true;
			assetInfo.Path = fmt::format("RuntimeAsset/{}/{}",assetInfo.ID.Get(), name);

			InternalAddAsset(assetInfo, asset);
			return asset.As<AssetType>();
		}

		template<class AssetType, typename ... Args, std::enable_if_t<Is_Compatible<AssetType, Asset>::value, int> = 0>
		static Count<AssetType> CreateRuntimeAsset(Args&&... args)
		{
			static_assert(!std::is_same<AssetType, class World>::value, "Cannot craet  a world like this");
			Count<Asset> asset = Count<AssetType>::Create(std::forward<Args>(args)...);
			AssetInfo assetInfo;
			assetInfo.State = AssetState::Ready;
			assetInfo.ID = CreateID();
			assetInfo.Type = asset->GetAssetType();
			assetInfo.RuntimeAsset = true;
			assetInfo.Path = fmt::format("RuntimeAsset/{}/{}",assetInfo.ID.Get(),fmt::format("UnnamedAsset {}", assetInfo.ID.Get()));

			InternalAddAsset(assetInfo, asset);
			return asset.As<AssetType>();
		}
		template <class AssetType, class... Args,std::enable_if_t<Is_Compatible<AssetType, Asset>::value, int> = 0>
		static Count<AssetType> CreateRuntimeOnlyRendererAsset(const std::string& name, Args&&... args)
		{
			static_assert(!std::is_same<AssetType, class World>::value, "Cannot create a world like this");

			// Use a fold expression to discard the first argument (name) and forward the rest
			Count<Asset> asset = AssetType::Create(std::forward<Args>(args)...);

			AssetInfo assetInfo;
			assetInfo.State = AssetState::Ready;
			assetInfo.ID = CreateID();
			assetInfo.Type = asset->GetAssetType();
			assetInfo.RuntimeAsset = true;
			assetInfo.Path = fmt::format("RuntimeAsset/{}/{}", assetInfo.ID.Get(), name);

			InternalAddAsset(assetInfo, asset);
			return asset.As<AssetType>();
		}

		static Count<Asset> GetDefaultAsset(DefaultRuntimeAssets asset);

		static bool IsDefaultAsset(AssetID ID);
		/*
		*path Pass the full path
		*/
		static void NewAssetSource(const std::filesystem::path& path, AssetType type);
		static bool IsAssetLoaded(AssetID ID);
		template<class T>
		static Count<T>GetAsset(AssetID ID) 
		{
			return InternalGetAsset(ID).As<T>();
		}
		/**
		 * .
		 * 
		 * @param path Pass the full path
		 * \return 
		 */
		template<class T>
		static Count<T>GetAsset(const std::filesystem::path& path) 
		{
			return InternalGetAsset(path).As<T>();
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
		static const std::unordered_set<AssetID>& GetAllAssetType(AssetType type);
		
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
		static const AssetInfo& GetAssetInfo(const std::filesystem::path& path);
		static const AssetInfo& GetAssetInfo(const Count< Asset>& asset) 
		{
			return GetAssetInfo(asset->GetID());
		}
		static const AssetInfo& GetAssetInfo(AssetID ID);
		static void LoadMultipleAsset(std::set<AssetID> assetLoadIn);

		static AssetID CreateID();
		// reutns the fule file path
		static std::filesystem::path GetAssetFileSystemPath(const std::filesystem::path& path);
		// returns relative to the asset manager folder
		static std::filesystem::path GetAssetFileSystemPathRelative(const std::filesystem::path& path);
		static std::filesystem::path GetDirectory();

		// loops through all the assets in the directory of assetManager and generates 
		// an assetSource for each needed asset
		static void GenerateAllSourceAssets();
		static bool LoadAsset(AssetID ID);
		static void UnloadAsset(AssetID ID);
		static void SaveAsset(AssetID Id);

		static const std::unordered_map<AssetID, AssetContainer>& GetAssets();
		// change assetPath
		// pass the full path
		static void ChangeAssetPath(AssetID ID, const std::filesystem::path& newPath);
		// loops trhough all assets and saves them
		static void SaveAllAssets();
		// just saves the assetanager inot the assetManager file
		static void SaveAssetManager();
	private:
		// make sure the assetInfo is all ready and does not matter if the asset is loaded or not
		// asset can be nullptr
		static void InternalAddAsset(AssetInfo info, Count<Asset> asset);
		static Count<Asset> InternalGetAsset(AssetID Id);
		static Count<Asset> InternalGetAsset(const std::filesystem::path& path);
		static void Init(AssetManagerConfiguration& assetManagerConfiguration);
		static void ShutDown();
		friend class Application;
		friend class AssetManagerPanel;
		friend class ContentBrowserPanel;
		friend class NewContentBrowserPanel;
		friend class PhysicsEngine;
	};
}	