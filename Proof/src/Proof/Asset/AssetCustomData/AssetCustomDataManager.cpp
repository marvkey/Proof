#include "Proofprch.h"
#include "AssetCustomDataManager.h"
#include "MeshSourceSavedSettings.h"
#include "AssetCustomDataSerelizer.h"
#include "AssetCustomDataInfo.h"
namespace Proof 
{
	static std::unordered_map<AssetCustomDataType, Special<AssetCustomDataSerializer>> AssetCustomDataSerilizer;// path
	static std::unordered_map<AssetCustomDataType, std::filesystem::path> AssetCustomDataSerilizerPaths;// path
	void AssetCustomDataManager::Init()
	{
		ScopeTimer scopeTimer(__FUNCTION__);
		AssetCustomDataSerilizer[AssetCustomDataType::MeshSourceSavedSettings] = CreateSpecial<MeshSourceSavedSettingSerializer>();
		AssetCustomDataSerilizerPaths[AssetCustomDataType::MeshSourceSavedSettings] = "MeshSourceSettings" + Utils::AssetCustomDataTypeMap.at(AssetCustomDataType::MeshSourceSavedSettings);
		LoadAllCustomData();
	}

	void AssetCustomDataManager::DeleteAsset(AssetID ID)
	{
		//if its a mesh source Data
		MeshSourceSavedSettings::DeleteMeshSourceData(ID);
	}
	void AssetCustomDataManager::SaveAssetCustomData()
	{
		for (auto& [customType, serelizier] : AssetCustomDataSerilizer)
			serelizier->Save(AssetCustomDataSerilizerPaths.at(customType));
	}
	void AssetCustomDataManager::LoadAllCustomData()
	{
		for (auto& [customType, serelizier] : AssetCustomDataSerilizer)
			serelizier->Load(AssetCustomDataSerilizerPaths.at(customType));
	}
}
