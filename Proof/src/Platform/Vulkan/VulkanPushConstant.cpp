#include "Proofprch.h"
#include "VulkanPushConstant.h"
#include "VulkanUtils/VulkanConvert.h"
#include "VulkanCommandBuffer.h"
#include "VulkanPipeLineLayout.h"
namespace Proof
{
	VulkanPushConstant::VulkanPushConstant(uint32_t size, uint32_t offset, ShaderStage stage)
	{
		//this push constant range starts at the beginning
		m_Offset= offset;
		m_ShaderStage = stage;
		m_Size = size;
	}
	const VkPushConstantRange VulkanPushConstant::GetRange()
	{
		return
		{
			(uint32_t)Utils::ProofShaderToVulkanShader(m_ShaderStage),
			m_Offset,
			m_Size
		};
	}
	void VulkanPushConstant::PushData(Count<CommandBuffer> commandBuffer, Count<PipeLineLayout> pipeLinelayout, const void* data)
	{
		vkCmdPushConstants(commandBuffer->As<VulkanCommandBuffer>()->GetCommandBuffer(), pipeLinelayout->As<VulkanPipeLineLayout>()->GetPipeLineLayout(), 
			Utils::ProofShaderToVulkanShader(m_ShaderStage), m_Offset, m_Size, data);
	}
}
