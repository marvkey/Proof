#pragma once
#include<vulkan/vulkan.h>
#include<vector>
#include "VulkanSwapChain.h"
#include "VulkanGraphicsPipeline.h"
#
namespace Proof
{
	class VulkanCommandBuffer {
	public:
		VulkanCommandBuffer(Count<VulkanSwapChain> swapChain,Count<VulkanGraphicsPipeline> pipeline);
		void Bind(uint32_t index =0);
		VkCommandBuffer& GetBuffer(uint32_t index=0) {
			return m_CommandBuffer[index];
		}
		uint32_t GetSize() { return m_CommandBuffer.size(); }
		template<typename T>
		void Record(int index, T func) {
			VkCommandBufferBeginInfo beginInfo{};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			if (vkBeginCommandBuffer(m_CommandBuffer[index], &beginInfo) != VK_SUCCESS)
				PF_CORE_ASSERT(false, "Failed to begin recording command buffer");


			VkRenderPassBeginInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = m_SwapChain->GetRenderPass();
			// teh frameBuffer we are writing
			renderPassInfo.framebuffer = m_SwapChain->GetFrameBuffer(index);

			// the area shader loads and 
			renderPassInfo.renderArea.offset = { 0,0 };
			// for high displays swap chain extent could be higher than windows extent
			renderPassInfo.renderArea.extent = m_SwapChain->GetSwapChainExtent();

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
			vkCmdBeginRenderPass(m_CommandBuffer[index], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
			
			//-------//
			//DYNAMIC VIEPORT
			VkViewport viewport{};
			viewport.x = 0.0f;
			viewport.y = 0.0f;
			viewport.width = static_cast<float>(CurrentWindow::GetWindowWidth());
			viewport.height = static_cast<float>(CurrentWindow::GetWindowHeight());
			viewport.minDepth = 0.0f;
			viewport.maxDepth = 1.0f;
			VkRect2D scissor{ {0, 0}, {CurrentWindow::GetWindowWidth(),CurrentWindow::GetWindowHeight()} };
			vkCmdSetViewport(m_CommandBuffer[index], 0, 1, &viewport);
			vkCmdSetScissor(m_CommandBuffer[index], 0, 1, &scissor);
			//---------------------//
			Bind(index);
			func(m_CommandBuffer[index]);
			vkCmdDraw(m_CommandBuffer[index], 3, 1, 0, 0);

			vkCmdEndRenderPass(m_CommandBuffer[index]);
			if (vkEndCommandBuffer(m_CommandBuffer[index]) != VK_SUCCESS)
				PF_CORE_ASSERT(false, "Faied to record command Buffers");
		}
		void Recreate();
		void FreeCommandBuffer();
	private:
		std::vector<VkCommandBuffer> m_CommandBuffer;

		Count<VulkanGraphicsPipeline> m_GraphicsPipeline;
		Count<VulkanSwapChain> m_SwapChain;
	};
}