#pragma once
#include "Proof/Core/Core.h"
#include <vulkan/vulkan.h>
#include "Proof/Renderer/GraphicsPipeLine.h"
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

		void Bind(Count<class RenderCommandBuffer> commandBuffer);
		Count<class Shader> GetShader()const { return m_Config.Shader; }
		VkPipeline GetPipline() { return m_GraphicsPipeline; }

		const GraphicsPipelineConfiguration& GetConfig()const { return m_Config; };
		VkPipelineLayout GetPipelineLayout() { return m_PipeLineLayout; }
		VkPipeline GetPipeline() { return m_GraphicsPipeline; }

		void Build();
		void Release();

		void PushlineWidth(float lineWidth);
		void PoplineWidth();

		void PushDrawType(DrawType draw);
		void PopDrawType();

		void PushCullMode(CullMode cull);
		void PopCullMode();

		const std::vector<float>& GetLineWidthStack()const { return m_LineWidthStack; }
		const std::vector<DrawType>& GetDrawTypeStack()const { return m_DrawTypeStack; }
		const std::vector<CullMode>& GetCullModeStack()const { return m_CullModeStack; }
	private:
		std::vector<float> m_LineWidthStack;
		std::vector<DrawType> m_DrawTypeStack;
		std::vector<CullMode> m_CullModeStack;

		VkRenderPass m_CompatibilityRenderPass =nullptr;
		VkPipelineLayout m_PipeLineLayout  = nullptr;
		GraphicsPipelineConfiguration m_Config;
		VkPipeline m_GraphicsPipeline = nullptr;

		uint32_t m_ShaderReloadCallbackIndex;

	private:
		void CreatePipelineLayout();
		friend class VulkanRenderPass;
	};
}