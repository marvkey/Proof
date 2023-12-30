#include "Proofprch.h"
#include "RenderPass.h"
#include "Proof/Platform/Vulkan/VulkanRenderPass.h"
#include "Renderer.h"
namespace Proof
{
    Count<RenderPass> Proof::RenderPass::Create(const RenderPassConfig& config) {
		switch (Renderer::GetAPI()) {
			case Renderer::API::None: PF_CORE_ASSERT(false, "RENDERER:API None is not a default value!") return nullptr;
			case Renderer::API::OpenGL: return nullptr;
			case Renderer::API::Vulkan: return Count<VulkanRenderPass>::Create(config);
		}
    }
}
