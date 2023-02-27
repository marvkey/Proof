#include "Proofprch.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Platform/Vulkan/VulkanBuffer.h"
#include "RendererAPI.h"
namespace Proof
{
	Count<VertexBuffer>VertexBuffer::Create(const void* Data, uint32_t amount) {
		switch (RendererAPI::GetAPI()) {
			case RendererAPI::API::None:  PF_CORE_ASSERT(false, "Vertex Buffer None it needs an api"); return nullptr;
			case RendererAPI::API::OpenGL: return nullptr;// CreateCount< OpenGLVertexBuffer>(Data, amount);
			case RendererAPI::API::Vulkan: return Count< VulkanVertexBuffer>::Create(Data, amount);
		}
	}

	Count<VertexBuffer> VertexBuffer::Create(uint32_t Size) {
		switch (RendererAPI::GetAPI()) {
			case RendererAPI::API::None:  PF_CORE_ASSERT(false, "Vertex Buffer None it needs an api"); return nullptr;
			case RendererAPI::API::OpenGL: return nullptr;// CreateCount< OpenGLVertexBuffer>(Size);
			case RendererAPI::API::Vulkan: return Count< VulkanVertexBuffer>::Create(Size);
		}
	}

	Count<IndexBuffer>IndexBuffer::Create(const void* Data, uint32_t count) {
		switch (RendererAPI::GetAPI()) {
			case RendererAPI::API::None:  PF_CORE_ASSERT(false, "Index Buffer None it needs an api "); return nullptr;
			case RendererAPI::API::OpenGL: return  nullptr;// CreateCount< OpenGLIndexBuffer>(Data, Count);
			case RendererAPI::API::Vulkan: return Count<VulkanIndexBuffer>::Create(Data,count);
		}
		return nullptr;

	}
}