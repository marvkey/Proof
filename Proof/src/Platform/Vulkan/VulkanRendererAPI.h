#pragma once
#include "Proof/Renderer/RendererAPI.h"
namespace Proof {
   class VulkanRendererAPI: public RendererAPI {
   public:
	  virtual void DrawArrays(Count<class RenderCommandBuffer> commandBuffer, uint32_t vertexCount,uint32_t instanceCount,uint32_t firstVertex = 0,uint32_t firstInstance = 0) override;
	  virtual void DrawElementIndexed(Count<class RenderCommandBuffer> commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstInstance=0) override;
	  virtual void DrawElementIndexed(Count<class RenderCommandBuffer> commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex=0, int32_t vertexOffset=0, uint32_t firstInstance=0);

	  virtual void BeginRenderPass(Count<class RenderCommandBuffer> commandBuffer, Count<class RenderPass> renderPass, bool explicitClear = false);
	  virtual void BeginRenderPass(Count<class RenderCommandBuffer> commandBuffer, Count<class RenderPass> renderPass,Count<class RenderMaterial> renderMaterial, bool explicitClear = false);
	  virtual void EndRenderPass(Count<class RenderPass> renderPass)override;

   	  virtual void BeginComputePass(Count<class RenderCommandBuffer> commandBuffer,Count<class ComputePass> computPass);
   	  virtual void BeginComputePass(Count<class RenderCommandBuffer> commandBuffer,Count<class ComputePass> computPass,Count<class RenderMaterial> renderMaterial);
   	  virtual void EndComputePass(Count<class ComputePass> computPass);

	  virtual void SubmitCommandBuffer(Count<class RenderCommandBuffer> commandBuffer) override;
	  virtual void SubmitDatafree(std::function<void()> func)override;
	  virtual void Submit(std::function<void(CommandBuffer*)>func)override;

	  virtual void BeginCommandBuffer(Count<class RenderCommandBuffer> commandBuffer)override;
	  virtual void EndCommandBuffer(Count<class RenderCommandBuffer> commandBuffer)override;
	  virtual CurrentFrame GetCurrentFrame();
	  virtual RendererConfig GetConfig();
	  virtual void OnWindowResize(WindowResizeEvent& e);
	  VulkanRendererAPI();
	  virtual ~VulkanRendererAPI() {};
   private:
	   virtual void SetGraphicsContext(Count<GraphicsContext> context);
	   virtual void Init()override;
	   virtual void Destroy()override;
	   virtual void BeginFrame()override;
	   virtual void EndFrame()override;
   };
}

