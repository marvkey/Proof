#include "Proofprch.h"
#include "VulkanResourceBuffer.h"
#include "Proof/Core/Core.h"
#include "VulkanCommandBuffer.h"
#include "VulkanGraphicsContext.h"
#include "VulkanRenderer.h"
#include "VulkanAllocator.h"
namespace Proof{


	// setting m_MemoryUsage(VulkanMemmoryUsage::CpuToGpU)
	// because a uniform buffer will most liekely always be updated every frame so this is a better approach
	VulkanUniformBuffer::VulkanUniformBuffer(uint64_t size) :
		m_Size(size), m_MemoryUsage(VulkanMemmoryUsage::CpuToGpU)
	{
		m_LocalBuffer.Allocate(size);
		Build();
	}
	VulkanUniformBuffer::VulkanUniformBuffer(Buffer data)
		:m_Size(data.GetSize()), m_MemoryUsage(VulkanMemmoryUsage::CpuToGpU)
	{

		m_LocalBuffer = Buffer::Copy(data);
		Count<VulkanUniformBuffer> instance = this;
		Renderer::Submit([instance]() mutable
			{
				instance->RT_Build();
				instance->RT_SetData(instance->m_LocalBuffer);
				instance->m_LocalBuffer.Release();
			});
	}

	void VulkanUniformBuffer::Build()
	{
		Count<VulkanUniformBuffer> instance = this;

		Renderer::Submit([instance]
		{
				instance->RT_Build();
		});
	}
	void VulkanUniformBuffer::RT_Build()
	{

		VulkanAllocator allocator("VulkanUniformBufferBuild");

		VkBufferCreateInfo uniformBufferInfo = {};
		uniformBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		uniformBufferInfo.pNext = nullptr;
		uniformBufferInfo.size = m_Size;
		uniformBufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

		allocator.AllocateBuffer(uniformBufferInfo, Utils::ProofVulkanMemmoryUsageToVMAMemoryUsage(m_MemoryUsage), m_UniformBuffer);
	}
	VulkanUniformBuffer::~VulkanUniformBuffer() 
	{
		Release();
	}
	void VulkanUniformBuffer::SetData(Buffer data, uint64_t offset) 
	{
		if (m_LocalBuffer.GetSize() != m_Size)
		{
			m_LocalBuffer.Allocate(m_Size);
		}
		PF_CORE_ASSERT(data.Size <= m_LocalBuffer.Size);
		memcpy(m_LocalBuffer.Data, (uint8_t*)data.Data + offset, data.Size);

		Count<VulkanUniformBuffer> instance = this;
		Renderer::Submit([instance, offset]() mutable
		{
			instance->RT_SetData(instance->m_LocalBuffer, offset);
			instance->m_LocalBuffer.Release();
		});
	}

	void VulkanUniformBuffer::RT_SetData(Buffer data, uint64_t offset)
	{
		// only becausse we use cpu to gpu
		VulkanAllocator allocator("VulkanUniformBufferSetData");
		uint8_t* pData = allocator.MapMemory<uint8_t>(m_UniformBuffer.Allocation);
		memcpy(pData, data.Get() + offset, data.GetSize());
		allocator.UnmapMemory(m_UniformBuffer.Allocation);
	}

	void VulkanUniformBuffer::Release()
	{
		Renderer::SubmitResourceFree([buffer = m_UniformBuffer]() {
			VulkanAllocator allocator("VulkanUniformBufferRelease");
			allocator.DestroyBuffer(buffer);
		});
		m_UniformBuffer.Allocation = nullptr;
		m_UniformBuffer.Buffer = nullptr;
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

	// setting m_MemoryUsage(VulkanMemmoryUsage::CpuToGpU)
	// because a uniform buffer will most liekely always be updated every frame so this is a better approach
	VulkanStorageBuffer::VulkanStorageBuffer(uint64_t size)
		:m_Size(size), m_MemoryUsage(VulkanMemmoryUsage::CpuToGpU)

	{
		m_LocalBuffer.Allocate(size);
		Build();
	}

	VulkanStorageBuffer::VulkanStorageBuffer(Buffer data)
		:m_Size(data.GetSize()), m_MemoryUsage(VulkanMemmoryUsage::CpuToGpU)

	{
		m_LocalBuffer = Buffer::Copy(data);
		Count<VulkanStorageBuffer> instance = this;
		Renderer::Submit([instance]() mutable
			{
				instance->RT_Build();
				instance->RT_SetData(instance->m_LocalBuffer);
				//instance->m_LocalBuffer.Release();
			});

	}
	void VulkanStorageBuffer::Build()
	{
		Count<VulkanStorageBuffer> instance = this;

		Renderer::Submit([instance]
		{
			instance->RT_Build();
		});
	}
	void VulkanStorageBuffer::RT_Build()
	{
		VulkanAllocator allocator("VulkanStorageBufferBuild");

		VkBufferCreateInfo storageBufferInfo = {};
		storageBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		storageBufferInfo.pNext = nullptr;
		storageBufferInfo.size = m_Size;
		storageBufferInfo.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;

		allocator.AllocateBuffer(storageBufferInfo, Utils::ProofVulkanMemmoryUsageToVMAMemoryUsage(m_MemoryUsage), m_StorageBuffer);
	}
	VulkanStorageBuffer::~VulkanStorageBuffer()
	{
		Release();
	}	
	void VulkanStorageBuffer::Resize(uint64_t size)
	{
		Count<VulkanStorageBuffer> instance = this;

		if (m_Size == size)
		{
			Renderer::Submit([instance,size]() mutable
			{
				VulkanAllocator allocator("VulkanStorageBufferSetData");
				uint8_t* pData = allocator.MapMemory<uint8_t>(instance->m_StorageBuffer.Allocation);
				memset(pData, 0, static_cast<size_t>(size));
				allocator.UnmapMemory(instance->m_StorageBuffer.Allocation);
			});

			return;
		}
		Release();

		m_Size = size;
		Build();
	}
	void VulkanStorageBuffer::Resize(Buffer data)
	{
		if (m_Size == data.GetSize())
		{
			SetData(data);
			return;
		}
		Release();
		m_Size = data.GetSize();

		Build();
		SetData(data);
	}
	void VulkanStorageBuffer::SetData(Buffer data, uint64_t offset)
	{
		if (m_LocalBuffer.GetSize() != m_Size)
		{
			m_LocalBuffer.Allocate(m_Size);
		}
		PF_CORE_ASSERT(data.Size <= m_LocalBuffer.Size);
		memcpy(m_LocalBuffer.Data, (uint8_t*)data.Data + offset, data.Size);

		Count<VulkanStorageBuffer> instance = this;
		Renderer::Submit([instance, offset]() mutable
		{
			instance->RT_SetData(instance->m_LocalBuffer, offset);
			//instance->m_LocalBuffer.Release();
		});

	}
	void VulkanStorageBuffer::RT_SetData(Buffer data, uint64_t offset)
	{
		VulkanAllocator allocator("VulkanStorageBufferSetData");
		uint8_t* pData = allocator.MapMemory<uint8_t>(m_StorageBuffer.Allocation);
		memcpy(pData, data.Data + offset, data.GetSize());
		allocator.UnmapMemory(m_StorageBuffer.Allocation);
	}
	void VulkanStorageBuffer::Release()
	{
		Renderer::SubmitResourceFree([buffer = m_StorageBuffer]() 
			{
				VulkanAllocator allocator("VulkanStorageBufferRelease");
				allocator.DestroyBuffer(buffer);
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
