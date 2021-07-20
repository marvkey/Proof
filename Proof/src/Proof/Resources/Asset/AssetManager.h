#pragma once
#include "Proof/Core/Core.h"
#include <unordered_map>
namespace Proof{
	class Proof_API AssetManager {
	public:
		static AssetManager& Get(){ return *s_AssetManager;}
		static void NewAsset(uint32_t ID,class Asset* asset);
		static bool HasID(uint32_t ID);
		static Asset* GetAsset(uint32_t ID);
		static uint32_t CreateID();
		static void Remove(uint32_t ID);
		AssetManager(const AssetManager&) = delete;
	private:
		static AssetManager* s_AssetManager;
		std::unordered_map<uint32_t, class Asset*> m_AllAssets;
		AssetManager() {};
		static void InitilizeAssets(const std::string& Path);
		
		static bool IsFileValid(const std::string& Path);
		static std::string GetAssetType(const std::string& Path);
		friend class Application;

	};
}
