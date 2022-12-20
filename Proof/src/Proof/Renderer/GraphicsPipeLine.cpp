#include "Proofprch.h"
#include "GraphicsPipeLine.h"
#include "platform/Vulkan/VulkanGraphicsPipeline.h"
#include "Renderer.h"
namespace Proof
{
    Count<GraphicsPipeline> GraphicsPipeline::Create(Count<class Shader> shader, Count<class RenderPass> renderPass, Count<class PipeLineLayout> pipeline, Count < VertexArray> vertexArray) {
		switch (RendererAPI::GetAPI()) {
			case RendererAPI::API::None:  PF_CORE_ASSERT(false, "Uniform Buffer None it needs an api"); return nullptr;
			case RendererAPI::API::OpenGL: return nullptr;
			case RendererAPI::API::Vulkan:return CreateCount<VulkanGraphicsPipeline>(shader, renderPass, pipeline, vertexArray);
			
		}
		PF_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
    }
}
