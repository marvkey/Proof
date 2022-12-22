#pragma once
#include<vulkan/vulkan.h>
#include<vector>
#include "Proof/Renderer/Renderer.h"
#include "Proof/Renderer/CommandBuffer.h"
#include <glm/glm.hpp>
namespace Proof
{
	class VulkanSwapChain;
	class VulkanGraphicsPipeline;
	class VulkanCommandBuffer : public CommandBuffer {
	public:
		VulkanCommandBuffer();
		//VulkanCommandBuffer(std::vector<void>&commandBuffer);
		virtual ~VulkanCommandBuffer();
		const VkCommandBuffer& GetCommandBuffer(uint32_t frameIndex = Renderer::GetCurrentFrame().FrameinFlight)const {
			return m_CommandBuffer[frameIndex];
		}
		//returns 0 IN COMMAND BUFFER
		virtual void* Get() {
			return m_CommandBuffer[0];
		}

		void Resize();
	private:
		VulkanCommandBuffer(bool empty) {
			m_CommandBuffer.resize(Renderer::GetConfig().FramesFlight);
		}
		void FreeCommandBuffer();
		void Bind(uint32_t frameIndex = Renderer::GetCurrentFrame().FrameinFlight,VkPipelineBindPoint bindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS);
		void BeginRecord(Count<class GraphicsPipeline> graphicsPipeLine,uint32_t frameIndex= Renderer::GetCurrentFrame().FrameinFlight, bool viewScreen =false);
		void EndRecord(uint32_t frameIndex = Renderer::GetCurrentFrame().FrameinFlight);
		bool m_RenderPassEnabled = false;
		bool m_Recording = false;
		Count<class GraphicsPipeline> m_GraphicspipeLine;
		friend class VulkanSwapChain;
		uint32_t m_FrameIndex;
		// frame used to start render pass
		std::vector<VkCommandBuffer> m_CommandBuffer;
		friend class VulkanRenderPass;
		friend class VulkanRendererAPI;
	};
}