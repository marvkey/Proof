#include "Proofprch.h"
#include "VulkanPipeLineLayout.h"
#include "VulkanGraphicsContext.h"
#include "Proof/Renderer/Renderer.h"
#include "VulkanPushConstant.h"
namespace Proof
{
	VkPipelineLayout VulkanPipeLineLayout::s_DefaultPipelineLayout;
	VulkanPipeLineLayout::VulkanPipeLineLayout(Count<VulkanPushConstant> pushConstant) {
		auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		// pipeline layout is used to pass data to pipeline other than vertex and fragment data
		// this includes texture and uniform buffer objects
		pipelineLayoutInfo.setLayoutCount = 0; // emty layout
		pipelineLayoutInfo.pSetLayouts = nullptr;
		// very efficiently send small data to shader proggramm
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstant->m_PushConstant;
		if (vkCreatePipelineLayout(graphicsContext->GetDevice(), &pipelineLayoutInfo, nullptr, &m_PipeLineLayout) != VK_SUCCESS) {
			PF_CORE_ASSERT(false, "Failed to create PipeLine Layout");
		}
	}
	void VulkanPipeLineLayout::SetUpDefaultPipeLine() {
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		// pipeline layout is used to pass data to pipeline other than vertex and fragment data
		// this includes texture and uniform buffer objects
		pipelineLayoutInfo.setLayoutCount = 0; // emty layout
		pipelineLayoutInfo.pSetLayouts = nullptr;
		// very efficiently send small data to shader proggramm
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;
		if (vkCreatePipelineLayout(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), &pipelineLayoutInfo, nullptr, &s_DefaultPipelineLayout) != VK_SUCCESS)
			PF_ASSERT(false, "failed to create pipeline layout");
	}
}
