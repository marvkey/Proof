#pragma once
#include<vulkan/vulkan.h>
#include<vector>
#include "VulkanSwapChain.h"
#include "VulkanGraphicsPipeline.h"
namespace Proof
{
	class VulkanCommandBuffer {
	public:
		VulkanCommandBuffer(VulkanSwapChain& swapChain,VulkanGraphicsPipeline& pipeline,Count< VulkanGraphicsPipeline>pipelineShared);
		void Bind(uint32_t index =0);
		VkCommandBuffer& GetBuffer(uint32_t index=0) {
			return m_CommandBuffer[index];
		}

	private:
		std::vector<VkCommandBuffer> m_CommandBuffer;

		VulkanGraphicsContext& m_GraphicsContext;
		VulkanGraphicsPipeline& m_GraphicsPipeline;
	};
}