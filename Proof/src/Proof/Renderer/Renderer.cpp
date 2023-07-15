#include "Proofprch.h"
#include "Renderer.h"
#include "RendererBase.h"
#include "CommandQueue.h"
#include "Texture.h"
#include "RendererAPI.h"
#include "ComputePass.h"

namespace Proof {
	RendererAPI* Renderer::s_RendererAPI;
	static std::vector<CommandQueue*> s_RenderCommandQueue;
	static uint32_t s_CommandQueueIndex = 1; // so we start with 0 command qeue index
	void Renderer::DrawElementIndexed(Count<RenderCommandBuffer> commandBuffer, uint32_t indexCount,uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance)
	{
		s_RendererAPI->DrawElementIndexed(commandBuffer, indexCount, instanceCount, firstIndex,vertexOffset,firstInstance);
	}

	void Renderer::DrawArrays(Count<RenderCommandBuffer> commandBuffer, uint32_t vertexCount, uint32_t instanceCount,
		uint32_t firstVertex, uint32_t firstInstance)
	{
		s_RendererAPI->DrawArrays(commandBuffer,vertexCount,instanceCount,firstVertex,firstInstance);
	}

	void Renderer::BeginCommandBuffer(Count<RenderCommandBuffer> commandBuffer)
	{
		s_RendererAPI->BeginCommandBuffer(commandBuffer);
	}

	void Renderer::EndCommandBuffer(Count<RenderCommandBuffer> commandBuffer)
	{
		s_RendererAPI->EndCommandBuffer(commandBuffer);
	}

	void Renderer::BeginRenderPass(Count<RenderCommandBuffer> commandBuffer, Count<RenderPass> renderPass, bool explicitClear )
	{
		s_RendererAPI->BeginRenderPass(commandBuffer, renderPass, explicitClear);
	}

	void Renderer::BeginRenderPass(Count<RenderCommandBuffer> commandBuffer, Count<RenderPass> renderPass,Count<RenderMaterial> renderMaterial, bool explicitClear)
	{
		s_RendererAPI->BeginRenderPass(commandBuffer, renderPass,renderMaterial,explicitClear);

	}

	void Renderer::EndRenderPass(Count<RenderPass> renderPass)
	{
		s_RendererAPI->EndRenderPass(renderPass);
	}

	void Renderer::BeginComputePass(Count<RenderCommandBuffer> commandBuffer, Count<ComputePass> computPass)
	{
		s_RendererAPI->BeginComputePass(commandBuffer,computPass);

	}

	void Renderer::BeginComputePass(Count<RenderCommandBuffer> commandBuffer, Count<ComputePass> computPass,Count<RenderMaterial> renderMaterial)
	{
		s_RendererAPI->BeginComputePass(commandBuffer,computPass,renderMaterial);
	}

	void Renderer::EndComputePass(Count<ComputePass> computPass)
	{
		s_RendererAPI->EndComputePass(computPass);
	}

	void Renderer::SubmitCommandBuffer(Count<RenderCommandBuffer> commandBuffer)
	{
		s_RendererAPI->SubmitCommandBuffer(commandBuffer);
	}

	CurrentFrame Renderer::GetCurrentFrame()
	{
		return s_RendererAPI->GetCurrentFrame();
	}

	const RendererConfig Renderer::GetConfig()
	{
		return s_RendererAPI->GetConfig();
	}

	Count<GraphicsContext> Renderer::GetGraphicsContext()
	{
		return RendererBase::GetGraphicsContext();
	}

	void Renderer::SubmitCommand(std::function<void(CommandBuffer*)> func)
	{
		s_RendererAPI->Submit(func);
	}

	void Renderer::SubmitDatafree(std::function<void()> func)
	{
		s_RendererAPI->SubmitDatafree(func);
	}

	Renderer::API Renderer::GetAPI()
	{ return RendererAPI::GetAPI(); }

	void Renderer::OnWindowResize(WindowResizeEvent& e)
	{
		s_RendererAPI->OnWindowResize(e);
	}

	Count<Texture2D>Renderer::GetWhiteTexture(){
		return RendererBase::s_BaseTextures->WhiteTexture;
	}

	CommandQueue& Renderer::GetRenderCommandQueue()
	{
		return *s_RenderCommandQueue[s_CommandQueueIndex];
	}

	void Renderer::BeginFrame()
	{
		if (s_CommandQueueIndex == 1)
			s_CommandQueueIndex = 0;
		else
			s_CommandQueueIndex = 1;

		s_RendererAPI->BeginFrame();
	}

	void Renderer::EndFrame()
	{
		//GetRenderCommandQueue().Execute();
		s_RendererAPI->EndFrame();
	}

	void Renderer::Init()
	{
		s_RenderCommandQueue.resize(2);
		s_RenderCommandQueue[0] = new CommandQueue();
		s_RenderCommandQueue[1] = new CommandQueue();
	}

	void Renderer::Unit()
	{
		delete s_RenderCommandQueue[0];
		delete s_RenderCommandQueue[1];
		s_RenderCommandQueue.clear();
	}

}