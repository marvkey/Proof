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
		VulkanGraphicsPipeline(const GraphicsPipelineConfig& config);
		VkPipeline GetPipline() { return m_GraphicsPipeline; }
		static void DefaultPipelineConfigInfo(PipelineConfigInfo& configInfo,const GraphicsPipelineConfig& graphicsConfig);

	private:
		std::string m_DebugName;
		float m_LineWidth = 1.0f;
		bool m_WriteDepth = true;
		bool m_Depth = true;
		DepthCompareOperator m_DepthCompareOperator = DepthCompareOperator::Less;
		// temporary
		friend class VulkanRenderPass;
		VkPipeline m_GraphicsPipeline;
	};
}