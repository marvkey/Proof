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
		
		virtual ~Texture2DAsset(){
			m_Texture = nullptr;
			m_Texture.reset();
		}
		virtual std::string GetName(){
			return "AssetType::Texture2DAsset";
		}
		virtual bool IsImageIDNUll(){
			return m_Texture == nullptr;
		}

		static std::string GetStaticName() {
			return "AssetType::Texture2DAsset";
		}
		virtual uint32_t GetImageID();
		Count<Texture2D> m_Texture;
	};
}