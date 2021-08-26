#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Resources/Asset/Asset.h"
namespace Proof{
	class Proof_API Texture2DAsset: public Asset {
	public:
		Texture2DAsset(const std::string& FilePath,const std::string& AssetSavePath);
		virtual void SaveAsset() override;
		virtual bool LoadAsset(const std::string& FilePath)override;
		Texture2DAsset(){
			m_AssetType = AssetType::Texture2DAsset;
		};
		
		virtual ~Texture2DAsset(){
		}
		virtual std::string GetAssetTypeName(){
			return "AssetType::Texture2DAsset";
		}
		virtual bool IsImageIDNUll(){
			return m_Texture == nullptr;
		}

		static std::string GetAssetTypeStaticName() {
			return "AssetType::Texture2DAsset";
		}
		virtual uint32_t GetImageID();
		Count<class Texture2D> m_Texture;
		friend class AssetManager;

	};
}