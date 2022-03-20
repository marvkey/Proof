#pragma once
#include "Proof/Renderer/Buffer.h"
#include "Proof/Core/Core.h"
#include<vulkan/vulkan.h>
namespace Proof
{
	struct VulkanVertex {
		glm::vec2 position;
		// how to read our data
		static std::vector<VkVertexInputBindingDescription> GetBindingDescriptions() {
			std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
			bindingDescriptions[0].binding = 0;
			bindingDescriptions[0].stride = sizeof(VulkanVertex);
			bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
			return bindingDescriptions;
		}
		static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions() {
			std::vector<VkVertexInputAttributeDescription> attributeDescriptions(1);
			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
			attributeDescriptions[0].offset = 0;
			return attributeDescriptions;
		}
	};
	class VulkanVertexBuffer : public VertexBuffer {
	public:
		VulkanVertexBuffer(const void* data, uint32_t size);
		virtual void Bind(VkCommandBuffer commandBuffer);
		virtual void AddData(const void* Data, uint32_t Size, uint32_t SizeOfVertexBuffer = 0){}
		/**
		*removes as Current vertex Buffer
		*/
		virtual void UnBind(){}

	private:
		VkBuffer m_VertexBuffer;
		VkDeviceMemory m_VertexBufferMemory;
		uint32_t m_VertexCount;
	};
}