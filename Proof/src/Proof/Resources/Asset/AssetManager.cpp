#include "Proofprch.h"
#include "AssetManager.h"
#include "Asset.h"
#include "Proof3D/Math/Math.h"
namespace Proof{
	AssetManager* AssetManager::s_AssetManager = new AssetManager();
	void AssetManager::NewAsset(uint32_t ID,Asset* asset) {
		if(HasID(ID) ==false){
			s_AssetManager->m_AllAssets.insert({ID,asset});
			return;
		}
		PF_ASSERT(false,"Asset Maneger Has ID");
	}
	bool AssetManager::HasID(uint32_t ID) {
		return s_AssetManager->m_AllAssets.find(ID) != s_AssetManager->m_AllAssets.end();
	}
	Asset* AssetManager::GetAsset(uint32_t ID) {
		if (HasID(ID) == true) {
			return s_AssetManager->m_AllAssets.find(ID)->second;
		}
	}
	uint32_t AssetManager::CreateID() {
		uint32_t ID = Math::RandUINT(1,1000000);
		while (HasID(ID)== true) {
			uint32_t ID = Math::RandUINT(1,1000000);
		}
		return ID;
	}
	void AssetManager::Remove(uint32_t ID) {
		if (HasID(ID) == true) {
			delete  s_AssetManager->m_AllAssets.find(ID)->second;
			s_AssetManager->m_AllAssets.erase(ID);
			return;
		}
		PF_ASSERT(false,"Asset Manager Has ID");
	}
}