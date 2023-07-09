#pragma once
#include "Proof/Core/Core.h"
#include<vulkan/vulkan.h>
#include "Proof/Renderer/Renderer.h"
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
		std::vector<Count<class RenderCommandBuffer>> CommandBuffers;
		std::vector<DeletionQueue> ResourceFreeQueue;

	};
	

	class VulkanRenderer {
	public:
		static void Init();
		static void Destroy();
		static void SubmitCommandBuffer(Count<class RenderCommandBuffer> commandBuffer);
		static void SubmitDatafree(std::function<void()> func);
		
		static void BeginFrame();
		static void EndFrame();
		static void DrawFrame();

		static Count<class VulkanGraphicsContext> GetGraphicsContext();

	private:
		static void SetGraphicsContext(Count<GraphicsContext> graphics);
		static void OnWindowResize(class WindowResizeEvent& e);
		static RendererConfig s_RendererConfig;
		static CurrentFrame s_CurrentFrame;
		friend class VulkanRendererAPI;
		friend class RendererBase;
	};
}