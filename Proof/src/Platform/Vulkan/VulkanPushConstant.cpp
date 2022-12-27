#include "Proofprch.h"
#include "VulkanPushConstant.h"
#include "VulkanUtils/VulkanConvert.h"
#include "VulkanCommandBuffer.h"
#include "VulkanPipeLineLayout.h"
namespace Proof
{
	VulkanPushConstant::VulkanPushConstant(uint32_t size, uint32_t offset, ShaderStage flags)
	{
		//this push constant range starts at the beginning
		m_PushConstant.offset = offset;
		//this push constant range takes up the size of the data
		m_PushConstant.size = size;
		//this push constant range is accessible only in the vertex shader
		m_PushConstant.stageFlags = Utils::ProofShaderToVulkanShader(flags);
		m_size = size;
	}
	void VulkanPushConstant::Bind(Count<class CommandBuffer> commandBuffer, Count<class PipeLineLayout> pipeLinelayout, const void* pValues)
	{
		vkCmdPushConstants(commandBuffer->As<VulkanCommandBuffer>()->GetCommandBuffer(), pipeLinelayout->As<VulkanPipeLineLayout>()->GetPipeLineLayout(), m_PushConstant.stageFlags, m_PushConstant.offset, m_PushConstant.size, &pValues);

	}
}
