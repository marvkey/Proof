#include "Proofprch.h"
#include "VulkanBuffer.h"
#include "Proof/Renderer/Renderer.h"
#include "VulkanGraphicsContext.h"
namespace Proof
{
	VulkanVertexBuffer::VulkanVertexBuffer(const void* data, uint32_t size) {
		m_VertexCount = size;
		Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->CreateBuffer(
			m_VertexCount,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,// we are creating to hold vertex input data
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, // host visible wants meory accessible from cpu, 
			m_VertexBuffer,
			m_VertexBufferMemory
		);

		void* vertexData;
		// sets data to point begeinign of the map memory range(offset is 0)
		vkMapMemory(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), m_VertexBufferMemory, 0,m_VertexCount,0, &vertexData);

		memcpy(vertexData, data, (size_t)m_VertexCount);
		vkUnmapMemory(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), m_VertexBufferMemory);
	}
	void VulkanVertexBuffer::Bind(VkCommandBuffer commandBuffer) {
		VkBuffer buffers[] = { m_VertexBuffer };
		VkDeviceSize offset[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offset);
	}
}
