#include "Proofprch.h"
#include "VulkanGraphicsPipeline.h"
#include "VulkanShader.h"
#include "Proof/Renderer/Renderer.h"
#include "VulkanGraphicsContext.h"
#include "VulkanShader.h"

namespace Proof
{
	VulkanPipeLineLayout::VulkanPipeLineLayout(uint32_t pushConstantRngeCount, VkPushConstantRange* pushConstantRange, uint32_t layoutCount, VkDescriptorSetLayout layoutDescriptor, VkStructureType structureType) {
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		// pipeline layout is used to pass data to pipeline other than vertex and fragment data
		// this includes texture and uniform buffer objects
		pipelineLayoutInfo.setLayoutCount = layoutCount; // emty layout
		pipelineLayoutInfo.pSetLayouts = &layoutDescriptor;
		// very efficiently send small data to shader proggramm
		pipelineLayoutInfo.pushConstantRangeCount = pushConstantRngeCount;
		pipelineLayoutInfo.pPushConstantRanges = pushConstantRange;
		if (vkCreatePipelineLayout(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), &pipelineLayoutInfo, nullptr, &PipelineLayout) != VK_SUCCESS)
			PF_ASSERT(false, "failed to create pipeline layout");
	}

	VulkanGraphicsPipeline::~VulkanGraphicsPipeline() {
		vkDestroyPipeline(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), m_GraphicsPipeline, nullptr);
	}
	VulkanGraphicsPipeline::VulkanGraphicsPipeline(Count<Shader> shader, const PipelineConfigInfo& info, uint32_t attributeSize, uint32_t bindingSize, const VkVertexInputAttributeDescription* attributeData, const VkVertexInputBindingDescription* bindingData) {
		m_Shader = std::dynamic_pointer_cast<VulkanShader>(shader);
		PF_CORE_ASSERT(info.PipelineLayout ,"Cannot create Graphics Pipeline:: no pipelineLayout provided in configInfo");
		PF_CORE_ASSERT(info.RenderPass,"Cannot create Graphics Pipeline:: no renderpass provided in configInfo");
		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		// we are hardcoding values into the vertex data
		vertexInputInfo.vertexAttributeDescriptionCount = attributeSize;
		vertexInputInfo.vertexBindingDescriptionCount = bindingSize;

		vertexInputInfo.pVertexAttributeDescriptions = attributeData;
		vertexInputInfo.pVertexBindingDescriptions = bindingData;
		
		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		// teh stages of the pipeline we are going to use
		// we are only using the fragment and vertex stages
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = m_Shader->m_ShaderStages;;
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &info.InputAssemblyInfo;
		pipelineInfo.pViewportState = &info.ViewportInfo;
		pipelineInfo.pRasterizationState = &info.RasterizationInfo;
		pipelineInfo.pMultisampleState = &info.MultisampleInfo;
		pipelineInfo.pColorBlendState = &info.ColorBlendInfo;
		pipelineInfo.pDepthStencilState = &info.DepthStencilInfo;
		// some functionality to configure the viewport or line width without restarting the whole pipeline
		pipelineInfo.pDynamicState = &info.DynamicStateInfo;


		pipelineInfo.layout = info.PipelineLayout;
		pipelineInfo.renderPass = info.RenderPass;
		pipelineInfo.subpass = info.Subpass;

		// can be used for performance increase
		pipelineInfo.basePipelineIndex = -1;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

		if (vkCreateGraphicsPipelines((Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice()), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr,&m_GraphicsPipeline) != VK_SUCCESS)
			PF_CORE_ASSERT(false, "Failed to Create Graphics Pipeline");
	}
	void VulkanGraphicsPipeline::DefaultPipelineConfigInfo(PipelineConfigInfo& configInfo) {

		configInfo.InputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		configInfo.InputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		configInfo.InputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

		configInfo.ViewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		configInfo.ViewportInfo.viewportCount = 1;
		configInfo.ViewportInfo.pViewports = nullptr;
		configInfo.ViewportInfo.scissorCount = 1;
		configInfo.ViewportInfo.pScissors = nullptr;

		configInfo.RasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		configInfo.RasterizationInfo.depthClampEnable = VK_FALSE;
		configInfo.RasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
		configInfo.RasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
		configInfo.RasterizationInfo.lineWidth = 1.0f;
		configInfo.RasterizationInfo.cullMode = VK_CULL_MODE_NONE;
		configInfo.RasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
		configInfo.RasterizationInfo.depthBiasEnable = VK_FALSE;
		configInfo.RasterizationInfo.depthBiasConstantFactor = 0.0f;  // Optional
		configInfo.RasterizationInfo.depthBiasClamp = 0.0f;           // Optional
		configInfo.RasterizationInfo.depthBiasSlopeFactor = 0.0f;     // Optional

		configInfo.MultisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		configInfo.MultisampleInfo.sampleShadingEnable = VK_FALSE;
		configInfo.MultisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		configInfo.MultisampleInfo.minSampleShading = 1.0f;           // Optional
		configInfo.MultisampleInfo.pSampleMask = nullptr;             // Optional
		configInfo.MultisampleInfo.alphaToCoverageEnable = VK_FALSE;  // Optional
		configInfo.MultisampleInfo.alphaToOneEnable = VK_FALSE;       // Optional

		configInfo.ColorBlendAttachment.colorWriteMask =
			VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
			VK_COLOR_COMPONENT_A_BIT;
		configInfo.ColorBlendAttachment.blendEnable = VK_FALSE;
		configInfo.ColorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
		configInfo.ColorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
		configInfo.ColorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;              // Optional
		configInfo.ColorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
		configInfo.ColorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
		configInfo.ColorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;              // Optional

		configInfo.ColorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		configInfo.ColorBlendInfo.logicOpEnable = VK_FALSE;
		configInfo.ColorBlendInfo.logicOp = VK_LOGIC_OP_COPY;  // Optional
		configInfo.ColorBlendInfo.attachmentCount = 1;
		configInfo.ColorBlendInfo.pAttachments = &configInfo.ColorBlendAttachment;
		configInfo.ColorBlendInfo.blendConstants[0] = 0.0f;  // Optional
		configInfo.ColorBlendInfo.blendConstants[1] = 0.0f;  // Optional
		configInfo.ColorBlendInfo.blendConstants[2] = 0.0f;  // Optional
		configInfo.ColorBlendInfo.blendConstants[3] = 0.0f;  // Optional

		configInfo.DepthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		configInfo.DepthStencilInfo.depthTestEnable = VK_TRUE;
		configInfo.DepthStencilInfo.depthWriteEnable = VK_TRUE;
		configInfo.DepthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
		configInfo.DepthStencilInfo.depthBoundsTestEnable = VK_FALSE;
		configInfo.DepthStencilInfo.minDepthBounds = 0.0f;  // Optional
		configInfo.DepthStencilInfo.maxDepthBounds = 1.0f;  // Optional
		configInfo.DepthStencilInfo.stencilTestEnable = VK_FALSE;
		configInfo.DepthStencilInfo.front = {};  // Optional
		configInfo.DepthStencilInfo.back = {};   // Optional


		// DYNAMIC VIEWPORT
		// expect dynamic sciscor and dynamic viewport to be put in later

		configInfo.DynamicStateEnables = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
		configInfo.DynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		configInfo.DynamicStateInfo.pDynamicStates = configInfo.DynamicStateEnables.data();
		configInfo.DynamicStateInfo.dynamicStateCount =
			static_cast<uint32_t>(configInfo.DynamicStateEnables.size());
		configInfo.DynamicStateInfo.flags = 0;
	}

	void VulkanGraphicsPipeline::Recreate(Count<Shader> shader, const PipelineConfigInfo& info, uint32_t attributeSize, uint32_t bindingSize , const VkVertexInputAttributeDescription* attributeData , const VkVertexInputBindingDescription* bindingData ) {
		vkDestroyPipeline(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), m_GraphicsPipeline, nullptr);
		*this = VulkanGraphicsPipeline(shader, info, attributeSize, bindingSize, attributeData, bindingData);
	}

	

}
