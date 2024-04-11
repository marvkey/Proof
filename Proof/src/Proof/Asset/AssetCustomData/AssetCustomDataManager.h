#pragma once
#include "AssetCustomDataInfo.h"
#include "../AssetTypes.h"
namespace Proof
{
	class AssetCustomDataManager
	{
	private:
		static void Init();
		static void DeleteAsset(AssetID ID);
		static void SaveAssetCustomData();
		static void LoadAllCustomData();
		static void ShutDown();
		friend class AssetManager;
	};
}