#include "Proofprch.h"
#include "VulkanBuffer.h"
#include "Proof/Renderer/Renderer.h"
#include "Proof/Renderer/Vertex.h"
#include "VulkanGraphicsContext.h"
#include <vulkan/VulkanProofExternalLibs/vk_mem_alloc.h>
#include "VulkanRenderer/VulkanRenderer.h"
#include "VulkanCommandBuffer.h"
namespace Proof
{
	
	VulkanVertexBuffer::VulkanVertexBuffer(const void* data, uint64_t size): 
		m_VertexSize(size)
	{
		Build();
		SetData(data, size);
	}
	VulkanVertexBuffer::VulkanVertexBuffer(uint64_t size):
		m_VertexSize(size)
	{
		Build();
	}
	VulkanVertexBuffer::~VulkanVertexBuffer() 
	{
		Release();
	}
	void VulkanVertexBuffer::Build()
	{
		PF_CORE_ASSERT(m_VertexSize, "Vertex Size cannot equal to zero")
		auto graphicsContext = VulkanRenderer::GetGraphicsContext();
		Renderer::SubmitCommand([&](CommandBuffer* cmdBuffer) {
			VkBufferCreateInfo vertexBufferInfo = {};
			vertexBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			vertexBufferInfo.pNext = nullptr;
			//this is the total size, in bytes, of the buffer we are allocating
			vertexBufferInfo.size = m_VertexSize;
			//this buffer is going to be used as a Vertex Buffer
			// ad goign to be a desitination bit
			vertexBufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT ;

			VmaAllocationCreateInfo vmaallocInfo = {};
				//let the VMA library know that this data should be GPU native
			vmaallocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

			graphicsContext->CreateVmaBuffer(vertexBufferInfo, vmaallocInfo, m_VertexBuffer);
		});
	}
	void VulkanVertexBuffer::Resize(uint64_t size)
	{
		Release();

		m_VertexSize = size;
		Build();
	}

	void VulkanVertexBuffer::Resize(const void* data, uint64_t size)
	{
		//TODO WORKING EXPECTED
		//if (m_VertexSize == size)
		//{
		//	SetData(data, m_VertexSize, 0);
		//	return;
		//}
		Release();

		m_VertexSize = size;
		Build();
		SetData(data, size, 0);
	}
	void VulkanVertexBuffer::Bind(Count<RenderCommandBuffer> commandBuffer, uint64_t binding, uint64_t offset )const {
		VkDeviceSize instanceOffset[1] = {(VkDeviceSize) offset };
		vkCmdBindVertexBuffers(commandBuffer.As<VulkanRenderCommandBuffer>()->GetCommandBuffer(Renderer::GetCurrentFrame().FrameinFlight), binding,1, &m_VertexBuffer.Buffer, instanceOffset);
	}
	void VulkanVertexBuffer::SetData(const void* data, uint64_t size,uint64_t offset){
		if (size == 0)
		{
			PF_ENGINE_ERROR("Vertex Cannot set Data with a size of 0");
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
			vkCmdCopyBuffer(cmdBuffer->As<VulkanCommandBuffer>()->GetCommandBuffer(Renderer::GetCurrentFrame().FrameinFlight), stagingBuffer.Buffer, m_VertexBuffer.Buffer, 1, &copy);
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


	VulkanIndexBuffer::VulkanIndexBuffer(uint64_t count)
		:
		m_Count(count)
	{
		Build();
	}

	VulkanIndexBuffer::VulkanIndexBuffer(const void* data, uint64_t count)
	:
		m_Count(count)
	{
		Build();
		SetData(data, m_Count, 0);
	}
	void VulkanIndexBuffer::Build()
	{
		PF_CORE_ASSERT(GetSize(), "Index Size cannot equal to zero");
		auto graphicsContext = VulkanRenderer::GetGraphicsContext();
		Renderer::SubmitCommand([&](CommandBuffer* cmdBuffer) {
			VkBufferCreateInfo vertexBufferInfo = {};
			vertexBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			vertexBufferInfo.pNext = nullptr;
			//this is the total size, in bytes, of the buffer we are allocating
			vertexBufferInfo.size = GetSize();
			//this buffer is going to be used as a Vertex Buffer
			// ad goign to be a desitination bit
			vertexBufferInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

			VmaAllocationCreateInfo vmaallocInfo = {};
				//let the VMA library know that this data should be GPU native
			vmaallocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

			graphicsContext->CreateVmaBuffer(vertexBufferInfo, vmaallocInfo, m_IndexBuffer);
		});
	}
	VulkanIndexBuffer::~VulkanIndexBuffer() {
		Release();
	}
	void VulkanIndexBuffer::Bind(Count<RenderCommandBuffer>commandBuffer)const {
		vkCmdBindIndexBuffer(commandBuffer.As<VulkanRenderCommandBuffer>()->GetCommandBuffer(Renderer::GetCurrentFrame().FrameinFlight), m_IndexBuffer.Buffer, 0, VK_INDEX_TYPE_UINT32);
	}
	void VulkanIndexBuffer::SetData(const void* data, uint64_t count, uint64_t offsetCount )
	{
		if (count == 0)
		{
			PF_ENGINE_ERROR("Vertex Cannot set Data with a size of 0");
			return;
		}
		uint64_t dataSize = count * sizeof(uint32_t);
		auto graphicsContext = VulkanRenderer::GetGraphicsContext();

		VkBufferCreateInfo stagingBufferInfo = {};
		stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		stagingBufferInfo.pNext = nullptr;

		stagingBufferInfo.size = dataSize;
		stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

		//let the VMA library know that this data should be on CPU RAM
		VmaAllocationCreateInfo vmaallocInfo = {};
		vmaallocInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;

		VulkanBuffer stagingBuffer;

		graphicsContext->CreateVmaBuffer(stagingBufferInfo, vmaallocInfo, stagingBuffer);
		void* stagingData;
		vmaMapMemory(graphicsContext->GetVMA_Allocator(), stagingBuffer.Allocation, &stagingData);

		memcpy(stagingData, data, dataSize);

		vmaUnmapMemory(graphicsContext->GetVMA_Allocator(), stagingBuffer.Allocation);

		Renderer::SubmitCommand([&](CommandBuffer* cmdBuffer) {
			VkBufferCopy copy;
			copy.dstOffset = offsetCount * sizeof(uint32_t);
			copy.srcOffset = 0;
			copy.size = dataSize;
			vkCmdCopyBuffer(cmdBuffer->As<VulkanCommandBuffer>()->GetCommandBuffer(Renderer::GetCurrentFrame().FrameinFlight), stagingBuffer.Buffer, m_IndexBuffer.Buffer, 1, &copy);
		});
		vmaDestroyBuffer(graphicsContext->GetVMA_Allocator(), stagingBuffer.Buffer, stagingBuffer.Allocation);
	}
	void VulkanIndexBuffer::Resize(uint64_t count)
	{
		Release();

		m_Count = count;
		Build();
	}
	void VulkanIndexBuffer::Resize(const void* data, uint64_t count)
	{

		//TODO WORKING EXPECTED
		//if (m_Count == count)
		//{
		//	SetData(data, count, 0);
		//	return;
		//}
		Release();

		m_Count = count;
		Build();
		SetData(data, count, 0);
	}
	std::vector<uint64_t> VulkanIndexBuffer::GetData()const
	{
		std::vector<uint64_t> data;
		void* vertexData;
		vmaMapMemory(VulkanVertexBuffer::GetGraphicsAllocator(), m_IndexBuffer.Allocation, &vertexData);
		uint64_t* indices = static_cast<uint64_t*>(vertexData);
		for (size_t i = 0; i < m_Count; i++)
		{
			data.emplace_back(indices[i]);
		}
		vmaUnmapMemory(VulkanVertexBuffer::GetGraphicsAllocator(), m_IndexBuffer.Allocation);
		return data;
	}

	void VulkanIndexBuffer::Release()
	{
		if (m_IndexBuffer.Buffer == nullptr)
			return;
		Renderer::SubmitDatafree([indexBuffer = m_IndexBuffer]() {
			auto graphicsContext = VulkanRenderer::GetGraphicsContext();
			vmaDestroyBuffer(graphicsContext->GetVMA_Allocator(), indexBuffer.Buffer, indexBuffer.Allocation);
		});
		m_IndexBuffer.Buffer = nullptr;
		m_IndexBuffer.Allocation = nullptr;
	}
}
