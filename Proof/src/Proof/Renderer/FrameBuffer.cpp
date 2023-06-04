#include "Proofprch.h"
#include "FrameBuffer.h"
#include "RendererAPI.h"
#include "Platform/Vulkan/VulkanFrameBuffer.h"
#include "GraphicsContext.h"
#include "SwapChain.h"
namespace Proof {
	Count<FrameBuffer> FrameBuffer::Create(const FrameBufferConfig& config)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None: PF_CORE_ASSERT(false, "RENDERER:API None is not a default value!") return nullptr;
			case RendererAPI::API::OpenGL: return nullptr;
			case RendererAPI::API::Vulkan: return Count<VulkanFrameBuffer>::Create(config);
		}
	}

	Count<ScreenFrameBuffer> ScreenFrameBuffer::Create(uint32_t Width, uint32_t Height)
	{
		auto screenBuffer= Count<ScreenFrameBuffer>::Create();
		auto swapChain = Application::Get()->GetWindow()->GetSwapChain();
		FrameBufferConfig config;
		config.DebugName = "Screen FrameBuffer";
		config.Attachments = { swapChain->GetImageFormat(),swapChain->GetDepthFormat()};
		config.Size = { (float)Width,(float)Height };
		screenBuffer->m_FrameBuffer = FrameBuffer::Create(config);
		return screenBuffer;
	}
	Count<ScreenFrameBuffer> ScreenFrameBuffer::Create(const FrameBufferConfig& config)
	{
		auto screenBuffer = Count<ScreenFrameBuffer>::Create();
		screenBuffer->m_FrameBuffer = FrameBuffer::Create(config);
		return screenBuffer;
	}
	void ScreenFrameBuffer::Resize(Vector2 imageSize)
	{
		m_FrameBuffer->Resize(imageSize);
	}
	uint32_t ScreenFrameBuffer::GetFrameWidth()
	{
		return m_FrameBuffer->GetConfig().Size.X;
	}
	uint32_t ScreenFrameBuffer::GetFrameHeight()
	{
		return m_FrameBuffer->GetConfig().Size.Y;
	}
	Image ScreenFrameBuffer::GetImage()
	{
		return m_FrameBuffer->GetColorAttachmentImage(0);
	}
}
