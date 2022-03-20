#include "Proofprch.h"
#include "VulkanCommandBuffer.h"
#include "Proof/Renderer/Renderer.h"
#include "VulkanGraphicsContext.h"
namespace Proof
{
	VulkanCommandBuffer::VulkanCommandBuffer(VulkanSwapChain& swapChain, VulkanGraphicsPipeline& pipeline, Count< VulkanGraphicsPipeline>pipelineShared):
		m_GraphicsContext(*Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()), m_GraphicsPipeline(pipeline)
	{
		
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

		// RECORD OR DRAW COMMANDS TO EACH BUFFER
		for (int i = 0; i < m_CommandBuffer.size(); i++) {
			VkCommandBufferBeginInfo beginInfo{};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			if (vkBeginCommandBuffer(m_CommandBuffer[i], &beginInfo) != VK_SUCCESS)
				PF_CORE_ASSERT(false, "Failed to begin recording command buffer");


			VkRenderPassBeginInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = swapChain.GetRenderPass();
			// teh frameBuffer we are writing
			renderPassInfo.framebuffer = swapChain.GetFrameBuffer(i);

			// the area shader loads and 
			renderPassInfo.renderArea.offset = { 0,0 };
			// for high displays swap chain extent could be higher than windows extent
			renderPassInfo.renderArea.extent = swapChain.GetSwapChainExtent();

			// Clear values
			std::array<VkClearValue, 2> clearValues{};
			clearValues[0].color = { 0.1f,0.1f,0.1f,1.0f }; // color of screen
			// teh reason we are not settign [0].depthStencil is because 
			//we set color atachmetna as index 0 and depth as index 1 in 
			// the render pass
			clearValues[1].depthStencil = { 1.0f,0 };

			renderPassInfo.clearValueCount = (uint32_t)clearValues.size();
			renderPassInfo.pClearValues = clearValues.data();


			// INLINE
			//MEANS that the render commands will be in the  command buffer itself
			// NO seccondary command buffers will be used
			vkCmdBeginRenderPass(m_CommandBuffer[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
			//Bind(i);
			vkCmdBindPipeline(m_CommandBuffer[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineShared->m_GraphicsPipeline);

			vkCmdDraw(m_CommandBuffer[i], 3, 1, 0, 0);

			vkCmdEndRenderPass(m_CommandBuffer[i]);
			if(vkEndCommandBuffer(m_CommandBuffer[i]) != VK_SUCCESS)
				PF_CORE_ASSERT(false,"Faied to record command Buffers");
		}
	}
	void VulkanCommandBuffer::Bind(uint32_t index ) {

		vkCmdBindPipeline(m_CommandBuffer[index], VK_PIPELINE_BIND_POINT_GRAPHICS, m_GraphicsPipeline.GetPipline());
	}
};
