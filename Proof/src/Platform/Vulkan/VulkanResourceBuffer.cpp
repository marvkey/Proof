#include "Proofprch.h"
#include "VulkanResourceBuffer.h"
#include "Proof/Core/Core.h"
#include "VulkanCommandBuffer.h"
#include "VulkanGraphicsContext.h"
#include "VulkanRenderer/VulkanRenderer.h"
namespace Proof{

	VulkanUniformBuffer::VulkanUniformBuffer(uint64_t size) :
		m_Size(size)
	{
		Build();
	}
	VulkanUniformBuffer::VulkanUniformBuffer(Buffer data)
		:m_Size(data.GetSize())
	{
		Build();
		PF_CORE_ASSERT(data, "cannot be nullptr");
		SetData(data, 0);
	}

	void VulkanUniformBuffer::Build()
	{
		PF_CORE_ASSERT(m_Size != 0, "Vulkan uniform buffer cannot have a size less than or equal to 0");
		auto graphicsContext = VulkanRenderer::GetGraphicsContext();

		VkBufferCreateInfo uniformBufferInfo = {};
		uniformBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		uniformBufferInfo.pNext = nullptr;

		uniformBufferInfo.size = m_Size;
		uniformBufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

		VmaAllocationCreateInfo vmaallocInfo = {};
		vmaallocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
		graphicsContext->CreateVmaBuffer(uniformBufferInfo, vmaallocInfo, m_UniformBuffers);
	}
	VulkanUniformBuffer::~VulkanUniformBuffer() 
	{
		Release();
	}
	void VulkanUniformBuffer::SetData(Buffer data, uint64_t offset) {
		auto graphicsContext = VulkanRenderer::GetGraphicsContext();
		VulkanBuffer stagingBuffer;

		VkBufferCreateInfo stagingBufferInfo = {};
		stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		stagingBufferInfo.pNext = nullptr;

		stagingBufferInfo.size = data.GetSize();
		stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

		//let the VMA library know that this data should be on CPU RAM
		VmaAllocationCreateInfo vmaallocInfo = {};
		vmaallocInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;

		graphicsContext->CreateVmaBuffer(stagingBufferInfo, vmaallocInfo, stagingBuffer);
		void* stagingData;
		vmaMapMemory(graphicsContext->GetVMA_Allocator(), stagingBuffer.Allocation, &stagingData);

		memcpy(stagingData, data.Get(), data.GetSize());

		vmaUnmapMemory(graphicsContext->GetVMA_Allocator(), stagingBuffer.Allocation);

		Renderer::SubmitCommand([&](CommandBuffer* cmdBuffer) {
			VkBufferCopy copy;
			copy.dstOffset = offset;
			copy.srcOffset = 0;
			copy.size = data.GetSize();
			vkCmdCopyBuffer(cmdBuffer->As<VulkanCommandBuffer>()->GetCommandBuffer(), stagingBuffer.Buffer, m_UniformBuffers.Buffer, 1, &copy);
		});
		vmaDestroyBuffer(graphicsContext->GetVMA_Allocator(), stagingBuffer.Buffer, stagingBuffer.Allocation);
	}

	void VulkanUniformBuffer::Resize(uint64_t size)
	{
		Release();
		m_Size = size;
		Build();
	}

	void VulkanUniformBuffer::Resize(Buffer data)
	{
		//TODO WORKING EXPECTED
		//if (m_Size == m_Size)
		//{
		//	SetData(data, size, 0);
		//	return;
		//}
		Release();
		m_Size = data.GetSize();

		Build();
		SetData(data, 0);
	}

	void VulkanUniformBuffer::Release()
	{
		auto graphicsContext = VulkanRenderer::GetGraphicsContext();
		Renderer::SubmitDatafree([context = graphicsContext, buffer = m_UniformBuffers]() {
			vmaDestroyBuffer(context->GetVMA_Allocator(), buffer.Buffer, buffer.Allocation);
		});
		m_UniformBuffers.Allocation = nullptr;
		m_UniformBuffers.Buffer = nullptr;
	}
	
	VulkanUniformBufferSet::VulkanUniformBufferSet(uint64_t size)
	{
		for (uint32_t frameIndex = 0; frameIndex < Renderer::GetConfig().FramesFlight; frameIndex++)
		{
			m_Buffers[frameIndex] = UniformBuffer::Create(size);
		}
	}

	VulkanUniformBufferSet::VulkanUniformBufferSet(Buffer data)
	{
		for (uint32_t frameIndex = 0; frameIndex < Renderer::GetConfig().FramesFlight; frameIndex++)
		{
			m_Buffers[frameIndex] = UniformBuffer::Create(data);
		}
	}

	VulkanUniformBufferSet::~VulkanUniformBufferSet()
	{
		m_Buffers.clear();
	}

	void VulkanUniformBufferSet::Resize(uint32_t index, uint64_t size)
	{
		PF_CORE_ASSERT(m_Buffers.contains(index), "Uniform Buffer set does not contain index");
		m_Buffers[index]->Resize(size);
	}

	void VulkanUniformBufferSet::Resize(uint32_t index,Buffer data)
	{
		PF_CORE_ASSERT(m_Buffers.contains(index), "Uniform Buffer set does not contain index");
		m_Buffers[index]->Resize(data);
	}

	void VulkanUniformBufferSet::SetData(uint32_t index, Buffer data, uint64_t offset)
	{
		PF_CORE_ASSERT(m_Buffers.contains(index), "Uniform Buffer set does not contain index");
		m_Buffers[index]->SetData(data,offset);
	}

	Count<UniformBuffer> VulkanUniformBufferSet::GetBuffer(uint32_t index)
	{
		PF_CORE_ASSERT(m_Buffers.contains(index), "Uniform Buffer set does not contain index");
		return m_Buffers[index];
	}

	VulkanStorageBuffer::VulkanStorageBuffer(uint64_t size)
		:m_Size(size)

	{
		Build();
	}

	VulkanStorageBuffer::VulkanStorageBuffer(Buffer data)
		:m_Size(data.GetSize())

	{
		Build();
		SetData(data, 0);

	}
	void VulkanStorageBuffer::Build()
	{
		PF_CORE_ASSERT(m_Size != 0, "Cannot create storageBuffer with a size of 0");

		auto graphicsContext = VulkanRenderer::GetGraphicsContext();
		VkBufferCreateInfo uniformBufferInfo = {};
		uniformBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		uniformBufferInfo.pNext = nullptr;

		uniformBufferInfo.size = m_Size;
		uniformBufferInfo.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

		VmaAllocationCreateInfo vmaallocInfo = {};
		vmaallocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
		graphicsContext->CreateVmaBuffer(uniformBufferInfo, vmaallocInfo, m_StorageBuffer);
	}
	VulkanStorageBuffer::~VulkanStorageBuffer()
	{
		Release();
	}	
	void VulkanStorageBuffer::Resize(uint64_t size)
	{
		Release();
		m_Size = size;
		Build();
	}
	void VulkanStorageBuffer::Resize(Buffer data)
	{
		//if (m_Size == data.GetSize())
		//{
		//	SetData(data);
		//	return;
		//}
		Release();
		m_Size = data.GetSize();

		Build();
		SetData(data);
	}
	void VulkanStorageBuffer::SetData(Buffer data, uint64_t offset)
	{
		auto graphicsContext = VulkanRenderer::GetGraphicsContext();
		VulkanBuffer stagingBuffer;

		VkBufferCreateInfo stagingBufferInfo = {};
		stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		stagingBufferInfo.pNext = nullptr;

		stagingBufferInfo.size = data.GetSize();
		stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

		//let the VMA library know that this data should be on CPU RAM
		VmaAllocationCreateInfo vmaallocInfo = {};
		vmaallocInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;

		graphicsContext->CreateVmaBuffer(stagingBufferInfo, vmaallocInfo, stagingBuffer);
		void* stagingData;
		vmaMapMemory(graphicsContext->GetVMA_Allocator(), stagingBuffer.Allocation, &stagingData);

		memcpy(stagingData, data.Get(), data.GetSize());

		vmaUnmapMemory(graphicsContext->GetVMA_Allocator(), stagingBuffer.Allocation);

		Renderer::SubmitCommand([&](CommandBuffer* cmdBuffer) {
			VkBufferCopy copy;
			copy.dstOffset = offset;
			copy.srcOffset = 0;
			copy.size = data.GetSize();
			vkCmdCopyBuffer(cmdBuffer->As<VulkanCommandBuffer>()->GetCommandBuffer(), stagingBuffer.Buffer, m_StorageBuffer.Buffer, 1, &copy);
		});
		vmaDestroyBuffer(graphicsContext->GetVMA_Allocator(), stagingBuffer.Buffer, stagingBuffer.Allocation);
	}
	void VulkanStorageBuffer::Release()
	{
		auto allocator = VulkanRenderer::GetGraphicsContext()->GetVMA_Allocator();

		Renderer::SubmitDatafree([allocator = allocator, buffer = m_StorageBuffer]() {
			vmaDestroyBuffer(allocator, buffer.Buffer, buffer.Allocation);
		});
		m_StorageBuffer.Allocation = nullptr;
		m_StorageBuffer.Buffer = nullptr;
	}

	VulkanStorageBufferSet::VulkanStorageBufferSet(uint64_t size)
	{
		for (uint32_t frameIndex = 0; frameIndex < Renderer::GetConfig().FramesFlight; frameIndex++)
		{
			m_Buffers[frameIndex] = StorageBuffer::Create(size);
		}
	}

	VulkanStorageBufferSet::VulkanStorageBufferSet(Buffer data)
	{
		for (uint32_t frameIndex = 0; frameIndex < Renderer::GetConfig().FramesFlight; frameIndex++)
		{
			m_Buffers[frameIndex] = StorageBuffer::Create(data);
		}
	}

	VulkanStorageBufferSet::~VulkanStorageBufferSet()
	{
		m_Buffers.clear();
	}

	Count<StorageBuffer> VulkanStorageBufferSet::GetBuffer(uint32_t index)
	{
		PF_CORE_ASSERT(m_Buffers.contains(index), "Storage Buffer set does not contain index");
		return m_Buffers[index];
	}

	void VulkanStorageBufferSet::Resize(uint32_t index, uint64_t size)
	{
		PF_CORE_ASSERT(m_Buffers.contains(index), "Storage Buffer set does not contain index");
		m_Buffers[index]->Resize(size);
	}

	void VulkanStorageBufferSet::Resize(uint32_t index, Buffer data)
	{
		PF_CORE_ASSERT(m_Buffers.contains(index), "Storage Buffer set does not contain index");
		m_Buffers[index]->Resize(data);
	}

	void VulkanStorageBufferSet::SetData(uint32_t index, Buffer data, uint64_t offset)
	{
		PF_CORE_ASSERT(m_Buffers.contains(index), "Storage Buffer set does not contain index");
		m_Buffers[index]->SetData(data,offset);
	}

}
