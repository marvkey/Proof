#include "Proofprch.h"
#include "SwapChain.h"
#include "Platform/Vulkan/VulkanSwapChain.h"

namespace Proof
{
    Count<SwapChain> SwapChain::Create(ScreenSize size) {
		switch (RendererAPI::GetAPI()) {
			case RendererAPI::API::None: PF_CORE_ASSERT(false, "RENDERER:API None is not a default value!") return nullptr;
			case RendererAPI::API::OpenGL: return nullptr;
			case RendererAPI::API::Vulkan: return CreateCount<VulkanSwapChain>(size);
		}
    }
}
