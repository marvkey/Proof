#include "Proofprch.h"
#include "VulkanBuffer.h"
#include "Proof/Renderer/RendererBase.h"
#include "Proof/Renderer/Renderer.h"
#include "Proof/Renderer/Vertex.h"
#include "VulkanGraphicsContext.h"
#include <vulkan/VulkanProofExternalLibs/vk_mem_alloc.h>
#include "VulkanRenderer/VulkanRenderer.h"
#include "VulkanCommandBuffer.h"
namespace Proof
{
	VulkanVertexBuffer::~VulkanVertexBuffer() {
		Renderer::SubmitDatafree([vertexBuffer = m_VertexBuffer]() {
			auto graphicsContext = VulkanRenderer::GetGraphicsContext();
			vmaDestroyBuffer(graphicsContext->GetVMA_Allocator(), vertexBuffer.Buffer, vertexBuffer.Allocation);
		});
	}
	VulkanVertexBuffer::VulkanVertexBuffer(const void* data, uint32_t size) {
		m_VertexSize = size;
		if (m_VertexSize == 0)
		{
			PF_ENGINE_ERROR("Vertex buffer cannot be createed with a size of 0 ");
			return;
		}
		auto graphicsContext = VulkanRenderer::GetGraphicsContext();

		VkBufferCreateInfo stagingBufferInfo = {};
		stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		stagingBufferInfo.pNext = nullptr;

		stagingBufferInfo.size = m_VertexSize;
		stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

		//let the VMA library know that this data should be on CPU RAM
		VmaAllocationCreateInfo vmaallocInfo = {};
		vmaallocInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;

		VulkanBuffer stagingBuffer;

		graphicsContext->CreateVmaBuffer(stagingBufferInfo, vmaallocInfo, stagingBuffer);
		void* stagingData;
		vmaMapMemory(graphicsContext->GetVMA_Allocator(), stagingBuffer.Allocation, &stagingData);

		memcpy(stagingData, data, m_VertexSize);

		vmaUnmapMemory(graphicsContext->GetVMA_Allocator(), stagingBuffer.Allocation);
		Renderer::SubmitCommand([&](CommandBuffer* cmdBuffer) {
			VkBufferCreateInfo vertexBufferInfo = {};
			vertexBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			vertexBufferInfo.pNext = nullptr;
			//this is the total size, in bytes, of the buffer we are allocating
			vertexBufferInfo.size = m_VertexSize;
			//this buffer is going to be used as a Vertex Buffer
			// ad goign to be a desitination bit
			vertexBufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

			//let the VMA library know that this data should be GPU native
			vmaallocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

			graphicsContext->CreateVmaBuffer(vertexBufferInfo, vmaallocInfo, m_VertexBuffer);

			VkBufferCopy copy;
			copy.dstOffset = 0;
			copy.srcOffset = 0;
			copy.size = m_VertexSize;
			vkCmdCopyBuffer(cmdBuffer->As<VulkanCommandBuffer>()->GetCommandBuffer(), stagingBuffer.Buffer, m_VertexBuffer.Buffer, 1, &copy);
		});
		vmaDestroyBuffer(graphicsContext->GetVMA_Allocator(), stagingBuffer.Buffer, stagingBuffer.Allocation);
	}
	VulkanVertexBuffer::VulkanVertexBuffer(uint32_t size) {
		m_VertexSize = size;
		if (m_VertexSize == 0)
		{
			PF_ENGINE_ERROR("Vertex buffer cannot be createed with a size of 0 ");
			return;
		}
		auto graphicsContext = VulkanRenderer::GetGraphicsContext();
		Renderer::SubmitCommand([&](CommandBuffer* cmdBuffer) {
			VkBufferCreateInfo vertexBufferInfo = {};
			vertexBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			vertexBufferInfo.pNext = nullptr;
			//this is the total size, in bytes, of the buffer we are allocating
			vertexBufferInfo.size = m_VertexSize;
			//this buffer is going to be used as a Vertex Buffer
			// ad goign to be a desitination bit
			vertexBufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

			VmaAllocationCreateInfo vmaallocInfo = {};
				//let the VMA library know that this data should be GPU native
			vmaallocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

			graphicsContext->CreateVmaBuffer(vertexBufferInfo, vmaallocInfo, m_VertexBuffer);
		});
	}

	void VulkanVertexBuffer::Resize(uint32_t size)
	{
		Release();

		m_VertexSize = size;
		if (m_VertexSize == 0)
		{
			PF_ENGINE_ERROR("Vertex buffer cannot be createed with a size of 0 ");
			return;
		}
		auto graphicsContext = VulkanRenderer::GetGraphicsContext();
		Renderer::SubmitCommand([&](CommandBuffer* cmdBuffer) {
			VkBufferCreateInfo vertexBufferInfo = {};
			vertexBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			vertexBufferInfo.pNext = nullptr;
			//this is the total size, in bytes, of the buffer we are allocating
			vertexBufferInfo.size = m_VertexSize;
			//this buffer is going to be used as a Vertex Buffer
			// ad goign to be a desitination bit
			vertexBufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

			VmaAllocationCreateInfo vmaallocInfo = {};
				//let the VMA library know that this data should be GPU native
			vmaallocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

			graphicsContext->CreateVmaBuffer(vertexBufferInfo, vmaallocInfo, m_VertexBuffer);
		});
	}

	void VulkanVertexBuffer::Resize(const void* data, uint32_t size)
	{
		Release();

		m_VertexSize = size;
		if (m_VertexSize == 0)
		{
			PF_ENGINE_ERROR("Vertex buffer cannot be createed with a size of 0 ");
			return;
		}
		auto graphicsContext = VulkanRenderer::GetGraphicsContext();

		VkBufferCreateInfo stagingBufferInfo = {};
		stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		stagingBufferInfo.pNext = nullptr;

		stagingBufferInfo.size = m_VertexSize;
		stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

		//let the VMA library know that this data should be on CPU RAM
		VmaAllocationCreateInfo vmaallocInfo = {};
		vmaallocInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;

		VulkanBuffer stagingBuffer;

		graphicsContext->CreateVmaBuffer(stagingBufferInfo, vmaallocInfo, stagingBuffer);
		void* stagingData;
		vmaMapMemory(graphicsContext->GetVMA_Allocator(), stagingBuffer.Allocation, &stagingData);

		memcpy(stagingData, data, m_VertexSize);

		vmaUnmapMemory(graphicsContext->GetVMA_Allocator(), stagingBuffer.Allocation);
		Renderer::SubmitCommand([&](CommandBuffer* cmdBuffer) {
			VkBufferCreateInfo vertexBufferInfo = {};
			vertexBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			vertexBufferInfo.pNext = nullptr;
			//this is the total size, in bytes, of the buffer we are allocating
			vertexBufferInfo.size = m_VertexSize;
			//this buffer is going to be used as a Vertex Buffer
			// ad goign to be a desitination bit
			vertexBufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

			//let the VMA library know that this data should be GPU native
			vmaallocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

			graphicsContext->CreateVmaBuffer(vertexBufferInfo, vmaallocInfo, m_VertexBuffer);

			VkBufferCopy copy;
			copy.dstOffset = 0;
			copy.srcOffset = 0;
			copy.size = m_VertexSize;
			vkCmdCopyBuffer(cmdBuffer->As<VulkanCommandBuffer>()->GetCommandBuffer(), stagingBuffer.Buffer, m_VertexBuffer.Buffer, 1, &copy);
		});
		vmaDestroyBuffer(graphicsContext->GetVMA_Allocator(), stagingBuffer.Buffer, stagingBuffer.Allocation);
	}
	VkDeviceSize offset[1] = { 0 };
	void VulkanVertexBuffer::Bind(Count<RenderCommandBuffer> commandBuffer, uint32_t binding )const {
		vkCmdBindVertexBuffers(commandBuffer.As<VulkanRenderCommandBuffer>()->GetCommandBuffer(), binding,1, &m_VertexBuffer.Buffer, offset);
	}
	void VulkanVertexBuffer::SetData(const void* data, uint32_t size,uint32_t offset){
		if (size == 0)
		{
			PF_ENGINE_ERROR("Vertex buffer cannot add data with a size of 0");
			return;
		}
		auto graphicsContext = VulkanRenderer::GetGraphicsContext();

		VkBufferCreateInfo stagingBufferInfo = {};
		stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		stagingBufferInfo.pNext = nullptr;

		stagingBufferInfo.size = size;
		stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

		//let the VMA library know that this data should be on CPU RAM
		VmaAllocationCreateInfo vmaallocInfo = {};
		vmaallocInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;

		VulkanBuffer stagingBuffer;

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
			vkCmdCopyBuffer(cmdBuffer->As<VulkanCommandBuffer>()->GetCommandBuffer(), stagingBuffer.Buffer, m_VertexBuffer.Buffer, 1, &copy);
		});
		vmaDestroyBuffer(graphicsContext->GetVMA_Allocator(), stagingBuffer.Buffer, stagingBuffer.Allocation);
	}
	Buffer VulkanVertexBuffer::GetDataRaw()
	{
		Buffer buffer(m_VertexSize);
		void* vertexData;
		vmaMapMemory(VulkanVertexBuffer::GetGraphicsAllocator(), m_VertexBuffer.Allocation, &vertexData);
		std::memcpy(buffer.Get(), vertexData, m_VertexSize); // so we can use delte
		vmaUnmapMemory(VulkanVertexBuffer::GetGraphicsAllocator(), m_VertexBuffer.Allocation);
		return buffer;
	}

	void VulkanVertexBuffer::Release()
	{
		if (m_VertexBuffer.Buffer == nullptr)
			return;
		Renderer::SubmitDatafree([vertexBuffer = m_VertexBuffer]() {
			auto graphicsContext = VulkanRenderer::GetGraphicsContext();
			vmaDestroyBuffer(graphicsContext->GetVMA_Allocator(), vertexBuffer.Buffer, vertexBuffer.Allocation);
		});
		m_VertexBuffer.Buffer = nullptr;
		m_VertexBuffer.Allocation = nullptr;
	}

	VmaAllocator VulkanVertexBuffer::GetGraphicsAllocator()
	{
		auto graphicsContext = VulkanRenderer::GetGraphicsContext();

		return graphicsContext->GetVMA_Allocator();
	}
	VulkanIndexBuffer::VulkanIndexBuffer(const void* data, uint32_t size) {
		m_Size = size * sizeof(uint32_t);
		m_Count = size;
		if (m_Size == 0)
		{
			PF_ENGINE_ERROR("Index Buffer size of 0 cannot be created");
			return;
		}
		auto graphicsContext = VulkanRenderer::GetGraphicsContext();

		VkBufferCreateInfo stagingBufferInfo = {};
		stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		stagingBufferInfo.pNext = nullptr;

		stagingBufferInfo.size = m_Size;
		stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

		//let the VMA library know that this data should be on CPU RAM
		VmaAllocationCreateInfo vmaallocInfo = {};
		vmaallocInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;

		VulkanBuffer stagingBuffer;

		graphicsContext->CreateVmaBuffer(stagingBufferInfo, vmaallocInfo, stagingBuffer);
		void* stagingData;
		vmaMapMemory(graphicsContext->GetVMA_Allocator(), stagingBuffer.Allocation, &stagingData);

		memcpy(stagingData, data, m_Size);

		vmaUnmapMemory(graphicsContext->GetVMA_Allocator(), stagingBuffer.Allocation);

		Renderer::SubmitCommand([&](CommandBuffer* cmdBuffer) {
			VkBufferCreateInfo indexBufferInfo = {};
			indexBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			indexBufferInfo.pNext = nullptr;
			//this is the total size, in bytes, of the buffer we are allocating
			indexBufferInfo.size = m_Size;
			//this buffer is going to be used as a index buffer
			// ad goign to be a desitination bit
			indexBufferInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

			//let the VMA library know that this data should be GPU native
			vmaallocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

			graphicsContext->CreateVmaBuffer(indexBufferInfo, vmaallocInfo, m_IndexBuffer);

			VkBufferCopy copy;
			copy.dstOffset = 0;
			copy.srcOffset = 0;
			copy.size = m_Size;
			vkCmdCopyBuffer(cmdBuffer->As<VulkanCommandBuffer>()->GetCommandBuffer(), stagingBuffer.Buffer, m_IndexBuffer.Buffer, 1, &copy);
		});
		vmaDestroyBuffer(graphicsContext->GetVMA_Allocator(), stagingBuffer.Buffer, stagingBuffer.Allocation);
	}
	VulkanIndexBuffer::~VulkanIndexBuffer() {
		Renderer::SubmitDatafree([buffer = m_IndexBuffer]() {
			auto graphicsContext = VulkanRenderer::GetGraphicsContext();
			vmaDestroyBuffer(graphicsContext->GetVMA_Allocator(), buffer.Buffer, buffer.Allocation);
		});
	}
	void VulkanIndexBuffer::Bind(Count<RenderCommandBuffer>commandBuffer)const {
		vkCmdBindIndexBuffer(commandBuffer.As<VulkanRenderCommandBuffer>()->GetCommandBuffer(), m_IndexBuffer.Buffer, 0, VK_INDEX_TYPE_UINT32);
	}
	std::vector<uint32_t> VulkanIndexBuffer::GetData()const
	{
		std::vector<uint32_t> data;
		void* vertexData;
		vmaMapMemory(VulkanVertexBuffer::GetGraphicsAllocator(), m_IndexBuffer.Allocation, &vertexData);
		uint32_t* indices = static_cast<uint32_t*>(vertexData);
		for (size_t i = 0; i < m_Count; i++)
		{
			data.emplace_back(indices[i]);
		}
		vmaUnmapMemory(VulkanVertexBuffer::GetGraphicsAllocator(), m_IndexBuffer.Allocation);
		return data;
	}


}
