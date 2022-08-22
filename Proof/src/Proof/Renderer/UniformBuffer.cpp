#include "Proofprch.h"
#include "UniformBuffer.h"
#include "Platform/OpenGL/OpenGLUniformBuffer.h"
#include "RendererAPI.h"
namespace Proof{
    Count<UniformBuffer> Proof::UniformBuffer::Create(uint32_t size,uint32_t bindingPoint) {
		switch (RendererAPI::GetAPI()) {
		case RendererAPI::API::None:  PF_CORE_ASSERT(false,"Uniform Buffer None it needs an api"); return nullptr;
		case RendererAPI::API::OpenGL: return CreateCount<OpenGLUniformBuffer>(size,bindingPoint);
		case RendererAPI::API::Vulkan: return nullptr;
		}
		PF_CORE_ASSERT(false,"Unknown RendererAPI!");
		return nullptr;
    }
}
