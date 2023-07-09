#include "Proofprch.h"
#include "VulkanResourceBuffer.h"
#include "Proof/Core/Core.h"
#include "VulkanCommandBuffer.h"
#include "VulkanGraphicsContext.h"
#include "VulkanRenderer/VulkanRenderer.h"
namespace Proof{

	VulkanUniformBuffer::VulkanUniformBuffer(uint32_t size) :
		m_Size(size)
	{
		auto graphicsContext = VulkanRenderer::GetGraphicsContext();

		m_UniformBuffers.resize(Renderer::GetConfig().FramesFlight);
		VkBufferCreateInfo uniformBufferInfo = {};
		uniformBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		uniformBufferInfo.pNext = nullptr;

		uniformBufferInfo.size = m_Size;
		uniformBufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

		VmaAllocationCreateInfo vmaallocInfo = {};
		vmaallocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
		for (int i = 0; i < m_UniformBuffers.size(); i++)
		{
			graphicsContext->CreateVmaBuffer(uniformBufferInfo, vmaallocInfo, m_UniformBuffers[i]);
		}
	}
	VulkanUniformBuffer::VulkanUniformBuffer(const void* data, uint32_t size)
		:m_Size(size)
	{
		auto graphicsContext = VulkanRenderer::GetGraphicsContext();

		m_UniformBuffers.resize(Renderer::GetConfig().FramesFlight);
		VkBufferCreateInfo uniformBufferInfo = {};
		uniformBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		uniformBufferInfo.pNext = nullptr;

		uniformBufferInfo.size = m_Size;
		uniformBufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

		VmaAllocationCreateInfo vmaallocInfo = {};
		vmaallocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
		for (int i = 0; i < m_UniformBuffers.size(); i++)
		{
			graphicsContext->CreateVmaBuffer(uniformBufferInfo, vmaallocInfo, m_UniformBuffers[i]);
		}
		if (data != nullptr)
			SetData(data, size, 0, Renderer::GetCurrentFrame().FrameinFlight);
	}
	VulkanUniformBuffer::~VulkanUniformBuffer() {
		auto graphicsContext = VulkanRenderer::GetGraphicsContext();
		for (int i = 0; i < m_UniformBuffers.size(); i++)
		{
			Renderer::SubmitDatafree([context = graphicsContext, buffer = m_UniformBuffers[i]]() {
				vmaDestroyBuffer(context->GetVMA_Allocator(), buffer.Buffer, buffer.Allocation);
			});
		}
	}
	void VulkanUniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset, uint32_t frameIndex) {
		auto graphicsContext = VulkanRenderer::GetGraphicsContext();
		VulkanBuffer stagingBuffer;

		VkBufferCreateInfo stagingBufferInfo = {};
		stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		stagingBufferInfo.pNext = nullptr;

		stagingBufferInfo.size = size;
		stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

		//let the VMA library know that this data should be on CPU RAM
		VmaAllocationCreateInfo vmaallocInfo = {};
		vmaallocInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;

		graphicsContext->CreateVmaBuffer(stagingBufferInfo, vmaallocInfo, stagingBuffer);
		void* stagingData;
		vmaMapMemory(graphicsContext->GetVMA_Allocator(), stagingBuffer.Allocation, &stagingData);

		memcpy(stagingData, data, size);

		vmaUnmapMemory(graphicsContext->GetVMA_Allocator(), stagingBuffer.Allocation);

		Renderer::SubmitCommand([&](CommandBuffer* cmdBuffer) {
			VkBufferCopy copy;
			copy.dstOffset = offset;
			copy.srcOffset = 0;
			copy.size = size;
			vkCmdCopyBuffer(cmdBuffer->As<VulkanCommandBuffer>()->GetCommandBuffer(), stagingBuffer.Buffer, m_UniformBuffers[frameIndex].Buffer, 1, &copy);
		});
		vmaDestroyBuffer(graphicsContext->GetVMA_Allocator(), stagingBuffer.Buffer, stagingBuffer.Allocation);
	}

	VulkanStorageBuffer::~VulkanStorageBuffer()
	{
		auto allocator = VulkanRenderer::GetGraphicsContext()->GetVMA_Allocator();

		for (int i = 0; i < m_StorageBuffer.size(); i++)
		{
			Renderer::SubmitDatafree([allocator = allocator, buffer = m_StorageBuffer[i]]() {
				vmaDestroyBuffer(allocator, buffer.Buffer, buffer.Allocation);
			});
		}
	}

	VulkanStorageBuffer::VulkanStorageBuffer(const void* data, uint32_t size, uint32_t offset, uint32_t frameIndex)
	{
		m_StorageBuffer.resize(Renderer::GetConfig().FramesFlight);
		if (size == 0)
		{
			PF_ENGINE_WARN("Cannot set storage buffer with size 0");
			return;
		}
		m_Size = size;
		auto graphicsContext = VulkanRenderer::GetGraphicsContext();

		{
			Renderer::SubmitCommand([&](CommandBuffer* cmdBuffer) {

				m_StorageBuffer.resize(Renderer::GetConfig().FramesFlight);
				VkBufferCreateInfo uniformBufferInfo = {};
				uniformBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
				uniformBufferInfo.pNext = nullptr;

				uniformBufferInfo.size = m_Size;
				uniformBufferInfo.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

				VmaAllocationCreateInfo vmaallocInfo = {};
				vmaallocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
				for (int i = 0; i < m_StorageBuffer.size(); i++)
				{
					graphicsContext->CreateVmaBuffer(uniformBufferInfo, vmaallocInfo, m_StorageBuffer[i]);
				}
			});
		}

		VulkanBuffer stagingBuffer;

		VkBufferCreateInfo stagingBufferInfo = {};
		stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		stagingBufferInfo.pNext = nullptr;

		stagingBufferInfo.size = m_Size;
		stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

		//let the VMA library know that this data should be on CPU RAM
		VmaAllocationCreateInfo vmaallocInfo = {};
		vmaallocInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;

		graphicsContext->CreateVmaBuffer(stagingBufferInfo, vmaallocInfo, stagingBuffer);
		void* stagingData;
		vmaMapMemory(graphicsContext->GetVMA_Allocator(), stagingBuffer.Allocation, &stagingData);

		memcpy(stagingData, data, m_Size);

		vmaUnmapMemory(graphicsContext->GetVMA_Allocator(), stagingBuffer.Allocation);
		Renderer::SubmitCommand([&](CommandBuffer* cmdBuffer) {
			VkBufferCopy copy;
			copy.dstOffset = offset;
			copy.srcOffset = 0;
			copy.size = m_Size;
			vkCmdCopyBuffer(cmdBuffer->As<VulkanCommandBuffer>()->GetCommandBuffer(), stagingBuffer.Buffer, m_StorageBuffer[Renderer::GetCurrentFrame().FrameinFlight].Buffer, 1, &copy);
		});
		vmaDestroyBuffer(graphicsContext->GetVMA_Allocator(), stagingBuffer.Buffer, stagingBuffer.Allocation);

	}
}
