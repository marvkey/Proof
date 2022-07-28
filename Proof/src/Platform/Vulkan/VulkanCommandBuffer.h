#pragma once
#include<vulkan/vulkan.h>
#include<vector>
#include "VulkanSwapChain.h"
#include "VulkanGraphicsPipeline.h"
namespace Proof
{
	class VulkanCommandBuffer {
	public:
		VulkanCommandBuffer(Count<VulkanSwapChain> swapChain,Count<VulkanGraphicsPipeline> pipeline);
		void Bind(VkPipelineBindPoint bindPoint= VK_PIPELINE_BIND_POINT_GRAPHICS);

		const VkCommandBuffer& GetCommandBuffer()const {
			return m_CommandBuffer;
		}
		uint32_t GetSize() { return 0; }

		void BeginRenderPass(const VkRenderPassBeginInfo& renderPassInfo) {
			VkCommandBufferBeginInfo beginInfo{};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			//begin the command buffer recording. We will use this command buffer exactly once, so we want to let vulkan know that
			beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
			if (vkBeginCommandBuffer(m_CommandBuffer, &beginInfo) != VK_SUCCESS)
				PF_CORE_ASSERT(false, "Failed to begin recording command buffer");

			vkCmdBeginRenderPass(m_CommandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
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
		VkCommandBuffer m_CommandBuffer;

		Count<VulkanGraphicsPipeline> m_GraphicsPipeline;
		Count<VulkanSwapChain> m_SwapChain;
	};
}