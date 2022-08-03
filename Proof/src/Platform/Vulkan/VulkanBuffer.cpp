#include "Proofprch.h"
#include "VulkanBuffer.h"
#include "Proof/Renderer/Renderer.h"
#include "VulkanGraphicsContext.h"
#include <vulkan/VulkanProofExternalLibs/vk_mem_alloc.h>

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

		graphicsContext->CreateBuffer(
			m_VertexSize,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,// we are creating to hold vertex input data
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, // host visible wants meory accessible from cpu, 
			m_VertexBuffer,
			m_VertexBufferMemory
		);
		void* vertexData;
		vkMapMemory(graphicsContext->GetDevice(), m_VertexBufferMemory, 0, m_VertexSize, 0, &vertexData);

		memcpy(vertexData, data, (size_t)m_VertexSize);
		vkUnmapMemory(graphicsContext->GetDevice(), m_VertexBufferMemory);
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
		
		graphicsContext->CreateBuffer(
			m_Size,
			VK_BUFFER_USAGE_INDEX_BUFFER_BIT,// we are creating to hold vertex input data
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, // host visible wants meory accessible from cpu, 
			m_IndexBuffer,
			m_IndexBufferMemory
		);
		void* indexData;
		vkMapMemory(graphicsContext->GetDevice(), m_IndexBufferMemory, 0, m_Size, 0, &indexData);

		memcpy(indexData, data, m_Size);
		vkUnmapMemory(graphicsContext->GetDevice(), m_IndexBufferMemory);

	}
	VulkanIndexBuffer::~VulkanIndexBuffer() {
	
	}
	void VulkanIndexBuffer::Bind(VkCommandBuffer commandBuffer) {
		vkCmdBindIndexBuffer(commandBuffer, m_IndexBuffer, 0, VK_INDEX_TYPE_UINT32);
	}

}
