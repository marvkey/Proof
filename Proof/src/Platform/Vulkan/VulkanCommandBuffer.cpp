#include "Proofprch.h"
#include "VulkanCommandBuffer.h"
#include "Proof/Renderer/Renderer.h"
#include "VulkanGraphicsContext.h"
#include "VulkanSwapChain.h"
#include "VulkanGraphicsPipeline.h"
namespace Proof
{
	VulkanCommandBuffer::VulkanCommandBuffer(Count<VulkanSwapChain> swapChain)
	{
		m_CommandBuffer.resize(Renderer::GetConfig().FramesFlight);
		auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();
		m_SwapChain = swapChain;

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
	void VulkanCommandBuffer::BeginRenderPass(uint32_t imageIndex, Count<VulkanGraphicsPipeline> graphicsPipeLine,const glm::vec4& Color, float Depth, uint32_t stencil, uint32_t frameIndex){
		PF_CORE_ASSERT(m_RenderPassEnabled == false, "cannot start render pass when previous render pass is not closed");
		//we can safely reset the command buffer to begin recording again.
		auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();
		vkResetCommandBuffer(m_CommandBuffer[frameIndex], 0);


		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { Color.x, Color.y, Color.z, Color.a };
		// color of screen
		// teh reason we are not settign [0].depthStencil is because 
		//we set color atachmetna as index 0 and depth as index 1 in 
		// the render pass
		clearValues[1].depthStencil = { Depth,stencil };


		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_SwapChain->GetRenderPass();
		// teh frameBuffer we are writing
		renderPassInfo.framebuffer = m_SwapChain->GetFrameBuffer(imageIndex);

		// the area shader loads and 
		renderPassInfo.renderArea.offset = { 0,0 };
		// for high displays swap chain extent could be higher than windows extent
		renderPassInfo.renderArea.extent = m_SwapChain->GetSwapChainExtent();


		renderPassInfo.clearValueCount = (uint32_t)clearValues.size();
		renderPassInfo.pClearValues = clearValues.data();
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		//begin the command buffer recording. We will use this command buffer exactly once, so we want to let vulkan know that
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		if (vkBeginCommandBuffer(m_CommandBuffer[frameIndex], &beginInfo) != VK_SUCCESS)
			PF_CORE_ASSERT(false, "Failed to begin recording command buffer");

		vkCmdBeginRenderPass(m_CommandBuffer[frameIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		m_GraphicspipeLine = graphicsPipeLine;
		m_FrameIndex = frameIndex;
	}
	void VulkanCommandBuffer::Bind(uint32_t frameIndex, VkPipelineBindPoint bindPoint ) {
		vkCmdBindPipeline(m_CommandBuffer[frameIndex], bindPoint, m_GraphicspipeLine->GetPipline());
	}

	void VulkanCommandBuffer::EndRenderPass() {
		PF_CORE_ASSERT(m_RenderPassEnabled == false, "cannot End render pass when render pass is not started");
		vkCmdEndRenderPass(m_CommandBuffer[m_FrameIndex]);
		if (vkEndCommandBuffer(m_CommandBuffer[m_FrameIndex]) != VK_SUCCESS)
			PF_CORE_ASSERT(false, "Faied to record command Buffers");
		m_GraphicspipeLine = nullptr;
		m_RenderPassEnabled = false;
		m_FrameIndex = 0;
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
