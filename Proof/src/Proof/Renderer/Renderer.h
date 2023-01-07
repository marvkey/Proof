#pragma once
#include <glm/glm.hpp>
#include "RendererAPI.h"
namespace Proof {
	
	class Renderer {
	public:
		static void DrawElementIndexed(Count<class CommandBuffer> commandBuffer, uint32_t indexCount,uint32_t instanceCount,uint32_t firstInstance){
			s_RendererAPI->DrawElementIndexed(commandBuffer,indexCount,instanceCount,firstInstance);
		}
		static void DrawArrays(Count<class CommandBuffer> commandBuffer, uint32_t vertexCount,uint32_t instanceCount, uint32_t firstInstance = 0) {
			s_RendererAPI->DrawArrays(commandBuffer,vertexCount,instanceCount,firstInstance);
		}
		static void BeginCommandBuffer(Count<class CommandBuffer> commandBuffer) {
			s_RendererAPI->BeginCommandBuffer(commandBuffer);
		}
		static void EndCommandBuffer(Count<class CommandBuffer> commandBuffer) {
			s_RendererAPI->EndCommandBuffer(commandBuffer);
		}

		static void BeginRenderPass(Count<class CommandBuffer> commandBuffer, Count<class RenderPass> renderPass, Count<class ScreenFrameBuffer> frameBuffer, bool viewScreen = false) {
			s_RendererAPI->BeginRenderPass(commandBuffer, renderPass, frameBuffer, viewScreen);
		}
		static void RecordRenderPass(Count<class RenderPass> renderPass, Count<class GraphicsPipeline>pipeline, std::function<void(Count<CommandBuffer> commandBuffer)> data) {
			s_RendererAPI->RecordRenderPass(renderPass, pipeline,data);
		}
		static void EndRenderPass(Count<class RenderPass> renderPass) {
			s_RendererAPI->EndRenderPass(renderPass);
		}
		static void SubmitCommandBuffer(Count<class CommandBuffer> commandBuffer) {
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
		static void Submit(std::function<void(class CommandBuffer*)> func) {
			s_RendererAPI->Submit(func);
		}
		static void SubmitDatafree(std::function<void()> func) {
			s_RendererAPI->SubmitDatafree(func);
		}
		
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

		static Count<class Texture2D> GetWhiteTexture();
	private:
		static void OnWindowResize(WindowResizeEvent& e) {
			s_RendererAPI->OnWindowResize(e);
		}
		static void BeginFrame() {
			s_RendererAPI->BeginFrame();
		}
		static void EndFrame() {
			s_RendererAPI->EndFrame();
		}
		
		static RendererAPI* s_RendererAPI;
		friend class RendererBase;
		friend class Application;
	};
}
