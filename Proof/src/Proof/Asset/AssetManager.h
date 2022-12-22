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
		AssetType Type;
		AssetID ID;
		bool Loaded = false;
		bool AssetSource;
		std::string GetName()const {
			return Utils::FileDialogs::GetFileName(Path);
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
			return info.Loaded;
		}
		static void NewAsset(AssetID ID,const Count<Asset>& asset);
		static void NewAssetSource(AssetID ID, const std::filesystem::path& path);
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
			if (it.Info.Loaded == false)
				LoadAsset(ID);
			return std::dynamic_pointer_cast<T>(it.Asset);
		}
		static AssetID GetAssetSourceID(const std::filesystem::path& path);
		static std::string GetAssetSourcePath(AssetID ID);
		static AssetID CreateID();
		static void Remove(AssetID ID);
		static AssetType GetAssetFromFilePath(const std::filesystem::path& path);
		static bool IsFileValid(const std::string& Path);
		static void LoadMultipleAsset(std::set<AssetID> assetLoadIn);

	private:
		static std::unordered_map<AssetID, AssetContainer>& GetAssets();
		static void Init(AssetManagerConfiguration& assetManagerConfiguration);
		static void UnInizilize();
		static bool ResetAssetInfo(AssetID ID,const std::string& path) {
			if (HasID(ID) == false)return false;
			auto assetInfo = GetAssetInfo(ID);
			auto asset = GetAsset<Asset>(ID);
			if (assetInfo.Loaded) {
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