#pragma once
#include "Proof/Core/Core.h"
#include <glm/glm.hpp>
#include "RendererResouce.h"
#include "Proof/Math/Vector.h"
#include "Proof/Core/Assert.h"
#include "Proof/Resources/EnumReflection.h"

namespace Proof
{
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

	enum class ImageUsage {
		None = 0,
		Texture,
		Attachment,
		Storage,
		SwapChain, //TODO Remove dont use it only for swapchain going to be removes
		HostRead //HostRead
	};
	struct ImageConfiguration
	{
		std::string DebugName;
		bool Transfer = false;
		ImageFormat Format = ImageFormat::RGBA;
		ImageUsage Usage = ImageUsage::Texture;
		uint32_t Width = 1, Height = 1;
		uint32_t Mips = 1;
		uint32_t Layers = 1;
	};


	class Image : public RendererResource
	{
	public:
		virtual ~Image() = default;
		virtual Vector2U GetSize() = 0;
		virtual float GetAspectRatio() = 0;;
		virtual uint32_t GetWidth() = 0;
		virtual uint32_t GetHeight() = 0;

	};
	class Image2D : public Image {
	public:
		virtual ~Image2D() = default;
		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual void Resize(Vector2U size) = 0;
		virtual const ImageConfiguration& GetSpecification()const = 0;
		static Count<Image2D>Create(const ImageConfiguration& specification);
		RENDER_VIEW_RESOURCE_CLASS_TYPE(Image2D);
	};
	enum class ImageViewType {
		None = 0,
		View1D,
		View2D,
		View3D,
		ViewCube,
		View1DArray,
		View2DArray,
		ViewCubeArray
	};
	struct ImageViewConfiguration
	{
		std::string DebugName;
		Count<Image2D> Image;

		uint32_t Layer = 0;
		uint32_t Mip = 0;

		uint32_t LayerCount = 1;
		uint32_t MipCount = 1;

		ImageViewType View = ImageViewType::View2D;
	};
	class ImageView : public Image {
	public:
		virtual ~ImageView() = default;
		static Count<ImageView>Create(const ImageViewConfiguration& specification);
		virtual const ImageViewConfiguration& GetSpecification()const = 0;
		virtual Count<Image2D> GetImage()const = 0;
		virtual glm::uvec2 GetMipSize() = 0;
		RENDER_VIEW_RESOURCE_CLASS_TYPE(ImageView);

	};

	// basiclly for framebuffers and swapchain which store 3 iamges
	// image index
	struct ImageLayouts2D {

		ImageLayouts2D(const ImageLayouts2D&) = default;
		ImageLayouts2D(Count<Image2D> image);
		ImageLayouts2D(std::initializer_list<Count<Image2D>> images);
		ImageLayouts2D(const std::vector<Count<Image2D>>& images);
		ImageLayouts2D(Count<ImageView> image);
		ImageLayouts2D(std::initializer_list<Count<ImageView>> images);
		ImageLayouts2D(const std::vector<Count<ImageView>>& images);
		ImageLayouts2D();

		bool HasImages()const {
			return Images.size() > 0;
		}

		Count<Image> GetImageIndex(uint32_t index)const
		{
			PF_CORE_ASSERT(index < Images.size(), " Does not contain image Index");
			return Images[index];
		}
		// only image2D AND ImageView
		std::vector<Count<Image>> Images;
	};

	namespace Utils
	{
		//Note that the stencil component is always 1 byte per pixel in these formats.
		//from chatgpt
		inline uint32_t BytesPerPixel(ImageFormat format) {

			switch (format)
			{
				case Proof::ImageFormat::None:
					return 0;
					break;
				case Proof::ImageFormat::R:
					return 1;
					break;
				case Proof::ImageFormat::RG:
					return 2;
					break;
				case Proof::ImageFormat::RGB:
					return 3;
					break;
				case Proof::ImageFormat::BGR8:
					return 3;
					break;
				case Proof::ImageFormat::RGBA:
					return 4;
					break;
				case Proof::ImageFormat::BGRA8:
					return 4;
					break;
				case Proof::ImageFormat::R8UI:
					return 1;
					break;
				case Proof::ImageFormat::RG8UI:
					return 2;
					break;
				case Proof::ImageFormat::RGB8UI:
					return 3;
					break;
				case Proof::ImageFormat::BGR8UI:
					return 3;
					break;
				case Proof::ImageFormat::RGBA8UI:
					return 4;
					break;
				case Proof::ImageFormat::BGRA8UI:
					return 4;
					break;
				case Proof::ImageFormat::STENCIL8UI:
					return 1;
					break;
				case Proof::ImageFormat::R8SNORM:
					return 1;
					break;
				case Proof::ImageFormat::RG8SNORM:
					return 2;
					break;
				case Proof::ImageFormat::RGB8SNORM:
					return 3;
					break;
				case Proof::ImageFormat::BGR8SNORM:
					return 3;
					break;
				case Proof::ImageFormat::RGBA8SNORM:
					return 4;
					break;
				case Proof::ImageFormat::BRGA8SNORM:
					return 4;
					break;
				case Proof::ImageFormat::R8I:
					return 1;
					break;
				case Proof::ImageFormat::RG8I:
					return 2;
					break;
				case Proof::ImageFormat::RGB8I:
					return 3;
					break;
				case Proof::ImageFormat::BGR8I:
					return 3;
					break;
				case Proof::ImageFormat::RGBA8I:
					return 4;
					break;
				case Proof::ImageFormat::BGRA8I:
					return 4;
					break;
				case Proof::ImageFormat::R16:
					return 2;
					break;
				case Proof::ImageFormat::RG16:
					return 4;
					break;
				case Proof::ImageFormat::RGB16:
					return 6;
					break;
				case Proof::ImageFormat::RGBA16:
					return 8;
					break;
				case Proof::ImageFormat::R16UI:
					return 2;
					break;
				case Proof::ImageFormat::RG16UI:
					return 4;
					break;
				case Proof::ImageFormat::RGB16UI:
					return 6;
					break;
				case Proof::ImageFormat::RGBA16UI:
					return 8;
					break;
				case Proof::ImageFormat::DEPTH16:
					return 2;
					break;
				case Proof::ImageFormat::DEPTH16STENCIL8UI:
					return 3;
					break;
				case Proof::ImageFormat::R16SNORM:
					return 2;
					break;
				case Proof::ImageFormat::RG16SNORM:
					return 4;
					break;
				case Proof::ImageFormat::RGB16SNORM:
					return 6;
					break;
				case Proof::ImageFormat::RGBA16SNORM:
					return 8;
					break;
				case Proof::ImageFormat::R16I:
					return 2;
					break;
				case Proof::ImageFormat::RG16I:
					return 4;
					break;
				case Proof::ImageFormat::RGB16I:
					return 6;
					break;
				case Proof::ImageFormat::RGBA16I:
					return 8;
					break;
				case Proof::ImageFormat::R16F:
					return 2;
					break;
				case Proof::ImageFormat::RG16F:
					return 4;
					break;
				case Proof::ImageFormat::RGB16F:
					return 6;
					break;
				case Proof::ImageFormat::RGBA16F:
					return 8;
					break;
				case Proof::ImageFormat::R32UI:
					return 4;
					break;
				case Proof::ImageFormat::RG32UI:
					return 8;
					break;
				case Proof::ImageFormat::RGB32UI:
					return 12;
					break;
				case Proof::ImageFormat::RGBA32UI:
					return 16;
					break;
				case Proof::ImageFormat::X8DEPTH24PACK32:
					return 4;
					break;
				case Proof::ImageFormat::DEPTH24STENCIL8UI:
					return 4;
					break;
				case Proof::ImageFormat::R32I:
					return 4;
					break;
				case Proof::ImageFormat::RG32I:
					return 8;
					break;
				case Proof::ImageFormat::RGB32I:
					return 12;
					break;
				case Proof::ImageFormat::RGBA32I:
					return 16;
					break;
				case Proof::ImageFormat::R32F:
					return 4;
					break;
				case Proof::ImageFormat::RG32F:
					return 8;
					break;
				case Proof::ImageFormat::RGB32F:
					return 12;
					break;
				case Proof::ImageFormat::RGBA32F:
					return 16;
					break;
				case Proof::ImageFormat::DEPTH32F:
					return 4;
					break;
				case Proof::ImageFormat::DEPTH32FSTENCIL8UI:
					return 5;
					break;
				case Proof::ImageFormat::R64UI:
					return 8;
					break;
				case Proof::ImageFormat::RG64UI:
					return 16;
					break;
				case Proof::ImageFormat::RGB64UI:
					return 24;
					break;
				case Proof::ImageFormat::RGBA64UI:
					return 32;
					break;
				case Proof::ImageFormat::R64I:
					return 8;
					break;
				case Proof::ImageFormat::RG64I:
					return 16;
					break;
				case Proof::ImageFormat::RGB64I:
					return 24;
					break;
				case Proof::ImageFormat::RGBA64I:
					return 32;
					break;
				case Proof::ImageFormat::R64F:
					return 8;
					break;
				case Proof::ImageFormat::RG64F:
					return 16;
					break;
				case Proof::ImageFormat::RGB64F:
					return 24;
					break;
				case Proof::ImageFormat::RGBA64F:
					return 32;
					break;
				default:
					break;
			}
			PF_CORE_ASSERT(false, fmt::format("not supported yet {}", EnumReflection::EnumString(format)));

		}

		inline uint64_t GetImageMemorySize(ImageFormat format, uint32_t width, uint32_t height)
		{
			return width * height * BytesPerPixel(format);
		}
	}
}