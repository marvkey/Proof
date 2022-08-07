#include "Proofprch.h"
#include "VulkanBuffer.h"
#include "Proof/Renderer/Renderer.h"
#include "VulkanGraphicsContext.h"
#include <vulkan/VulkanProofExternalLibs/vk_mem_alloc.h>
#include "VulkanRenderer/VulkanRenderer.h"
namespace Proof
{
	VulkanVertexBuffer::~VulkanVertexBuffer() {
		auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();
		vkDestroyBuffer(graphicsContext->GetDevice(), m_VertexBuffer, nullptr);
		vkFreeMemory(graphicsContext->GetDevice(), m_VertexBufferMemory, nullptr);
	}
	VulkanVertexBuffer::VulkanVertexBuffer(const void* data, uint32_t size,uint32_t count) {
		m_VertexSize = size;
		m_Count = count;
		auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		graphicsContext->CreateBuffer(m_VertexSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);
		void* stagingData;
		vkMapMemory(graphicsContext->GetDevice(), stagingBufferMemory, 0, m_VertexSize, 0, &stagingData);

		memcpy(stagingData, data, m_VertexSize);
		vkUnmapMemory(graphicsContext->GetDevice(), stagingBufferMemory);


		graphicsContext->CreateBuffer(
			m_VertexSize,
			VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
			m_VertexBuffer,
			m_VertexBufferMemory
		);
		CopyBuffer(stagingBuffer, m_VertexBuffer, size);
	}
	void VulkanVertexBuffer::Bind(VkCommandBuffer commandBuffer) {
		VkBuffer buffers[] = { m_VertexBuffer };
		VkDeviceSize offset[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offset);
	}
	VulkanIndexBuffer::VulkanIndexBuffer(const void* data, uint32_t size) {
		m_Size = size * sizeof(uint32_t);
		m_Count = size;
		auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		graphicsContext->CreateBuffer(m_Size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);
		void* stagingData;
		vkMapMemory(graphicsContext->GetDevice(), stagingBufferMemory, 0, m_Size, 0, &stagingData);

		memcpy(stagingData, data, m_Size);
		vkUnmapMemory(graphicsContext->GetDevice(), stagingBufferMemory);
		graphicsContext->CreateBuffer(
			m_Size,
			VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
			m_IndexBuffer,
			m_IndexBufferMemory
		);
		CopyBuffer(stagingBuffer, m_IndexBuffer, m_Size);
	}
	VulkanIndexBuffer::~VulkanIndexBuffer() {
	
	}
	void VulkanIndexBuffer::Bind(VkCommandBuffer commandBuffer) {
		vkCmdBindIndexBuffer(commandBuffer, m_IndexBuffer, 0, VK_INDEX_TYPE_UINT32);
	}

	void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
		auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = graphicsContext->GetCommandPool();
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(graphicsContext->GetDevice(), &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		VkBufferCopy copyRegion{};
		copyRegion.size = size;
		vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(graphicsContext->GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(graphicsContext->GetGraphicsQueue());

		vkFreeCommandBuffers(graphicsContext->GetDevice(), graphicsContext->GetCommandPool(), 1, &commandBuffer);
	}

}
