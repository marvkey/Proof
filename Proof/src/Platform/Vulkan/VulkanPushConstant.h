#pragma once
#include "Proof/Core/Core.h"
#include<vulkan/vulkan.h>
#include "Proof/Renderer/Shader.h"
#include "Proof/Renderer/PushConstant.h"
namespace Proof
{
	class VulkanPushConstant : public PushConstant {
	public:
		VulkanPushConstant(uint32_t size, uint32_t offset = 0, ShaderStage flags = ShaderStage::Vertex);
		uint32_t GetSize() {
			return m_size;
		}
		const VkPushConstantRange& GetRange() {
			return m_PushConstant;
		}
		virtual ~VulkanPushConstant(){};
		void Bind(Count<class CommandBuffer>commandBuffer,Count<class PipeLineLayout> pipeLinelayout, const void* pValues);
	private:
		uint32_t m_size;
		VkPushConstantRange m_PushConstant;
		friend class VulkanPipeLineLayout;
	};
}
