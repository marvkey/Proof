#pragma once
#include "Proof/Core/Core.h"
#include <unordered_map>
namespace Proof
{
	using AssetID = uint64_t;
	class Proof_API AssetManager {
	public:
		static AssetManager& Get() { return *s_AssetManager; }
		static void NewAsset(AssetID ID,const Count<class Asset>& asset);
		static bool HasID(AssetID ID);
		template<class T>
		static T* GetAsset(AssetID ID) {
			auto it = s_AssetManager->m_AllAssets.find(ID);
			if (it != s_AssetManager->m_AllAssets.end()) {
				return dynamic_cast<T*>(it->second.get());
			}
			return nullptr;
		}
		template<class T>
		static Count<T>GetAssetShared(AssetID ID){
			auto it = s_AssetManager->m_AllAssets.find(ID);
			if (it != s_AssetManager->m_AllAssets.end()) {
				return std::dynamic_pointer_cast<T>(it->second);
			}
			return nullptr;
		}
		static AssetID CreateID();
		static void Remove(AssetID ID);
		AssetManager(const AssetManager&) = delete;
	private:
		static void SaveAllAsset();
		static AssetManager* s_AssetManager;
		std::unordered_map<AssetID,Count<class Asset>> m_AllAssets;
		AssetManager() {};
		static void InitilizeAssets(const std::string& Path);
		static void MakeDirectory(const std::string& path);
		static bool IsFileValid(const std::string& Path);
		static std::string GetAssetType(const std::string& Path);
		friend class Application;

	};
}	