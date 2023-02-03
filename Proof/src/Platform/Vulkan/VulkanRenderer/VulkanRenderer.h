#pragma once
#include "Proof/Renderer/3DRenderer/Renderer3DPBR.h"
#include "Proof/Renderer/FrameBuffer.h"
#include "Platform/Vulkan/VulkanGraphicsPipeline.h"
#include "Platform/Vulkan/VulkanSwapChain.h"
#include<vulkan/vulkan.h>
#include "Proof/Renderer/CommandBuffer.h"
#include "Platform/Vulkan/VulkanBuffer.h"
#include "../VulkanPipeLineLayout.h"
#include "../VulkanPushConstant.h"
#include "../VulkanGraphicsContext.h"
#include "../VulkanDescriptorSet.h"
#include "../VulkanFrameBuffer.h"
namespace Proof
{
	struct DeletionQueue {

		void Push(std::function<void()>&& function) {
			Deletors.push_back(function);
		}

		void Flush() {

			for (auto& func : Deletors)
				func();

			Deletors.clear();
		}
		std::vector<std::function<void()>> Deletors;
	private:

	};

	struct VulkanRendererData {
		std::vector<Count<RenderCommandBuffer>> CommandBuffers;
		std::vector<DeletionQueue> ResourceFreeQueue;

	};
	

	class VulkanRenderer {
	public:
		static void Init();
		static void Destroy();
		static void SubmitCommandBuffer(Count<RenderCommandBuffer> commandBuffer);
		static void SubmitDatafree(std::function<void()> func);
		
		static void BeginFrame();
		static void EndFrame();
		static void DrawFrame();

	private:
		static void OnWindowResize(WindowResizeEvent& e);
		static RendererConfig s_RendererConfig;
		static CurrentFrame s_CurrentFrame;
		friend class VulkanRendererAPI;
	};
}