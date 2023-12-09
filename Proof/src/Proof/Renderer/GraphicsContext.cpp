#include "Proofprch.h"
#include "GraphicsContext.h"
#include "Renderer.h"
//#include "Platform/OpenGL/OpenGLGraphicsContext.h"
#include "Proof/Platform/Vulkan/VulkanGraphicsContext.h"
namespace Proof {
	Count<GraphicsContext> GraphicsContext::Create() 
	{
		switch (Renderer::GetAPI()) {
			case Renderer::API::None: PF_CORE_ASSERT(false,"Window handle can not be nullptr") return nullptr;
			case Renderer::API::OpenGL: return nullptr;// CreateCount<OpenGLGraphicsContext>(WindowHandle);
			case Renderer::API::Vulkan: return Count<VulkanGraphicsContext>::Create();
		}
	}
}