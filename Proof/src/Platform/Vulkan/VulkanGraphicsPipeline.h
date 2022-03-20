#pragma once
#include "Proof/Renderer/Shader.h"
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
		~VulkanGraphicsPipeline();
		VulkanGraphicsPipeline(Count<Shader> shader,const PipelineConfigInfo& info,uint32_t attributeSize=0,uint32_t bindingSize=0,const VkVertexInputAttributeDescription* attributeData=nullptr,const VkVertexInputBindingDescription* bindingData=nullptr);
		static void DefaultPipelineConfigInfo(PipelineConfigInfo& configInfo, uint32_t width, uint32_t height);
		VkPipeline GetPipline() { return m_GraphicsPipeline; }
		VkPipeline m_GraphicsPipeline;

	private:
		Count<class VulkanShader> m_Shader = nullptr;
		PipelineConfigInfo m_ConfigInfo = {};
		VkPipelineViewportStateCreateInfo m_ViewportInfo;
	};
}