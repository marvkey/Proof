#include "Proofprch.h"
#include "VertexArray.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "Platform/Vulkan/VulkanVertexArray.h"
#include "RendererAPI.h"
namespace Proof {
	
	Count<VertexArray> VertexArray::Create(const std::vector<VertexInputDataConfig>& rate) {
		switch (RendererAPI::GetAPI()) {
			case RendererAPI::API::None:  PF_CORE_ASSERT(false, "Vertex Array None it needs an api"); return nullptr;
			case RendererAPI::API::OpenGL: return nullptr;// CreateCount<OpenGLVertexArray>(Size);
			case RendererAPI::API::Vulkan: return CreateCount<VulkanVertexArray>(rate);
		}
		PF_CORE_ASSERT(false, "Vertex Array None it needs an api");
		return nullptr;
	}
}