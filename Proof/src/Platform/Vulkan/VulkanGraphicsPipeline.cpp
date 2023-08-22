#include "Proofprch.h"
#include "VulkanGraphicsPipeline.h"
#include "VulkanShader.h"
#include "VulkanGraphicsContext.h"
#include "VulkanBuffer.h"
#include "VulkanRenderer/VulkanRenderer.h"
#include "VulkanVertexArray.h"
#include "VulkanDescriptorManager.h"
#include "VulkanRenderPass.h"
#include "VulkanCommandBuffer.h"
namespace Proof
{
	std::array<VkDynamicState,2> dynamicState{ VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
	namespace Utils {
		static VkPrimitiveTopology ProofTopologyToVulkanTopology(DrawType type) 
		{
			switch (type)
			{
				case Proof::DrawType::Triangle:
					return VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
					break;
				case Proof::DrawType::TriangleStrip:
					return VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
					break;
				case Proof::DrawType::TriangleFan:
					return VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
					break;
				case Proof::DrawType::Line:
					return VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
					break;
				case Proof::DrawType::LineStrip:
					return VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
					break;
				case Proof::DrawType::Point:
					return VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
					break;
				default:
					break;
			}
			PF_CORE_ASSERT(false, "Operand not supported");
		}
		VkCompareOp ProofCompareOpToVulkanCompareOp(DepthCompareOperator compare) {
			switch (compare)
			{
				case Proof::DepthCompareOperator::Never:
					return VK_COMPARE_OP_NEVER;
					break;
				case Proof::DepthCompareOperator::Less:
					return VK_COMPARE_OP_LESS;
					break;
				case Proof::DepthCompareOperator::Equal:
					return VK_COMPARE_OP_EQUAL;
					break;
				case Proof::DepthCompareOperator::LessOrEqual:
					return VK_COMPARE_OP_LESS_OR_EQUAL;
					break;
				case Proof::DepthCompareOperator::Greater:
					return VK_COMPARE_OP_GREATER;
					break;
				case Proof::DepthCompareOperator::NotEqual:
					return VK_COMPARE_OP_NOT_EQUAL;
					break;
				case Proof::DepthCompareOperator::GreaterOrEqual:
					return VK_COMPARE_OP_GREATER_OR_EQUAL;
					break;
				case Proof::DepthCompareOperator::Always:
					return VK_COMPARE_OP_ALWAYS;
					break;
				default:
					break;
			}
			PF_CORE_ASSERT(false, "Operand not supported");
		}

		static VkCullModeFlagBits ProofFormatToVulkanFormat(CullMode mode) {
			switch (mode)
			{
				case Proof::CullMode::None:
					return VK_CULL_MODE_NONE;
					break;
				case Proof::CullMode::Front:
					return VK_CULL_MODE_FRONT_BIT;
					break;
				case Proof::CullMode::Back:
					return VK_CULL_MODE_BACK_BIT;
					break; 
				case Proof::CullMode::FrontAndBck:
					return VK_CULL_MODE_FRONT_AND_BACK;
					break;
				default:
					break;
			}
			PF_CORE_ASSERT(false, "Operand not supported ");
		}

		static VkFrontFace ProofFormatToVulkanFormat(FrontFace mode) {
			switch (mode)
			{
				case Proof::FrontFace::ClockWise:
					return VkFrontFace::VK_FRONT_FACE_CLOCKWISE;
					break;
				case Proof::FrontFace::CounterClockWise:
					return VkFrontFace::VK_FRONT_FACE_COUNTER_CLOCKWISE;
					break;
				default:
					break;
			}
			PF_CORE_ASSERT(false, "Operand not supported ");

		}
	}
	
	

	VulkanGraphicsPipeline::VulkanGraphicsPipeline(const GraphicsPipelineConfig& config) :
		m_Config(config)
	{
		Build();
		WeakCount<VulkanGraphicsPipeline> instanceWeakCount = this;
		m_ShaderReloadCallbackIndex = GetShader()->AddShaderReloadCallback([instanceWeakCount]
		{
			if (!instanceWeakCount.IsValid())
				return;
			auto graphicsPipeline = instanceWeakCount.Lock();

			graphicsPipeline->Release();
			graphicsPipeline->Build();
		});
	}
	
	VulkanGraphicsPipeline::~VulkanGraphicsPipeline()
	{
		Release();
		GetShader()->RemoveShaderReloadCallback(m_ShaderReloadCallbackIndex);
	}
	void VulkanGraphicsPipeline::Build()
	{
		PF_CORE_ASSERT(m_Config.TargetBuffer);
		// need for combaitible render pass layout
		RenderPassConfig redfdfdasfaConfig("Graphics pipline compatibility renderPass", m_Config.TargetBuffer);
		if (m_Config.Shader != NULL && m_Config.Shader->GetName() == "EnvironmentIrradianceNonCompute")
			redfdfdasfaConfig.MultiView = true;

		CreatePipelineLayout();
		PipelineConfigInfo pipelineConfig;
		DefaultPipelineConfigInfo(pipelineConfig, m_Config);
		//pipelineConfig.RenderPass = config.RenderPass.As<VulkanRenderPass>()->GetRenderPass();;
		pipelineConfig.RenderPass = Count<VulkanRenderPass>::Create(redfdfdasfaConfig)->GetRenderPass();;
		pipelineConfig.PipelineLayout = m_PipeLineLayout;
		auto vulkanShader = m_Config.Shader.As<VulkanShader>();
		PF_CORE_ASSERT(pipelineConfig.PipelineLayout, "Cannot create Graphics Pipeline:: no pipelineLayout provided in configInfo");
		//PF_CORE_ASSERT(pipelineConfig.RenderPass, "Cannot create Graphics Pipeline:: no renderpass provided in configInfo");
		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		// we are hardcoding values into the vertex data
		VulkanVertexInput vertexInput;
		if (m_Config.VertexArray != nullptr)
		{
			vertexInput = m_Config.VertexArray.As<VulkanVertexArray>()->GetData();
			vertexInputInfo.vertexAttributeDescriptionCount = vertexInput.GetAttributes().size();
			vertexInputInfo.vertexBindingDescriptionCount = vertexInput.GetDescriptions().size();

			vertexInputInfo.pVertexAttributeDescriptions = vertexInput.GetAttributes().data();
			vertexInputInfo.pVertexBindingDescriptions = vertexInput.GetDescriptions().data();
		}
		else
		{
			vertexInputInfo.vertexAttributeDescriptionCount = 0;
			vertexInputInfo.vertexBindingDescriptionCount = 0;

			vertexInputInfo.pVertexAttributeDescriptions = nullptr;
			vertexInputInfo.pVertexBindingDescriptions = nullptr;
		}
		std::vector< VkPipelineShaderStageCreateInfo> stages;
		for (const auto& [stage, info] : vulkanShader->GetShaderStages())
		{
			stages.push_back(info);
		}
		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		// teh stages of the pipeline we are going to use
		// we are only using the fragment and vertex stages
		if (vulkanShader != nullptr)
		{
			pipelineInfo.stageCount = vulkanShader->GetStageCount();
			pipelineInfo.pStages = stages.data();
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
		pipelineInfo.basePipelineIndex = -1;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

		if (vkCreateGraphicsPipelines((VulkanRenderer::GetGraphicsContext()->GetDevice()), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_GraphicsPipeline) != VK_SUCCESS)
			PF_CORE_ASSERT(false, "Failed to Create Graphics Pipeline");

		VulkanDescriptorManagerConfig descriptorConfig;
		descriptorConfig.DebugName = m_Config.DebugName + " Pipeline DescritporManager";
		descriptorConfig.Shader = m_Config.Shader.As<VulkanShader>();
	}
	void VulkanGraphicsPipeline::Release()
	{
		Renderer::SubmitDatafree([pipline = m_GraphicsPipeline, piplinelayout = m_PipeLineLayout]() {
			vkDestroyPipeline(VulkanRenderer::GetGraphicsContext()->GetDevice(), pipline, nullptr);
			vkDestroyPipelineLayout(VulkanRenderer::GetGraphicsContext()->GetDevice(), piplinelayout, nullptr);
		});
		m_GraphicsPipeline = nullptr;
		m_PipeLineLayout = nullptr;
	}

	void VulkanGraphicsPipeline::Bind(Count<class RenderCommandBuffer> commandBuffer)
	{
		vkCmdBindPipeline(commandBuffer.As<VulkanRenderCommandBuffer>()->GetCommandBuffer(Renderer::GetCurrentFrame().FrameinFlight),
			VK_PIPELINE_BIND_POINT_GRAPHICS, m_GraphicsPipeline);

	}
	
	void VulkanGraphicsPipeline::CreatePipelineLayout()
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

	#define IM_ARRAYSIZEERE(_ARR)          ((int)(sizeof(_ARR) / sizeof(*(_ARR))))     // Size of a static C-style array. Don't use on pointers!
	void VulkanGraphicsPipeline::DefaultPipelineConfigInfo(PipelineConfigInfo& configInfo, const GraphicsPipelineConfig& graphicsConfig)
	{

		configInfo.InputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		configInfo.InputAssemblyInfo.topology = Utils::ProofTopologyToVulkanTopology(graphicsConfig.DrawMode);
		configInfo.InputAssemblyInfo.primitiveRestartEnable = VK_FALSE;
		configInfo.InputAssemblyInfo.pNext = nullptr;
		configInfo.InputAssemblyInfo.flags = 0;


		configInfo.ViewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		configInfo.ViewportState.viewportCount = 1;
		configInfo.ViewportState.pNext = nullptr;
		configInfo.ViewportState.scissorCount = 1;
		configInfo.ViewportState.flags = 0;

		//configInfo.ViewportState.pScissors = &Scissor;//leave

		configInfo.RasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		configInfo.RasterizationInfo.depthClampEnable = VK_FALSE;
		configInfo.RasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
		configInfo.RasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
		// we are setting it 1
		// so in teh renderpeass when we set the width we can do a check to see if not 1 then 
		// we can change teh width
		// because most of the times it will be 1.f
		configInfo.RasterizationInfo.lineWidth = graphicsConfig.LineWidth;
		configInfo.RasterizationInfo.cullMode = Utils::ProofFormatToVulkanFormat(graphicsConfig.CullMode);
		configInfo.RasterizationInfo.frontFace = Utils::ProofFormatToVulkanFormat(graphicsConfig.FrontFace);
		configInfo.RasterizationInfo.depthBiasEnable = VK_FALSE;
		configInfo.RasterizationInfo.depthBiasConstantFactor = 0.0f;  // Optional
		configInfo.RasterizationInfo.depthBiasClamp = 0.0f;           // Optional
		configInfo.RasterizationInfo.depthBiasSlopeFactor = 0.0f;     // Optional
		configInfo.RasterizationInfo.pNext = nullptr;
		configInfo.RasterizationInfo.flags = 0;

		configInfo.MultisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		configInfo.MultisampleInfo.sampleShadingEnable = VK_FALSE;
		configInfo.MultisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		configInfo.MultisampleInfo.minSampleShading = 1.0f;           // Optional
		configInfo.MultisampleInfo.pSampleMask = nullptr;             // Optional
		configInfo.MultisampleInfo.alphaToCoverageEnable = VK_FALSE;  // Optional
		configInfo.MultisampleInfo.alphaToOneEnable = VK_FALSE;       // Optional
		configInfo.MultisampleInfo.pSampleMask = nullptr;     // Optional
		configInfo.MultisampleInfo.flags = 0;
		configInfo.MultisampleInfo.pNext = nullptr;

		configInfo.ColorBlendAttachment.colorWriteMask =
			VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
			VK_COLOR_COMPONENT_A_BIT;
		if (graphicsConfig.Blend)
		{
			configInfo.ColorBlendAttachment.blendEnable = VK_TRUE;
			configInfo.ColorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;   // Optional
			configInfo.ColorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA; //from chatgpt

		}
		else
		{
			configInfo.ColorBlendAttachment.blendEnable = VK_FALSE;
			configInfo.ColorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
			configInfo.ColorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
		}
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
		configInfo.ColorBlendInfo.pNext = nullptr;
		configInfo.ColorBlendInfo.flags = 0;

		configInfo.DepthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		configInfo.DepthStencilInfo.depthTestEnable = graphicsConfig.DepthTest;
		configInfo.DepthStencilInfo.depthWriteEnable = graphicsConfig.WriteDepth;
		configInfo.DepthStencilInfo.depthCompareOp = Utils::ProofCompareOpToVulkanCompareOp(graphicsConfig.DepthCompareOperator);
		configInfo.DepthStencilInfo.depthBoundsTestEnable = VK_FALSE;
		configInfo.DepthStencilInfo.minDepthBounds = 0.0f;  // Optional
		configInfo.DepthStencilInfo.maxDepthBounds = 1.0f;  // Optional
		configInfo.DepthStencilInfo.stencilTestEnable = VK_FALSE;
		configInfo.DepthStencilInfo.front = {};  // Optional
		configInfo.DepthStencilInfo.back = {};   // Optional
		configInfo.DepthStencilInfo.pNext = nullptr;
		configInfo.DepthStencilInfo.flags = 0;

		// this is gettting deleted from memrory thats the problem
		//VkDynamicState dynamic_states[2] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

		configInfo.DynamicSate.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		configInfo.DynamicSate.dynamicStateCount = dynamicState.size();
		configInfo.DynamicSate.pNext = nullptr;
		configInfo.DynamicSate.flags = 0;
		configInfo.DynamicSate.pDynamicStates = dynamicState.data();
	}
}