#pragma once
#include "Proof/Core/Core.h"
#include <unordered_map>
#include "Proof/Core/UUID.h"
namespace Proof
{
	class Proof_API AssetManager {
	public:
		static AssetManager& Get() { return *s_AssetManager; }
		static void NewAsset(UUID ID,const Count<class Asset>& asset);
		static bool HasID(UUID ID);
		template<class T>
		static T* GetAsset(UUID ID) {
			auto it = s_AssetManager->m_AllAssets.find(ID);
			if (it != s_AssetManager->m_AllAssets.end()) {
				return dynamic_cast<T*>(it->second.get());
			}
			return nullptr;
		}
		template<class T>
		static Count<T>GetAssetShared(UUID ID){
			auto it = s_AssetManager->m_AllAssets.find(ID);
			if (it != s_AssetManager->m_AllAssets.end()) {
				return std::dynamic_pointer_cast<T>(it->second);
			}
			return nullptr;
		}
		static UUID CreateID();
		static void Remove(UUID ID);
		AssetManager(const AssetManager&) = delete;
	private:
		static void SaveAllAsset();
		static AssetManager* s_AssetManager;
		std::unordered_map<UUID,Count<class Asset>> m_AllAssets;
		AssetManager() {};
		static void InitilizeAssets(const std::string& Path);
		static void MakeDirectory(const std::string& path);
		static bool IsFileValid(const std::string& Path);
		static std::string GetAssetType(const std::string& Path);
		friend class Application;

	};
}	