#include "Proofprch.h"
#include "FrameBuffer.h"
#include "Renderer.h"
#include "Proof/Platform/Vulkan/VulkanFrameBuffer.h"
#include "GraphicsContext.h"
#include "SwapChain.h"
namespace Proof {
	Count<FrameBuffer> FrameBuffer::Create(const FrameBufferConfig& config)
	{
		switch (Renderer::GetAPI())
		{
			case Renderer::API::None: PF_CORE_ASSERT(false, "RENDERER:API None is not a default value!") return nullptr;
			case Renderer::API::OpenGL: return nullptr;
			case Renderer::API::Vulkan: return Count<VulkanFrameBuffer>::Create(config);
		}
	}

}
