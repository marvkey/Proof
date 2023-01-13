#pragma once
#include<iostream>
#include "Proof/Core/Core.h"
namespace Proof {

	enum class ImageFormat {
		//https://docs.rs/vulkano/0.6.2/vulkano/format/index.html#:~:text=Unorm%20means%20that%20the%20values,minimum%20representable%20value%20becomes%200.0%20.
		None = 0,
		//  unsigned ints
		// 8 bytes
		RGBA,
		RGBA16,
		
		RGBA16F,
		RGBA32F,

		// temporary
		RGBAScreenFrameBuffer
	};
	struct Image {
	public:
		Image() {

		}
		Image(const void* image,ImageFormat format, ScreenSize size) :
			SourceImage(image), Format(format), Size(size)
		{

		}
		bool HasImage() {
			if (SourceImage == nullptr)return false;
			return true;
		}
		ImageFormat Format = ImageFormat::None;
		ScreenSize Size;
		const void* SourceImage = nullptr;
	};
	class Proof_API Texture {
	public:
		virtual Image GetImage()const = 0;
	};
	


	class Proof_API Texture2D: public Texture {
	public:
		template<class T>
		T* As() {
			return  dynamic_cast<T*>(this);
		}
	
		virtual std::string GetPath() = 0;

		static Count<Texture2D> Create(const std::string& Path);
		static Count<Texture2D>	Create(uint32_t ImageWidth,uint32_t ImageHeight);
		static Count<Texture2D> Create(uint32_t width, uint32_t height, ImageFormat format, const void* data);
	};

	class Proof_API CubeMap: public Texture{
	public:
		static Count<CubeMap> Create(const std::filesystem::path& Path);
	};

	class Proof_API HDRTexture: public Texture{
	public:
		static Count<HDRTexture> Create(const std::string& path);
	};
}
