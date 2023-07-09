#include "Proofprch.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"
#include "RendererBase.h"
#include "CommandQueue.h"
#include "Texture.h"
namespace Proof {
	RendererAPI* Renderer::s_RendererAPI;
	static std::vector<CommandQueue*> s_RenderCommandQueue;
	static uint32_t s_CommandQueueIndex = 1; // so we start with 0 command qeue index
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