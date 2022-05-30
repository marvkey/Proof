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
		virtual void SetData(void* data, uint32_t size, uint32_t offset = 0);
		virtual void Bind();
		virtual void Recreate();
		VkDescriptorBufferInfo DescriptorInfo(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0) {
			return VkDescriptorBufferInfo{
			m_UniformBuffer,
			offset,
			size,
			};
		}
		VkBuffer GetBuffer() {
			return m_UniformBuffer;
		}
		uint32_t GetSize() {
			return m_Size;
		}
	private:
		VkBuffer m_UniformBuffer =nullptr;
		VkDeviceMemory m_UniformBufferMemory =nullptr;
		void* m_Mapped = nullptr;
		uint32_t m_Size = 0;
		uint32_t m_Binding = 0;
	};
}