#include "Proofprch.h"
#include "GraphicsPipeLine.h"
#include "Proof/platform/Vulkan/VulkanGraphicsPipeline.h"
#include "Renderer.h"
namespace Proof
{
  
	Count<GraphicsPipeline> GraphicsPipeline::Create(const GraphicsPipelineConfiguration& piplineConfig)
	{
		switch (Renderer::GetAPI())
		{
			case Renderer::API::None:  PF_CORE_ASSERT(false, "Uniform Buffer None it needs an api"); return nullptr;
			case Renderer::API::OpenGL: return nullptr;
			case Renderer::API::Vulkan:return Count<VulkanGraphicsPipeline>::Create(piplineConfig);

		}
		PF_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
