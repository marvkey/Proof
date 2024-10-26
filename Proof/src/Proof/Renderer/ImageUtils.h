#pragma once
#include "Proof/Core/Core.h"
#include <glm/glm.hpp>
namespace Proof
{
	static uint32_t ConvertToBytes(const glm::vec3& color)
	{
		uint8_t r = (uint8_t)(color.x * 255.0f);
		uint8_t g = (uint8_t)(color.y * 255.0f);
		uint8_t b = (uint8_t)(color.z * 255.0f);
		uint8_t a = (uint8_t)(1 * 255.0f);

		uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;
		return result;
	}

	static uint32_t ConvertToBytes(const glm::vec4& color)
	{
		uint8_t r = (uint8_t)(color.x * 255.0f);
		uint8_t g = (uint8_t)(color.y * 255.0f);
		uint8_t b = (uint8_t)(color.z * 255.0f);
		uint8_t a = (uint8_t)(color.w * 255.0f);

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
	enum class ImageFormat
	{
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

		B10G11R11_UF_PACK32,
		E5B9G9R9_UF_PACK32,
		D16,
		X8_D24_PACK32,
		D32_F,
		S8_UI,
		D16_S8_UI,
		D24_S8_UI,
		D32_FS8_UI,
		BC1_RGB_BLOCK,
		BC1_RGB_SRGB_BLOCK,
		BC1_RGBA_BLOCK,
		BC1_RGBA_SRGB_BLOCK,
		BC2_BLOCK,
		BC2_SRGB_BLOCK,
		BC3_BLOCK,
		BC3_SRGB_BLOCK,
		BC4_BLOCK,
		BC4_SNORM_BLOCK,
		BC5_BLOCK,
		BC5_SNORM_BLOCK,
		BC6H_UF_BLOCK,
		BC6H_F_BLOCK,
		BC7__BLOCK,
		BC7_SRGB_BLOCK,

		ETC2_R8G8B8_BLOCK,
		ETC2_R8G8B8_SRGB_BLOCK,
		ETC2_R8G8B8A1_BLOCK,
		ETC2_R8G8B8A1_SRGB_BLOCK,
		ETC2_R8G8B8A8_BLOCK,
		ETC2_R8G8B8A8_SRGB_BLOCK,
		EAC_R11_BLOCK,
		EAC_R11_SNORM_BLOCK,
		EAC_R11G11_BLOCK,
		EAC_R11G11_SNORM_BLOCK,
		ASTC_4x4_BLOCK,
		ASTC_4x4_SRGB_BLOCK,
		ASTC_5x4_BLOCK,
		ASTC_5x4_SRGB_BLOCK,
		ASTC_5x5_BLOCK,
		ASTC_5x5_SRGB_BLOCK,
		ASTC_6x5_BLOCK,
		ASTC_6x5_SRGB_BLOCK,
		ASTC_6x6_BLOCK,
		ASTC_6x6_SRGB_BLOCK,
		ASTC_8x5_BLOCK,
		ASTC_8x5_SRGB_BLOCK,
		ASTC_8x6_BLOCK,
		ASTC_8x6_SRGB_BLOCK,
		ASTC_8x8_BLOCK,
		ASTC_8x8_SRGB_BLOCK,
		ASTC_10x5_BLOCK,
		ASTC_10x5_SRGB_BLOCK,
		ASTC_10x6_BLOCK,
		ASTC_10x6_SRGB_BLOCK,
		ASTC_10x8_BLOCK,
		ASTC_10x8_SRGB_BLOCK,
		ASTC_10x10_BLOCK,
		ASTC_10x10_SRGB_BLOCK,
		ASTC_12x10_BLOCK,
		ASTC_12x10_SRGB_BLOCK,
		ASTC_12x12_BLOCK,
		ASTC_12x12_SRGB_BLOCK,
	};
	enum class SamplerWrap
	{
		Repeat,
		MirroredRepeat,
		ClampEdge,
		MirroredClampEdge,
		ClampBorder,
	};

	enum class SamplerFilter
	{
		Linear,
		Nearest,
		Cubic
	};
	namespace Utils
	{
		//Note that the stencil component is always 1 byte per pixel in these formats.
		//from chatgpt
		uint32_t BytesPerPixel(ImageFormat format);

		inline uint64_t GetImageMemorySize(ImageFormat format, uint32_t width, uint32_t height)
		{
			return width * height * BytesPerPixel(format);
		}
	}
}