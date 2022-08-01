#pragma once
#include<vulkan/vulkan.h>
#include<vector>
#include "VulkanSwapChain.h"
#include "VulkanGraphicsPipeline.h"
#include <glm/glm.hpp>
namespace Proof
{
	class VulkanCommandBuffer {
	public:
		VulkanCommandBuffer(Count<VulkanSwapChain> swapChain);
	

		const VkCommandBuffer& GetCommandBuffer()const {
			return m_CommandBuffer;
		}
		uint32_t GetSize() { return 0; }
		template <typename T>
		void BeginRenderPass(uint32_t SwapIndex, VkPipeline pipeLine, T func, const glm::vec4& Color={0.1,0.1,0.1,1}, float Depth = 1.0f, uint32_t stencil = 0)
		{
			std::array<VkClearValue, 2> clearValues{};
			clearValues[0].color = { Color.x, Color.y, Color.z, Color.a };
			// color of screen
			// teh reason we are not settign [0].depthStencil is because 
			//we set color atachmetna as index 0 and depth as index 1 in 
			// the render pass
			clearValues[1].depthStencil = {Depth,stencil };


			VkRenderPassBeginInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = m_SwapChain->GetRenderPass();
			// teh frameBuffer we are writing
			renderPassInfo.framebuffer = m_SwapChain->GetFrameBuffer(SwapIndex);

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
			if (vkBeginCommandBuffer(m_CommandBuffer, &beginInfo) != VK_SUCCESS)
				PF_CORE_ASSERT(false, "Failed to begin recording command buffer");

			vkCmdBeginRenderPass(m_CommandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
			Bind(pipeLine);
			func(m_CommandBuffer);
		}

		void EndRenderPass() {
			vkCmdEndRenderPass(m_CommandBuffer);
			if (vkEndCommandBuffer(m_CommandBuffer) != VK_SUCCESS)
				PF_CORE_ASSERT(false, "Faied to record command Buffers");
		}
		void Recreate();
		~VulkanCommandBuffer() {
			void FreeCommandBuffer();
		}
		void FreeCommandBuffer();

	private:
		void Bind(VkPipeline pipeLine, VkPipelineBindPoint bindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS);
		VkCommandBuffer m_CommandBuffer;

		Count<VulkanSwapChain> m_SwapChain;
	};
}