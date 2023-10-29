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
	void VulkanComputePass::Dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ)
	{
		Count<VulkanComputePass> instance = this;
		Renderer::Submit([instance, groupCountX,groupCountY,groupCountZ] () mutable
		{
			PF_CORE_ASSERT(instance->m_RenderPassEnabled, "Cannot dispatch unless start a compute pass");
			VkCommandBuffer buffer = instance->m_CommandBuffer.As<VulkanRenderCommandBuffer>()->GetActiveCommandBuffer();
			PF_CORE_ASSERT(buffer);
			vkCmdDispatch(buffer, groupCountX, groupCountY, groupCountZ);
		});

	}
	void VulkanComputePass::Build()
	{
	}
	void VulkanComputePass::BeginComputePassBase(Count<class RenderCommandBuffer> command)
	{
		Count<VulkanComputePass> instance = this;
		Renderer::Submit([instance, command]()
		{
			PF_CORE_ASSERT(instance->m_RenderPassEnabled == false, fmt::format("cannot start {} render pass when previous render pass is not closed", instance->m_Config.DebugName).c_str());
			instance->m_CommandBuffer = command;
			instance->m_RenderPassEnabled = true;

			vkCmdBindPipeline(instance->m_CommandBuffer.As<VulkanRenderCommandBuffer>()->GetActiveCommandBuffer(), VK_PIPELINE_BIND_POINT_COMPUTE, instance->m_Config.Pipeline.As<VulkanComputePipeline>()->GetComputePipeline());
		});

	}
	void VulkanComputePass::BeginComputePass(Count<class RenderCommandBuffer> command)
	{
		BeginComputePassBase(command);

		Count<VulkanComputePass> instance = this;
		Renderer::Submit([instance]()
		{
			instance->m_DescritptorSetManager->RT_Bind();
			auto& frameSet = instance->m_DescritptorSetManager->GetDescriptorSets()[Renderer::RT_GetCurrentFrameInFlight()];
			for (auto& [set, setInfo] : frameSet)
			{
				// basically we have to define a set layout for each descriptor set 0-3
				// but some set may not have data and we do note creata a descriptor set for it
				// so we basically just seeing if thats teh case we dont bind it
				if (setInfo.Set == nullptr)
					continue;
				vkCmdBindDescriptorSets(
					instance->m_CommandBuffer.As<VulkanRenderCommandBuffer>()->GetActiveCommandBuffer(),
					VK_PIPELINE_BIND_POINT_COMPUTE,
					instance->m_Config.Pipeline.As<VulkanComputePipeline>()->GetPipelinelayout(),
					(int)set,
					1,
					&setInfo.Set,
					0,
					nullptr);
			}
		});
	}
	void VulkanComputePass::BeginRenderMaterialComputePass(Count<class RenderCommandBuffer> command)
	{
		BeginComputePassBase(command);

		Count<VulkanComputePass> instance = this;
		Renderer::Submit([instance]()
		{
			PF_CORE_ASSERT(instance->m_MaterialRenderPass == false, "cannot start material render pass if previous material render pass not disabled");

			instance->m_MaterialRenderPass = true;

			
			instance->m_DescritptorSetManager->RT_Bind();

			auto& frameSet = instance->m_DescritptorSetManager->GetDescriptorSets()[Renderer::RT_GetCurrentFrameInFlight()];
			for (auto& [set, setInfo] : frameSet)
			{
				// set0 is for te material to bind to 
				// basically we have to define a set layout for each descriptor set 0-3
				// but some set may not have data and we do note creata a descriptor set for it
				// so we basically just seeing if thats teh case we dont bind it
				if (set == 0 || setInfo.Set == nullptr)continue;
				vkCmdBindDescriptorSets(
					instance->m_CommandBuffer.As<VulkanRenderCommandBuffer>()->GetActiveCommandBuffer(),
					VK_PIPELINE_BIND_POINT_COMPUTE,
					instance->m_Config.Pipeline.As<VulkanComputePipeline>()->GetPipelinelayout(),
					(int)set,
					1,
					&setInfo.Set,
					0,
					nullptr);
			}
		});
	}
	void VulkanComputePass::ComputePassPushRenderMaterial(Count<class RenderMaterial> renderMaterial)
	{
		
		Count<VulkanComputePass> instance = this;
		Renderer::Submit([instance, renderMaterial]()
		{
			PF_CORE_ASSERT(instance->m_RenderPassEnabled == true, "cannot Push material fi render pass not enabled");
			PF_CORE_ASSERT(instance->m_MaterialRenderPass == true, "cannot Push if not a material Render Pass");
			renderMaterial.As<VulkanRenderMaterial>()->RT_Bind(instance->m_CommandBuffer.As<VulkanRenderCommandBuffer>(), instance);
		});
	}
	void VulkanComputePass::EndComputePass()
	{
		Count<VulkanComputePass> instance = this;
		Renderer::Submit([instance]()
		{
			PF_CORE_ASSERT(instance->m_RenderPassEnabled == true, "cannot End render pass when render pass is not started");
			instance->m_CommandBuffer = nullptr;
			instance->m_MaterialRenderPass = false;
			instance->m_RenderPassEnabled = false;
		});

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
			instance->m_LocalStorage.Release();
		});
	}
	void VulkanComputePass::RT_PushData(std::string_view name, const void* data)
	{
		auto vkShader = m_Config.Pipeline->GetShader().As<VulkanShader>();
		std::string str = std::string(name);
		PF_CORE_ASSERT(vkShader->GetPushConstants().contains(str));
		const auto& pushRange = vkShader->GetPushConstants().at(str);
		vkCmdPushConstants(m_CommandBuffer.As<VulkanRenderCommandBuffer>()->GetActiveCommandBuffer(), m_Config.Pipeline.As<VulkanComputePipeline>()->GetPipelinelayout(),
			pushRange.stageFlags, pushRange.offset, pushRange.size, data);
	}
}
