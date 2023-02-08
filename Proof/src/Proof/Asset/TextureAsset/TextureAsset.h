#pragma once
#include "Proof/Core/Core.h"
#include "../Asset.h"
#include "Proof/Renderer/Texture.h"
namespace Proof{
	class Proof_API Texture2DAsset: public Asset {
	public:
		Texture2DAsset(const std::string& texturePath,const std::string& savePath);
		virtual void SaveAsset() override;
		virtual bool LoadAsset(const std::string& filePath)override;
		Texture2DAsset():
			Asset(AssetType::Texture) 
		{
		};
		
		virtual ~Texture2DAsset(){
		}
		static std::string StaticGetExtension() {
			return "Texture.ProofAsset";
		}
		virtual std::string GetExtension()const {
			return "Texture.ProofAsset";
		}
		virtual bool IsEditorImageNull(){
			return m_Texture == nullptr;
		}
		virtual Image GetImage();
		friend class AssetManager;
		bool HasTexture(){
			return m_Texture != nullptr;
		};

		Image GetTextureID(){
			if(HasTexture())
				return m_Texture->GetImage();
			return Image();
		}
		Count<class Texture2D> GetTexture(){
			return m_Texture;
		}
		AssetID GetAssetSource() {
			return m_Source;
		}
	private:
		AssetID m_Source{0};
		Count<class Texture2D> m_Texture;
	};
}