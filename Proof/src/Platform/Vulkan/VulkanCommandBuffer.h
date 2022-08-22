#pragma once
#include<vulkan/vulkan.h>
#include<vector>
#include "Proof/Renderer/Renderer.h"

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
		const VkCommandBuffer& GetCommandBuffer(uint32_t frameIndex = Renderer::GetCurrentFrame())const {
			return m_CommandBuffer[frameIndex];
		}
		uint32_t GetSize() { return 0; }
		void BeginRenderPass(uint32_t imageIndex, Count<VulkanGraphicsPipeline> graphicsPipeLine, const glm::vec4& Color = { 0.1,0.1,0.1,1 }, float Depth = 1.0f, uint32_t stencil = 0, uint32_t frameIndex = Renderer::GetCurrentFrame());
		template <typename T>
		void Record(T func) {
			PF_CORE_ASSERT(m_RenderPassEnabled == false, "cannot record if render pass is not started");
			Bind(m_FrameIndex);
			func(m_CommandBuffer[m_FrameIndex]);
		}
		void EndRenderPass();
		void Recreate();
		void FreeCommandBuffer();
	private:
		bool m_RenderPassEnabled = false;
		void Bind(uint32_t frameIndex = Renderer::GetCurrentFrame(),VkPipelineBindPoint bindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS);
		std::vector<VkCommandBuffer> m_CommandBuffer;
		Count<VulkanGraphicsPipeline> m_GraphicspipeLine;
		Count<VulkanSwapChain> m_SwapChain;
		// frame used to start render pass
		uint32_t m_FrameIndex = 0;
	};
}