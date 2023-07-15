#include "Proofprch.h"
#include "VulkanComputePipeline.h"
#include "VulkanShader.h"
#include "VulkanRenderer/VulkanRenderer.h"
#include "VulkanGraphicsContext.h"

#

namespace Proof {
	VulkanComputePipeline::VulkanComputePipeline(const ComputePipelineConfig& config):m_Config(config)
	{
		Build();
	}
	VulkanComputePipeline::~VulkanComputePipeline()
	{
	}
	
	void VulkanComputePipeline::Build()
	{
		auto device = VulkanRenderer::GetGraphicsContext()->GetDevice();

		BuildPipeline();
		auto vulkanShader = m_Config.Shader.As<VulkanShader>();

		VkComputePipelineCreateInfo pipelineCreateInfo = {};
		pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
		pipelineCreateInfo.stage = vulkanShader->GetShaderStages().at(ShaderStage::Compute);
		pipelineCreateInfo.layout = m_PipeLineLayout;
		pipelineCreateInfo.pNext = nullptr;

		VK_CHECK_RESULT(vkCreateComputePipelines(device, nullptr, 1, & pipelineCreateInfo, nullptr, &m_ComputePipeline));
	}
	void VulkanComputePipeline::BuildPipeline()
	{

		auto device = VulkanRenderer::GetGraphicsContext()->GetDevice();

		auto shader = m_Config.Shader.As<VulkanShader>();
		std::vector< VkDescriptorSetLayout> descriptorLayout;
		std::vector< VkPushConstantRange> pushConstantsRange;

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.pNext = nullptr;

		for (auto& [set, resource] : shader->GetDescriptorResource())
		{
			descriptorLayout.emplace_back(resource.Layout);
		}
		if (!descriptorLayout.empty())
		{
			pipelineLayoutInfo.pSetLayouts = descriptorLayout.data();
			pipelineLayoutInfo.setLayoutCount = descriptorLayout.size();
		}
		else
		{
			pipelineLayoutInfo.pSetLayouts = nullptr;
			pipelineLayoutInfo.setLayoutCount = 0;
		}
		for (auto& [name, range] : shader->GetPushConstants())
			pushConstantsRange.emplace_back(range);
		if (!pushConstantsRange.empty())
		{
			pipelineLayoutInfo.pushConstantRangeCount = pushConstantsRange.size();
			pipelineLayoutInfo.pPushConstantRanges = pushConstantsRange.data();
		}
		else
		{
			pipelineLayoutInfo.pushConstantRangeCount = 0;
			pipelineLayoutInfo.pPushConstantRanges = nullptr;
		}
		VK_CHECK_RESULT(vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &m_PipeLineLayout));
	}
	void VulkanComputePipeline::Release()
	{
		Renderer::SubmitDatafree([pipline = m_ComputePipeline, piplinelayout = m_PipeLineLayout]() {
			vkDestroyPipeline(VulkanRenderer::GetGraphicsContext()->GetDevice(), pipline, nullptr);
			vkDestroyPipelineLayout(VulkanRenderer::GetGraphicsContext()->GetDevice(), piplinelayout, nullptr);
		});
		m_ComputePipeline = nullptr;
		m_PipeLineLayout = nullptr;
	}
}
