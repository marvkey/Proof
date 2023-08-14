#include "Proofprch.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Platform/Vulkan/VulkanBuffer.h"
#include "Platform/Vulkan/VulkanResourceBuffer.h"
#include "Renderer.h"
namespace Proof
{
	Count<VertexBuffer>VertexBuffer::Create(const void* Data, uint64_t amount) {
		switch (Renderer::GetAPI()) {
			case Renderer::API::None:  PF_CORE_ASSERT(false, "Vertex Buffer None it needs an api"); return nullptr;
			case Renderer::API::OpenGL: return nullptr;// CreateCount< OpenGLVertexBuffer>(Data, amount);
			case Renderer::API::Vulkan: return Count< VulkanVertexBuffer>::Create(Data, amount);
		}
	}

	Count<VertexBuffer> VertexBuffer::Create(uint64_t Size) {
		switch (Renderer::GetAPI()) {
			case Renderer::API::None:  PF_CORE_ASSERT(false, "Vertex Buffer None it needs an api"); return nullptr;
			case Renderer::API::OpenGL: return nullptr;// CreateCount< OpenGLVertexBuffer>(Size);
			case Renderer::API::Vulkan: return Count< VulkanVertexBuffer>::Create(Size);
		}
	}

	Count<IndexBuffer>IndexBuffer::Create(const void* Data, uint32_t count) {
		switch (Renderer::GetAPI()) {
			case Renderer::API::None:  PF_CORE_ASSERT(false, "Index Buffer None it needs an api "); return nullptr;
			case Renderer::API::OpenGL: return  nullptr;// CreateCount< OpenGLIndexBuffer>(Data, Count);
			case Renderer::API::Vulkan: return Count<VulkanIndexBuffer>::Create(Data,count);
		}
		return nullptr;
	}
	Count<IndexBuffer> IndexBuffer::Create(uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
			case Renderer::API::None:  PF_CORE_ASSERT(false, "Index Buffer None it needs an api "); return nullptr;
			case Renderer::API::OpenGL: return  nullptr;// CreateCount< OpenGLIndexBuffer>(Data, Count);
			case Renderer::API::Vulkan: return Count<VulkanIndexBuffer>::Create(count);
		}
		return nullptr;
	}
}