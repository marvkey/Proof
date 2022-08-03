#pragma once
#include "Proof/Core/Core.h"
#include<vulkan/vulkan.h>
namespace Proof
{
	class VulkanPushConstant {
	public:
		uint32_t GetSize() {
			return m_size;
		}
		VulkanPushConstant::VulkanPushConstant(uint32_t size, uint32_t offset = 0, VkShaderStageFlags flags = VK_SHADER_STAGE_VERTEX_BIT ) {
			//this push constant range starts at the beginning
			m_PushConstant.offset = offset;
			//this push constant range takes up the size of the data
			m_PushConstant.size = size;
			//this push constant range is accessible only in the vertex shader
			m_PushConstant.stageFlags = flags;
			m_size = size;
		}
		void Bind(VkCommandBuffer buffer, VkPipelineLayout layout, VkShaderStageFlags flags,uint32_t size, void* data) {
			vkCmdPushConstants(buffer, layout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, size, &data);
		}
	private:
		uint32_t m_size;
		VkPushConstantRange m_PushConstant;
		friend class VulkanPipeLineLayout;
	};
}
