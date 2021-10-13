#include "Proofprch.h"
#include "GraphicsContext.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLGraphicsContext.h"
namespace Proof {
	Count<GraphicsContext> GraphicsContext::Create(Window* WindowHandle) {
		switch (RendererAPI::GetAPI()) {
		case RendererAPI::API::None: PF_CORE_ASSERT(false,"Window handle can not be null ptr") return nullptr;
		case RendererAPI::API::OpenGL: return CreateCount<OpenGLGraphicsContext>(WindowHandle);
		}
	}
}