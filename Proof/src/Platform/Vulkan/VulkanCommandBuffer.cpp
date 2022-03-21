#include "Proofprch.h"
#include "VulkanCommandBuffer.h"
#include "Proof/Renderer/Renderer.h"
#include "VulkanGraphicsContext.h"
#include "VulkanSwapChain.h"
namespace Proof
{
	VulkanCommandBuffer::VulkanCommandBuffer(VulkanSwapChain& swapChain, VulkanGraphicsPipeline& pipeline, Count< VulkanGraphicsPipeline>pipelineShared):
		m_GraphicsContext(*Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()), m_GraphicsPipeline(pipeline)
	{
		m_SwapChain = &swapChain;
		m_CommandBuffer.resize(swapChain.GetImageCount());

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		// primary can be submited to a que for submition, cannot be called by other command buffers
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

		// command pool opaque objects
		// that command buffer memory is allocated from
		allocInfo.commandPool = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetCommandPool();
		
		allocInfo.commandBufferCount = (uint32_t)m_CommandBuffer.size();

		if (vkAllocateCommandBuffers(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), &allocInfo, m_CommandBuffer.data()) != VK_SUCCESS)
			PF_CORE_ASSERT(false, "Failed to allocate command buffer");

	}
	void VulkanCommandBuffer::Bind(uint32_t index ) {
		vkCmdBindPipeline(m_CommandBuffer[index], VK_PIPELINE_BIND_POINT_GRAPHICS, m_GraphicsPipeline.GetPipline());
	}
};
