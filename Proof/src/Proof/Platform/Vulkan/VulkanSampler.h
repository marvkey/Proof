#pragma once
#include "Proof/Renderer/RendererSampler.h"
#include <vulkan/vulkan.h>
namespace Proof
{
	
	class VulkanRenderSampler : public RenderSampler
	{
	public:
		VulkanRenderSampler(const SamplerResourceConfig& config);
		virtual ~VulkanRenderSampler();
		virtual const SamplerResourceConfig& GetConfig() const { return m_Config; };
		const VkDescriptorImageInfo& GetDescriptorInfoVulkan()const { return *(VkDescriptorImageInfo*)GetResourceDescriptorInfo(); };
		virtual ResourceDescriptorInfo GetResourceDescriptorInfo()const { return (ResourceDescriptorInfo)&m_DescriptorImageInfo; }
		VkSampler GetSampler() { return m_VulkanSampler; }
	private:
		SamplerResourceConfig m_Config;
		VkSampler m_VulkanSampler;
		VkDescriptorImageInfo m_DescriptorImageInfo;
	};
}