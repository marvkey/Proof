#pragma once
#include <glm/glm.hpp>
#include <functional>
#include "RendererAPI.h"
namespace Proof {
	
	class Renderer {
	public:
		static void DrawElementIndexed(Count<class RenderCommandBuffer> commandBuffer, uint32_t indexCount, uint32_t instanceCount=1, uint32_t firstIndex = 0, int32_t vertexOffset = 0, uint32_t firstInstance = 0)
		{
			s_RendererAPI->DrawElementIndexed(commandBuffer, indexCount, instanceCount, firstIndex,vertexOffset,firstInstance);
		}

		static void DrawArrays(Count<class RenderCommandBuffer> commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex = 0, uint32_t firstInstance = 0) {
			s_RendererAPI->DrawArrays(commandBuffer,vertexCount,instanceCount,firstVertex,firstInstance);
		}
		static void BeginCommandBuffer(Count<class RenderCommandBuffer> commandBuffer) {
			s_RendererAPI->BeginCommandBuffer(commandBuffer);
		}
		static void EndCommandBuffer(Count<class RenderCommandBuffer> commandBuffer) {
			s_RendererAPI->EndCommandBuffer(commandBuffer);
		}
		static void BeginRenderPass(Count<class RenderCommandBuffer> commandBuffer, Count<class RenderPass> renderPass, Count<class GraphicsPipeline> pipline) {
			s_RendererAPI->BeginRenderPass(commandBuffer, renderPass, pipline);
		}
		static void EndRenderPass(Count<class RenderPass> renderPass) {
			s_RendererAPI->EndRenderPass(renderPass);
		}
		static void SubmitCommandBuffer(Count<class RenderCommandBuffer> commandBuffer) {
			s_RendererAPI->SubmitCommandBuffer(commandBuffer);
		}
		static CurrentFrame GetCurrentFrame() {
			return s_RendererAPI->GetCurrentFrame();
		}
		static const RendererConfig GetConfig() {
			return s_RendererAPI->GetConfig();
		}
		static Count<class GraphicsContext> GetGraphicsContext() {
			return RendererBase::GetGraphicsContext();
		}
		static void SubmitCommand(std::function<void(class CommandBuffer*)> func) {
			s_RendererAPI->Submit(func);
		}
		static void SubmitDatafree(std::function<void()> func) {
			s_RendererAPI->SubmitDatafree(func);
		}
		
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

		static Count<class Texture2D> GetWhiteTexture();

		//submit to render thread
		template<class FuncT>
		static void Submit(FuncT&& func)
		{
			//https://www.youtube.com/watch?v=WeqxJeme_88
			//(1:14:02)
			auto renderCommandQueue = [](void* ptr)
			{
				auto pFunc = (FuncT*)ptr;
				(*pFunc)();
		
				pFunc->~FuncT();
			};
			auto storageBuffer = GetRenderCommandQueue().Allocate(renderCommandQueue, sizeof(func));
			new (storageBuffer)FuncT(std::forward<FuncT>(func));
		};
		static class CommandQueue& GetRenderCommandQueue();
	private:
		static void OnWindowResize(WindowResizeEvent& e) {
			s_RendererAPI->OnWindowResize(e);
		}
		static void BeginFrame();
		static void EndFrame();
		static void Init();
		static void Unit();
		static RendererAPI* s_RendererAPI;
		friend class RendererBase;
		friend class Application;
	};
}
