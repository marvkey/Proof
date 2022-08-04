#pragma once
#include<vulkan/vulkan.h>
#include<vector>
#include <glm/glm.hpp>
namespace Proof
{
	class VulkanSwapChain;
	class VulkanGraphicsPipeline;
	class VulkanCommandBuffer {
	public:
		VulkanCommandBuffer(Count<VulkanSwapChain> swapChain);
		virtual ~VulkanCommandBuffer() {
			void FreeCommandBuffer();
		}
		const VkCommandBuffer& GetCommandBuffer()const {
			return m_CommandBuffer;
		}
		uint32_t GetSize() { return 0; }
		void BeginRenderPass(uint32_t SwapIndex, Count<VulkanGraphicsPipeline> graphicsPipeLine, const glm::vec4& Color = { 0.1,0.1,0.1,1 }, float Depth = 1.0f, uint32_t stencil = 0);
		template <typename T>
		void Record(T func) {
			PF_CORE_ASSERT(m_RenderPassEnabled == false, "cannot record if render pass is not started");
			Bind();
			func(m_CommandBuffer);
		}
		void EndRenderPass();
		void Recreate();
		void FreeCommandBuffer();
	private:
		bool m_RenderPassEnabled = false;
		void Bind(VkPipelineBindPoint bindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS);
		VkCommandBuffer m_CommandBuffer;
		Count<VulkanGraphicsPipeline> m_GraphicspipeLine;
		Count<VulkanSwapChain> m_SwapChain;
	};
}