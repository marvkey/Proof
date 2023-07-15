#pragma once
#include <glm/glm.hpp>
#include <functional>
#include "Proof/Math/Vector.h" 
#include "Proof/Events/WindowEvent.h"
namespace Proof {
	struct  RendererConfig {
		uint32_t FramesFlight = 2;
		uint32_t MaxImageCount = 2;
	};

	struct CurrentFrame {
		uint32_t FrameinFlight;
		uint32_t ImageIndex;
	};
	struct Viewport {
		// upper left and right corner
		float X;
		float Y;

		float Width = 0.0f;
		float Height = 0.0f;

		float MinDepth = 0.0f;
		float MaxDepth = 1.0f;
	};

	struct ViewportScissor {
		Vector2 Offset = { 0,0 };
		Vector2 Extent = { 0,0 };
	};
	class Renderer {
	public:
		enum class API {
			None =0, OpenGL =1, Vulkan=2
		};
		static void DrawElementIndexed(Count<class RenderCommandBuffer> commandBuffer, uint32_t indexCount, uint32_t instanceCount=1, uint32_t firstIndex = 0, int32_t vertexOffset = 0, uint32_t firstInstance = 0);
		static void DrawArrays(Count<class RenderCommandBuffer> commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex = 0, uint32_t firstInstance = 0);

		static void BeginCommandBuffer(Count<class RenderCommandBuffer> commandBuffer);
		static void EndCommandBuffer(Count<class RenderCommandBuffer> commandBuffer);
		static void SubmitCommandBuffer(Count<class RenderCommandBuffer> commandBuffer);
		static void SubmitCommand(std::function<void(class CommandBuffer*)> func);
		static void SubmitDatafree(std::function<void()> func);

		// explicit clear means the framebuffer will overide its base command and clear all attachemtns when set true
		static void BeginRenderPass(Count<class RenderCommandBuffer> commandBuffer, Count<class RenderPass> renderPass, bool explicitClear = false);
		// explicit clear means the framebuffer will overide its base command and clear all attachemtns when set true
		static void BeginRenderPass(Count<class RenderCommandBuffer> commandBuffer, Count<class RenderPass> renderPass, Count<class RenderMaterial> renderMaterial,bool explicitClear = false);
		static void EndRenderPass(Count<class RenderPass> renderPass);

		static void BeginComputePass(Count<class RenderCommandBuffer> commandBuffer,Count<class ComputePass> computPass);
		static void BeginComputePass(Count<class RenderCommandBuffer> commandBuffer,Count<class ComputePass> computPass,Count<class RenderMaterial> renderMaterial);
		static void EndComputePass(Count<class ComputePass> computPass);

		static Count<class Texture2D> GenerateBRDFLut();
		static class CommandQueue& GetRenderCommandQueue();
		static CurrentFrame GetCurrentFrame();
		static const RendererConfig GetConfig();
		static Count<class GraphicsContext> GetGraphicsContext();
		static Renderer::API GetAPI();

		static Count<class Texture2D> GetWhiteTexture();
		static Count<class Texture2D> GetBlackTexture();

		//submit to render thread
		//template<class FuncT>
		//static void Submit(FuncT&& func)
		//{
		//	//https://www.youtube.com/watch?v=WeqxJeme_88
		//	//(1:14:02)
		//	auto renderCommandQueue = [](void* ptr)
		//	{
		//		auto pFunc = (FuncT*)ptr;
		//		(*pFunc)();
		//
		//		pFunc->~FuncT();
		//	};
		//	auto storageBuffer = GetRenderCommandQueue().Allocate(renderCommandQueue, sizeof(func));
		//	new (storageBuffer)FuncT(std::forward<FuncT>(func));
		//};
	private:
		static void OnWindowResize(WindowResizeEvent& e);
		static void BeginFrame();
		static void EndFrame();
		static void Init();
		static void Unit();
		static class RendererAPI* s_RendererAPI;
		friend class RendererBase;
		friend class Application;
	};
}
