#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Resources/Asset/Asset.h"
namespace Proof{
	class Proof_API Texture2DAsset: public Asset {
	public:
		Texture2DAsset(const std::string& texturePath,const std::string& savePath);
		virtual void SaveAsset() override;
		virtual bool LoadAsset(const std::string& filePath)override;
		Texture2DAsset(){
		};
		
		virtual ~Texture2DAsset(){
		}
		virtual const std::string& GetAssetTypeVirtual()const {
			return GetAssetType();
		};
		const static std::string& GetAssetType() {
			static std::string assetType = "AssetType::Texture2DAsset";
			return assetType;
		}
		virtual bool IsEditorImageNull(){
			return m_Texture == nullptr;
		}
		virtual uint32_t GetImageID();
		friend class AssetManager;
		bool HasTexture(){
			return m_Texture != nullptr;
		};

		uint32_t GetTextureID(){
			if(HasTexture())
				return m_Texture->GetID();
			return 0;
		}
		virtual uint32_t GetIamgeEdtorID() {
			return GetTextureID();
		}
		const Count<class Texture2D>& GetTexture()const{
			return m_Texture;
		}
	private:
		std::string m_TexturePath;
		Count<class Texture2D> m_Texture;
	};
}