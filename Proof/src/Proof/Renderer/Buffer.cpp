#include "Proofprch.h"
#include "Proof/Platform/Vulkan/VulkanBuffer.h"
#include "Proof/Platform/Vulkan/VulkanResourceBuffer.h"
#include "Renderer.h"
#include "Buffer.h"
namespace Proof
{
	Count<VertexBuffer>VertexBuffer::Create(const void* Data, uint64_t size) {
		switch (Renderer::GetAPI()) {
			case Renderer::API::None:  PF_CORE_ASSERT(false, "Vertex Buffer None it needs an api"); return nullptr;
			case Renderer::API::OpenGL: return nullptr;// CreateCount< OpenGLVertexBuffer>(Data, amount);
			case Renderer::API::Vulkan: return Count< VulkanVertexBuffer>::Create(Data, size);
		}
	}

	Count<VertexBuffer> VertexBuffer::Create(uint64_t Size) {
		switch (Renderer::GetAPI()) {
			case Renderer::API::None:  PF_CORE_ASSERT(false, "Vertex Buffer None it needs an api"); return nullptr;
			case Renderer::API::OpenGL: return nullptr;// CreateCount< OpenGLVertexBuffer>(Size);
			case Renderer::API::Vulkan: return Count< VulkanVertexBuffer>::Create(Size);
		}
	}

	Count<IndexBuffer>IndexBuffer::Create(const void* Data, uint32_t Size) {
		switch (Renderer::GetAPI()) {
			case Renderer::API::None:  PF_CORE_ASSERT(false, "Index Buffer None it needs an api "); return nullptr;
			case Renderer::API::OpenGL: return  nullptr;// CreateCount< OpenGLIndexBuffer>(Data, Count);
			case Renderer::API::Vulkan: return Count<VulkanIndexBuffer>::Create(Data, Size);
		}
		return nullptr;
	}
	Count<IndexBuffer> IndexBuffer::Create(uint32_t Size)
	{
		switch (Renderer::GetAPI())
		{
			case Renderer::API::None:  PF_CORE_ASSERT(false, "Index Buffer None it needs an api "); return nullptr;
			case Renderer::API::OpenGL: return  nullptr;// CreateCount< OpenGLIndexBuffer>(Data, Count);
			case Renderer::API::Vulkan: return Count<VulkanIndexBuffer>::Create(Size);
		}
		return nullptr;
	}
	VertexBufferSet::VertexBufferSet(uint64_t Size)
	{
		for (uint32_t index = 0; index < Renderer::GetConfig().FramesFlight; index++)
		{
			m_VertexBuffers[index] = VertexBuffer::Create(Size);
		}
	}
	VertexBufferSet::VertexBufferSet(const void* Data, uint64_t Size)
	{
		for (uint32_t index = 0; index < Renderer::GetConfig().FramesFlight; index++)
		{
			m_VertexBuffers[index] = VertexBuffer::Create(Data,Size);
		}
	}
	Count<VertexBuffer> VertexBufferSet::GetVertexBufferIndex(uint32_t set)
	{
		PF_CORE_ASSERT(m_VertexBuffers.contains(set), fmt::format("Vertex Buffer Set Does not contain set: {}", set));
		return m_VertexBuffers.at(set);
	}
	Count<VertexBuffer> VertexBufferSet::GetVertexBuffer()
	{
		return GetVertexBufferIndex(Renderer::GetCurrentFrameInFlight());
	}
	Count<VertexBuffer> VertexBufferSet::RT_GetVertexBuffer()
	{
		return GetVertexBufferIndex(Renderer::RT_GetCurrentFrameInFlight());
	}
}