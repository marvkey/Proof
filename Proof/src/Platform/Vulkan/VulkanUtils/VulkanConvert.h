#pragma once
#include<vulkan/vulkan.h>
#include "Proof/Renderer/Shader.h"
#include "Proof/Renderer/Texture.h"
#include "../Vulkan.h"
// MAKE STATIC SO WEHN INCLUDING IN FILE IT DOES NOT REDFINDE ITSELF
namespace Proof
{
	namespace Utils
	{
		static bool IsIntegerBased(ImageFormat format)
		{
			switch (format)
			{
				case ImageFormat::R8UI:
				case ImageFormat::RG8UI:
				case ImageFormat::RGB8UI:
				case ImageFormat::RGBA8UI:
				case ImageFormat::R16UI:
				case ImageFormat::RG16UI:
				case ImageFormat::RGB16UI:
				case ImageFormat::RGBA16UI:
				case ImageFormat::R32UI:
				case ImageFormat::RG32UI:
				case ImageFormat::RGB32UI:
				case ImageFormat::RGBA32UI:
				case ImageFormat::STENCIL8UI:
				case ImageFormat::DEPTH16STENCIL8UI:
				case ImageFormat::DEPTH24STENCIL8UI:
				case ImageFormat::X8DEPTH24PACK32:
					return true;
				default:
					return false;
			}
			PF_CORE_ASSERT(false, "Not supported");
		}
		
		static VkShaderStageFlagBits ProofShaderToVulkanShader(ShaderStage stage) {
			switch (stage)
			{
				case Proof::ShaderStage::Vertex:
					return VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT;
					break;
				case Proof::ShaderStage::Fragment:
					return VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT;
					break;
				case Proof::ShaderStage::Compute:
					return VkShaderStageFlagBits::VK_SHADER_STAGE_COMPUTE_BIT;
					break;
				case Proof::ShaderStage::Geometry:
					return VkShaderStageFlagBits::VK_SHADER_STAGE_GEOMETRY_BIT;
					break;
			}
			PF_CORE_ASSERT(false, "Not supported");
		}
		static VkFormat ProofFormatToVulkanFormat(ImageFormat format) {
			//refence
			//https://github.com/KhronosGroup/Vulkan-Samples-Deprecated/blob/master/external/include/vulkan/vk_format.h

			switch (format)
			{
				#pragma region BitsPerComponent8

				#pragma region UnsignedByte
				case Proof::ImageFormat::None:
					return VK_FORMAT_UNDEFINED;
					break;
				case Proof::ImageFormat::R:
					return VK_FORMAT_R8_UNORM;
					break;
				case Proof::ImageFormat::RG:
					return VK_FORMAT_R8G8_UNORM;
					break;
				case Proof::ImageFormat::RGB:
					return VK_FORMAT_R8G8B8_UNORM;
					break;
				case Proof::ImageFormat::BGR8:
					return  VK_FORMAT_B8G8R8_UNORM;
					break;
				case Proof::ImageFormat::RGBA:
					return VK_FORMAT_R8G8B8A8_UNORM;
					break;
				case Proof::ImageFormat::BGRA8:
					return VK_FORMAT_B8G8R8A8_UNORM;
					break;
				case Proof::ImageFormat::R8UI:
					return VK_FORMAT_R8_UINT;
					break;
				case Proof::ImageFormat::RG8UI:
					return VK_FORMAT_R8G8_UINT;
					break;
				case Proof::ImageFormat::RGB8UI:
					return  VK_FORMAT_R8G8B8_UINT;
					break;
				case Proof::ImageFormat::BGR8UI:
					return VK_FORMAT_B8G8R8_UINT;
					break;
				case Proof::ImageFormat::RGBA8UI:
					return VK_FORMAT_R8G8B8A8_UINT;
					break;
				case Proof::ImageFormat::BGRA8UI:
					return VK_FORMAT_B8G8R8A8_UINT;
					break;
				case Proof::ImageFormat::STENCIL8UI:
					return VK_FORMAT_S8_UINT;
					break;
					#pragma endregion 

					#pragma region Int8_t

				case Proof::ImageFormat::R8SNORM:
					return VK_FORMAT_R8_SNORM;
					break;
				case Proof::ImageFormat::RG8SNORM:
					return VK_FORMAT_R8G8_SNORM;
					break;
				case Proof::ImageFormat::RGB8SNORM:
					return VK_FORMAT_R8G8B8_SNORM;
					break;
				case Proof::ImageFormat::BGR8SNORM:
					return VK_FORMAT_B8G8R8_SNORM;
					break;
				case Proof::ImageFormat::RGBA8SNORM:
					return VK_FORMAT_R8G8B8A8_SNORM;
					break;
				case Proof::ImageFormat::BRGA8SNORM:
					return VK_FORMAT_B8G8R8A8_SNORM;
					break;
				case Proof::ImageFormat::R8I:
					return VK_FORMAT_R8_SINT;
					break;
				case Proof::ImageFormat::RG8I:
					return VK_FORMAT_R8G8_SINT;
					break;
				case Proof::ImageFormat::RGB8I:
					return VK_FORMAT_R8G8B8_SINT;
					break;
				case Proof::ImageFormat::BGR8I:
					return VK_FORMAT_B8G8R8_SINT;
					break;
				case Proof::ImageFormat::RGBA8I:
					return VK_FORMAT_R8G8B8A8_SINT;
					break;
				case Proof::ImageFormat::BGRA8I:
					return VK_FORMAT_B8G8R8A8_SINT;
					break;
					#pragma endregion

					#pragma endregion 

					#pragma region BitsPerComponent16

					#pragma region unsiged16Int

				case Proof::ImageFormat::R16:
					return VK_FORMAT_R16_UNORM;
					break;
				case Proof::ImageFormat::RG16:
					return VK_FORMAT_R16G16_UNORM;
					break;
				case Proof::ImageFormat::RGB16:
					return VK_FORMAT_R16G16B16_UNORM;
					break;
				case Proof::ImageFormat::RGBA16:
					return VK_FORMAT_R16G16B16A16_UNORM;
					break;
				case Proof::ImageFormat::R16UI:
					return VK_FORMAT_R16_UINT;
					break;
				case Proof::ImageFormat::RG16UI:
					return VK_FORMAT_R16G16_UINT;
					break;
				case Proof::ImageFormat::RGB16UI:
					return VK_FORMAT_R16G16B16_UINT;
					break;
				case Proof::ImageFormat::RGBA16UI:
					return VK_FORMAT_R16G16B16A16_UINT;
					break;
				case Proof::ImageFormat::DEPTH16:
					return VK_FORMAT_D16_UNORM;
					break;
				case Proof::ImageFormat::DEPTH16STENCIL8UI:
					return VK_FORMAT_D16_UNORM_S8_UINT;
					break;
					#pragma endregion

					#pragma region Int16
				case Proof::ImageFormat::R16SNORM:
					return VK_FORMAT_R16_SNORM;
					break;
				case Proof::ImageFormat::RG16SNORM:
					return VK_FORMAT_R16G16_SNORM;
					break;
				case Proof::ImageFormat::RGB16SNORM:
					return VK_FORMAT_R16G16B16_SNORM;
					break;
				case Proof::ImageFormat::RGBA16SNORM:
					return VK_FORMAT_R16G16B16A16_SNORM;
					break;
				case Proof::ImageFormat::R16I:
					return VK_FORMAT_R16_SINT;
					break;
				case Proof::ImageFormat::RG16I:
					return VK_FORMAT_R16G16_SINT;
					break;
				case Proof::ImageFormat::RGB16I:
					return VK_FORMAT_R16G16B16_SINT;
					break;
				case Proof::ImageFormat::RGBA16I:
					return VK_FORMAT_R16G16B16A16_SINT;
					break;
					#pragma endregion

					#pragma region flaot16
				case Proof::ImageFormat::R16F:
					return VK_FORMAT_R16_SFLOAT;
					break;
				case Proof::ImageFormat::RG16F:
					return VK_FORMAT_R16G16_SFLOAT;
					break;
				case Proof::ImageFormat::RGB16F:
					return VK_FORMAT_R16G16B16_SFLOAT;
					break;
				case Proof::ImageFormat::RGBA16F:
					return VK_FORMAT_R16G16B16A16_SFLOAT;
					break;
					#pragma endregion

					#pragma endregion

					#pragma region BitsPerComponent32

					#pragma region UNSGINEDInt32
				case Proof::ImageFormat::R32UI:
					return VK_FORMAT_R32_UINT;
					break;
				case Proof::ImageFormat::RG32UI:
					return VK_FORMAT_R32G32_UINT;
					break;
				case Proof::ImageFormat::RGB32UI:
					return VK_FORMAT_R32G32B32_UINT;
					break;
				case Proof::ImageFormat::RGBA32UI:
					return VK_FORMAT_R32G32B32A32_UINT;
					break;
				case Proof::ImageFormat::X8DEPTH24PACK32:
					return VK_FORMAT_X8_D24_UNORM_PACK32;
					break;
				case Proof::ImageFormat::DEPTH24STENCIL8UI:
					return VK_FORMAT_D24_UNORM_S8_UINT;
					break;
					#pragma endregion 

					#pragma region Int32
				case Proof::ImageFormat::R32I:
					return VK_FORMAT_R32_SINT;
					break;
				case Proof::ImageFormat::RG32I:
					return VK_FORMAT_R32G32_SINT;
					break;
				case Proof::ImageFormat::RGB32I:
					return VK_FORMAT_R32G32B32_SINT;
					break;
				case Proof::ImageFormat::RGBA32I:
					return VK_FORMAT_R32G32B32A32_SINT;
					break;
					#pragma endregion 

					#pragma region FLOAT

				case Proof::ImageFormat::R32F:
					return VK_FORMAT_R32_SFLOAT;
					break;
				case Proof::ImageFormat::RG32F:
					return VK_FORMAT_R32G32_SFLOAT;
					break;
				case Proof::ImageFormat::RGB32F:
					return VK_FORMAT_R32G32B32_SFLOAT;
					break;
				case Proof::ImageFormat::RGBA32F:
					return VK_FORMAT_R32G32B32A32_SFLOAT;
					break;
				case Proof::ImageFormat::DEPTH32F:
					return VK_FORMAT_D32_SFLOAT;
					break;
				case Proof::ImageFormat::DEPTH32FSTENCIL8UI:
					return VK_FORMAT_D32_SFLOAT_S8_UINT;
					break;
					#pragma endregion 

					#pragma endregion 

					#pragma region BitsPerComponent64

					#pragma region UNSIGNEDInt64

				case Proof::ImageFormat::R64UI:
					return VK_FORMAT_R64_UINT;
					break;
				case Proof::ImageFormat::RG64UI:
					return VK_FORMAT_R64G64_UINT;
					break;
				case Proof::ImageFormat::RGB64UI:
					return VK_FORMAT_R64G64B64_UINT;
					break;
				case Proof::ImageFormat::RGBA64UI:
					return VK_FORMAT_R64G64B64A64_UINT;
					break;
					#pragma endregion 

					#pragma region Int64

				case Proof::ImageFormat::R64I:
					return VK_FORMAT_R64_SINT;
					break;
				case Proof::ImageFormat::RG64I:
					return VK_FORMAT_R64G64_SINT;
					break;
				case Proof::ImageFormat::RGB64I:
					return VK_FORMAT_R64G64B64_SINT;
					break;
				case Proof::ImageFormat::RGBA64I:
					return VK_FORMAT_R64G64B64A64_SINT;
					break;
					#pragma endregion 

					#pragma region Double

				case Proof::ImageFormat::R64F:
					return VK_FORMAT_R64_SFLOAT;
					break;
				case Proof::ImageFormat::RG64F:
					return VK_FORMAT_R64G64_SFLOAT;
					break;
				case Proof::ImageFormat::RGB64F:
					return VK_FORMAT_R64G64B64_SFLOAT;
					break;
				case Proof::ImageFormat::RGBA64F:
					return VK_FORMAT_R64G64B64A64_SFLOAT;
					break;
					#pragma endregion 

					#pragma endregion 

			}
			PF_CORE_ASSERT(false, "Type is not supported");
			return (VkFormat)0;
		}

		static ImageFormat VulkanFormatToProofFormat(VkFormat format) {
			//reference
			//https://github.com/KhronosGroup/Vulkan-Samples-Deprecated/blob/master/external/include/vulkan/vk_format.h
			switch (format)
			{
				case VK_FORMAT_R8G8B8A8_UNORM:    return ImageFormat::RGBA; //ImageFormat::RGBA8UNORM
				case VK_FORMAT_R16G16B16A16_UNORM:    return ImageFormat::RGBA16;

				case VK_FORMAT_R16G16B16A16_SFLOAT: return  ImageFormat::RGBA16F;
				case VK_FORMAT_R32G32B32A32_SFLOAT: return  ImageFormat::RGBA32F;

				case VK_FORMAT_B8G8R8A8_UNORM: return  ImageFormat::BGRA8;

				case VK_FORMAT_B8G8R8_UNORM:return ImageFormat::BGR8;
				case VK_FORMAT_R8G8B8_UNORM:return  ImageFormat::RGB;

				case VK_FORMAT_D16_UNORM: return  ImageFormat::DEPTH16;
				case VK_FORMAT_D32_SFLOAT: return  ImageFormat::DEPTH32F;
				case  VK_FORMAT_D16_UNORM_S8_UINT: return  ImageFormat::DEPTH16STENCIL8UI;
				case VK_FORMAT_D24_UNORM_S8_UINT: return  ImageFormat::DEPTH24STENCIL8UI;
				case VK_FORMAT_D32_SFLOAT_S8_UINT: return  ImageFormat::DEPTH32FSTENCIL8UI;
				case VK_FORMAT_S8_UINT: return  ImageFormat::STENCIL8UI;
			}
			PF_ENGINE_INFO("proof does not vulkan vk format support {}", format);
			PF_CORE_ASSERT(false);
			return ImageFormat::None;
		}

		static bool IsDepthFormat(ImageFormat format) {
			switch (format)
			{
				case Proof::ImageFormat::DEPTH16:
				case Proof::ImageFormat::DEPTH16STENCIL8UI:
				case Proof::ImageFormat::DEPTH24STENCIL8UI:
				case Proof::ImageFormat::DEPTH32F:
				case Proof::ImageFormat::DEPTH32FSTENCIL8UI:
				case Proof::ImageFormat::X8DEPTH24PACK32:
					return true;
			}
			return false;
		}

		static bool ContainStencilFormat(ImageFormat format) {
			switch (format)
			{
				case ImageFormat::STENCIL8UI:
				case ImageFormat::DEPTH16STENCIL8UI:
				case ImageFormat::DEPTH24STENCIL8UI:
				case ImageFormat::DEPTH32FSTENCIL8UI:
					return true;
			}
			return false;
		}

		static bool IsColorFormat(ImageFormat format) {
			if (IsDepthFormat(format))
				return false;
			if (ContainStencilFormat(format))
				return false;

			return true;
		}
	}
}
