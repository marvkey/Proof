#pragma once
#include "Proof/Renderer/RendererSampler.h"
#include <vulkan/vulkan.h>
namespace Proof
{
	
	class VulkanRendererSampler : public RenderSampler
	{
	public:
		VulkanRendererSampler(const SamplerResourceConfig& config);
		virtual ~VulkanRendererSampler();
		virtual const SamplerResourceConfig& GetConfig() const { return m_Config; };
		const VkDescriptorImageInfo& GetDescriptorInfoVulkan()const { return *(VkDescriptorImageInfo*)GetResourceDescriptorInfo(); };
		virtual ResourceDescriptorInfo GetResourceDescriptorInfo()const { return (ResourceDescriptorInfo)&m_DescriptorImageInfo; }
	private:
		SamplerResourceConfig m_Config;
		VkSampler m_VulkanSampler;
		VkDescriptorImageInfo m_DescriptorImageInfo;
	};
}