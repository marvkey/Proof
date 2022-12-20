#pragma once
#include "Proof/Renderer/RendererAPI.h"
namespace Proof {
   class VulkanRendererAPI: public RendererAPI {
   public:
	  virtual void DrawArrays(Count<class CommandBuffer> commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstInstance = 0) override;
	  virtual void DrawElementIndexed(Count<class CommandBuffer> commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstInstance=0) override;
	
	  virtual void BeginRenderPass(Count<class CommandBuffer> commandBuffer, Count<class RenderPass> renderPass, Count<class ScreenFrameBuffer> frameBuffer, bool viewScreen = false) override;
	  virtual void RecordRenderPass(Count<class RenderPass> renderPass,std::function<void(Count<CommandBuffer> commandBuffer)> data) override;
	  virtual void EndRenderPass(Count<class RenderPass> renderPass) override;
	  virtual void SubmitCommandBuffer(Count<class CommandBuffer> commandBuffer) override;
	  virtual void SubmitDatafree(std::function<void()> func)override;
	  virtual void Submit(std::function<void(CommandBuffer* )>func)override;
	  virtual CurrentFrame GetCurrentFrame();
	  virtual RendererConfig GetConfig();
	  VulkanRendererAPI();
   private:
	   virtual void Init()override;
	   virtual void Destroy()override;
	   virtual void BeginFrame()override;
	   virtual void EndFrame()override;
   };
}

