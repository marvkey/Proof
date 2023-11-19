#pragma once
#include "Vulkan.h"
#include "Proof/Renderer/ComputePipeline.h"
namespace Proof
{
	
	class VulkanComputePipeline : public ComputePipeline
	{
	public:
		VulkanComputePipeline(const ComputePipelineConfig& config,bool isRenderThread = false);
		~VulkanComputePipeline();

		virtual Count<class Shader> GetShader()const {return m_Config.Shader;}
		VkPipelineLayout GetPipelinelayout() const { return m_PipeLineLayout; }
		VkPipeline GetComputePipeline() const { return m_ComputePipeline; };
		const ComputePipelineConfig& GetConfig()const override { return m_Config; }

		void Build();
		void Release();
	private:
		void BuildPipeline();
		VkPipelineLayout m_PipeLineLayout = nullptr;
		VkPipeline m_ComputePipeline;
		VkPipelineCache m_PipelineCache = NULL;
		ComputePipelineConfig m_Config;
		uint32_t m_ShaderReloadCallbackIndex;
	};
}
