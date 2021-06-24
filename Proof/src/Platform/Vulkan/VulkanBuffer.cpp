#include "Proofprch.h"
#include "VulkanBuffer.h"

namespace Proof {
	void VulkanVertexBuffer::BindVertexBuffer() {
		//vkCmdBindPipeline(vertexBuffer,VK_PIPELINE_BIND_POINT_GRAPHICS,Graphics->graphicsPipeline);
		//vkCmdBindVertexBuffers(VkCommandBuffer
	}
	void VulkanVertexBuffer::AddVertexBufferData(void * Data,unsigned int Size) {
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = Size;
		bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		if (vkCreateBuffer(Graphics->device,&bufferInfo,nullptr,&vertexBuffer) != VK_SUCCESS) {
			PF_ENGINE_ERROR("Vulkan failed to create vertex buffer!");
		}
		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(Graphics->device,vertexBuffer,&memRequirements);
		vkBindBufferMemory(Graphics->device,vertexBuffer,vertexBufferMemory,0);

		vkMapMemory(Graphics->device,vertexBufferMemory,0,bufferInfo.size,0,&Data);
		//memcpy(Data,Graphics->vertices.data(),(size_t)bufferInfo.size);
		vkUnmapMemory(Graphics->device,vertexBufferMemory);
	}
	void VulkanVertexBuffer::UnBind() {
		IsDeleted = true;

		vkDestroyBuffer(Graphics->device,vertexBuffer,nullptr);
		vkFreeMemory(Graphics->device,vertexBufferMemory,nullptr);
	}
	VulkanVertexBuffer::~VulkanVertexBuffer() {
		if (IsDeleted == false) {
			vkDestroyBuffer(Graphics->device,vertexBuffer,nullptr);
		}
	}
	uint32_t VulkanVertexBuffer::FindMemoryType(uint32_t typeFilter,VkMemoryPropertyFlags properties) {
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(Graphics->physicalDevice,&memProperties);
		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
				return i;
			}
		}
		PF_ENGINE_ERROR("Vulkan failed to find suitable memory type! VertexBuffer");
	}
}