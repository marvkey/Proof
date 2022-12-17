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
		VulkanCommandBuffer();
		//VulkanCommandBuffer(std::vector<void>&commandBuffer);
		virtual ~VulkanCommandBuffer() {
			void FreeCommandBuffer();
		}
		const VkCommandBuffer& GetCommandBuffer(uint32_t frameIndex = Renderer::GetCurrentFrame().FrameinFlight)const {
			return m_CommandBuffer[frameIndex];
		}
		uint32_t GetSize() { return 0; }
	
		void Recreate();
		std::vector<VkCommandBuffer> m_CommandBuffer;
		void FreeCommandBuffer();
	private:
		void Bind(uint32_t frameIndex = Renderer::GetCurrentFrame().FrameinFlight,VkPipelineBindPoint bindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS);
		void BeginRecord(Count<VulkanGraphicsPipeline> graphicsPipeLine,uint32_t frameIndex= Renderer::GetCurrentFrame().FrameinFlight, bool viewScreen =false);
		void EndRecord(uint32_t frameIndex = Renderer::GetCurrentFrame().FrameinFlight);
		friend class VulkanRenderer;
		bool m_RenderPassEnabled = false;
		bool m_Recording = false;
		Count<VulkanGraphicsPipeline> m_GraphicspipeLine;
		Count<VulkanSwapChain> m_SwapChain;
		friend class VulkanSwapChain;
		// frame used to start render pass
		uint32_t m_FrameIndex = 0;
	};
}