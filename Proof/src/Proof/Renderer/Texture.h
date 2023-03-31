#pragma once
#include "Proof/Core/Core.h"
#include<iostream>
#include <any>
namespace Proof {
	static uint32_t ConvertToBytes(const Vector& color)
	{
		uint8_t r = (uint8_t)(color.X * 255.0f);
		uint8_t g = (uint8_t)(color.Y * 255.0f);
		uint8_t b = (uint8_t)(color.Z * 255.0f);
		uint8_t a = (uint8_t)(1 * 255.0f);

		uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;
		return result;
	}

	static uint32_t ConvertToBytes(const Vector4& color)
	{
		uint8_t r = (uint8_t)(color.X * 255.0f);
		uint8_t g = (uint8_t)(color.Y * 255.0f);
		uint8_t b = (uint8_t)(color.Z * 255.0f);
		uint8_t a = (uint8_t)(color.W * 255.0f);

		uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;
		return result;
	}

	/**
	*https://docs.rs/vulkano/0.6.2/vulkano/format/index.html#:~:text=Unorm%20means%20that%20the%20values,minimum%20representable%20value%20becomes%200.0%20.
	* TODO(ADD ALL THE TYPES ON HE THIS ARITCLE)
	* https://github.com/KhronosGroup/Vulkan-Samples-Deprecated/blob/master/external/include/vulkan/vk_format.h
	 * R= Red (GL_RED)
	 * G = green
	 * B = blue
	 * I = INT
	 * UI = UNSINGED INT
	 * NO TYPE IS UNORM : Unsigned UNORM
	 * SNORM = Signed NORM
	 * FLOAT = SIGNED FLOAT
	 * al floats are signed
	 * any type that does not have a F,UI,I,is a UNORM
	 * 
	 */
	enum class ImageFormat {
		None = 0,
		//
		// 8 bits per component
		//
		#pragma region BitsPerComponent8
		#pragma region UnsignedByte

		// this are s autoamically
		R, //R8
		RG,//RG8
		RGB,//RGB8
		BGR8,
		RGBA,//RGBA8
		BGRA8,
		R8UI,
		RG8UI,
		RGB8UI,
		BGR8UI,
		RGBA8UI,
		BGRA8UI,
		STENCIL8UI,
		#pragma endregion 

		#pragma region Int8_t
		R8SNORM,
		RG8SNORM,
		RGB8SNORM,
		BGR8SNORM,
		RGBA8SNORM,
		BRGA8SNORM,
		R8I,
		RG8I,
		RGB8I,
		BGR8I,
		RGBA8I,
		BGRA8I,
		#pragma endregion 
		#pragma endregion 

		#pragma region BitsPerComponent16
		#pragma region unsiged16Int
		R16,
		RG16,
		RGB16,
		RGBA16,
		R16UI,
		RG16UI,
		RGB16UI,
		RGBA16UI,
		DEPTH16,
		DEPTH16STENCIL8UI,
		#pragma endregion
		#pragma region Int16
		R16SNORM,
		RG16SNORM,
		RGB16SNORM,
		RGBA16SNORM,
		R16I,
		RG16I,
		RGB16I,
		RGBA16I,
		#pragma endregion

		#pragma region flaot16
		R16F,
		RG16F,
		RGB16F,
		RGBA16F,
		#pragma endregion
		#pragma endregion 

		#pragma region BitsPerComponent32
		#pragma region UNSGINEDInt32
		R32UI,
		RG32UI,
		RGB32UI,
		RGBA32UI,
		X8DEPTH24PACK32,
		DEPTH24STENCIL8UI, //VK_FORMAT_D24_UNORM_S8_UINT
		#pragma endregion 
		#pragma region Int32
		R32I,
		RG32I,
		RGB32I,
		RGBA32I,
		#pragma endregion 
		#pragma region FLOAT
		R32F,
		RG32F,
		RGB32F,
		RGBA32F,
		DEPTH32F,
		DEPTH32FSTENCIL8UI,
		#pragma endregion 
		#pragma endregion 

		#pragma region BitsPerComponent64

		#pragma region UNSIGNEDInt64
		R64UI,
		RG64UI,
		RGB64UI,
		RGBA64UI,
		#pragma endregion 

		#pragma region Int64
		R64I,
		RG64I,
		RGB64I,
		RGBA64I,
		#pragma endregion 
		#pragma region Double
		R64F,
		RG64F,
		RGB64F,
		RGBA64F,
		#pragma endregion 
		#pragma endregion 
	};
	struct Image {
	public:
		Image() {

		}
		Image(const void* image, ImageFormat format, Vector2 size) :
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
	class Proof_API Texture : public Asset
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
		uint32_t width = 10, Height = 10;
		AdressType  Address = AdressType::Repeat;
	};
	class Proof_API Texture2D : public Texture {
	public:
		virtual uint32_t GetHeight() = 0;

		virtual uint32_t GetWidth() = 0;
		virtual void SetData(const void* data)= 0;
		virtual void Recreate(const std::string& path) = 0;
		static Count<Texture2D> GenerateBRDF(uint32_t dimension = 512, uint32_t sampleCount = 1024);
		static Count<Texture2D> Create(TextureConfig config);
		static Count<Texture2D> Create(const std::string& Path);
		static Count<Texture2D>	Create(uint32_t ImageWidth, uint32_t ImageHeight);
		static Count<Texture2D> Create(uint32_t width, uint32_t height, ImageFormat format, const void* data);
	};

	class Proof_API CubeMap : public Texture {
	public:
		static Count<CubeMap> Create(Count<Texture2D> textrure, uint32_t dimension = 512, bool generateMips = false);
		static Count<CubeMap> Create(const std::filesystem::path& Path, uint32_t dimension = 512, bool generateMips = false);
		static Count<CubeMap> Create(Count<CubeMap>map, Count<class Shader> shader, uint32_t dimension = 64, bool generateMips = false);
		static Count<CubeMap> GeneratePrefiltered(Count<CubeMap>map, uint32_t dimension = 128, uint32_t numSamples = 1024);
	};

}
