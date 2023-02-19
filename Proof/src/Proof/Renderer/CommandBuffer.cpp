#include "Proofprch.h"
#include "CommandBuffer.h"
#include "Platform/Vulkan/VulkanCommandBuffer.h"
#include "RendererAPI.h"
namespace Proof
{
    Count<CommandBuffer> Proof::CommandBuffer::Create() {
		switch (RendererAPI::GetAPI()) {
			case RendererAPI::API::None: PF_CORE_ASSERT(false, "RENDERER:API None is not a default value!") return nullptr;
			case RendererAPI::API::OpenGL: return nullptr;
			case RendererAPI::API::Vulkan: return CreateCount<VulkanCommandBuffer>();
		}
		PF_CORE_ASSERT(false, "API is not supported");
    }
	Count<RenderCommandBuffer> RenderCommandBuffer::Create(CommandBuffer* buffer)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None: PF_CORE_ASSERT(false, "RENDERER:API None is not a default value!") return nullptr;
			case RendererAPI::API::OpenGL: return nullptr;
			case RendererAPI::API::Vulkan: return CreateCount<VulkanRenderCommandBuffer>(buffer);
		}
		PF_CORE_ASSERT(false, "API is not supported");
	}
}