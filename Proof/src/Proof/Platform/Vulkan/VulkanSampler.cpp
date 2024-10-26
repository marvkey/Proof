#include "Proofprch.h"
#include "VulkanSampler.h"
#include "VulkanRenderer.h"
#include "VulkanGraphicsContext.h"
#include "VulkanDevice.h"
namespace Proof
{
	namespace Utils
	{
		inline VkFilter VulkanSamplerFilter(SamplerFilter filter)
		{
			switch (filter)
			{
			case Proof::SamplerFilter::Linear:
				return VK_FILTER_LINEAR;
			case Proof::SamplerFilter::Nearest:
				return VK_FILTER_NEAREST;
			case Proof::SamplerFilter::Cubic:
				return VK_FILTER_CUBIC_IMG;
				break;
			}
			PF_CORE_ASSERT(false, "Not supported");
			return (VkFilter)0;
		}

		inline VkSamplerAddressMode VulkanSamplerWrap(SamplerWrap wrap)
		{
			switch (wrap)
			{
			case Proof::SamplerWrap::Repeat:
				return VK_SAMPLER_ADDRESS_MODE_REPEAT;
				break;
			case Proof::SamplerWrap::MirroredRepeat:

				return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
				break;
			case Proof::SamplerWrap::ClampEdge:
				return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
				break;
			case Proof::SamplerWrap::MirroredClampEdge:
				return VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE;
				break;
			case Proof::SamplerWrap::ClampBorder:
				return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
				break;
			}
			PF_CORE_ASSERT(false, "Not supported");
			return (VkSamplerAddressMode)0;
		}


		inline VkBorderColor VulkanBorderColor(SamplerBorderColor borderColor)
		{
			switch (borderColor)
			{
				case SamplerBorderColor::TransperantBlack:
					return VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
				case SamplerBorderColor::OpaqueBlack:
					return VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
				case SamplerBorderColor::OpaqueWhite:
					return VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
				case SamplerBorderColor::TransperantBlackInt:
					return VK_BORDER_COLOR_INT_TRANSPARENT_BLACK;
				case SamplerBorderColor::OpaqueBlackInt:
					return VK_BORDER_COLOR_INT_OPAQUE_BLACK;
				case SamplerBorderColor::OpaqueWhiteInt:
					return VK_BORDER_COLOR_INT_OPAQUE_WHITE;
			}

			PF_CORE_ASSERT(false, "Not supported");
			return (VkBorderColor)0;
		}

		VkSamplerMipmapMode ConvertToVulkanMipMapMode(SamplerMipMapMode mipMapMode)
		{
			switch (mipMapMode)
			{
			case SamplerMipMapMode::Nearest:
				return VK_SAMPLER_MIPMAP_MODE_NEAREST;
			case SamplerMipMapMode::Linear:
				return VK_SAMPLER_MIPMAP_MODE_LINEAR;
			}
			PF_CORE_ASSERT(false, "Not supported");
			return (VkSamplerMipmapMode)0;
		}

		VkCompareOp ConvertToVulkanCompareOp(SamplerCompare compareMode)
		{
			switch (compareMode)
			{
			case SamplerCompare::Never:
				return VK_COMPARE_OP_NEVER;
			case SamplerCompare::Less:
				return VK_COMPARE_OP_LESS;
			case SamplerCompare::Equal:
				return VK_COMPARE_OP_EQUAL;
			case SamplerCompare::LessOrEqual:
				return VK_COMPARE_OP_LESS_OR_EQUAL;
			case SamplerCompare::Greater:
				return VK_COMPARE_OP_GREATER;
			case SamplerCompare::NotEqual:
				return VK_COMPARE_OP_NOT_EQUAL;
			case SamplerCompare::GreaterOrEqual:
				return VK_COMPARE_OP_GREATER_OR_EQUAL;
			case SamplerCompare::Always:
				return VK_COMPARE_OP_ALWAYS;
			}
			PF_CORE_ASSERT(false, "Not supported");
			return (VkCompareOp)0;

		}
	}
	VulkanRenderSampler::VulkanRenderSampler(const SamplerResourceConfig& config)
	{
		m_Config = config;
		m_Config.Anisotropy = glm::min(15.f, m_Config.Anisotropy);
		Count<VulkanRenderSampler> sampler = this;
		Renderer::Submit([sampler, config = m_Config] () mutable
			{
				auto device = VulkanRenderer::GetGraphicsContext()->GetDevice()->GetVulkanDevice();

				VkSamplerCreateInfo samplerCreateInfo = {};
				samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
				samplerCreateInfo.maxAnisotropy = glm::min(16.0f, config.Anisotropy);
				samplerCreateInfo.anisotropyEnable = samplerCreateInfo.maxAnisotropy > 1;
				samplerCreateInfo.mipLodBias = 0.0f;
				samplerCreateInfo.minLod = 0.0f;
				samplerCreateInfo.maxLod = std::numeric_limits<float>::max();; 

				samplerCreateInfo.addressModeU = Utils::VulkanSamplerWrap(config.Wrap.AddressU);
				samplerCreateInfo.addressModeV = Utils::VulkanSamplerWrap(config.Wrap.AddressV);
				samplerCreateInfo.addressModeW = Utils::VulkanSamplerWrap(config.Wrap.AddressW);

				samplerCreateInfo.minFilter = Utils::VulkanSamplerFilter(config.Filter.MinFilter);
				samplerCreateInfo.magFilter = Utils::VulkanSamplerFilter(config.Filter.MagFilter);

				samplerCreateInfo.borderColor = Utils::VulkanBorderColor(config.BorderColor);
				samplerCreateInfo.mipmapMode = Utils::ConvertToVulkanMipMapMode(config.MipMapMode);
				samplerCreateInfo.compareOp = Utils::ConvertToVulkanCompareOp(config.CompareOp);
				samplerCreateInfo.compareEnable = config.CompareOp != SamplerCompare::Never ? VK_TRUE : VK_FALSE;

				vkCreateSampler(VulkanRenderer::GetGraphicsContext()->GetDevice()->GetVulkanDevice(), &samplerCreateInfo, nullptr, &sampler->m_VulkanSampler);
				VulkanUtils::SetDebugUtilsObjectName(device,VK_OBJECT_TYPE_SAMPLER, config.DebugName, sampler->m_VulkanSampler);

				sampler->m_DescriptorImageInfo.imageView = nullptr;
				sampler->m_DescriptorImageInfo.sampler = sampler->m_VulkanSampler;
			});
	}
	VulkanRenderSampler::~VulkanRenderSampler()
	{
		Renderer::SubmitResourceFree([sampler = m_VulkanSampler]()
			{
				auto vulkanDevice = VulkanRenderer::GetGraphicsContext()->GetDevice()->GetVulkanDevice();
				auto graphics = VulkanRenderer::GetGraphicsContext();
				vkDestroySampler(vulkanDevice, sampler, nullptr);
			});
	}
}
