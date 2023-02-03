#include "Proofprch.h"
#include "RendererAPI.h"
#include "FrameBuffer.h"
namespace Proof {
	RendererAPI::API RendererAPI::ActiveAPI = RendererAPI::API::Vulkan;

	void RendererAPI::BeginRenderPass(Count<class RenderCommandBuffer> commandBuffer, Count<class RenderPass> renderPass, Count<class ScreenFrameBuffer> frameBuffer)
	{
		BeginRenderPass(commandBuffer, renderPass, frameBuffer->GetFrameBuffer());
	}

}