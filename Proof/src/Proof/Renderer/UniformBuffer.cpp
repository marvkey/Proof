#include "Proofprch.h"
#include "UniformBuffer.h"
#include "Platform/Vulkan/VulkanResourceBuffer.h"
#include "Platform/Vulkan/VulkanCommandBuffer.h"
#include "Platform/Vulkan/VulkanTexutre.h"
#include "Renderer.h"
namespace Proof
{
	Count<UniformBuffer> UniformBuffer::Create(uint32_t size) {
		switch (Renderer::GetAPI()) {
			case Renderer::API::None:  PF_CORE_ASSERT(false, "Uniform Buffer None it needs an api"); return nullptr;
			case Renderer::API::OpenGL: return nullptr;
			case Renderer::API::Vulkan: return Count<VulkanUniformBuffer>::Create(size);
		}
		PF_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
	Count<UniformBuffer> UniformBuffer::Create(const void* data,uint32_t size) {
		switch (Renderer::GetAPI())
		{
			case Renderer::API::None:  PF_CORE_ASSERT(false, "Uniform Buffer None it needs an api"); return nullptr;
			case Renderer::API::OpenGL: return nullptr;
			case Renderer::API::Vulkan: return Count<VulkanUniformBuffer>::Create(data,size);
		}
		PF_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Count<StorageBuffer> StorageBuffer::Create(uint32_t size)
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

	Count<StorageBuffer> StorageBuffer::Create(const void* data, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case Renderer::API::None:  PF_CORE_ASSERT(false, "Uniform Buffer None it needs an api"); return nullptr;
			case Renderer::API::OpenGL: return nullptr;
			case Renderer::API::Vulkan: return Count<VulkanStorageBuffer>::Create(data,size);
		}
		PF_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}


