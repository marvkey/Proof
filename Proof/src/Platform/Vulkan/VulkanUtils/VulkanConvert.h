#pragma once
#include<vulkan/vulkan.h>
#include "Proof/Renderer/Shader.h"
#include "Proof/Renderer/Texture.h"
// MAKE STATIC SO WEHN INCLUDING IN FILE IT DOES NOT REDFINDE ITSELF
namespace Proof
{
	namespace Utils
	{
		
		static VkShaderStageFlagBits ProofShaderToVulkanShader(ShaderStage stage) {
			switch (stage) {
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
		static uint32_t BytesPerPixel(ImageFormat format) {
			switch (format)
			{
				case ImageFormat::RGBA:    return 4;
				//case ImageFormat::RGBA16F:	return 8;
				case ImageFormat::RGBA32F: return 16;
			}
			PF_CORE_ASSERT(false, fmt::format("not supported yet {}",EnumReflection::EnumString(format)));

		}

		static VkFormat ProofFormatToVulkanFormat(ImageFormat format) {
			switch (format)
			{
				case ImageFormat::RGBA:    return VK_FORMAT_R8G8B8A8_UNORM;
				case ImageFormat::RGBA16:    return VK_FORMAT_R16G16B16A16_UNORM;

				case ImageFormat::RGBA16F: return VK_FORMAT_R16G16B16A16_SFLOAT;
				case ImageFormat::RGBA32F: return VK_FORMAT_R32G32B32A32_SFLOAT;

				case ImageFormat::BGRA8UNORM: return VK_FORMAT_B8G8R8A8_UNORM;
				case ImageFormat::RGBA8UNORM: return VK_FORMAT_R8G8B8A8_UNORM;
				case ImageFormat::BGR8UNORM:return VK_FORMAT_B8G8R8_UNORM;
				case ImageFormat::RGB8UNORM:return VK_FORMAT_R8G8B8_UNORM;

				case ImageFormat::DEPTH16UNORM: return VK_FORMAT_D16_UNORM;
				case ImageFormat::DEPTH32F: return VK_FORMAT_D32_SFLOAT;
				case ImageFormat::DEPTH16UNORMSTENCIL8UINT: return VK_FORMAT_D16_UNORM_S8_UINT;
				case ImageFormat::DEPTH24UNORMSTENCIL8UINT: return VK_FORMAT_D24_UNORM_S8_UINT;
				case ImageFormat::DEPTH32FSTENCIL8UINT: return VK_FORMAT_D32_SFLOAT_S8_UINT;
				case ImageFormat::STENCIL8UINT: return VK_FORMAT_S8_UINT;

			}
			return (VkFormat)0;
		}

		static ImageFormat VulkanFormatToProofFormat(VkFormat format) {
			switch (format)
			{
				case VK_FORMAT_R8G8B8A8_UNORM:    return ImageFormat::RGBA; //ImageFormat::RGBA8UNORM
				case VK_FORMAT_R16G16B16A16_UNORM:    return ImageFormat::RGBA16;

				case VK_FORMAT_R16G16B16A16_SFLOAT: return  ImageFormat::RGBA16F;
				case VK_FORMAT_R32G32B32A32_SFLOAT: return  ImageFormat::RGBA32F;

				case VK_FORMAT_B8G8R8A8_UNORM: return  ImageFormat::BGRA8UNORM;

				case VK_FORMAT_B8G8R8_UNORM:return ImageFormat::BGR8UNORM;
				case VK_FORMAT_R8G8B8_UNORM:return  ImageFormat::RGB8UNORM;

				case VK_FORMAT_D16_UNORM: return  ImageFormat::DEPTH16UNORM;
				case VK_FORMAT_D32_SFLOAT: return  ImageFormat::DEPTH32F;
				case  VK_FORMAT_D16_UNORM_S8_UINT: return  ImageFormat::DEPTH16UNORMSTENCIL8UINT;
				case VK_FORMAT_D24_UNORM_S8_UINT: return  ImageFormat::DEPTH24UNORMSTENCIL8UINT;
				case VK_FORMAT_D32_SFLOAT_S8_UINT: return  ImageFormat::DEPTH32FSTENCIL8UINT;
				case VK_FORMAT_S8_UINT: return  ImageFormat::STENCIL8UINT;
			}
			PF_ENGINE_INFO("proof does not vulkan vk format support {}", format);
			PF_CORE_ASSERT(false);
			return ImageFormat::None;
		}

		static bool IsDepthFormat(ImageFormat format) {
			switch (format)
			{
				case Proof::ImageFormat::DEPTH16UNORM:
				case Proof::ImageFormat::DEPTH32F:
				case Proof::ImageFormat::DEPTH24UNORMSTENCIL8UINT:
				case Proof::ImageFormat::DEPTH32FSTENCIL8UINT:
					return true;

			}
			return false;
		}

		static bool IsStencilFormat(ImageFormat format) {
			return format == ImageFormat::STENCIL8UINT;
		}

		static bool IsColorFormat(ImageFormat format) {
			if (IsDepthFormat(format))
				return false;
			if (IsStencilFormat(format))
				return false;

			return true;
		}
	}
}
