#pragma once
#include "VulkanUtils/vk_mem_alloc.h"
#include "VulkanUtils/VulkanBufferBase.h"
namespace Proof
{
	struct GPUMemoryStats
	{
		uint64_t Used = 0;
		uint64_t Free = 0;
	};
	class VulkanAllocator
	{
	public:
		VulkanAllocator() = default;
		VulkanAllocator(const std::string& debugName);

		static VmaAllocator GetVmaAllocator();

		void AllocateBuffer(VkBufferCreateInfo bufferInfo, VmaMemoryUsage usage, VulkanBuffer& buffer);
		void AllocateBuffer(VkBufferCreateInfo bufferInfo, VmaMemoryUsage usage, VulkanBuffer& buffer, VmaAllocationCreateFlagBits bits );
		void AllocateImage(VkImageCreateInfo bufferInfo, VmaMemoryUsage usage, VulkanImageAlloc& image);

		void DestroyBuffer(VulkanBuffer buffer);
		void DestroyImage(VulkanImageAlloc image);

		template<typename T>
		T* MapMemory(VmaAllocation allocation)
		{
			T* mappedMemory;
			vmaMapMemory(VulkanAllocator::GetVmaAllocator(), allocation, (void**)&mappedMemory);

			//if (vmaMapMemory(VulkanAllocator::GetVmaAllocator(), allocation, (void**)&mappedMemory) == VK_ERROR_MEMORY_MAP_FAILED)
			//	return nullptr;
			return mappedMemory;
		}
		static GPUMemoryStats GetStats();
		static void DumpStats();
		void UnmapMemory(VmaAllocation allocation);

		void InvalidateMemory(VmaAllocation allocation, VkDeviceSize offset = 0, VkDeviceSize size = VK_WHOLE_SIZE);

	private:
		static void Init(Count<class VulkanDevice> device);
		static void ShutDown();
		friend class VulkanGraphicsContext;
		std::string m_DebugName ;
	};
}