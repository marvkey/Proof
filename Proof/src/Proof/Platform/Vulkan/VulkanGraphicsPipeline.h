#pragma once
#include "Proof/Core/Core.h"
#include <vulkan/vulkan.h>
#include "Proof/Core/Buffer.h"
#include "Proof/Renderer/GraphicsPipeLine.h"
#include <stack>
namespace Proof
{
	namespace Utils {
		static VkPolygonMode ConvertToVulkanPolygonMode(PolygonFillMode fillMode) {
			switch (fillMode)
			{
				case PolygonFillMode::Fill:
					return VK_POLYGON_MODE_FILL;
				case PolygonFillMode::Line:
					return VK_POLYGON_MODE_LINE;
				case PolygonFillMode::Points:
					return VK_POLYGON_MODE_POINT;
			}
			PF_CORE_ASSERT(false);
		}
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
		static VkCompareOp ProofCompareOpToVulkanCompareOp(DepthCompareOperator compare) {
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
		static VkBlendFactor ProofToVulkanBlendFactor(BlendFactor blendFactor)
		{
			switch (blendFactor)
			{
				case BlendFactor::Zero: return VK_BLEND_FACTOR_ZERO;
				case BlendFactor::One: return VK_BLEND_FACTOR_ONE;
				case BlendFactor::SrcColor: return VK_BLEND_FACTOR_SRC_COLOR;
				case BlendFactor::OneMinusSrcColor: return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
				case BlendFactor::DstColor: return VK_BLEND_FACTOR_DST_COLOR;
				case BlendFactor::OneMinusDstColor: return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
				case BlendFactor::SrcAlpha: return VK_BLEND_FACTOR_SRC_ALPHA;
				case BlendFactor::OneMinusSrcAlpha: return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
				case BlendFactor::DstAlpha: return VK_BLEND_FACTOR_DST_ALPHA;
				case BlendFactor::OneMinusDstAlpha: return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
				case BlendFactor::ConstantColor: return VK_BLEND_FACTOR_CONSTANT_COLOR;
				case BlendFactor::OneMinusConstantColor: return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
				case BlendFactor::ConstantAlpha: return VK_BLEND_FACTOR_CONSTANT_ALPHA;
				case BlendFactor::OneMinusConstantAlpha: return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;
				case BlendFactor::SrcAlphaSaturate: return VK_BLEND_FACTOR_SRC_ALPHA_SATURATE;
				case BlendFactor::Src1Color: return VK_BLEND_FACTOR_SRC1_COLOR;
				case BlendFactor::OneMinusSrc1Color: return VK_BLEND_FACTOR_ONE_MINUS_SRC1_COLOR;
				case BlendFactor::Src1Alpha: return VK_BLEND_FACTOR_SRC1_ALPHA;
				case BlendFactor::OneMinusSrc1Alpha: return VK_BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA;
				
			}
			PF_CORE_ASSERT(false, "Operand not supported ");
		}

		static VkBlendOp ProofToVulkanBlendOperation(BlendOperation blendOperation)
		{
			switch (blendOperation)
			{
				case BlendOperation::Add: return VK_BLEND_OP_ADD;
				case BlendOperation::Subtract: return VK_BLEND_OP_SUBTRACT;
				case BlendOperation::ReverseSubtract: return VK_BLEND_OP_REVERSE_SUBTRACT;
				case BlendOperation::Min: return VK_BLEND_OP_MIN;
				case BlendOperation::Max: return VK_BLEND_OP_MAX;
			}
			PF_CORE_ASSERT(false, "Operand not supported ");
		}

		static VkPipelineColorBlendAttachmentState ProofToVulkanBlendAttachmentState(bool blendEnable, const BlendAttachmentState& blendAttachmentState)
		{
			VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
			colorBlendAttachment.blendEnable = blendEnable ? VK_TRUE : VK_FALSE;

			colorBlendAttachment.srcColorBlendFactor = ProofToVulkanBlendFactor(blendAttachmentState.SrcColorBlendFactor);
			colorBlendAttachment.dstColorBlendFactor = ProofToVulkanBlendFactor(blendAttachmentState.DstColorBlendFactor);
			colorBlendAttachment.colorBlendOp = ProofToVulkanBlendOperation(blendAttachmentState.ColorBlendOperation);

			colorBlendAttachment.srcAlphaBlendFactor = ProofToVulkanBlendFactor(blendAttachmentState.SrcAlphaBlendFactor);
			colorBlendAttachment.dstAlphaBlendFactor = ProofToVulkanBlendFactor(blendAttachmentState.DstAlphaBlendFactor);
			colorBlendAttachment.alphaBlendOp = ProofToVulkanBlendOperation(blendAttachmentState.AlphaBlendOperation);

			colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

			return colorBlendAttachment;
		}
	}
	struct PipelineConfigInfo {
		VkPipelineInputAssemblyStateCreateInfo InputAssemblyInfo;
		VkPipelineViewportStateCreateInfo ViewportState;
		VkPipelineRasterizationStateCreateInfo RasterizationInfo;
		VkPipelineMultisampleStateCreateInfo MultisampleInfo;
		VkPipelineDepthStencilStateCreateInfo DepthStencilInfo;
		VkPipelineColorBlendStateCreateInfo ColorBlendInfo;
		VkPipelineLayout PipelineLayout = nullptr;
		VkPipelineDynamicStateCreateInfo DynamicSate;
		VkRenderPass RenderPass = nullptr;
		uint32_t Subpass = 0;
	};
	class  VulkanGraphicsPipeline : public GraphicsPipeline {
	public:
		VulkanGraphicsPipeline(const GraphicsPipelineConfiguration& config);
		virtual ~VulkanGraphicsPipeline();

		//void Bind(Count<class RenderCommandBuffer> commandBuffer);
		void RT_Bind(Count<class RenderCommandBuffer> commandBuffer);
		Count<class Shader> GetShader()const { return m_Config.Shader; }
		VkPipeline GetPipline() { return m_GraphicsPipeline; }

		const GraphicsPipelineConfiguration& GetConfig()const { return m_Config; };
		VkPipelineLayout GetPipelineLayout() { return m_PipeLineLayout; }
		VkPipeline GetPipeline() { return m_GraphicsPipeline; }

		void RT_Build();
		void Build();
		void Release();

		void PushlineWidth(float lineWidth);
		void PoplineWidth();

		void PushDrawType(DrawType draw);
		void PopDrawType();

		void PushCullMode(CullMode cull);
		void PopCullMode();

		void PushDepthTest(bool depthTest);
		void PopDepthTest();

		const std::stack<float>& GetLineWidthStack()const { return m_LineWidthStack; }
		const std::stack<DrawType>& GetDrawTypeStack()const { return m_DrawTypeStack; }
		const std::stack<CullMode>& GetCullModeStack()const { return m_CullModeStack; }
		const std::stack<bool>& GetDepthTestStack()const { return m_DepthTestStack; }

	private:
		std::stack<float> m_LineWidthStack;
		std::stack<DrawType> m_DrawTypeStack;
		std::stack<CullMode> m_CullModeStack;
		std::stack<bool> m_DepthTestStack;

		VkRenderPass m_CompatibilityRenderPass =nullptr;
		VkPipelineLayout m_PipeLineLayout  = nullptr;
		GraphicsPipelineConfiguration m_Config;
		VkPipeline m_GraphicsPipeline = nullptr;

		uint32_t m_ShaderReloadCallbackIndex;

	private:
		void CreatePipelineLayout();
		friend class VulkanRenderPass;
		Buffer m_LocalStorage;
	};
}