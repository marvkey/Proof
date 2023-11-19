#pragma once
#include <glm/glm.hpp>
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
		virtual void BeginRenderMaterialRenderPass(Count<class RenderCommandBuffer> commandBuffer, Count<class RenderPass> renderPass, bool explicitClear = false)= 0;
		virtual void EndRenderPass(Count<class RenderPass> renderPass) = 0;
		virtual void RenderPassPushRenderMaterial(Count<class RenderPass> renderPass, Count<class RenderMaterial> renderMaterial) = 0;

		virtual void BeginComputePass(Count<class RenderCommandBuffer> commandBuffer, Count<class ComputePass> computPass) = 0;
		virtual void BeginRenderMaterialComputePass(Count<class RenderCommandBuffer> commandBuffer, Count<class ComputePass> computPass) = 0;
		virtual void EndComputePass(Count<class ComputePass> computPass) = 0;
		virtual void ComputePassPushRenderMaterial(Count<class ComputePass> computePass, Count<class RenderMaterial> renderMaterial) = 0;

		virtual void SubmitCommandBuffer(Count<class RenderCommandBuffer>commandBuffer) = 0;
		inline static Renderer::API GetAPI() { return ActiveAPI; }

	private:
		virtual void Init() = 0;
		virtual void ShutDown() =0;
		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;
		friend class RendererBase;
		friend class Renderer;
		static Renderer::API ActiveAPI;
	};
}