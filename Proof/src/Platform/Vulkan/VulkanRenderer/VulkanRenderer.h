#pragma once
#include "Proof/Renderer/3DRenderer/Renderer3DPBR.h"
#include "Proof/Renderer/ScreenFrameBuffer.h"
#include "Platform/Vulkan/VulkanGraphicsPipeline.h"
#include "Platform/Vulkan/VulkanSwapChain.h"
#include<vulkan/vulkan.h>
#include "Platform/Vulkan/VulkanCommandBuffer.h"
#include "Platform/Vulkan/VulkanBuffer.h"
#include "../VulkanPipeLineLayout.h"

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
		template<typename T>
		static void BeginRenderPass(VkPipeline pipeLine, T func, const glm::vec4& color = { 0.1,0.1,0.1,1 }, float depth = 1.0f, uint32_t stencil = 0) {
			s_Pipeline->CommandBuffer->BeginRenderPass(swapchainImageIndex, pipeLine,
				[&](VkCommandBuffer& buffer) {
					func(buffer);
				},color,depth,stencil);
		}
		static void EndRenderPass();

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