#pragma once
#include<vulkan/vulkan.h>
#include "Proof/Renderer/CommandBuffer.h"
#include<vector>
#include "Proof/Renderer/Renderer.h"
#include <glm/glm.hpp>
namespace Proof
{
	class VulkanSwapChain;
	class VulkanGraphicsPipeline;
	// for submiting straight to quee so does not need funcitonality
	class VulkanCommandBuffer : public CommandBuffer {
	public:
		VulkanCommandBuffer();
		virtual ~VulkanCommandBuffer();
		VkCommandBuffer GetCommandBuffer(uint32_t frameIndex = Renderer::GetCurrentFrame().FrameinFlight){
			return m_CommandBuffer[frameIndex];
		}
	private:
		friend class VulkanSwapChain;
		std::vector<VkCommandBuffer> m_CommandBuffer;
		friend class VulkanRenderPass;
		friend class VulkanRendererAPI;
		friend class VulkanTextureCube;
	};


	class VulkanRenderCommandBuffer : public RenderCommandBuffer {
	public:
		VulkanRenderCommandBuffer(CommandBuffer* commandBuffer = nullptr);
		virtual ~VulkanRenderCommandBuffer();
		VkCommandBuffer GetCommandBuffer(uint32_t frameIndex = Renderer::GetCurrentFrame().FrameinFlight);
	private:
		void BeginRecord(uint32_t frameIndex = Renderer::GetCurrentFrame().FrameinFlight);
		void EndRecord(uint32_t frameIndex = Renderer::GetCurrentFrame().FrameinFlight);
		CommandBuffer* m_NormalCommandBuffer =nullptr;
		void Init();
		void Release();
		bool m_Recording = false;

		std::vector<VkCommandBuffer> m_CommandBuffers;
		friend class VulkanRenderPass;
		friend class VulkanSwapChain;
		friend class VulkanRendererAPI;
	};
}
