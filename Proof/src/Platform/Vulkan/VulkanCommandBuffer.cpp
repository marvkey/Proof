#include "Proofprch.h"
#include "VulkanCommandBuffer.h"
#include "Proof/Renderer/Renderer.h"
#include "VulkanGraphicsContext.h"
#include "VulkanSwapChain.h"
#include "VulkanGraphicsPipeline.h"
namespace Proof
{
	VulkanCommandBuffer::VulkanCommandBuffer()
	{
		m_CommandBuffer.resize(Renderer::GetConfig().FramesFlight);
		auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();
		m_SwapChain = graphicsContext->GetSwapChain();

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		// primary can be submited to a que for submition, cannot be called by other command buffers
		allocInfo.pNext = nullptr;


		// command pool opaque objects
		// that command buffer memory is allocated from
		allocInfo.commandPool = graphicsContext->GetCommandPool();
		
		allocInfo.commandBufferCount = m_CommandBuffer.size();

		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

		if (vkAllocateCommandBuffers(graphicsContext->GetDevice(), &allocInfo, m_CommandBuffer.data()) != VK_SUCCESS)
			PF_CORE_ASSERT(false, "Failed to allocate command buffer");

	}
	
	void VulkanCommandBuffer::BeginRecord(Count<VulkanGraphicsPipeline> graphicsPipeLine, uint32_t frameIndex,bool viewScreen ){
		PF_CORE_ASSERT(m_Recording == false, "cannot start recoridng when command buffer is still recording");
		m_Recording = true;
		//we can safely reset the command buffer to begin recording again.
		auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();
		//vkResetCommandBuffer(m_CommandBuffer[frameIndex], 0);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		// means taht it will beshown in the window, if not we  wotn show it in teh window
		if(viewScreen)
			//begin the command buffer recording. We will use this command buffer exactly once, so we want to let vulkan know that
			beginInfo.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		if (vkBeginCommandBuffer(m_CommandBuffer[frameIndex], &beginInfo) != VK_SUCCESS)
			PF_CORE_ASSERT(false, "Failed to begin recording command buffer");

		m_FrameIndex = frameIndex;
		m_GraphicspipeLine = graphicsPipeLine;

	}
	void VulkanCommandBuffer::EndRecord(uint32_t frameIndex ) {
		PF_CORE_ASSERT(m_Recording == true, "cannot End recording when recoring never started");
		if (vkEndCommandBuffer(m_CommandBuffer[m_FrameIndex]) != VK_SUCCESS)
			PF_CORE_ASSERT(false, "Faied to record command Buffers");
		m_GraphicspipeLine = nullptr;
		m_Recording = false;
		m_FrameIndex = 0;
	}
	
	void VulkanCommandBuffer::Bind(uint32_t frameIndex, VkPipelineBindPoint bindPoint ) {
		PF_CORE_ASSERT(m_RenderPassEnabled == false, "cannot bind if render pass is not started");
		vkCmdBindPipeline(m_CommandBuffer[frameIndex], bindPoint, m_GraphicspipeLine->GetPipline());
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
		auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();

		for (uint32_t i = 0; i < Renderer::GetConfig().FramesFlight; i++) {
			vkFreeCommandBuffers(
				graphicsContext->GetDevice(),
				graphicsContext->GetCommandPool(),
				1,
				&m_CommandBuffer[i]);
		}
	}
};
