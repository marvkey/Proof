#include "Proofprch.h"
#include "VulkanGraphicsPipeline.h"
#include "VulkanShader.h"
#include "Proof/Renderer/RendererBase.h"
#include "VulkanGraphicsContext.h"
#include "VulkanShader.h"
#include "VulkanBuffer.h"
#include "Proof/Renderer/Shader.h"
#include "VulkanVertexArray.h"
#include "VulkanPipeLineLayout.h"
#include "VulkanVertexArray.h"
#include "VulkanRenderPass.h"
namespace Proof
{
	VulkanGraphicsPipeline::~VulkanGraphicsPipeline() {
		vkDestroyPipeline(RendererBase::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), m_GraphicsPipeline, nullptr);
	}
	VulkanGraphicsPipeline::VulkanGraphicsPipeline(Count<class Shader> shader, Count<class RenderPass> renderPass, Count<class PipeLineLayout> pipeline, Count<VertexArray > vertexArray ) {
		PipelineConfigInfo pipelineConfig{};
		VulkanGraphicsPipeline::DefaultPipelineConfigInfo(pipelineConfig, CurrentWindow::GetWindow().GetWidth(), CurrentWindow::GetWindow().GetHeight());
		pipelineConfig.RenderPass = renderPass->As<VulkanRenderPass>()->GetRenderPass();
		pipelineConfig.PipelineLayout = pipeline->As<VulkanPipeLineLayout>()->GetPipeLineLayout();
		auto vulkanShader = shader->As<VulkanShader>();
		PF_CORE_ASSERT(pipelineConfig.PipelineLayout, "Cannot create Graphics Pipeline:: no pipelineLayout provided in configInfo");
		PF_CORE_ASSERT(pipelineConfig.RenderPass, "Cannot create Graphics Pipeline:: no renderpass provided in configInfo");
		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		// we are hardcoding values into the vertex data
		VulkanVertexInput vertexInput;
		if (vertexArray != nullptr) {
			vertexInput = vertexArray->As<VulkanVertexArray>()->GetData();
			vertexInputInfo.vertexAttributeDescriptionCount = vertexInput.GetAttributes().size();
			vertexInputInfo.vertexBindingDescriptionCount = vertexInput.GetDescriptions().size();

			vertexInputInfo.pVertexAttributeDescriptions = vertexInput.GetAttributes().data();
			vertexInputInfo.pVertexBindingDescriptions = vertexInput.GetDescriptions().data();
		}
		else {
			vertexInputInfo.vertexAttributeDescriptionCount = 0;
			vertexInputInfo.vertexBindingDescriptionCount = 0;

			vertexInputInfo.pVertexAttributeDescriptions = nullptr;
			vertexInputInfo.pVertexBindingDescriptions = nullptr;
		}

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		// teh stages of the pipeline we are going to use
		// we are only using the fragment and vertex stages
		if (shader != nullptr) {
			//pipelineInfo.stageCount = 0;
			//pipelineInfo.pStages = nullptr;
			pipelineInfo.stageCount = vulkanShader->m_ShaderStages.size();
			pipelineInfo.pStages = vulkanShader->m_ShaderStages.data();
		}
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &pipelineConfig.InputAssemblyInfo;
		pipelineInfo.pViewportState = &pipelineConfig.ViewportState;
		pipelineInfo.pRasterizationState = &pipelineConfig.RasterizationInfo;
		pipelineInfo.pMultisampleState = &pipelineConfig.MultisampleInfo;
		pipelineInfo.pColorBlendState = &pipelineConfig.ColorBlendInfo;
		pipelineInfo.pDepthStencilState = &pipelineConfig.DepthStencilInfo;
		// some functionality to configure the viewport or line width without restarting the whole pipeline
		pipelineInfo.pDynamicState = &pipelineConfig.DynamicSate;

		//pipelineInfo.sha
		pipelineInfo.layout = pipelineConfig.PipelineLayout;
		pipelineInfo.renderPass = pipelineConfig.RenderPass;
		pipelineInfo.subpass = pipelineConfig.Subpass;

		// can be used for performance increase
		//	pipelineInfo.basePipelineIndex = -1;
		//	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

		if (vkCreateGraphicsPipelines((RendererBase::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice()), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_GraphicsPipeline) != VK_SUCCESS)
			PF_CORE_ASSERT(false, "Failed to Create Graphics Pipeline");
	}

	void VulkanGraphicsPipeline::DefaultPipelineConfigInfo(PipelineConfigInfo& configInfo, uint32_t width, uint32_t height) {

		configInfo.InputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		configInfo.InputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		configInfo.InputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

		configInfo.ViewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		configInfo.ViewportState.viewportCount = 1;
		configInfo.ViewportState.scissorCount = 1;

		configInfo.RasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		configInfo.RasterizationInfo.depthClampEnable = VK_FALSE;
		configInfo.RasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
		configInfo.RasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
		configInfo.RasterizationInfo.lineWidth = 1.0f;
		configInfo.RasterizationInfo.cullMode = VK_CULL_MODE_BACK_BIT;
		configInfo.RasterizationInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		configInfo.RasterizationInfo.depthBiasEnable = VK_FALSE;

		configInfo.MultisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		configInfo.MultisampleInfo.sampleShadingEnable = VK_FALSE;
		configInfo.MultisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

		configInfo.DepthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		configInfo.DepthStencilInfo.depthTestEnable = VK_TRUE;
		configInfo.DepthStencilInfo.depthWriteEnable = VK_TRUE;
		configInfo.DepthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
		configInfo.DepthStencilInfo.depthBoundsTestEnable = VK_FALSE;
		configInfo.DepthStencilInfo.stencilTestEnable = VK_FALSE;

		configInfo.ColorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		configInfo.ColorBlendAttachment.blendEnable = VK_FALSE;

		configInfo.ColorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		configInfo.ColorBlendInfo.logicOpEnable = VK_FALSE;
		configInfo.ColorBlendInfo.logicOp = VK_LOGIC_OP_COPY;
		configInfo.ColorBlendInfo.attachmentCount = 1;
		configInfo.ColorBlendInfo.pAttachments = &configInfo.ColorBlendAttachment;
		configInfo.ColorBlendInfo.blendConstants[0] = 0.0f;
		configInfo.ColorBlendInfo.blendConstants[1] = 0.0f;
		configInfo.ColorBlendInfo.blendConstants[2] = 0.0f;
		configInfo.ColorBlendInfo.blendConstants[3] = 0.0f;

		std::vector<VkDynamicState> dynamicStates = {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};
		configInfo.DynamicSate.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		configInfo.DynamicSate.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
		configInfo.DynamicSate.pDynamicStates = dynamicStates.data();

	}



}