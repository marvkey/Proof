#include<iostream>

#pragma once
namespace Proof {

	enum class ImageFormat {
		None = 0,
		RGBA,
		RGBA32F
	};
	class Proof_API Texture {
	public:
		virtual void* GetID()const = 0;
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
		virtual std::string GetPath() = 0;

		virtual TextureType GetTextureType() { return TextureType::None; };
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
