#pragma once
#include "Proof/Renderer/3DRenderer/Renderer3DPBR.h"
#include "Proof/Renderer/ScreenFrameBuffer.h"
#include "Platform/Vulkan/VulkanGraphicsPipeline.h"
#include "Platform/Vulkan/VulkanSwapChain.h"
#include<vulkan/vulkan.h>
#include "Platform/Vulkan/VulkanCommandBuffer.h"
#include "Platform/Vulkan/VulkanBuffer.h"
namespace Proof
{
	struct DrawPipeline {
		Count<VulkanGraphicsPipeline> GraphicsPipeline = nullptr;
		Count<Shader> Shader = nullptr;
		Count<VulkanSwapChain> SwapChain = nullptr;
		Count<VulkanCommandBuffer> CommandBuffer = NULL;
		Count<VulkanVertexBuffer> VertexBuffer;

	};
	class VulkanRenderer {
	public:
		static void Init();
		static void BeginContext(const glm::mat4& projection, const glm::mat4& view, const Vector<>& Position, Count<ScreenFrameBuffer>& frameBuffer, RendererData& renderSpec);
		static void EndContext();
		static void Destroy();
		static VkCommandBuffer GetCurrentCommandBuffer();
		static void BeginRenderPass();
		static void EndRenderPass();

		//IMGUI
		static void BeginFrame() {};
		static void EndFrame() {};
	private:
		static DrawPipeline* s_Pipeline;
		static bool s_InContext;
		static void RecreateSwapChain();
		static void DrawFrame();
	};

	struct MeshPipeLine {
		Count<Shader> Shader = nullptr;
		
	};
}