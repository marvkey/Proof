#pragma once
#include "Proof/Renderer/3DRenderer/Renderer3DPBR.h"
#include "Proof/Renderer/ScreenFrameBuffer.h"
#include "Platform/Vulkan/VulkanGraphicsPipeline.h"
#include "Platform/Vulkan/VulkanSwapChain.h"
#include<vulkan/vulkan.h>
#include "Platform/Vulkan/VulkanCommandBuffer.h"
#include "Platform/Vulkan/VulkanBuffer.h"
#include "../VulkanPipeLineLayout.h"
#include "../VulkanPushConstant.h"
#include "../VulkanGraphicsContext.h"
#include "../VulkanDescriptorSet.h"
namespace Proof
{
	struct DrawPipeline {
		Count<VulkanSwapChain> SwapChain = nullptr;
		Count<VulkanCommandBuffer> CommandBuffer = nullptr;
	};	

	class VulkanRenderer {
	public:
		static void Init();
		static void BeginContext(const glm::mat4& projection, const glm::mat4& view, const Vector<>& Position, Count<ScreenFrameBuffer>& frameBuffer, RendererData& renderSpec);
		static void EndContext();
		static void Destroy();
		static VkCommandBuffer GetCurrentCommandBuffer();
		static void BeginRenderPass(Count<VulkanGraphicsPipeline> graphicsPipeLine, const glm::vec4& color = { 0.01,0.01,0.01,1 }, float depth = 1.0f, uint32_t stencil = 0) {
			s_Pipeline->CommandBuffer->BeginRenderPass(swapchainImageIndex, graphicsPipeLine, color, depth, stencil);
		}
		static void EndRenderPass();
		static uint32_t swapchainImageIndex;

		template <class T>
		static void Submit(T func) {
			auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();

			VkCommandBufferAllocateInfo allocInfo{};
			allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			allocInfo.commandPool = graphicsContext->GetCommandPool();
			allocInfo.commandBufferCount = 1;

			VkCommandBuffer commandBuffer;
			vkAllocateCommandBuffers(graphicsContext->GetDevice(), &allocInfo, &commandBuffer);

			VkCommandBufferBeginInfo beginInfo{};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

			vkBeginCommandBuffer(commandBuffer, &beginInfo);

			//VkBufferCopy copyRegion{};
			//copyRegion.size = size;
			//vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
			func(commandBuffer);
			vkEndCommandBuffer(commandBuffer);

			VkSubmitInfo submitInfo{};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &commandBuffer;

			vkQueueSubmit(graphicsContext->GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
			vkQueueWaitIdle(graphicsContext->GetGraphicsQueue());

			vkFreeCommandBuffers(graphicsContext->GetDevice(), graphicsContext->GetCommandPool(), 1, &commandBuffer);
		}
		//IMGUI
		static void BeginFrame() {};
		static void EndFrame() {};
		static DrawPipeline* s_Pipeline;
	private:
		static bool s_InContext;
		static void RecreateSwapChain();
		static void DrawFrame();
	};

	struct TrianglePipeLine {
		Count<VulkanGraphicsPipeline> GraphicsPipeline;
		Count<Shader> Shader;
		void Init();
	};
	struct MeshPipeLine {
		Count<VulkanGraphicsPipeline> GraphicsPipeline;
		Count<Shader> Shader;
		Count<VulkanPipeLineLayout> PipeLineLayout;
		Count<VulkanPushConstant> PushConstant;
		void Init();
	};
}