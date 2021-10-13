#pragma once
#include "Proof/Core/Core.h"
#include <unordered_map>
namespace Proof{
	using AssetID = uint64_t;
	class Proof_API AssetManager {
	public:
		static AssetManager& Get(){ return *s_AssetManager;}
		static void NewAsset(AssetID ID,class Asset* asset);
		static bool HasID(AssetID ID);
		template<class T>
		static T* GetAsset(AssetID ID){
			if (HasID(ID) == true) {
				return dynamic_cast<T*>(s_AssetManager->m_AllAssets.find(ID)->second);
			}
		}
		static AssetID CreateID();
		static void Remove(AssetID ID);
		AssetManager(const AssetManager&) = delete;
	private:
		static AssetManager* s_AssetManager;
		std::unordered_map<AssetID, class Asset*> m_AllAssets;
		AssetManager() {};
		static void InitilizeAssets(const std::string& Path);
		
		static bool IsFileValid(const std::string& Path);
		static std::string GetAssetType(const std::string& Path);
		friend class Application;

	};
}
