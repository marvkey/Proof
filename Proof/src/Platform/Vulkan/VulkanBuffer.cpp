#include "Proofprch.h"
#include "VulkanBuffer.h"
#include "Proof/Renderer/Renderer.h"
#include "VulkanGraphicsContext.h"
#include "VulkanRenderer.h"
#include "VulkanCommandBuffer.h"
#include "VulkanAllocator.h"
#include "VulkanDevice.h"
#include "VulkanUtils/vk_mem_alloc.h"

namespace Proof
{


	// cpu to gpu is better for data that is updated ofen
	// gpu only is bettter for data that is updated once
	VulkanVertexBuffer::VulkanVertexBuffer(const void* data, uint64_t size): 
		m_VertexSize(size), m_Usage (VulkanMemmoryUsage::GpuOnly)
	{
		m_LocalBuffer.Copy(data, size);
		Count<VulkanVertexBuffer> instance = this;
		Renderer::Submit([instance]() mutable
		{
			instance->Build();
			instance->RT_SetData(instance->m_LocalBuffer.Data, instance->m_LocalBuffer.Size);
			instance->m_LocalBuffer.Release();
		});

	}
	VulkanVertexBuffer::VulkanVertexBuffer(uint64_t size):
		m_VertexSize(size), m_Usage(VulkanMemmoryUsage::CpuToGpU)
	{
		Count<VulkanVertexBuffer> instance = this;
		Renderer::Submit([instance]() mutable
		{
			instance->Build();
		});

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
		Count<VulkanVertexBuffer> instance = this;

		Renderer::Submit([instance]() mutable
		{
				instance->Build();
		});
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
	void VulkanVertexBuffer::Bind(Count<RenderCommandBuffer> commandBuffer, uint64_t binding, uint64_t offset )
	{
		Count<VulkanVertexBuffer> instance = this;
		Renderer::Submit([instance,commandBuffer,binding,offset]() mutable
			{
				VkDeviceSize instanceOffset[1] = { (VkDeviceSize)offset };
				vkCmdBindVertexBuffers(commandBuffer.As<VulkanRenderCommandBuffer>()->GetActiveCommandBuffer(), binding, 1, &instance->m_VertexBuffer.Buffer, instanceOffset);
			});
	}
	void VulkanVertexBuffer::SetData(const void* data, uint64_t size,uint64_t offset)
	{
		PF_PROFILE_FUNC();

		if (m_LocalBuffer.GetSize() != m_VertexSize)
		{
			m_LocalBuffer.Allocate(m_VertexSize);
		}
		PF_CORE_ASSERT(size <= m_LocalBuffer.Size);
		memcpy(m_LocalBuffer.Data, (uint8_t*)data + offset, size);;

		Count<VulkanVertexBuffer> instance = this;

		Renderer::Submit([instance]() mutable
			{
				instance->RT_SetData(instance->m_LocalBuffer.Data, instance->m_LocalBuffer.Size);
			});
	}
	void VulkanVertexBuffer::RT_SetData(const void* data, uint64_t size, uint64_t offset)
	{
		PF_PROFILE_FUNC();

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

			VkCommandBuffer cmdBuffer = VulkanRenderer::GetGraphicsContext()->GetDevice()->GetCommandBuffer(true);
			VkBufferCopy copy;
			copy.dstOffset = offset;
			copy.srcOffset = 0;
			copy.size = size;
			vkCmdCopyBuffer(cmdBuffer, stagingBuffer.Buffer, m_VertexBuffer.Buffer, 1, &copy);
			VulkanRenderer::GetGraphicsContext()->GetDevice()->FlushCommandBuffer(cmdBuffer);

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
		Renderer::SubmitResourceFree([vertexBuffer = m_VertexBuffer]() 
		{
			VulkanAllocator allocator("VertexBufferRelease");
			allocator.DestroyBuffer(vertexBuffer);
		});
		m_VertexBuffer.Buffer = nullptr;
		m_VertexBuffer.Allocation = nullptr;
		m_LocalBuffer.Release();
	}

	VmaAllocator VulkanVertexBuffer::GetGraphicsAllocator()
	{
		return VulkanAllocator::GetVmaAllocator();
	}


	VulkanIndexBuffer::VulkanIndexBuffer(uint32_t size)
		:
		m_Size(size), m_Usage(VulkanMemmoryUsage::CpuToGpU)
	{
		m_LocalBuffer.Allocate(size);
		Count<VulkanIndexBuffer> instance = this;
		Renderer::Submit([instance]() mutable
			{
				instance->Build();
			});
	}

	VulkanIndexBuffer::VulkanIndexBuffer(const void* data, uint32_t size)
	:
		m_Size(size),m_Usage(VulkanMemmoryUsage::GpuOnly)
	{
		m_LocalBuffer.Copy(data,size);

		Count<VulkanIndexBuffer> instance = this;

		Renderer::Submit([instance]() mutable
			{
				instance->Build();
				instance->RT_SetData(instance->m_LocalBuffer.Data, instance->m_LocalBuffer.Size);
				instance->m_LocalBuffer.Release();
			});
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
	VulkanIndexBuffer::~VulkanIndexBuffer() 
	{
		Release();
	}
	void VulkanIndexBuffer::Bind(Count<RenderCommandBuffer>commandBuffer)
	{
		Count<VulkanIndexBuffer> instance = this;

		Renderer::Submit([instance, commandBuffer]() mutable
			{
				vkCmdBindIndexBuffer(commandBuffer.As<VulkanRenderCommandBuffer>()->GetActiveCommandBuffer(), instance->m_IndexBuffer.Buffer, 0, VK_INDEX_TYPE_UINT32);
			});
	}
	void VulkanIndexBuffer::RT_SetData(const void* data, uint32_t size, uint32_t offsetSize)
	{
		auto graphicsContext = VulkanRenderer::GetGraphicsContext();
		if (m_Usage == VulkanMemmoryUsage::GpuOnly)
		{
			VkBufferCreateInfo stagingBufferInfo = {};
			stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			stagingBufferInfo.pNext = nullptr;
			stagingBufferInfo.size = size;
			stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

			VulkanBuffer stagingBuffer;

			VulkanAllocator statingBufferAllocator("VulkanIndexBufferStagingBuffer");
			statingBufferAllocator.AllocateBuffer(stagingBufferInfo, VMA_MEMORY_USAGE_CPU_ONLY, stagingBuffer);

			uint8_t* stagingData = statingBufferAllocator.MapMemory<uint8_t>(stagingBuffer.Allocation);
			memcpy(stagingData, (void*)data, size);
			statingBufferAllocator.UnmapMemory(stagingBuffer.Allocation);

			VkCommandBuffer cmdBuffer = VulkanRenderer::GetGraphicsContext()->GetDevice()->GetCommandBuffer(true);
			VkBufferCopy copy;
			copy.dstOffset = offsetSize;
			copy.srcOffset = 0;
			copy.size = size;
			vkCmdCopyBuffer(cmdBuffer, stagingBuffer.Buffer, m_IndexBuffer.Buffer, 1, &copy);
			VulkanRenderer::GetGraphicsContext()->GetDevice()->FlushCommandBuffer(cmdBuffer);

			statingBufferAllocator.DestroyBuffer(stagingBuffer);
		}
		else if (m_Usage == VulkanMemmoryUsage::CpuToGpU)
		{
			VulkanAllocator allocator("VulkanIndexBufferSetData");
			uint8_t* pData = allocator.MapMemory<uint8_t>(m_IndexBuffer.Allocation);
			memcpy(pData, (uint8_t*)data + offsetSize, size);
			allocator.UnmapMemory(m_IndexBuffer.Allocation);
		}
	}
	void VulkanIndexBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
	{
		PF_PROFILE_FUNC();

		if (m_LocalBuffer.GetSize() != m_Size)
		{
			m_LocalBuffer.Allocate(m_Size);
		}
		PF_CORE_ASSERT(size <= m_LocalBuffer.Size);
		memcpy(m_LocalBuffer.Data, (uint8_t*)data + offset, size);;

		Count<VulkanIndexBuffer> instance = this;

		Renderer::Submit([instance]() mutable
			{
				instance->RT_SetData(instance->m_LocalBuffer.Data, instance->m_LocalBuffer.Size);
			});
	}
	void VulkanIndexBuffer::Resize(uint32_t size)
	{
		Release();

		m_Size = size;
		Count<VulkanIndexBuffer> instance = this;

		Renderer::Submit([instance]() mutable
			{
				instance->Build();
			});
	}
	void VulkanIndexBuffer::Resize(const void* data, uint32_t size)
	{

		if (m_Size == size)
		{
			SetData(data, size, 0);
			return;
		}
		Release();

		m_Size = size;

		Count<VulkanIndexBuffer> instance = this;
		Renderer::Submit([instance]() mutable
			{
				instance->Build();
			});
		SetData(data, m_Size, 0);
	}
	std::vector<uint32_t> VulkanIndexBuffer::GetData()const
	{
		void* mappedData;
		vmaMapMemory(VulkanVertexBuffer::GetGraphicsAllocator(), m_IndexBuffer.Allocation, &mappedData);

		size_t numIndices = m_Size / sizeof(uint32_t);

		// Create a std::vector to store the indices.
		std::vector<uint32_t> indices(numIndices);

		// Copy the data from the mapped memory to the vector.
		std::memcpy(indices.data(), mappedData, m_Size);

		// Unmap the index buffer memory.
		vmaUnmapMemory(VulkanVertexBuffer::GetGraphicsAllocator(), m_IndexBuffer.Allocation);
		return indices;
	}

	Buffer VulkanIndexBuffer::GetDataRaw()
	{
		Buffer buffer(m_Size);
		void* vertexData;
		vmaMapMemory(VulkanVertexBuffer::GetGraphicsAllocator(), m_IndexBuffer.Allocation, &vertexData);
		std::memcpy(buffer.Get(), vertexData, m_Size); // so we can use delte
		vmaUnmapMemory(VulkanVertexBuffer::GetGraphicsAllocator(), m_IndexBuffer.Allocation);
		return buffer;
	}

	void VulkanIndexBuffer::Release()
	{
		if (m_IndexBuffer.Buffer == nullptr)
			return;
		Renderer::SubmitResourceFree([indexBuffer = m_IndexBuffer]() 
		{
			VulkanAllocator allocator("VertexIndexRelease");
			allocator.DestroyBuffer(indexBuffer);
		});
		m_IndexBuffer.Buffer = nullptr;
		m_IndexBuffer.Allocation = nullptr;
	}
}
