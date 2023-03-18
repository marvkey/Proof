#pragma once
#include "Proof/Core/Core.h"
#include<iostream>
#include <any>
namespace Proof {
	static uint32_t ConvertToRGBA(const Vector& color)
	{
		uint8_t r = (uint8_t)(color.X * 255.0f);
		uint8_t g = (uint8_t)(color.Y * 255.0f);
		uint8_t b = (uint8_t)(color.Z * 255.0f);
		uint8_t a = (uint8_t)(1 * 255.0f);

		uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;
		return result;
	}

	static uint32_t ConvertToRGBA(const Vector4& color)
	{
		uint8_t r = (uint8_t)(color.X * 255.0f);
		uint8_t g = (uint8_t)(color.Y * 255.0f);
		uint8_t b = (uint8_t)(color.Z * 255.0f);
		uint8_t a = (uint8_t)(color.W * 255.0f);

		uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;
		return result;
	}
	enum class ImageFormat {
		//https://docs.rs/vulkano/0.6.2/vulkano/format/index.html#:~:text=Unorm%20means%20that%20the%20values,minimum%20representable%20value%20becomes%200.0%20.
		None = 0,
		//  unsigned ints
		// 8 bytes
		RGBA,
		RGBA16,
		
		RGBA16F,
		RGBA32F,

		DEPTH16UNORM,
		DEPTH32F,
		DEPTH16UNORMSTENCIL8UINT,
		DEPTH24UNORMSTENCIL8UINT,
		DEPTH32FSTENCIL8UINT,
		STENCIL8UINT,

		BGRA8UNORM,
		RGBA8UNORM,
		BGR8UNORM,
		RGB8UNORM,

	};
	struct Image {
	public:
		Image() {

		}
		Image(const void* image,ImageFormat format, Vector2 size) :
			SourceImage(image), Format(format), Size(size)
		{

		}
		bool HasImage()const {
			if (SourceImage == nullptr)return false;
			return true;
		}
		ImageFormat Format = ImageFormat::None;
		Vector2 Size;
		const void* SourceImage = nullptr;
	protected:
		// any other data that our current api may need for images
		// the reason its here is because if we put it in the substruct
		// casting it to an image will make the data be realeased
		// we dont wanna be throwing images as pointers to avoid that 
		// so we use this
		// on visual sutdio it std::any uses small stoarge minimum of 48 bytes
		// so when using this we have to make sure we dont put data in there that is bigger 
		// than 48 bytes as that data will get allocated on the heap
		//https://www.youtube.com/watch?v=7nPrUBNGRAk&t=518s
		std::any m_ExcessData;
	};


	// basiclly for framebuffers and swapchain which store 3 iamges
	// image index
	struct ImageLayouts {
		ImageLayouts(std::initializer_list<Image> attachments)
			: Images(attachments) {}

		ImageLayouts(const std::vector<Image>& attachments)
			: Images(attachments) {}

		ImageLayouts() {

		}

		bool HasImages()const {
			return Images.size() > 0;
		}
		std::vector<Image> Images;
	};
	class Proof_API Texture: public Asset
	{
	public:
		Texture()
		{

		}
		virtual Image GetImage()const = 0;
		// if mempty means string was constructed with no path
		virtual std::string GetPath()const = 0;


		ASSET_CLASS_TYPE(Texture);
	};
	struct TextureUsage {
		enum Enum : uint32_t {
			DoNotCare = (1u << 0),
			Color = (1u << 1),
			Depth = (1u << 2),
		};
	};

	enum class AdressType {
		Repeat,
		ClampEdge
	};
	struct TextureConfig {
		ImageFormat Format = ImageFormat::RGBA;
		TextureUsage::Enum Usage = TextureUsage::DoNotCare;
		uint32_t width=10, Height =10;
		AdressType  Address = AdressType::Repeat;
	};
	class Proof_API Texture2D: public Texture {
	public:
		virtual void Recreate(const std::string& path) = 0;
		static Count<Texture2D> GenerateBRDF(uint32_t dimension = 512, uint32_t sampleCount = 1024);
		static Count<Texture2D> Create(TextureConfig config);
		static Count<Texture2D> Create(const std::string& Path);
		static Count<Texture2D>	Create(uint32_t ImageWidth,uint32_t ImageHeight);
		static Count<Texture2D> Create(uint32_t width, uint32_t height, ImageFormat format, const void* data);
	};

	class Proof_API CubeMap: public Texture{
	public:
		static Count<CubeMap> Create(const std::filesystem::path& Path, uint32_t dimension = 512, bool generateMips = false);
		static Count<CubeMap> Create(Count<CubeMap>map, Count<class Shader> shader, uint32_t dimension = 64, bool generateMips = false);
		static Count<CubeMap> GeneratePrefiltered(Count<CubeMap>map, uint32_t dimension = 128,uint32_t numSamples =1024);
	};

}
