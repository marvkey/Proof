#pragma once
#include "AssetCustomDataInfo.h"
#include "../AssetTypes.h"
namespace Proof
{
	class AssetCustomDataManager
	{
	public:
		static void SaveAssetCustomData();
		static void SaveAssetCustomData(AssetCustomDataType type);
	private:
		static void Init();
		static void DeleteAsset(AssetID ID);
		static void LoadAllCustomData();
		static void ShutDown();
		friend class AssetManager;
	};
}