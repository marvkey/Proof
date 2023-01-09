#include "Proofprch.h"
#include "GraphicsPipeLine.h"
#include "platform/Vulkan/VulkanGraphicsPipeline.h"
#include "Renderer.h"
namespace Proof
{
  
	Count<GraphicsPipeline> GraphicsPipeline::Create(const GraphicsPipelineConfig& piplineConfig)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:  PF_CORE_ASSERT(false, "Uniform Buffer None it needs an api"); return nullptr;
			case RendererAPI::API::OpenGL: return nullptr;
			case RendererAPI::API::Vulkan:return CreateCount<VulkanGraphicsPipeline>(piplineConfig);

		}
		PF_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
