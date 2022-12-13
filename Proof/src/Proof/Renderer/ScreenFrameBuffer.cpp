#include "Proofprch.h"
#include "ScreenFrameBuffer.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLScreenFrameBuffer.h"
#include "Platform/Vulkan/VulkanFrameBuffer.h"
namespace Proof {
	Count<ScreenFrameBuffer> ScreenFrameBuffer::Create(uint32_t Width,uint32_t Height) {
		switch (RendererAPI::GetAPI()) {
			case RendererAPI::API::None: PF_CORE_ASSERT(false,"RENDERER:API None is not a default value!") return nullptr;
			case RendererAPI::API::OpenGL: return CreateCount<OpenGLScreenFrameBuffer>(Width, Height);
			case RendererAPI::API::Vulkan: return CreateCount<VulkanScreenFrameBuffer>(Vector2{(float) Width,(float)Height });
		}
	}
}
