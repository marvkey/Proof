#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Resources/Asset/Asset.h"
namespace Proof{
	class Proof_API Texture2DAsset: public Asset {
	public:
		Texture2DAsset(const std::string& FilePath);
		virtual void SaveAsset() override;
		virtual void CreateAsset(const std::string& FilePath)override;
		Texture2DAsset(){
			m_AssetType = AssetType::Texture2DAsset;
		};
		
		static std::string GetName(){
			return "AssetType::Texture2DAsset";
		}
		Count<Texture2D> m_Texture;
	};
}