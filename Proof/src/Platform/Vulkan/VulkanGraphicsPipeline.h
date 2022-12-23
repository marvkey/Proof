#pragma once
#include "Proof/Core/Core.h"
#include <vulkan/vulkan.h>
#include "Proof/Renderer/GraphicsPipeLine.h"
namespace Proof
{
	struct PipelineConfigInfo {
		VkPipelineInputAssemblyStateCreateInfo InputAssemblyInfo;
		VkPipelineViewportStateCreateInfo ViewportState;
		VkPipelineRasterizationStateCreateInfo RasterizationInfo;
		VkPipelineMultisampleStateCreateInfo MultisampleInfo;
		VkPipelineDepthStencilStateCreateInfo DepthStencilInfo;
		VkPipelineColorBlendAttachmentState ColorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo ColorBlendInfo;
		VkPipelineLayout PipelineLayout = nullptr;
		VkPipelineDynamicStateCreateInfo DynamicSate;
		VkRenderPass RenderPass = nullptr;
		uint32_t Subpass = 0;
	};
	class  VulkanGraphicsPipeline : public GraphicsPipeline {
	public:
		virtual ~VulkanGraphicsPipeline();
		VulkanGraphicsPipeline(Count<class Shader> shader, Count<class RenderPass>renderPass, Count<class PipeLineLayout>pipeline, Count<class VertexArray> vertexArray=nullptr);
		static void DefaultPipelineConfigInfo(PipelineConfigInfo& configInfo, uint32_t width, uint32_t height);
		VkPipeline GetPipline() { return m_GraphicsPipeline; }
	private:
		VkPipeline m_GraphicsPipeline;
	};
}