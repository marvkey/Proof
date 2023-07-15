#include "Proofprch.h"
#include "CommandBuffer.h"
#include "Platform/Vulkan/VulkanCommandBuffer.h"
#include "Renderer.h"
namespace Proof
{
    Count<CommandBuffer> Proof::CommandBuffer::Create() {
		switch (Renderer::GetAPI()) {
			case Renderer::API::None: PF_CORE_ASSERT(false, "RENDERER:API None is not a default value!") return nullptr;
			case Renderer::API::OpenGL: return nullptr;
			case Renderer::API::Vulkan: return Count<VulkanCommandBuffer>::Create();
		}
		PF_CORE_ASSERT(false, "API is not supported");
    }
	Count<RenderCommandBuffer> RenderCommandBuffer::Create(CommandBuffer* buffer)
	{
		switch (Renderer::GetAPI())
		{
			case Renderer::API::None: PF_CORE_ASSERT(false, "RENDERER:API None is not a default value!") return nullptr;
			case Renderer::API::OpenGL: return nullptr;
			case Renderer::API::Vulkan: return Count<VulkanRenderCommandBuffer>::Create(buffer);
		}
		PF_CORE_ASSERT(false, "API is not supported");
	}
}
