#pragma once
#include <glm/glm.hpp>
#include "RendererBase.h"
#include "Proof/Events/WindowEvent.h"
#include "Proof/Renderer/Renderer.h"
namespace Proof {
	class Proof_API RendererAPI {
	public:
		
		virtual ~RendererAPI();;
		virtual void BeginCommandBuffer(Count<class RenderCommandBuffer> commandBuffer) = 0;
		virtual void EndCommandBuffer(Count<class RenderCommandBuffer> commandBuffer) = 0;
		virtual void DrawArrays(Count<class RenderCommandBuffer> commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex = 0, uint32_t firstInstance = 0)=0;
		virtual void DrawElementIndexed(Count<class RenderCommandBuffer> commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex = 0, int32_t vertexOffset = 0, uint32_t firstInstance = 0) = 0;
		virtual void DrawElementIndexed(Count<class RenderCommandBuffer> commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstInstance =0) = 0;
		
		virtual void BeginRenderPass(Count<class RenderCommandBuffer> commandBuffer, Count<class RenderPass> renderPass, bool explicitClear = false) = 0;
		virtual void BeginRenderPass(Count<class RenderCommandBuffer> commandBuffer, Count<class RenderPass> renderPass,Count<class RenderMaterial> renderMaterial, bool explicitClear = false) = 0;
		virtual void EndRenderPass(Count<class RenderPass> renderPass)=0 ;

		virtual void BeginComputePass(Count<class RenderCommandBuffer> commandBuffer,Count<class ComputePass> computPass) = 0;
		virtual void BeginComputePass(Count<class RenderCommandBuffer> commandBuffer,Count<class ComputePass> computPass,Count<class RenderMaterial> renderMaterial) = 0;
		virtual void EndComputePass(Count<class ComputePass> computPass) = 0;

		virtual void SubmitCommandBuffer(Count<class RenderCommandBuffer>commandBuffer) = 0;
		virtual void Submit(std::function<void(class CommandBuffer*)> func ) = 0;
		virtual void SubmitDatafree(std::function<void()> func) = 0;
		virtual void OnWindowResize(WindowResizeEvent& e) = 0;
		virtual CurrentFrame GetCurrentFrame() = 0;
		virtual RendererConfig GetConfig() = 0;
		inline static Renderer::API GetAPI() { return ActiveAPI; }
	private:
		virtual void Init() = 0;
		virtual void SetGraphicsContext(Count<GraphicsContext> context) = 0;
		virtual void Destroy() =0;
		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;
		friend class RendererBase;
		friend class Renderer;
		static Renderer::API ActiveAPI;
	};
}