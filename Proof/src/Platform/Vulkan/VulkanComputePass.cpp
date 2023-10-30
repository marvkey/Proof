#include "Proofprch.h"
#include "VulkanComputePass.h"
#include "VulkanComputePipeline.h"
#include "VulkanCommandBuffer.h"
#include "VulkanRenderMaterial.h"
#include "VulkanRenderer.h"
namespace Proof
{
	

	
	Count<Shader> VulkanComputePass::GetShader()const
	{
		return m_Config.Pipeline->GetShader();
	}
	VulkanComputePass::VulkanComputePass(const ComputePassConfiguration& config)
		:m_Config(config)
	{
		Build();
		VulkanDescriptorManagerConfig descriptorConfig;
		descriptorConfig.DebugName = m_Config.DebugName + "Descriptor Manager";
		descriptorConfig.Shader = m_Config.Pipeline->GetShader().As<VulkanShader>();
		m_DescritptorSetManager = Count<VulkanDescriptorManager>::Create(descriptorConfig);
	}
	VulkanComputePass::~VulkanComputePass()
	{
		m_DescritptorSetManager = nullptr;
	}
	void VulkanComputePass::Build()
	{
	}
	void VulkanComputePass::BeginComputePassBase(Count<class RenderCommandBuffer> command)
	{
		Count<VulkanComputePass> instance = this;
		Renderer::Submit([instance,command]()
			{
				instance->RT_BeginComputePassBase(command);
			});

	}
	void VulkanComputePass::BeginComputePass(Count<class RenderCommandBuffer> command)
	{
		Count<VulkanComputePass> instance = this;
		Renderer::Submit([instance, command]()
			{
				instance->RT_BeginComputePass(command);
			});
	}
	void VulkanComputePass::BeginRenderMaterialComputePass(Count<class RenderCommandBuffer> command)
	{
		Count<VulkanComputePass> instance = this;
		Renderer::Submit([instance, command]()
			{
				instance->RT_BeginRenderMaterialComputePass(command);
			});
	}
	void VulkanComputePass::ComputePassPushRenderMaterial(Count<class RenderMaterial> renderMaterial)
	{
		Count<VulkanComputePass> instance = this;
		Renderer::Submit([instance, renderMaterial]()
			{
				instance->RT_ComputePassPushRenderMaterial(renderMaterial);
			});
	}
	void VulkanComputePass::EndComputePass()
	{
		Count<VulkanComputePass> instance = this;
		Renderer::Submit([instance]()
			{
				instance->RT_EndComputePass();
			});
	}
	void VulkanComputePass::Dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
	{
		Count<VulkanComputePass> instance = this;
		Renderer::Submit([instance, groupCountX,groupCountY,groupCountZ] () mutable
		{
				instance->RT_Dispatch(groupCountX, groupCountY, groupCountZ);
		});

	}
	
	void VulkanComputePass::RT_BeginComputePassBase(Count<class RenderCommandBuffer> command)
	{
		PF_CORE_ASSERT(m_RenderPassEnabled == false, fmt::format("cannot start {} render pass when previous render pass is not closed", m_Config.DebugName).c_str());
		m_CommandBuffer = command;
		m_RenderPassEnabled = true;

		vkCmdBindPipeline(m_CommandBuffer.As<VulkanRenderCommandBuffer>()->GetActiveCommandBuffer(), VK_PIPELINE_BIND_POINT_COMPUTE, m_Config.Pipeline.As<VulkanComputePipeline>()->GetComputePipeline());
	}
	
	void VulkanComputePass::RT_BeginComputePass(Count<class RenderCommandBuffer> command)
	{
		RT_BeginComputePassBase(command);

		m_DescritptorSetManager->RT_Bind();
		auto& frameSet = m_DescritptorSetManager->GetDescriptorSets()[Renderer::RT_GetCurrentFrameInFlight()];
		for (auto& [set, setInfo] : frameSet)
		{
			// basically we have to define a set layout for each descriptor set 0-3
			// but some set may not have data and we do note creata a descriptor set for it
			// so we basically just seeing if thats teh case we dont bind it
			if (setInfo.Set == nullptr)
				continue;
			vkCmdBindDescriptorSets(
				m_CommandBuffer.As<VulkanRenderCommandBuffer>()->GetActiveCommandBuffer(),
				VK_PIPELINE_BIND_POINT_COMPUTE,
				m_Config.Pipeline.As<VulkanComputePipeline>()->GetPipelinelayout(),
				(int)set,
				1,
				&setInfo.Set,
				0,
				nullptr);
		}
	}
	void VulkanComputePass::RT_BeginRenderMaterialComputePass(Count<class RenderCommandBuffer> command)
	{
		RT_BeginComputePassBase(command);

		PF_CORE_ASSERT(m_MaterialRenderPass == false, "cannot start material render pass if previous material render pass not disabled");

		m_MaterialRenderPass = true;

			
		m_DescritptorSetManager->RT_Bind();

		auto& frameSet = m_DescritptorSetManager->GetDescriptorSets()[Renderer::RT_GetCurrentFrameInFlight()];
		for (auto& [set, setInfo] : frameSet)
		{
			// set0 is for te material to bind to 
			// basically we have to define a set layout for each descriptor set 0-3
			// but some set may not have data and we do note creata a descriptor set for it
			// so we basically just seeing if thats teh case we dont bind it
			if (set == 0 || setInfo.Set == nullptr)continue;
			vkCmdBindDescriptorSets(
				m_CommandBuffer.As<VulkanRenderCommandBuffer>()->GetActiveCommandBuffer(),
				VK_PIPELINE_BIND_POINT_COMPUTE,
				m_Config.Pipeline.As<VulkanComputePipeline>()->GetPipelinelayout(),
				(int)set,
				1,
				&setInfo.Set,
				0,
				nullptr);
		}
	}
	void VulkanComputePass::RT_ComputePassPushRenderMaterial(Count<class RenderMaterial> renderMaterial)
	{
		
		PF_CORE_ASSERT(m_RenderPassEnabled == true, "cannot Push material fi render pass not enabled");
		PF_CORE_ASSERT(m_MaterialRenderPass == true, "cannot Push if not a material Render Pass");
		renderMaterial.As<VulkanRenderMaterial>()->RT_Bind(m_CommandBuffer.As<VulkanRenderCommandBuffer>(), this);
	}
	void VulkanComputePass::RT_EndComputePass()
	{
			PF_CORE_ASSERT(m_RenderPassEnabled == true, "cannot End render pass when render pass is not started");
			m_CommandBuffer = nullptr;
			m_MaterialRenderPass = false;
			m_RenderPassEnabled = false;
	}
	void VulkanComputePass::RT_Dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
	{
		PF_CORE_ASSERT(m_RenderPassEnabled, "Cannot dispatch unless start a compute pass");
		VkCommandBuffer buffer = m_CommandBuffer.As<VulkanRenderCommandBuffer>()->GetActiveCommandBuffer();
		PF_CORE_ASSERT(buffer);
		vkCmdDispatch(buffer, groupCountX, groupCountY, groupCountZ);
	}
	void VulkanComputePass::SetInput(std::string_view name, Count<class StorageBuffer> buffer)
	{
		m_DescritptorSetManager->SetInput(name, buffer);
	}
	void VulkanComputePass::SetInput(std::string_view name, Count<class ImageView> imageView)
	{
		m_DescritptorSetManager->SetInput(name, imageView);
	}
	void VulkanComputePass::SetInput(std::string_view name, const std::vector< Count<class ImageView>>& imageViews)
	{
		m_DescritptorSetManager->SetInput(name, imageViews);
	}
	void VulkanComputePass::SetInput(std::string_view name, Count<class Image2D>image)
	{
		m_DescritptorSetManager->SetInput(name, image);
	}
	void VulkanComputePass::SetInput(std::string_view name, const std::vector< Count<class Image2D>>& images)
	{
		m_DescritptorSetManager->SetInput(name, images);
	}
	void VulkanComputePass::AddGlobalInput(Count<class GlobalBufferSet> globalInputs)
	{
		m_DescritptorSetManager->SetGoalballInputs(globalInputs);
	}
	void VulkanComputePass::SetInput(std::string_view name, Count<class UniformBuffer> buffer)
	{
		m_DescritptorSetManager->SetInput(name, buffer);
	}
	void VulkanComputePass::SetInput(std::string_view name, Count<class Texture2D> image)
	{
		m_DescritptorSetManager->SetInput(name, image);
	}
	void VulkanComputePass::SetInput(std::string_view name, const std::vector<Count<class Texture2D>>& images)
	{
		m_DescritptorSetManager->SetInput(name, images);
	}
	void VulkanComputePass::SetInput(std::string_view name, Count<class TextureCube> imageCube)
	{
		m_DescritptorSetManager->SetInput(name, imageCube);
	}
	void VulkanComputePass::SetInput(std::string_view name, Count<class UniformBufferSet> buffer)
	{
		m_DescritptorSetManager->SetInput(name, buffer);
	}
	void VulkanComputePass::SetInput(std::string_view name, Count<class StorageBufferSet> buffer)
	{
		m_DescritptorSetManager->SetInput(name, buffer);
	}

	void VulkanComputePass::PushData(std::string_view name, const void* data)
	{
		std::string str = std::string(name);
		auto vkShader = m_Config.Pipeline->GetShader().As<VulkanShader>();
		PF_CORE_ASSERT(vkShader->GetPushConstants().contains(str));
		const auto& pushRange = vkShader->GetPushConstants().at(str);
			
		m_LocalStorage.Copy(data, pushRange.size);
		Count<VulkanComputePass> instance = this;
		Renderer::Submit([instance,str]() 
		{
			instance->RT_PushData(str, instance->m_LocalStorage.Data);
			//instance->m_LocalStorage.Release();
		});
	}
	void VulkanComputePass::RT_PushData(std::string_view name, const void* data)
	{
		PF_CORE_ASSERT(m_RenderPassEnabled, "Cannot dispatch unless start a compute pass");
		auto vkShader = m_Config.Pipeline->GetShader().As<VulkanShader>();
		std::string str = std::string(name);
		PF_CORE_ASSERT(vkShader->GetPushConstants().contains(str));
		const auto& pushRange = vkShader->GetPushConstants().at(str);
		vkCmdPushConstants(m_CommandBuffer.As<VulkanRenderCommandBuffer>()->GetActiveCommandBuffer(), m_Config.Pipeline.As<VulkanComputePipeline>()->GetPipelinelayout(),
			pushRange.stageFlags, pushRange.offset, pushRange.size, data);
	}
}
