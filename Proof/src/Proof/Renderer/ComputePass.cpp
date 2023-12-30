#include "Proofprch.h"	
#include "ComputePass.h"
#include "Proof/Platform//Vulkan/VulkanComputePass.h"
#include "RenderPass.h"
#include "Renderer.h"
namespace Proof
{
	Count<ComputePass> ComputePass::Create(const ComputePassConfiguration& config)
	{
		switch (Renderer::GetAPI())
		{
			case Renderer::API::None:  PF_CORE_ASSERT(false, "Uniform Buffer None it needs an api"); return nullptr;
			case Renderer::API::OpenGL: return nullptr;
			case Renderer::API::Vulkan: return Count<VulkanComputePass>::Create(config);

		}
		PF_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
