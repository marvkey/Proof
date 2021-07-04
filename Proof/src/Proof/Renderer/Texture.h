#include<iostream>
#pragma once
namespace Proof {
	class Proof_API Texture {
	public:
		virtual void BindTexture(uint32_t Slot =0) = 0;
		virtual unsigned int  GetID() = 0;
		virtual std::string GetPath() = 0;
		virtual void unBind() = 0;
	};

	class Proof_API Texture2D: public Texture {
	public:
		static enum class TextureType {
			None=0,
			Diffuse,
			Specular,
			Normal,
			Height,
		};
		virtual void SetData(void* data,uint32_t size) = 0;
		virtual TextureType GetTextureType() = 0;
		static Count<Texture2D> Create(const std::string& Path,TextureType _TextureType = TextureType::None);
		static Count<Texture2D>	Create(uint32_t ImageWidth,uint32_t ImageHeight,TextureType _TextureType= TextureType::None);
	};
}
