#include "Proofprch.h"
#include "VulkanBuffer.h"
#include "Proof/Renderer/Renderer.h"
#include "VulkanGraphicsContext.h"
#include <vulkan/VulkanProofExternalLibs/vk_mem_alloc.h>
#include "VulkanRenderer/VulkanRenderer.h"
#include "VulkanCommandBuffer.h"
#include "VulkanAllocator.h"
namespace Proof
{


	// cpu to gpu is better for data that is updated ofen
	// gpu only is bettter for data that is updated once
	VulkanVertexBuffer::VulkanVertexBuffer(const void* data, uint64_t size): 
		m_VertexSize(size), m_Usage (VulkanMemmoryUsage::GpuOnly)
	{
		
		Build();
		SetData(data, size);
	}
	VulkanVertexBuffer::VulkanVertexBuffer(uint64_t size):
		m_VertexSize(size), m_Usage(VulkanMemmoryUsage::CpuToGpU)
	{
		Build();
	}
	VulkanVertexBuffer::~VulkanVertexBuffer() 
	{
		Release();
	}
	void VulkanVertexBuffer::Build()
	{
		VulkanAllocator allocator("VertexBufferBuild");

		VkBufferCreateInfo vertexBufferInfo = {};
		vertexBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		vertexBufferInfo.pNext = nullptr;
		vertexBufferInfo.size = m_VertexSize;
		if(m_Usage == VulkanMemmoryUsage::CpuToGpU)
			vertexBufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		else
			vertexBufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		allocator.AllocateBuffer(vertexBufferInfo, Utils::ProofVulkanMemmoryUsageToVMAMemoryUsage(m_Usage), m_VertexBuffer);
	}
	void VulkanVertexBuffer::Resize(uint64_t size)
	{
		Release();

		m_VertexSize = size;
		Build();
	}

	void VulkanVertexBuffer::Resize(const void* data, uint64_t size)
	{
		if (m_VertexSize == size)
		{
			SetData(data, m_VertexSize, 0);
			return;
		}
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
		 
		if (m_Usage == VulkanMemmoryUsage::GpuOnly)
		{
			VkBufferCreateInfo stagingBufferInfo = {};
			stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			stagingBufferInfo.pNext = nullptr;
			stagingBufferInfo.size = size;
			stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

			VulkanBuffer stagingBuffer;

			VulkanAllocator statingBufferAllocator("VulkanVertexBufferStagingBuffer");
			statingBufferAllocator.AllocateBuffer(stagingBufferInfo, VMA_MEMORY_USAGE_CPU_ONLY, stagingBuffer);

			uint8_t* stagingData = statingBufferAllocator.MapMemory<uint8_t>(stagingBuffer.Allocation);
			memcpy(stagingData, (void*)data, size);
			statingBufferAllocator.UnmapMemory(stagingBuffer.Allocation);

			Renderer::SubmitCommand([&](CommandBuffer* cmdBuffer) {
				VkCommandBuffer copyCmd = cmdBuffer->As<VulkanCommandBuffer>()->GetCommandBuffer(Renderer::GetCurrentFrame().FrameinFlight);
				VkBufferCopy copy;
				copy.dstOffset = offset;
				copy.srcOffset = 0;
				copy.size = size;
				vkCmdCopyBuffer(copyCmd, stagingBuffer.Buffer, m_VertexBuffer.Buffer, 1, &copy);
			});
			statingBufferAllocator.DestroyBuffer(stagingBuffer);
		}
		else if (m_Usage == VulkanMemmoryUsage::CpuToGpU)
		{
			VulkanAllocator allocator("VulkanVertexBufferSetData");
			uint8_t* pData = allocator.MapMemory<uint8_t>(m_VertexBuffer.Allocation);
			memcpy(pData, (uint8_t*)data + offset, size);
			allocator.UnmapMemory(m_VertexBuffer.Allocation);
		}
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
			VulkanAllocator allocator("VertexBufferRelease");
			allocator.DestroyBuffer(vertexBuffer);
		});
		m_VertexBuffer.Buffer = nullptr;
		m_VertexBuffer.Allocation = nullptr;
	}

	VmaAllocator VulkanVertexBuffer::GetGraphicsAllocator()
	{
		return VulkanAllocator::GetVmaAllocator();
	}


	VulkanIndexBuffer::VulkanIndexBuffer(uint32_t count)
		:
		m_Count(count), m_Usage(VulkanMemmoryUsage::CpuToGpU)
	{
		Build();
	}

	VulkanIndexBuffer::VulkanIndexBuffer(const void* data, uint32_t count)
	:
		m_Count(count),m_Usage(VulkanMemmoryUsage::GpuOnly)
	{
		Build();
		SetData(data, m_Count, 0);
	}
	void VulkanIndexBuffer::Build()
	{
		VulkanAllocator allocator("VulkanIndexBufferBuild");

		VkBufferCreateInfo indexBufferInfo = {};
		indexBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		indexBufferInfo.pNext = nullptr;
		indexBufferInfo.size = GetSize();
		indexBufferInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
		if (m_Usage == VulkanMemmoryUsage::CpuToGpU)
			indexBufferInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
		else
			indexBufferInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

		allocator.AllocateBuffer(indexBufferInfo, Utils::ProofVulkanMemmoryUsageToVMAMemoryUsage(m_Usage), m_IndexBuffer);
	}
	VulkanIndexBuffer::~VulkanIndexBuffer() {
		Release();
	}
	void VulkanIndexBuffer::Bind(Count<RenderCommandBuffer>commandBuffer)const {
		vkCmdBindIndexBuffer(commandBuffer.As<VulkanRenderCommandBuffer>()->GetCommandBuffer(Renderer::GetCurrentFrame().FrameinFlight), m_IndexBuffer.Buffer, 0, VK_INDEX_TYPE_UINT32);
	}
	void VulkanIndexBuffer::SetData(const void* data, uint32_t count, uint32_t offsetCount)
	{
		uint32_t dataSize = count * sizeof(uint32_t);
		uint32_t offset = offsetCount * sizeof(uint32_t);
		auto graphicsContext = VulkanRenderer::GetGraphicsContext();
		if (m_Usage == VulkanMemmoryUsage::GpuOnly)
		{
			VkBufferCreateInfo stagingBufferInfo = {};
			stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			stagingBufferInfo.pNext = nullptr;
			stagingBufferInfo.size = dataSize;
			stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

			VulkanBuffer stagingBuffer;

			VulkanAllocator statingBufferAllocator("VulkanIndexBufferStagingBuffer");
			statingBufferAllocator.AllocateBuffer(stagingBufferInfo, VMA_MEMORY_USAGE_CPU_ONLY, stagingBuffer);

			uint8_t* stagingData =  statingBufferAllocator.MapMemory<uint8_t>(stagingBuffer.Allocation);
			memcpy(stagingData,(void*)data, dataSize);
			statingBufferAllocator.UnmapMemory(stagingBuffer.Allocation);

			Renderer::SubmitCommand([&](CommandBuffer* cmdBuffer) {
				VkCommandBuffer copyCmd = cmdBuffer->As<VulkanCommandBuffer>()->GetCommandBuffer(Renderer::GetCurrentFrame().FrameinFlight);
				VkBufferCopy copy;
				copy.dstOffset = offset;
				copy.srcOffset = 0;
				copy.size = dataSize;
				vkCmdCopyBuffer(copyCmd, stagingBuffer.Buffer, m_IndexBuffer.Buffer, 1, &copy);
			});
			statingBufferAllocator.DestroyBuffer(stagingBuffer);
		}
		else if (m_Usage == VulkanMemmoryUsage::CpuToGpU)
		{
			VulkanAllocator allocator("VulkanIndexBufferSetData");
			uint8_t* pData = allocator.MapMemory<uint8_t>(m_IndexBuffer.Allocation);
			memcpy(pData, (uint8_t*)data + offset, dataSize);
			allocator.UnmapMemory(m_IndexBuffer.Allocation);
		}
	}
	void VulkanIndexBuffer::Resize(uint32_t count)
	{
		Release();

		m_Count = count;
		Build();
	}
	void VulkanIndexBuffer::Resize(const void* data, uint32_t count)
	{

		if (m_Count == count)
		{
			SetData(data, count, 0);
			return;
		}
		Release();

		m_Count = count;
		Build();
		SetData(data, count, 0);
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

	void VulkanIndexBuffer::Release()
	{
		if (m_IndexBuffer.Buffer == nullptr)
			return;
		Renderer::SubmitDatafree([indexBuffer = m_IndexBuffer]() {
			VulkanAllocator allocator("VertexIndexRelease");
			allocator.DestroyBuffer(indexBuffer);
		});
		m_IndexBuffer.Buffer = nullptr;
		m_IndexBuffer.Allocation = nullptr;
	}
}
