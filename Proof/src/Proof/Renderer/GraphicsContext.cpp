#include "Proofprch.h"
#include "GraphicsContext.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLGraphicsContext.h"
#include "Platform/Vulkan/VulkanGraphicsContext.h"
namespace Proof {
	Count<GraphicsContext> GraphicsContext::Create(Window* WindowHandle) {
		switch (RendererAPI::GetAPI()) {
			case RendererAPI::API::None: PF_CORE_ASSERT(false,"Window handle can not be null ptr") return nullptr;
			case RendererAPI::API::OpenGL: return nullptr;// CreateCount<OpenGLGraphicsContext>(WindowHandle);
			case RendererAPI::API::Vulkan: return CreateCount<VulkanGraphicsContext>(WindowHandle);
		}
	}
}