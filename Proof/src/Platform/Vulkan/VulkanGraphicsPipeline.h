#pragma once
#include "Proof/Renderer/Shader.h"
#include "Proof/Core/Core.h"
#include <vulkan/vulkan.h>
#include <vector>
namespace Proof
{
	struct PipelineConfigInfo {
		// this 2prevent pointer problems
		//PipelineConfigInfo(const PipelineConfigInfo&) = delete;
		//PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

		VkPipelineViewportStateCreateInfo ViewportInfo;
		VkPipelineInputAssemblyStateCreateInfo InputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo RasterizationInfo;
		VkPipelineMultisampleStateCreateInfo MultisampleInfo;
		VkPipelineColorBlendAttachmentState ColorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo ColorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo DepthStencilInfo;

		std::vector<VkDynamicState>DynamicStateEnables; 
		VkPipelineDynamicStateCreateInfo DynamicStateInfo;
		VkPipelineLayout PipelineLayout = nullptr;
		VkRenderPass RenderPass = nullptr;
		uint32_t Subpass = 0;
	};
	class Proof_API VulkanGraphicsPipeline {
	public:

		~VulkanGraphicsPipeline();
		VulkanGraphicsPipeline(Count<Shader> shader,const PipelineConfigInfo& info,uint32_t attributeSize=0,uint32_t bindingSize=0,const VkVertexInputAttributeDescription* attributeData=nullptr,const VkVertexInputBindingDescription* bindingData=nullptr);
		static void DefaultPipelineConfigInfo(PipelineConfigInfo& configInfo);
		VkPipeline GetPipline() { return m_GraphicsPipeline; }
		void Recreate(Count<Shader> shader, const PipelineConfigInfo& info, uint32_t attributeSize = 0, uint32_t bindingSize = 0, const VkVertexInputAttributeDescription* attributeData = nullptr, const VkVertexInputBindingDescription* bindingData = nullptr);
	private:
		Count<class VulkanShader> m_Shader = nullptr;
		VkPipelineViewportStateCreateInfo m_ViewportInfo;
		VkPipeline m_GraphicsPipeline;
		void RecreatePipeLine();
	};
}