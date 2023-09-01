#include "Proofprch.h"
#include "VulkanComputePass.h"
#include "VulkanComputePipeline.h"
#include "VulkanCommandBuffer.h"
#include "VulkanRenderMaterial.h"
#include "VulkanRenderer/VulkanRenderer.h"
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
		PF_CORE_ASSERT(m_RenderPassEnabled, "Cannot dispatch unless start a compute pass");
		vkCmdDispatch(m_CommandBuffer.As<VulkanRenderCommandBuffer>()->GetCommandBuffer(Renderer::GetCurrentFrame().FrameinFlight), groupCountX, groupCountY, groupCountZ);
	}
	void VulkanComputePass::Build()
	{
	}
	void VulkanComputePass::BeginComputePassBase(Count<class RenderCommandBuffer> command)
	{
		PF_CORE_ASSERT(m_RenderPassEnabled == false, fmt::format("cannot start {} render pass when previous render pass is not closed", m_Config.DebugName).c_str());
		m_CommandBuffer = command;
		m_RenderPassEnabled = true;

		vkCmdBindPipeline(m_CommandBuffer.As<VulkanRenderCommandBuffer>()->GetCommandBuffer(Renderer::GetCurrentFrame().FrameinFlight), VK_PIPELINE_BIND_POINT_COMPUTE, m_Config.Pipeline.As<VulkanComputePipeline>()->GetComputePipeline());
	}
	void VulkanComputePass::BeginComputePass(Count<class RenderCommandBuffer> command)
	{
		BeginComputePassBase(command);
		m_DescritptorSetManager->Bind();
		auto& frameSet = m_DescritptorSetManager->GetDescriptorSets()[Renderer::GetCurrentFrame().FrameinFlight];
		for (auto& [set, setInfo] : frameSet)
		{
			// basically we have to define a set layout for each descriptor set 0-3
			// but some set may not have data and we do note creata a descriptor set for it
			// so we basically just seeing if thats teh case we dont bind it
			if (setInfo.Set == nullptr)
				continue;
			vkCmdBindDescriptorSets(
				m_CommandBuffer.As<VulkanRenderCommandBuffer>()->GetCommandBuffer(Renderer::GetCurrentFrame().FrameinFlight),
				VK_PIPELINE_BIND_POINT_COMPUTE,
				m_Config.Pipeline.As<VulkanComputePipeline>()->GetPipelinelayout(),
				(int)set,
				1,
				&setInfo.Set,
				0,
				nullptr);
		}
	}
	void VulkanComputePass::BeginRenderMaterialComputePass(Count<class RenderCommandBuffer> command)
	{
		BeginComputePassBase(command);
		PF_CORE_ASSERT(m_MaterialRenderPass == false, "cannot start material render pass if previous material render pass not disabled");

		m_MaterialRenderPass = true;

		m_DescritptorSetManager->Bind();

		auto& frameSet = m_DescritptorSetManager->GetDescriptorSets()[Renderer::GetCurrentFrame().FrameinFlight];
		for (auto& [set, setInfo] : frameSet)
		{
			// set0 is for te material to bind to 
			// basically we have to define a set layout for each descriptor set 0-3
			// but some set may not have data and we do note creata a descriptor set for it
			// so we basically just seeing if thats teh case we dont bind it
			if (set == 0 || setInfo.Set == nullptr)continue;
			vkCmdBindDescriptorSets(
				m_CommandBuffer.As<VulkanRenderCommandBuffer>()->GetCommandBuffer(Renderer::GetCurrentFrame().FrameinFlight),
				VK_PIPELINE_BIND_POINT_COMPUTE,
				m_Config.Pipeline.As<VulkanComputePipeline>()->GetPipelinelayout(),
				(int)set,
				1,
				&setInfo.Set,
				0,
				nullptr);
		}
	}
	void VulkanComputePass::ComputePassPushRenderMaterial(Count<class RenderMaterial> renderMaterial)
	{
		PF_CORE_ASSERT(m_RenderPassEnabled == true, "cannot Push material fi render pass not enabled");
		PF_CORE_ASSERT(m_MaterialRenderPass == true, "cannot Push if not a material Render Pass");

		Count< VulkanComputePass> pass = this;
		renderMaterial.As<VulkanRenderMaterial>()->Bind(m_CommandBuffer.As<VulkanRenderCommandBuffer>(), pass);
	}
	void VulkanComputePass::EndComputePass()
	{
		PF_CORE_ASSERT(m_RenderPassEnabled == true, "cannot End render pass when render pass is not started");
		m_CommandBuffer =nullptr;
		m_MaterialRenderPass = false;
		m_RenderPassEnabled = false;
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
		auto vkShader = m_Config.Pipeline->GetShader().As<VulkanShader>();
		std::string str = std::string(name);
		PF_CORE_ASSERT(vkShader->GetPushConstants().contains(str));
		const auto& pushRange = vkShader->GetPushConstants().at(str);
		vkCmdPushConstants(m_CommandBuffer.As<VulkanRenderCommandBuffer>()->GetCommandBuffer(Renderer::GetCurrentFrame().FrameinFlight), m_Config.Pipeline.As<VulkanComputePipeline>()->GetPipelinelayout(),
			pushRange.stageFlags, pushRange.offset, pushRange.size, data);
	}
}
