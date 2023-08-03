#pragma once
#include "Proof/Core/Core.h"
#include <vulkan/vulkan.h>
#include "Proof/Renderer/GraphicsPipeLine.h"
namespace Proof
{
	struct PipelineConfigInfo {
		VkPipelineInputAssemblyStateCreateInfo InputAssemblyInfo;
		VkPipelineViewportStateCreateInfo ViewportState;
		VkPipelineRasterizationStateCreateInfo RasterizationInfo;
		VkPipelineMultisampleStateCreateInfo MultisampleInfo;
		VkPipelineDepthStencilStateCreateInfo DepthStencilInfo;
		VkPipelineColorBlendAttachmentState ColorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo ColorBlendInfo;
		VkPipelineLayout PipelineLayout = nullptr;
		VkPipelineDynamicStateCreateInfo DynamicSate;
		VkRenderPass RenderPass = nullptr;
		uint32_t Subpass = 0;
	};
	class  VulkanGraphicsPipeline : public GraphicsPipeline {
	public:
		VulkanGraphicsPipeline(const GraphicsPipelineConfig& config);
		virtual ~VulkanGraphicsPipeline();

		void Bind(Count<class RenderCommandBuffer> commandBuffer);
		Count<class Shader> GetShader()const { return m_Config.Shader; }

		VkPipeline GetPipline() { return m_GraphicsPipeline; }
		static void DefaultPipelineConfigInfo(PipelineConfigInfo& configInfo,const GraphicsPipelineConfig& graphicsConfig);
		virtual Count<class FrameBuffer> GetTargetBuffer()const { return m_Config.TargetBuffer; };

		const GraphicsPipelineConfig& GetConfig()const { return m_Config; };
		VkPipelineLayout GetPipelineLayout() { return m_PipeLineLayout; }
		VkPipeline GetPipeline() { return m_GraphicsPipeline; }

		void Build();
		void Release();
	private:
		void CreatePipelineLayout();
		VkPipelineLayout m_PipeLineLayout  = nullptr;
		GraphicsPipelineConfig m_Config;
		VkPipeline m_GraphicsPipeline = nullptr;

		uint32_t m_ShaderReloadCallbackIndex;

		friend class VulkanRenderPass;
	};
}