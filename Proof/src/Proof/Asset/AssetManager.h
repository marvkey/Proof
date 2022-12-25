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
		std::string GetName()const {
			return Utils::FileDialogs::GetFileName(Path);
		}
		bool IsAssetSource() {
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
	class Proof_API AssetManager {
	public:
		static bool IsAssetLoaded(AssetID ID) {
			PF_CORE_ASSERT(HasID(ID) == false, "ID does not exist");
			auto info = GetAssets()[ID].Info;
			return info.State == AssetState::Ready;
		}
		static void NewAsset(const Count<Asset>& asset);
		static void NewAsset(AssetID ID, const std::filesystem::path& path);
		static void AddWorldAsset(AssetID ID, const std::filesystem::path& path);
		static void GenerateAllSourceAssets();


		static bool HasID(AssetID ID);
		static void UnloadAsset(AssetID ID);
		// does not check if the asset exist
		// gonna have to do a check
		static AssetInfo GetAssetInfo(AssetID ID);
		static bool LoadAsset(AssetID ID);
	
		//does not check if hte asset exist
		// gonna have to do a check to see if the asset exist
		// returns nullptr if the asset does not exist
		template<class T>
		static Count<T>GetAsset(AssetID ID){
			auto& it = GetAssets()[ID];
			if (it.Info.Type == AssetType::TextureSourceFile)
				return nullptr;
			if (it.Info.State == AssetState::Unloaded)
				LoadAsset(ID);
			if (it.Info.Type == AssetType::World)
				return nullptr;
			return std::dynamic_pointer_cast<T>(it.Asset);
		}

		static AssetID GetAssetSourceID(const std::filesystem::path& path,bool createIfnotexist = true);
		static std::string GetAssetSourcePath(AssetID ID);
		/**
		 * gets the asset info by saved path 
		 * does not check if path exist so may crash
		 * @param path weher we are chekcing for the asset
		 * @return teh asset info 
		 */
		static AssetInfo GetAssetBySavedPath(const std::filesystem::path& path);
		/**
		 * does not check if path exist so may crash
		 * @param path we are going to check
		 * @return true if an asset does have that path
		 */
		static bool HasAssetBySavedPath(const std::filesystem::path& path);

		static AssetID CreateID();
		/**
		 * removes an asset
		 * it does not check if the asset id exist
		 * so it may crash if you do not check
		 * @param ID assset handle we are removeing
		 */
		static void Remove(AssetID ID);

		/**
		 * basically tells us what type of asset it is from it extension 
		 * it does not care if this asset is stored in proof
		 * just checks the extension of the file
		 * @param path the fiel path of the asset
		 * @return the type of asset it would be in proof
		 */
		static AssetType GetAssetTypeFromFilePath(const std::filesystem::path& path);
		static void LoadMultipleAsset(std::set<AssetID> assetLoadIn);

	private:
		static std::unordered_map<AssetID, AssetContainer>& GetAssets();
		static void Init(AssetManagerConfiguration& assetManagerConfiguration);
		static void UnInizilize();
		static bool ResetAssetInfo(AssetID ID,const std::string& path) {
			if (HasID(ID) == false)return false;
			auto assetInfo = GetAssetInfo(ID);
			auto asset = GetAsset<Asset>(ID);
			if (assetInfo.State == AssetState::Ready) {
				asset->SetPath(path);
				asset->SaveAsset();
			}
			auto it = GetAssets().at(ID);
			it.Info.Path = path;
			return true;
		}

		static std::vector<std::string> s_PermitableMeshSourceFile;
	
		static void SaveAllAssets();
		static void InitilizeAssets();

		friend class Application;
		friend class AssetManagerPanel;
		friend class ContentBrowserPanel;
		friend class NewContentBrowserPanel;
		friend class PhysicsEngine;
	};
}	