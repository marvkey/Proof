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
#include "VulkanUtils/VulkanConvert.h"
#include "VulkanFrameBuffer.h"
namespace Proof
{
	
	
	

	VulkanGraphicsPipeline::VulkanGraphicsPipeline(const GraphicsPipelineConfiguration& config) :
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
		// need for combaitible render pass layout
		//RenderPassConfig redfdfdasfaConfig("Graphics pipline compatibility renderPass", m_Config.TargetBuffer);
		//if (m_Config.Shader != NULL && m_Config.Shader->GetName() == "EnvironmentIrradianceNonCompute")
		//	redfdfdasfaConfig.MultiView = true;

		VkDevice device = VulkanRenderer::GetGraphicsContext()->GetDevice();
		auto graphicsContext = VulkanRenderer::GetGraphicsContext();

		// Create compatibility renderPass
		bool depthImage = false;

		#if 1
		{
			uint32_t attachmentIndex = 0;
			std::vector<VkAttachmentReference> colorAttachmentReferences;
			std::vector<VkAttachmentDescription> attachmentDescriptions;
			VkAttachmentReference depthAttachmentReference;
			for (auto attachmentSpec : m_Config.Attachments.Attachments)
			{
				if (Utils::IsDepthFormat(attachmentSpec.Format))
				{

					PF_CORE_ASSERT(depthImage == false, "Cannot have more than one depth image");
					VkAttachmentDescription& attachmentDescription = attachmentDescriptions.emplace_back();
					attachmentDescription.flags = 0;
					attachmentDescription.format = Utils::ProofFormatToVulkanFormat(attachmentSpec.Format);
					attachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
					attachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
					attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
					attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
					attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
					attachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				   //dont usupport seperate stencil and depth yet
				  //  if (Utils::ContainStencilFormat(attachmentSpec.Format) )
					//{
					attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
					attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
					depthAttachmentReference = { attachmentIndex, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };
			   // }
			   // else
			   // {
			   //     attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL; 
			   //     attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL; 
			   //     depthAttachmentReference = { attachmentIndex, VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL };
			   // }
					depthImage = true;
				}
				else
				{

					VkAttachmentDescription& attachmentDescription = attachmentDescriptions.emplace_back();
					attachmentDescription.flags = 0;
					attachmentDescription.format = Utils::ProofFormatToVulkanFormat(attachmentSpec.Format);
					attachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
					attachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
					attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
					attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
					attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
					attachmentDescription.initialLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
					attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

					colorAttachmentReferences.emplace_back(VkAttachmentReference{ attachmentIndex, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL });
				}
				attachmentIndex++;
			}


			VkSubpassDescription subpassDescription = {};
			subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			subpassDescription.colorAttachmentCount = uint32_t(colorAttachmentReferences.size());
			subpassDescription.pColorAttachments = colorAttachmentReferences.data();
			if (depthImage)
				subpassDescription.pDepthStencilAttachment = &depthAttachmentReference;


			std::vector<VkSubpassDependency> dependencies;

			if (colorAttachmentReferences.size())
			{
				{
					VkSubpassDependency& depedency = dependencies.emplace_back();
					depedency.srcSubpass = VK_SUBPASS_EXTERNAL;
					depedency.dstSubpass = 0;
					depedency.srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
					depedency.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
					depedency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
					depedency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
					depedency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
				}
				{
					VkSubpassDependency& depedency = dependencies.emplace_back();
					depedency.srcSubpass = 0;
					depedency.dstSubpass = VK_SUBPASS_EXTERNAL;
					depedency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
					depedency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
					depedency.dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
					depedency.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
					depedency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
				}
			}

			if (depthImage)
			{
				{
					VkSubpassDependency& depedency = dependencies.emplace_back();
					depedency.srcSubpass = VK_SUBPASS_EXTERNAL;
					depedency.dstSubpass = 0;
					depedency.srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
					depedency.dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
					depedency.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
					depedency.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
					depedency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
				}

				{
					VkSubpassDependency& depedency = dependencies.emplace_back();
					depedency.srcSubpass = 0;
					depedency.dstSubpass = VK_SUBPASS_EXTERNAL;
					depedency.srcStageMask = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
					depedency.dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
					depedency.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
					depedency.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
					depedency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
				}
			}

			VkRenderPassCreateInfo renderPassInfo = {};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
			renderPassInfo.attachmentCount = static_cast<uint32_t>(attachmentDescriptions.size());
			renderPassInfo.pAttachments = attachmentDescriptions.data();
			renderPassInfo.subpassCount = 1;
			renderPassInfo.pSubpasses = &subpassDescription;
			renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
			renderPassInfo.pDependencies = dependencies.data();


			// default broadcast to all layers
			//https://anishbhobe.site/post/vulkan-render-to-cubemap-using-multiview/#3-attaching-the-cubemap-to-the-framebuffer
			//const uint32_t viewMask = 0b11111111;
			//const uint32_t viewMask = 0b111111;
			const uint32_t viewMask = 0b11111111;
			//const uint32_t viewMask = 0;

				/*
					Bit mask that specifies correlation between views
					An implementation may use this for optimizations (concurrent render)
				*/
			const uint32_t correlationMask = 0;

			VkRenderPassMultiviewCreateInfo renderPassMultiviewCI{};
			renderPassMultiviewCI.sType = VK_STRUCTURE_TYPE_RENDER_PASS_MULTIVIEW_CREATE_INFO;
			renderPassMultiviewCI.subpassCount = 1;
			renderPassMultiviewCI.pViewMasks = &viewMask;
			renderPassMultiviewCI.correlationMaskCount = 0;
			renderPassMultiviewCI.pCorrelationMasks = nullptr;
			renderPassMultiviewCI.pNext = nullptr;
			renderPassMultiviewCI.pViewOffsets = NULL;
			renderPassMultiviewCI.dependencyCount = 0;
			if (m_Config.Multiview)
			{
				renderPassInfo.pNext = &renderPassMultiviewCI;
			}

			VK_CHECK_RESULT(vkCreateRenderPass(device, &renderPassInfo, nullptr, &m_CompatibilityRenderPass));
			graphicsContext->SetDebugUtilsObjectName(VK_OBJECT_TYPE_RENDER_PASS, m_Config.DebugName + "Compatitbility render pass", m_CompatibilityRenderPass);
		}
		#endif
		CreatePipelineLayout();

		// pipelien config
		PipelineConfigInfo pipelineConfig;
		pipelineConfig.InputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		pipelineConfig.InputAssemblyInfo.topology = Utils::ProofTopologyToVulkanTopology(m_Config.DrawMode);
		pipelineConfig.InputAssemblyInfo.primitiveRestartEnable = VK_FALSE;
		pipelineConfig.InputAssemblyInfo.pNext = nullptr;
		pipelineConfig.InputAssemblyInfo.flags = 0;

		pipelineConfig.ViewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		pipelineConfig.ViewportState.viewportCount = 1;
		pipelineConfig.ViewportState.pNext = nullptr;
		pipelineConfig.ViewportState.scissorCount = 1;
		pipelineConfig.ViewportState.flags = 0;

		pipelineConfig.RasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		pipelineConfig.RasterizationInfo.depthClampEnable = VK_FALSE;
		pipelineConfig.RasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
		pipelineConfig.RasterizationInfo.polygonMode = Utils::ConvertToVulkanPolygonMode(m_Config.FillMode);
		pipelineConfig.RasterizationInfo.lineWidth = m_Config.LineWidth;
		pipelineConfig.RasterizationInfo.cullMode = Utils::ProofFormatToVulkanFormat(m_Config.CullMode);
		pipelineConfig.RasterizationInfo.frontFace = Utils::ProofFormatToVulkanFormat(m_Config.FrontFace);
		pipelineConfig.RasterizationInfo.depthBiasEnable = VK_FALSE;
		pipelineConfig.RasterizationInfo.depthBiasConstantFactor = 0.0f;  // Optional
		pipelineConfig.RasterizationInfo.depthBiasClamp = 0.0f;           // Optional
		pipelineConfig.RasterizationInfo.depthBiasSlopeFactor = 0.0f;     // Optional
		pipelineConfig.RasterizationInfo.pNext = nullptr;
		pipelineConfig.RasterizationInfo.flags = 0;

		pipelineConfig.MultisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		pipelineConfig.MultisampleInfo.sampleShadingEnable = VK_FALSE;
		pipelineConfig.MultisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		pipelineConfig.MultisampleInfo.minSampleShading = 1.0f;           // Optional
		pipelineConfig.MultisampleInfo.pSampleMask = nullptr;             // Optional
		pipelineConfig.MultisampleInfo.alphaToCoverageEnable = VK_FALSE;  // Optional
		pipelineConfig.MultisampleInfo.alphaToOneEnable = VK_FALSE;       // Optional
		pipelineConfig.MultisampleInfo.pSampleMask = nullptr;     // Optional
		pipelineConfig.MultisampleInfo.flags = 0;
		pipelineConfig.MultisampleInfo.pNext = nullptr;

		size_t colorAttachmentCount = m_Config.Attachments.Attachments.size();
		if (depthImage)
			colorAttachmentCount -= 1;
		std::vector<VkPipelineColorBlendAttachmentState> blendAttachmentStates(colorAttachmentCount);
		//if (framebuffer->GetSpecification().SwapChainTarget)
		//{
		//	blendAttachmentStates[0].colorWriteMask = 0xf;
		//	blendAttachmentStates[0].blendEnable = VK_TRUE;
		//	blendAttachmentStates[0].srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		//	blendAttachmentStates[0].dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		//	blendAttachmentStates[0].colorBlendOp = VK_BLEND_OP_ADD;
		//	blendAttachmentStates[0].alphaBlendOp = VK_BLEND_OP_ADD;
		//	blendAttachmentStates[0].srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		//	blendAttachmentStates[0].dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		//}
		//else
		for (size_t i = 0; i < colorAttachmentCount; i++)
		{
			if (!m_Config.Blend)
				break;

			blendAttachmentStates[i].colorWriteMask = 0xf;
			const auto& attachmentSpec = m_Config.Attachments.Attachments[i];

			blendAttachmentStates[i].blendEnable = attachmentSpec.Blend ? VK_TRUE : VK_FALSE;

			blendAttachmentStates[i].colorBlendOp = VK_BLEND_OP_ADD;
			blendAttachmentStates[i].alphaBlendOp = VK_BLEND_OP_ADD;
			blendAttachmentStates[i].srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
			blendAttachmentStates[i].dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;

			switch (m_Config.BlendMode)
			{
				case BlendMode::SrcAlphaOneMinusSrcAlpha:
					blendAttachmentStates[i].srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
					blendAttachmentStates[i].dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
					blendAttachmentStates[i].srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
					blendAttachmentStates[i].dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
					break;
				case BlendMode::OneZero:
					blendAttachmentStates[i].srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
					blendAttachmentStates[i].dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
					break;
				case BlendMode::Zero_SrcColor:
					blendAttachmentStates[i].srcColorBlendFactor = VK_BLEND_FACTOR_ZERO;	
					blendAttachmentStates[i].dstColorBlendFactor = VK_BLEND_FACTOR_SRC_COLOR;
					break;

				default:
					PF_CORE_ASSERT(false);
			}
		}
		pipelineConfig.ColorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		pipelineConfig.ColorBlendInfo.logicOpEnable = VK_FALSE;
		pipelineConfig.ColorBlendInfo.logicOp = VK_LOGIC_OP_COPY;  // Optional
		pipelineConfig.ColorBlendInfo.attachmentCount = 1;
		pipelineConfig.ColorBlendInfo.attachmentCount = (uint32_t)blendAttachmentStates.size();
		pipelineConfig.ColorBlendInfo.pAttachments = blendAttachmentStates.data();
		pipelineConfig.ColorBlendInfo.blendConstants[0] = 0.0f;  // Optional
		pipelineConfig.ColorBlendInfo.blendConstants[1] = 0.0f;  // Optional
		pipelineConfig.ColorBlendInfo.blendConstants[2] = 0.0f;  // Optional
		pipelineConfig.ColorBlendInfo.blendConstants[3] = 0.0f;  // Optional
		pipelineConfig.ColorBlendInfo.pNext = nullptr;
		pipelineConfig.ColorBlendInfo.flags = 0;

		pipelineConfig.DepthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		pipelineConfig.DepthStencilInfo.depthTestEnable = m_Config.DepthTest ? VK_TRUE : VK_FALSE;
		pipelineConfig.DepthStencilInfo.depthWriteEnable = m_Config.WriteDepth ? VK_TRUE : VK_FALSE;
		pipelineConfig.DepthStencilInfo.depthCompareOp = Utils::ProofCompareOpToVulkanCompareOp(m_Config.DepthCompareOperator);
		pipelineConfig.DepthStencilInfo.depthBoundsTestEnable = VK_FALSE;
		pipelineConfig.DepthStencilInfo.minDepthBounds = 0.0f;  // Optional
		pipelineConfig.DepthStencilInfo.maxDepthBounds = 1.0f;  // Optional
		pipelineConfig.DepthStencilInfo.stencilTestEnable = VK_FALSE;
		pipelineConfig.DepthStencilInfo.front = {};  // Optional
		pipelineConfig.DepthStencilInfo.back = {};   // Optional
		pipelineConfig.DepthStencilInfo.pNext = nullptr;
		pipelineConfig.DepthStencilInfo.flags = 0;


		std::vector<VkDynamicState> dynamicState{ VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
		if (m_Config.EditCullMode )
			dynamicState.push_back(VK_DYNAMIC_STATE_CULL_MODE);
		if (m_Config.EditDrawType )
			dynamicState.push_back(VK_DYNAMIC_STATE_PRIMITIVE_TOPOLOGY);
		if (m_Config.EditLineWidth || m_Config.LineWidth >1)
			dynamicState.push_back(VK_DYNAMIC_STATE_LINE_WIDTH);

		pipelineConfig.DynamicSate.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		pipelineConfig.DynamicSate.dynamicStateCount = dynamicState.size();
		pipelineConfig.DynamicSate.pNext = nullptr;
		pipelineConfig.DynamicSate.flags = 0;
		pipelineConfig.DynamicSate.pDynamicStates = dynamicState.data();

		////////////////////////
		//////End Pipeline config
		///////////////////////////
		pipelineConfig.RenderPass = m_CompatibilityRenderPass;
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

		PF_ENGINE_TRACE("Created GraphicsPipeline {}", m_Config.DebugName);
	}
	void VulkanGraphicsPipeline::Release()
	{
		Renderer::SubmitDatafree([pipline = m_GraphicsPipeline, piplinelayout = m_PipeLineLayout, renderPass = m_CompatibilityRenderPass]() {
			vkDestroyPipeline(VulkanRenderer::GetGraphicsContext()->GetDevice(), pipline, nullptr);
			vkDestroyPipelineLayout(VulkanRenderer::GetGraphicsContext()->GetDevice(), piplinelayout, nullptr);
			if(renderPass)
				vkDestroyRenderPass(VulkanRenderer::GetGraphicsContext()->GetDevice(), renderPass, nullptr);
		});
		m_GraphicsPipeline = nullptr;
		m_PipeLineLayout = nullptr;
		m_CompatibilityRenderPass = nullptr;
	}

	void VulkanGraphicsPipeline::PushlineWidth(float lineWidth)
	{
		if (m_Config.EditLineWidth == false)
		{
			PF_ENGINE_WARN("{} pipeline cannot push line because configuration linewidth is set to false remake pipeline", m_Config.DebugName);
			return;
		}
		PF_CORE_ASSERT(m_LineWidthStack.size() < 0, "Cannot push to stack if pop stack is not called");
		m_LineWidthStack.push_back(lineWidth);
	}

	void VulkanGraphicsPipeline::PoplineWidth()
	{

		if (m_Config.EditLineWidth == false)
		{
			PF_ENGINE_WARN("{} pipeline cannot pop line because configuration linewidth is set to false remake pipeline", m_Config.DebugName);
			return;
		}
		PF_CORE_ASSERT(m_LineWidthStack.size() == 1, "Cannot pop stack if stack is empty");
		m_LineWidthStack.pop_back();
	}

	void VulkanGraphicsPipeline::PushDrawType(DrawType draw)
	{
		if (m_Config.EditDrawType == false)
		{
			PF_ENGINE_WARN("{} pipeline cannot push line because configuration DrawType is set to false remake pipeline", m_Config.DebugName);
			return;
		}
		PF_CORE_ASSERT(m_DrawTypeStack.size() < 0, "Cannot push to stack if pop stack is not called");
		m_DrawTypeStack.push_back(draw);
	}

	void VulkanGraphicsPipeline::PopDrawType()
	{
		if (m_Config.EditDrawType == false)
		{
			PF_ENGINE_WARN("{} pipeline cannot pop line because configuration DrawType is set to false remake pipeline", m_Config.DebugName);
			return;
		}
		PF_CORE_ASSERT(m_DrawTypeStack.size() == 1, "Cannot pop stack if stack is empty");
		m_DrawTypeStack.pop_back();
	}

	void VulkanGraphicsPipeline::PushCullMode(CullMode cull)
	{
		if (m_Config.EditCullMode == false)
		{
			PF_ENGINE_WARN("{} pipeline cannot push line because configuration CullMode is set to false remake pipeline", m_Config.DebugName);
			return;
		}
		PF_CORE_ASSERT(m_CullModeStack.size() < 0, "Cannot push to stack if pop stack is not called");
		m_CullModeStack.push_back(cull);
	}

	void VulkanGraphicsPipeline::PopCullMode()
	{
		if (m_Config.EditCullMode == false)
		{
			PF_ENGINE_WARN("{} pipeline cannot pop line because configuration CullMode is set to false remake pipeline", m_Config.DebugName);
			return;
		}
		PF_CORE_ASSERT(m_CullModeStack.size() == 1, "Cannot pop stack if stack is empty");
		m_CullModeStack.pop_back();
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
}