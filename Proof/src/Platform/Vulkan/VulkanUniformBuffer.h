#pragma once
#include "Proof/Renderer/UniformBuffer.h"
#include<vector>
#include<vulkan/vulkan.h>
namespace Proof
{
	class VulkanUniformBuffer : public UniformBuffer{
	public:
		VulkanUniformBuffer(uint32_t size, uint32_t binding);
		~VulkanUniformBuffer();
		virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0);

	private:
		VkBuffer m_UniformBuffer =nullptr;
		VkDeviceMemory m_UniformBufferMemory =nullptr;
		void* m_Mapped = nullptr;
	};
}