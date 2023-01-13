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
			PF_CORE_ASSERT(false, "not supported yet");

		}

		static VkFormat ProofFormatToVulkanFormat(ImageFormat format) {
			switch (format)
			{
				case ImageFormat::RGBA:    return VK_FORMAT_R8G8B8A8_UNORM;
				case ImageFormat::RGBA16:    return VK_FORMAT_R16G16B16A16_UNORM;

				case ImageFormat::RGBA16F: return VK_FORMAT_R16G16B16A16_SFLOAT;
				case ImageFormat::RGBA32F: return VK_FORMAT_R32G32B32A32_SFLOAT;
				case ImageFormat::RGBAScreenFrameBuffer: return VK_FORMAT_B8G8R8A8_UNORM;
			}
			return (VkFormat)0;
		}

		static ImageFormat VulkanFormatToProofFormat(VkFormat format) {
			switch (format)
			{
				case VK_FORMAT_R8G8B8A8_UNORM: return ImageFormat::RGBA;
				case VK_FORMAT_R16G16B16A16_UNORM:    return ImageFormat::RGBA16;

				case VK_FORMAT_R16G16B16A16_SFLOAT: return ImageFormat::RGBA16F;
				case VK_FORMAT_R32G32B32A32_SFLOAT: return ImageFormat::RGBA32F;

				case VK_FORMAT_B8G8R8A8_UNORM: return ImageFormat::RGBAScreenFrameBuffer;
			}
			PF_ENGINE_INFO("proof does not vulkan vk format support {}", format);
			PF_CORE_ASSERT(false);
		}
	}
}
