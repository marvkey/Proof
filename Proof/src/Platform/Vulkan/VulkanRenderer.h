#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Renderer/RendererAPI.h"
#include "Proof/Renderer/Renderer.h"
#include <vulkan/vulkan.h>

namespace Proof
{

	class VulkanRenderer : public RendererAPI 
	{
	public:
		void Init();
		void ShutDown();
		void SubmitCommandBuffer(Count<class RenderCommandBuffer> commandBuffer);
		
		void BeginFrame();
		void EndFrame();

		static Count<class VulkanGraphicsContext> GetGraphicsContext();

		virtual void DrawArrays(Count<class RenderCommandBuffer> commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex = 0, uint32_t firstInstance = 0) override;
		virtual void DrawElementIndexed(Count<class RenderCommandBuffer> commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstInstance = 0) override;
		virtual void DrawElementIndexed(Count<class RenderCommandBuffer> commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex = 0, int32_t vertexOffset = 0, uint32_t firstInstance = 0);

		virtual void BeginRenderPass(Count<class RenderCommandBuffer> commandBuffer, Count<class RenderPass> renderPass, bool explicitClear = false)override;
		virtual void BeginRenderMaterialRenderPass(Count<class RenderCommandBuffer> commandBuffer, Count<class RenderPass> renderPass, bool explicitClear = false)override;
		virtual void EndRenderPass(Count<class RenderPass> renderPass)override;
		virtual void RenderPassPushRenderMaterial(Count<class RenderPass> renderPass, Count<class RenderMaterial> renderMaterial)override;

		virtual void BeginComputePass(Count<class RenderCommandBuffer> commandBuffer, Count<class ComputePass> computPass)override;
		virtual void BeginRenderMaterialComputePass(Count<class RenderCommandBuffer> commandBuffer, Count<class ComputePass> computPass)override;
		virtual void EndComputePass(Count<class ComputePass> computPass)override;
		virtual void ComputePassPushRenderMaterial(Count<class ComputePass> computePass, Count<class RenderMaterial> renderMaterial)override;

		virtual void BeginCommandBuffer(Count<class RenderCommandBuffer> commandBuffer)override;
		virtual void EndCommandBuffer(Count<class RenderCommandBuffer> commandBuffer)override;
		virtual ~VulkanRenderer() {};

		// dont use on rener thread
		virtual void PushSetCubeMapImage(Count<class TextureCube>cubeTexture, Count<class Texture2D> texture);

	private:
		friend class VulkanRendererAPI;
		friend class RendererBase;
	};
}