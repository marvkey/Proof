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
		virtual ~VulkanGraphicsPipeline();

		void BeginRecord(Count<class RenderCommandBuffer> commandBuffer);
		void EndRecord();
		virtual void SetInput(std::string_view name, Count<class UniformBuffer> buffer);
		virtual void SetInput(std::string_view name, Count<class Texture2D> iamge);
		virtual void SetInput(std::string_view name, const std::vector< Count<class Texture2D>>& images);
		virtual void SetInput(std::string_view name, Count<class StorageBuffer> buffer);
		virtual void PushData(std::string_view name, const void* data);
		VulkanGraphicsPipeline(const GraphicsPipelineConfig& config);
		VkPipeline GetPipline() { return m_GraphicsPipeline; }
		static void DefaultPipelineConfigInfo(PipelineConfigInfo& configInfo,const GraphicsPipelineConfig& graphicsConfig);
		const GraphicsPipelineConfig& GetConfig()const { return m_Config; };
		VkPipelineLayout GetPipelineLayout() { return m_PipeLineLayout; }
		VkPipeline GetPipeline() { return m_GraphicsPipeline; }
	private:
		Count<class RenderCommandBuffer> m_CommandBuffer;
		void CreatePipelineLayout();
		VkPipelineLayout m_PipeLineLayout;
		Count<class VulkanDescriptorManager> m_DescritptorSetManager;

		GraphicsPipelineConfig m_Config;
		// temporary
		friend class VulkanRenderPass;
		VkPipeline m_GraphicsPipeline;
	};
}