#include "Proofprch.h"
#include "VulkanComputePipeline.h"
#include "VulkanShader.h"
#include "VulkanRenderer.h"
#include "VulkanGraphicsContext.h"
#include "VulkanDevice.h"

namespace Proof {
	VulkanComputePipeline::VulkanComputePipeline(const ComputePipelineConfig& config, bool isRenderThread) :m_Config(config)
	{
		if (isRenderThread)
		{
			Build();
		}
		else
		{
			Count<VulkanComputePipeline> instance = this;
			Renderer::Submit([instance]
				{
					instance->Build();
				});
		}


		// not using count because we dont want this fucntion keeping a reference count therefore not letting this object being able to be deleted
		WeakCount<VulkanComputePipeline> instanceWeakCount = this;
		m_ShaderReloadCallbackIndex = GetShader()->AddShaderReloadCallback([instanceWeakCount]
			{
				if (!instanceWeakCount.IsValid())
					return;
				auto computePipeline = instanceWeakCount.Lock();

				computePipeline->Release();
				computePipeline->Build();
			});
	}
	VulkanComputePipeline::~VulkanComputePipeline()
	{
		Release();
		GetShader()->RemoveShaderReloadCallback(m_ShaderReloadCallbackIndex);
	}

	void VulkanComputePipeline::Build()
	{
		auto device = VulkanRenderer::GetGraphicsContext()->GetDevice()->GetVulkanDevice();

		BuildPipeline();
		auto vulkanShader = m_Config.Shader.As<VulkanShader>();
		VkPipelineCacheCreateInfo pipelineCacheCreateinfo;
		pipelineCacheCreateinfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
		pipelineCacheCreateinfo.pNext = nullptr;
		pipelineCacheCreateinfo.flags = 0;
		pipelineCacheCreateinfo.pInitialData = nullptr;
		pipelineCacheCreateinfo.initialDataSize = 0;
		//VK_CHECK_RESULT(vkCreatePipelineCache(device, &pipelineCacheCreateinfo, nullptr, &m_PipelineCache));

		VkComputePipelineCreateInfo pipelineCreateInfo = {};
		pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
		pipelineCreateInfo.stage = vulkanShader->GetShaderStages().at(ShaderStage::Compute);
		pipelineCreateInfo.layout = m_PipeLineLayout;
		pipelineCreateInfo.pNext = nullptr;

		VK_CHECK_RESULT(vkCreateComputePipelines(device, nullptr, 1, &pipelineCreateInfo, nullptr, &m_ComputePipeline));
		VulkanUtils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_PIPELINE, fmt::format("{} ComputePipeline", m_Config.DebugName), m_ComputePipeline);
	}
	void VulkanComputePipeline::BuildPipeline()
	{

		auto device = VulkanRenderer::GetGraphicsContext()->GetDevice()->GetVulkanDevice();

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
		VulkanUtils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_PIPELINE_LAYOUT, fmt::format("{} ComputePipelineLayout", m_Config.DebugName), m_PipeLineLayout);

	}
	void VulkanComputePipeline::Release()
	{
		if (m_ComputePipeline == nullptr)
			return;

		Renderer::SubmitResourceFree([pipline = m_ComputePipeline, piplinelayout = m_PipeLineLayout, piplineCache = m_PipelineCache]()
			{
				auto device = VulkanRenderer::GetGraphicsContext()->GetDevice()->GetVulkanDevice();

				vkDestroyPipeline(device, pipline, nullptr);
				vkDestroyPipelineLayout(device, piplinelayout, nullptr);
				vkDestroyPipelineCache(device, piplineCache, nullptr);
			});
		m_ComputePipeline = nullptr;
		m_PipeLineLayout = nullptr;
		m_PipelineCache = nullptr;
	}
}