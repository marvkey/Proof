#pragma once
#include "Proof/Renderer/Buffer.h"
#include "Proof/Core/Core.h"
#include<vulkan/vulkan.h>
#include <glm/glm.hpp>
namespace Proof
{
	struct VulkanVertex {
		glm::vec2 position;
		glm::vec3 color;
		// how to read our data
		static std::vector<VkVertexInputBindingDescription> GetBindingDescriptions() {
			std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
			bindingDescriptions[0].binding = 0;
			bindingDescriptions[0].stride = sizeof(VulkanVertex);
			bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
			return bindingDescriptions;
		}
		static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions() {
			std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2); //2 because they are 2 variables in the vertex
			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
			attributeDescriptions[0].offset = offsetof(VulkanVertex, position);

			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[1].offset = offsetof(VulkanVertex, color);
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
		uint32_t GetVertexCount() { return m_VertexCount; }
	private:
		VkBuffer m_VertexBuffer;
		VkDeviceMemory m_VertexBufferMemory;
		uint32_t m_VertexCount;
	};
}