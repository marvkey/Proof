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
			s_Pipeline->CommandBuffer->BeginRenderPass(swapchainImageIndex, graphicsPipeLine,color,depth,stencil);
		}
		static void EndRenderPass();
		static uint32_t swapchainImageIndex;

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