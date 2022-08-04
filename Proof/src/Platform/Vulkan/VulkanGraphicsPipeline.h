#pragma once
#include "Proof/Core/Core.h"
#include <vulkan/vulkan.h>
namespace Proof
{
	struct PipelineConfigInfo {
		VkViewport Viewport;
		VkRect2D Scissor;
		VkPipelineViewportStateCreateInfo ViewportInfo;
		VkPipelineInputAssemblyStateCreateInfo InputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo RasterizationInfo;
		VkPipelineMultisampleStateCreateInfo MultisampleInfo;
		VkPipelineColorBlendAttachmentState ColorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo ColorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo DepthStencilInfo;
		VkPipelineLayout PipelineLayout = nullptr;
		VkRenderPass RenderPass = nullptr;
		uint32_t Subpass = 0;
	};
	class Proof_API VulkanGraphicsPipeline {
	public:
		virtual ~VulkanGraphicsPipeline();
		VulkanGraphicsPipeline(Count<class Shader> shader, const PipelineConfigInfo& info, class VulkanVertexInput* input = nullptr);
		static void DefaultPipelineConfigInfo(PipelineConfigInfo& configInfo, uint32_t width, uint32_t height);
		VkPipeline GetPipline() { return m_GraphicsPipeline; }
	private:
		Count<class VulkanShader> m_Shader = nullptr;
		VkPipeline m_GraphicsPipeline;
	};
}