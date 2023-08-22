#pragma once
#include<vulkan/vulkan.h>
#include "Proof/Renderer/CommandBuffer.h"
#include<vector>
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
		VkCommandBuffer GetCommandBuffer(uint32_t frameInFlight){
			return m_CommandBuffer[frameInFlight];
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
		VulkanRenderCommandBuffer(std::string debugName = "", bool swapchaing = false);
		VulkanRenderCommandBuffer(CommandBuffer* buffer);
		virtual ~VulkanRenderCommandBuffer();
		VkCommandBuffer GetCommandBuffer(uint32_t frameInFlight );
	private:
		bool m_Swapcahin = false;
		void Submit();
		void BeginRecord(uint32_t frameIndex );
		void EndRecord(uint32_t frameIndex );
		CommandBuffer* m_NormalCommandBuffer =nullptr;
		void Init();
		void Release();
		bool m_Recording = false;

		std::vector<VkCommandBuffer> m_CommandBuffers;
		std::vector<VkFence> m_WaitFences;
		std::string m_DebugName;
		VkCommandPool m_CommandPool = nullptr;
		friend class VulkanRenderPass;
		friend class VulkanSwapChain;
		friend class VulkanRenderer;
		friend class VulkanRendererAPI;
	};
}
