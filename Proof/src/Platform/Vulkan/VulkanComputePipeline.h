#pragma once
#include "Vulkan.h"
#include "Proof/Renderer/ComputePipeline.h"
namespace Proof
{
	
	class VulkanComputePipeline : public ComputePipeline
	{
	public:
		VulkanComputePipeline(const ComputePipelineConfig& config);
		~VulkanComputePipeline();

		virtual Count<class Shader> GetShader()const {return m_Config.Shader;}
		VkPipelineLayout GetPipelinelayout() const { return m_PipeLineLayout; }
		VkPipeline GetComputePipeline() const { return m_ComputePipeline; };
		const ComputePipelineConfig& GetConfig()const override { return m_Config; }
	private:
		void Build();
		void BuildPipeline();
		VkPipelineLayout m_PipeLineLayout = nullptr;
		VkPipeline m_ComputePipeline;
		ComputePipelineConfig m_Config;
		void Release();
	};
}
