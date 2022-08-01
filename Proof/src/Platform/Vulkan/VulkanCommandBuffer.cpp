#include "Proofprch.h"
#include "VulkanCommandBuffer.h"
#include "Proof/Renderer/Renderer.h"
#include "VulkanGraphicsContext.h"
#include "VulkanSwapChain.h"
namespace Proof
{
	VulkanCommandBuffer::VulkanCommandBuffer(Count<VulkanSwapChain> swapChain)
	{
		auto device = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();
		m_SwapChain = swapChain;

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		// primary can be submited to a que for submition, cannot be called by other command buffers
		allocInfo.pNext = nullptr;


		// command pool opaque objects
		// that command buffer memory is allocated from
		allocInfo.commandPool = device->GetCommandPool();
		
		// FRAMES IN FLIGH COULD BE USED
		allocInfo.commandBufferCount = 1;

		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

		if (vkAllocateCommandBuffers(device->GetDevice(), &allocInfo, &m_CommandBuffer) != VK_SUCCESS)
			PF_CORE_ASSERT(false, "Failed to allocate command buffer");

	}
	void VulkanCommandBuffer::Bind(VkPipeline pipeLine,VkPipelineBindPoint bindPoint) {
		vkCmdBindPipeline(m_CommandBuffer, bindPoint, pipeLine);
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

	}

	void VulkanCommandBuffer::FreeCommandBuffer() {
		vkFreeCommandBuffers(
			Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(),
			Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetCommandPool(),
			1,
			&m_CommandBuffer);
	}
};
