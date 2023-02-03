#pragma once
#include "Proof/Core/Core.h"
#include<vulkan/vulkan.h>
#include "Proof/Renderer/Shader.h"
#include "Proof/Renderer/PushConstant.h"
namespace Proof
{
	class VulkanPushConstant : public PushConstant {
	public:
		VulkanPushConstant(uint32_t size, uint32_t offset = 0, ShaderStage stage = ShaderStage::Vertex);
		uint32_t GetSize() {
			return m_Size;
		}
		const VkPushConstantRange GetRange();
		virtual ~VulkanPushConstant(){};
		void PushData(Count<class RenderCommandBuffer>commandBuffer,Count<class PipeLineLayout> pipeLinelayout, const void* data);
	private:
		ShaderStage m_ShaderStage;
		uint32_t m_Size;
		uint32_t m_Offset = 0;
		friend class VulkanPipeLineLayout;
	};
}
