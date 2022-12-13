#include<iostream>
#include "Glad/glad.h"

#pragma once
namespace Proof {

	enum class ImageFormat {
		None = 0,
		RGBA,
		RGBA32F
	};
	class Proof_API Texture {
	public:
		virtual void*  GetID() = 0;
		virtual void unBind(){};
		virtual void Bind(uint32_t Slot = 0) {};
		virtual void GenerateMipMap(){};
	};
	enum class type:uint32_t{
		UnsignedInt =GL_UNSIGNED_INT,
		Float =GL_FLOAT,
		UnsignedByte = GL_UNSIGNED_BYTE
	};
	enum class InternalFormat:uint32_t {
		R8 = GL_R8,
		R16 = GL_R16,
		RG8 = GL_RG8,
		RG16 = GL_RG16,
		R16F = GL_R16F,
		R32F = GL_R32F,
		RG16F = GL_RG16F,
		RG32F = GL_RG32F,
		R8I = GL_R8I,
		R8UI = GL_R8UI,
		R16I = GL_R16I,
		R16UI = GL_R16UI,
		R32I = GL_R32I,
		R32UI = GL_R32UI,
		RG8I = GL_RG8I,
		RG8UI = GL_RG8UI,
		RG16I = GL_RG16I,
		RG16U = GL_RG16UI,
		RG32I = GL_RG32I,
		RG32U = GL_RG32UI,
		RGBA16F =GL_RGBA16F,
		RGBA = GL_RGBA,
	};

	enum class DataFormat:uint32_t {
		RGB = GL_RGB,
		RGBA = GL_RGBA,
		RG = GL_RG,
		DepthComponent = GL_DEPTH_COMPONENT,
		DepthStencil = GL_DEPTH_STENCIL
	};

	enum class TextureBaseTypes:uint32_t{
		ClampToEdge = GL_CLAMP_TO_EDGE,
		Linear = GL_LINEAR,
		Nearest = GL_NEAREST
	};

	struct Image {
	public:
		Image(const void* image, bool hasImage):
			m_Image(image),
			m_HasImage(m_HasImage)
		{
		}
		uint32_t GetImage() {
			return (uint32_t)m_Image;
		}
		bool HasImage() {
			return m_Image;
		}
	private:
		const void* m_Image = nullptr;
		const bool m_HasImage;
	};
	class Proof_API Texture2D: public Texture {
	public:
		template<class T>
		T* As() {
			return  dynamic_cast<T*>(this);
		}
		static enum class TextureType {
			None=0,
			Ambient,
			Diffuse,
			Specular,
			Normal,
			Height,
		};
		virtual void SetData(void* data,uint32_t size){};
		virtual std::string GetPath() = 0;

		virtual TextureType GetTextureType() { return TextureType::None; };
		static Count<Texture2D> Create(uint32_t width,uint32_t height,DataFormat dataFormat,InternalFormat internalFormat,TextureBaseTypes WrapS,TextureBaseTypes WrapT,TextureBaseTypes MinFilter,TextureBaseTypes MagFilter,type baseType,bool usWrap = true);
		static Count<Texture2D> Create(const std::string& Path,TextureType _TextureType = TextureType::None);
		static Count<Texture2D>	Create(uint32_t ImageWidth,uint32_t ImageHeight,TextureType _TextureType= TextureType::None);
		static Count<Texture2D> Create(uint32_t width, uint32_t height, ImageFormat format, const void* data);
	};


	class Proof_API CubeMap: public Texture{
	public:
		virtual void Bind(uint32_t Slot = 0)=0;
		static Count<CubeMap> Create(const std::vector<std::string>& Paths);
		static Count<CubeMap> Create(const std::string& Path);
		static Count<CubeMap> Create(uint32_t textureWidht = 512,uint32_t textureHeight = 512,bool generateMipMap=false);
	};

	class Proof_API HDRTexture: public Texture{
	public:
		static Count<HDRTexture> Create(const std::string& path);
	};
}
