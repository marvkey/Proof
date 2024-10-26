#include "Proofprch.h"
#include "ImageUtils.h"

namespace Proof
{
	namespace Utils 
	{
		uint32_t BytesPerPixel(ImageFormat format)
		{
			switch (format)
			{
				case ImageFormat::None:
					return 0;
					break;
				case ImageFormat::R:
					return 1;
					break;
				case ImageFormat::RG:
					return 2;
					break;
				case ImageFormat::RGB:
					return 3;
					break;
				case ImageFormat::BGR8:
					return 3;
					break;
				case ImageFormat::RGBA:
					return 4;
					break;
				case ImageFormat::BGRA8:
					return 4;
					break;
				case ImageFormat::R8UI:
					return 1;
					break;
				case ImageFormat::RG8UI:
					return 2;
					break;
				case ImageFormat::RGB8UI:
					return 3;
					break;
				case ImageFormat::BGR8UI:
					return 3;
					break;
				case ImageFormat::RGBA8UI:
					return 4;
					break;
				case ImageFormat::BGRA8UI:
					return 4;
					break;
				case ImageFormat::STENCIL8UI:
					return 1;
					break;
				case ImageFormat::R8SNORM:
					return 1;
					break;
				case ImageFormat::RG8SNORM:
					return 2;
					break;
				case ImageFormat::RGB8SNORM:
					return 3;
					break;
				case ImageFormat::BGR8SNORM:
					return 3;
					break;
				case ImageFormat::RGBA8SNORM:
					return 4;
					break;
				case ImageFormat::BRGA8SNORM:
					return 4;
					break;
				case ImageFormat::R8I:
					return 1;
					break;
				case ImageFormat::RG8I:
					return 2;
					break;
				case ImageFormat::RGB8I:
					return 3;
					break;
				case ImageFormat::BGR8I:
					return 3;
					break;
				case ImageFormat::RGBA8I:
					return 4;
					break;
				case ImageFormat::BGRA8I:
					return 4;
					break;
				case ImageFormat::R16:
					return 2;
					break;
				case ImageFormat::RG16:
					return 4;
					break;
				case ImageFormat::RGB16:
					return 6;
					break;
				case ImageFormat::RGBA16:
					return 8;
					break;
				case ImageFormat::R16UI:
					return 2;
					break;
				case ImageFormat::RG16UI:
					return 4;
					break;
				case ImageFormat::RGB16UI:
					return 6;
					break;
				case ImageFormat::RGBA16UI:
					return 8;
					break;
				case ImageFormat::DEPTH16:
					return 2;
					break;
				case ImageFormat::DEPTH16STENCIL8UI:
					return 3;
					break;
				case ImageFormat::R16SNORM:
					return 2;
					break;
				case ImageFormat::RG16SNORM:
					return 4;
					break;
				case ImageFormat::RGB16SNORM:
					return 6;
					break;
				case ImageFormat::RGBA16SNORM:
					return 8;
					break;
				case ImageFormat::R16I:
					return 2;
					break;
				case ImageFormat::RG16I:
					return 4;
					break;
				case ImageFormat::RGB16I:
					return 6;
					break;
				case ImageFormat::RGBA16I:
					return 8;
					break;
				case ImageFormat::R16F:
					return 2;
					break;
				case ImageFormat::RG16F:
					return 4;
					break;
				case ImageFormat::RGB16F:
					return 6;
					break;
				case ImageFormat::RGBA16F:
					return 8;
					break;
				case ImageFormat::R32UI:
					return 4;
					break;
				case ImageFormat::RG32UI:
					return 8;
					break;
				case ImageFormat::RGB32UI:
					return 12;
					break;
				case ImageFormat::RGBA32UI:
					return 16;
					break;
				case ImageFormat::X8DEPTH24PACK32:
					return 4;
					break;
				case ImageFormat::DEPTH24STENCIL8UI:
					return 4;
					break;
				case ImageFormat::R32I:
					return 4;
					break;
				case ImageFormat::RG32I:
					return 8;
					break;
				case ImageFormat::RGB32I:
					return 12;
					break;
				case ImageFormat::RGBA32I:
					return 16;
					break;
				case ImageFormat::R32F:
					return 4;
					break;
				case ImageFormat::RG32F:
					return 8;
					break;
				case ImageFormat::RGB32F:
					return 12;
					break;
				case ImageFormat::RGBA32F:
					return 16;
					break;
				case ImageFormat::DEPTH32F:
					return 4;
					break;
				case ImageFormat::DEPTH32FSTENCIL8UI:
					return 5;
					break;
				case ImageFormat::R64UI:
					return 8;
					break;
				case ImageFormat::RG64UI:
					return 16;
					break;
				case ImageFormat::RGB64UI:
					return 24;
					break;
				case ImageFormat::RGBA64UI:
					return 32;
					break;
				case ImageFormat::R64I:
					return 8;
					break;
				case ImageFormat::RG64I:
					return 16;
					break;
				case ImageFormat::RGB64I:
					return 24;
					break;
				case ImageFormat::RGBA64I:
					return 32;
					break;
				case ImageFormat::R64F:
					return 8;
					break;
				case ImageFormat::RG64F:
					return 16;
					break;
				case ImageFormat::RGB64F:
					return 24;
					break;
				case ImageFormat::RGBA64F:
					return 32;
					break;

				case ImageFormat::B10G11R11_UF_PACK32:
					return 4;
				case ImageFormat::E5B9G9R9_UF_PACK32:
					return 4;
				case ImageFormat::D16:
					return 2;
				case ImageFormat::X8_D24_PACK32:
					return 4;
				case ImageFormat::D32_F:
					return 4;
				case ImageFormat::S8_UI:
					return 1;
				case ImageFormat::D16_S8_UI:
					return 2;
				case ImageFormat::D24_S8_UI:
					return 4;
				case ImageFormat::D32_FS8_UI:
					return 8;
				case ImageFormat::BC1_RGB_BLOCK:
				case ImageFormat::BC1_RGB_SRGB_BLOCK:
					return 8;
				case ImageFormat::BC1_RGBA_BLOCK:
				case ImageFormat::BC1_RGBA_SRGB_BLOCK:
					return 8;
				case ImageFormat::BC2_BLOCK:
				case ImageFormat::BC2_SRGB_BLOCK:
					return 16;
				case ImageFormat::BC3_BLOCK:
				case ImageFormat::BC3_SRGB_BLOCK:
					return 16;
				case ImageFormat::BC4_BLOCK:
				case ImageFormat::BC4_SNORM_BLOCK:
					return 8;
				case ImageFormat::BC5_BLOCK:
				case ImageFormat::BC5_SNORM_BLOCK:
					return 16;
				case ImageFormat::BC6H_UF_BLOCK:
				case ImageFormat::BC6H_F_BLOCK:
					return 16;
				case ImageFormat::BC7__BLOCK:
				case ImageFormat::BC7_SRGB_BLOCK:
					return 16;
				case ImageFormat::ETC2_R8G8B8_BLOCK:
				case ImageFormat::ETC2_R8G8B8_SRGB_BLOCK:
					return 8;
				case ImageFormat::ETC2_R8G8B8A1_BLOCK:
				case ImageFormat::ETC2_R8G8B8A1_SRGB_BLOCK:
					return 8;
				case ImageFormat::ETC2_R8G8B8A8_BLOCK:
				case ImageFormat::ETC2_R8G8B8A8_SRGB_BLOCK:
					return 16;
				case ImageFormat::EAC_R11_BLOCK:
				case ImageFormat::EAC_R11_SNORM_BLOCK:
					return 8;
				case ImageFormat::EAC_R11G11_BLOCK:
				case ImageFormat::EAC_R11G11_SNORM_BLOCK:
					return 16;
				case ImageFormat::ASTC_4x4_BLOCK:
				case ImageFormat::ASTC_4x4_SRGB_BLOCK:
					return 16;
				case ImageFormat::ASTC_5x4_BLOCK:
				case ImageFormat::ASTC_5x4_SRGB_BLOCK:
					return 16;
				case ImageFormat::ASTC_5x5_BLOCK:
				case ImageFormat::ASTC_5x5_SRGB_BLOCK:
					return 16;
				case ImageFormat::ASTC_6x5_BLOCK:
				case ImageFormat::ASTC_6x5_SRGB_BLOCK:
					return 16;
				case ImageFormat::ASTC_6x6_BLOCK:
				case ImageFormat::ASTC_6x6_SRGB_BLOCK:
					return 16;
				case ImageFormat::ASTC_8x5_BLOCK:
				case ImageFormat::ASTC_8x5_SRGB_BLOCK:
					return 16;
				case ImageFormat::ASTC_8x6_BLOCK:
				case ImageFormat::ASTC_8x6_SRGB_BLOCK:
					return 16;
				case ImageFormat::ASTC_8x8_BLOCK:
				case ImageFormat::ASTC_8x8_SRGB_BLOCK:
					return 16;
				case ImageFormat::ASTC_10x5_BLOCK:
				case ImageFormat::ASTC_10x5_SRGB_BLOCK:
					return 16;
				case ImageFormat::ASTC_10x6_BLOCK:
				case ImageFormat::ASTC_10x6_SRGB_BLOCK:
					return 16;
				case ImageFormat::ASTC_10x8_BLOCK:
				case ImageFormat::ASTC_10x8_SRGB_BLOCK:
					return 16;
				case ImageFormat::ASTC_10x10_BLOCK:
				case ImageFormat::ASTC_10x10_SRGB_BLOCK:
					return 16;
				case ImageFormat::ASTC_12x10_BLOCK:
				case ImageFormat::ASTC_12x10_SRGB_BLOCK:
					return 16;
				case ImageFormat::ASTC_12x12_BLOCK:
				case ImageFormat::ASTC_12x12_SRGB_BLOCK:
					return 16;
				default:
					break;
			}
			PF_CORE_ASSERT(false, fmt::format("not supported yet {}", EnumReflection::EnumString(format)));

		}
	}
}

