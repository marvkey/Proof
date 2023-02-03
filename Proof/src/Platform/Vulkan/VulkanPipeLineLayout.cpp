#include "Proofprch.h"
#include "VulkanPipeLineLayout.h"
#include "VulkanGraphicsContext.h"
#include "Proof/Renderer/RendererBase.h"
#include "VulkanPushConstant.h"
#include "VulkanDescriptorSet.h"

namespace Proof
{
	VulkanPipeLineLayout::VulkanPipeLineLayout(Count<PushConstant> pushConstant, const std::vector<Count<DescriptorSet>>& descriptors ) {
		auto graphicsContext = RendererBase::GetGraphicsContext()->As<VulkanGraphicsContext>();
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		std::vector< VkDescriptorSetLayout> descriptorLayout;
		if (descriptors.empty() == false) {
			for (const auto& desc : descriptors) {
				descriptorLayout.emplace_back(desc->As<VulkanDescriptorSet>()->m_DescriptorSetLayout);
			}
			// pipeline layout is used to pass data to pipeline other than vertex and fragment data
			// this includes texture and uniform buffer objects
			pipelineLayoutInfo.setLayoutCount = descriptors.size(); // emty layout
			pipelineLayoutInfo.pSetLayouts = descriptorLayout.data();
		}
		else {
			// pipeline layout is used to pass data to pipeline other than vertex and fragment data
			// this includes texture and uniform buffer objects
			pipelineLayoutInfo.setLayoutCount = 0; // emty layout
			pipelineLayoutInfo.pSetLayouts = nullptr;
		}
		VkPushConstantRange range;
		if (pushConstant != nullptr) {
			range = pushConstant->As<VulkanPushConstant>()->GetRange();
			pipelineLayoutInfo.pushConstantRangeCount = 1;
			pipelineLayoutInfo.pPushConstantRanges = &range;
		}
		else {
			// very efficiently send small data to shader proggramm
			pipelineLayoutInfo.pushConstantRangeCount = 0;
			pipelineLayoutInfo.pPushConstantRanges = nullptr;
		}
		if (vkCreatePipelineLayout(graphicsContext->GetDevice(), &pipelineLayoutInfo, nullptr, &m_PipeLineLayout) != VK_SUCCESS) {
			PF_CORE_ASSERT(false, "Failed to create PipeLine Layout");
		}
	}

	VulkanPipeLineLayout::VulkanPipeLineLayout(VkPipelineLayoutCreateInfo& info) {
		auto graphicsContext = RendererBase::GetGraphicsContext()->As<VulkanGraphicsContext>();
		if (vkCreatePipelineLayout(graphicsContext->GetDevice(), &info, nullptr, &m_PipeLineLayout) != VK_SUCCESS) {
			PF_CORE_ASSERT(false, "Failed to create PipeLine Layout");
		}

	}
	VulkanPipeLineLayout::~VulkanPipeLineLayout()
	{
		Renderer::SubmitDatafree([pipline = m_PipeLineLayout]() {
			vkDestroyPipelineLayout(RendererBase::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), pipline, nullptr);
		});
	}
}
