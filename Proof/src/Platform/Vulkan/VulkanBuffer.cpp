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
	VulkanVertexBuffer::VulkanVertexBuffer(void* data, size_t size) {
		m_VertexCount = size;
		auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();

		Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->CreateBuffer(
			m_VertexCount,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,// we are creating to hold vertex input data
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, // host visible wants meory accessible from cpu, 
			m_VertexBuffer,
			m_VertexBufferMemory
		);
		void* vertexData;
		vkMapMemory(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), m_VertexBufferMemory, 0, m_VertexCount, 0, &vertexData);

		memcpy(vertexData, data, (size_t)m_VertexCount);
		vkUnmapMemory(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), m_VertexBufferMemory);
	}
	void VulkanVertexBuffer::Bind(VkCommandBuffer commandBuffer) {
		VkBuffer buffers[] = { m_VertexBuffer };
		VkDeviceSize offset[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offset);
	}
	VulkanIndexBuffer::VulkanIndexBuffer(void* data, uint32_t size) {
		m_Size = size * sizeof(uint32_t);
		auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();
		
		Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->CreateBuffer(
			m_Size,
			VK_BUFFER_USAGE_INDEX_BUFFER_BIT,// we are creating to hold vertex input data
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, // host visible wants meory accessible from cpu, 
			m_IndexBuffer,
			m_IndexBufferMemory
		);
		void* vertexData;
		vkMapMemory(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), m_IndexBufferMemory, 0, m_Size, 0, &vertexData);

		memcpy(vertexData, data, m_Size);
		vkUnmapMemory(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), m_IndexBufferMemory);

	}
	VulkanIndexBuffer::~VulkanIndexBuffer() {
	}
	void VulkanIndexBuffer::Bind(VkCommandBuffer commandBuffer) {
		vkCmdBindIndexBuffer(commandBuffer, m_IndexBuffer, 0, VK_INDEX_TYPE_UINT32);
	}

}
