#include "Proofprch.h"
#include "UniformBuffer.h"
#include "Platform/Vulkan/VulkanResourceBuffer.h"
#include "Platform/Vulkan/VulkanCommandBuffer.h"
#include "Platform/Vulkan/VulkanTexutre.h"
#include "Renderer.h"
namespace Proof
{
	Count<UniformBuffer> UniformBuffer::Create(uint64_t size) {
		switch (Renderer::GetAPI()) {
			case Renderer::API::None:  PF_CORE_ASSERT(false, "Uniform Buffer None it needs an api"); return nullptr;
			case Renderer::API::OpenGL: return nullptr;
			case Renderer::API::Vulkan: return Count<VulkanUniformBuffer>::Create(size);
		}
		PF_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
	Count<UniformBuffer> UniformBuffer::Create(Buffer data) {
		switch (Renderer::GetAPI())
		{
			case Renderer::API::None:  PF_CORE_ASSERT(false, "Uniform Buffer None it needs an api"); return nullptr;
			case Renderer::API::OpenGL: return nullptr;
			case Renderer::API::Vulkan: return Count<VulkanUniformBuffer>::Create(data);
		}
		PF_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
	Count<UniformBufferSet> UniformBufferSet::Create(uint64_t size)
	{
		switch (Renderer::GetAPI())
		{
			case Renderer::API::None:  PF_CORE_ASSERT(false, "Uniform Buffer None it needs an api"); return nullptr;
			case Renderer::API::OpenGL: return nullptr;
			case Renderer::API::Vulkan: return Count<VulkanUniformBufferSet>::Create(size);
		}
		PF_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Count<UniformBufferSet> UniformBufferSet::Create(Buffer data)
	{
		switch (Renderer::GetAPI())
		{
			case Renderer::API::None:  PF_CORE_ASSERT(false, "Uniform Buffer None it needs an api"); return nullptr;
			case Renderer::API::OpenGL: return nullptr;
			case Renderer::API::Vulkan: return Count<VulkanUniformBufferSet>::Create(data);
		}
		PF_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
	Count<StorageBuffer> StorageBuffer::Create(uint64_t size)
	{
		switch (Renderer::GetAPI())
		{
			case Renderer::API::None:  PF_CORE_ASSERT(false, "Uniform Buffer None it needs an api"); return nullptr;
			case Renderer::API::OpenGL: return nullptr;
			case Renderer::API::Vulkan: return Count<VulkanStorageBuffer>::Create(size);
		}
		PF_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Count<StorageBuffer> StorageBuffer::Create(Buffer data)
	{
		switch (Renderer::GetAPI())
		{
			case Renderer::API::None:  PF_CORE_ASSERT(false, "Uniform Buffer None it needs an api"); return nullptr;
			case Renderer::API::OpenGL: return nullptr;
			case Renderer::API::Vulkan: return Count<VulkanStorageBuffer>::Create(data);
		}
		PF_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Count<StorageBufferSet> StorageBufferSet::Create(uint64_t size)
	{
		switch (Renderer::GetAPI())
		{
			case Renderer::API::None:  PF_CORE_ASSERT(false, "Uniform Buffer None it needs an api"); return nullptr;
			case Renderer::API::OpenGL: return nullptr;
			case Renderer::API::Vulkan: return Count<VulkanStorageBufferSet>::Create(size);
		}
		PF_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Count<StorageBufferSet> StorageBufferSet::Create(Buffer data)
	{
		switch (Renderer::GetAPI())
		{
			case Renderer::API::None:  PF_CORE_ASSERT(false, "Uniform Buffer None it needs an api"); return nullptr;
			case Renderer::API::OpenGL: return nullptr;
			case Renderer::API::Vulkan: return Count<VulkanStorageBufferSet>::Create(data);
		}
		PF_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}



	void GlobalBufferSet::SetData(const std::string& name, Count<StorageBufferSet> set)
	{
		m_Buffers[name] = { RendererResourceType::StorageBufferSet, set };
	}
	void GlobalBufferSet::SetData(const std::string& name, Count<UniformBufferSet> set)
	{
		m_Buffers[name] = { RendererResourceType::UniformBufferSet, set };
	}
}


