#include "Proofprch.h"
#include "SwapChain.h"
#include "Platform/Vulkan/VulkanSwapChain.h"
#include "Renderer.h"
namespace Proof
{
    Count<SwapChain> SwapChain::Create(ScreenSize size,bool vsync) {
		switch (Renderer::GetAPI()) {
			case Renderer::API::None: PF_CORE_ASSERT(false, "RENDERER:API None is not a default value!") return nullptr;
			case Renderer::API::OpenGL: return nullptr;
			case Renderer::API::Vulkan: return Count<VulkanSwapChain>::Create(size, vsync);
		}
    }
}
