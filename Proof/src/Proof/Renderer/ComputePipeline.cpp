#include "Proofprch.h"
#include "ComputePipeline.h"
#include "Proof/Platform/Vulkan/VulkanComputePipeline.h"
#include "Renderer.h"

namespace Proof 
{
    Count<ComputePipeline> ComputePipeline::Create(const ComputePipelineConfig& config)
    {
		switch (Renderer::GetAPI())
		{
			case Renderer::API::None:  PF_CORE_ASSERT(false, "Uniform Buffer None it needs an api"); return nullptr;
			case Renderer::API::OpenGL: return nullptr;
			case Renderer::API::Vulkan:return Count<VulkanComputePipeline>::Create(config);

		}
		PF_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
