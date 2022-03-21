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
		VulkanCommandBuffer(VulkanSwapChain& swapChain,VulkanGraphicsPipeline& pipeline,Count< VulkanGraphicsPipeline>pipelineShared);
		void Bind(uint32_t index =0);
		VkCommandBuffer& GetBuffer(uint32_t index=0) {
			return m_CommandBuffer[index];
		}
		template<typename T>
		void Draw(T func) {
			// RECORD OR DRAW COMMANDS TO EACH BUFFER
			for (int i = 0; i < m_CommandBuffer.size(); i++) {
				VkCommandBufferBeginInfo beginInfo{};
				beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
				if (vkBeginCommandBuffer(m_CommandBuffer[i], &beginInfo) != VK_SUCCESS)
					PF_CORE_ASSERT(false, "Failed to begin recording command buffer");


				VkRenderPassBeginInfo renderPassInfo{};
				renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
				renderPassInfo.renderPass = m_SwapChain->GetRenderPass();
				// teh frameBuffer we are writing
				renderPassInfo.framebuffer = m_SwapChain->GetFrameBuffer(i);

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
				vkCmdBeginRenderPass(m_CommandBuffer[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
				Bind(i);
				func(m_CommandBuffer[i]);
				vkCmdDraw(m_CommandBuffer[i], 3, 1, 0, 0);

				vkCmdEndRenderPass(m_CommandBuffer[i]);
				if (vkEndCommandBuffer(m_CommandBuffer[i]) != VK_SUCCESS)
					PF_CORE_ASSERT(false, "Faied to record command Buffers");
			}
		}
	private:
		std::vector<VkCommandBuffer> m_CommandBuffer;

		VulkanGraphicsContext& m_GraphicsContext;
		VulkanGraphicsPipeline& m_GraphicsPipeline;
		const VulkanSwapChain* m_SwapChain;
	};
}