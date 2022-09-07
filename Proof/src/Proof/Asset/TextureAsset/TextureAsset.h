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
		AssetID m_Source;
		Count<class Texture2D> m_Texture;
	};
}