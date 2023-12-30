#include "Proofprch.h"
#include "VertexArray.h"
//#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "Proof/Platform/Vulkan/VulkanVertexArray.h"
#include "Renderer.h"
namespace Proof {
	
	Count<VertexArray> VertexArray::Create(const std::vector<VertexInputDataConfig>& rate) {
		switch (Renderer::GetAPI()) {
			case Renderer::API::None:  PF_CORE_ASSERT(false, "Vertex Array None it needs an api"); return nullptr;
			case Renderer::API::OpenGL: return nullptr;// CreateCount<OpenGLVertexArray>(Size);
			case Renderer::API::Vulkan: return Count<VulkanVertexArray>::Create(rate);
		}
		PF_CORE_ASSERT(false, "Vertex Array None it needs an api");
		return nullptr;
	}
}