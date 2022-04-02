#include "Proofprch.h"
#include "VulkanCommandBuffer.h"
#include "Proof/Renderer/Renderer.h"
#include "VulkanGraphicsContext.h"
#include "VulkanSwapChain.h"
namespace Proof
{
	VulkanCommandBuffer::VulkanCommandBuffer(Count<VulkanSwapChain> swapChain, Count<VulkanGraphicsPipeline> pipeline):
		m_GraphicsPipeline(pipeline)
	{
		m_SwapChain = swapChain;
		m_CommandBuffer.resize(VulkanSwapChain::MAX_FRAMES_IN_FLIGHT);

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
		vkCmdBindPipeline(m_CommandBuffer[index], VK_PIPELINE_BIND_POINT_GRAPHICS, m_GraphicsPipeline->GetPipline());
	}

	void VulkanCommandBuffer::Recreate() {
		//m_CommandBuffer.resize(m_SwapChain->GetImageCount());
		//
		//VkCommandBufferAllocateInfo allocInfo{};
		//allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		//allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		//allocInfo.commandPool = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetCommandPool();
		//allocInfo.commandBufferCount = (uint32_t)m_CommandBuffer.size();
		//
		//if (vkAllocateCommandBuffers(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), &allocInfo, m_CommandBuffer.data()) != VK_SUCCESS)
		//	PF_CORE_ASSERT(false, "Failed to allocate command buffer");

		*this = VulkanCommandBuffer(m_SwapChain, m_GraphicsPipeline);
	}

	void VulkanCommandBuffer::FreeCommandBuffer() {
		vkFreeCommandBuffers(
			Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(),
			Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetCommandPool(),
			static_cast<uint32_t>(m_CommandBuffer.size()),
			m_CommandBuffer.data());
		m_CommandBuffer.clear();
	}
};
