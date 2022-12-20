#include "Proofprch.h"
#include "RenderPass.h"
#include "Proof/Renderer/RendererBase.h"
#include "Platform/Vulkan/VulkanRenderPass.h"
#include "RendererAPI.h"
namespace Proof
{
    Count<RenderPass> Proof::RenderPass::Create(RenderPassType type) {
		switch (RendererAPI::GetAPI()) {
			case RendererAPI::API::None: PF_CORE_ASSERT(false, "RENDERER:API None is not a default value!") return nullptr;
			case RendererAPI::API::OpenGL: return nullptr;
			case RendererAPI::API::Vulkan: return CreateCount<VulkanRenderPass>(type);
		}
    }
}
