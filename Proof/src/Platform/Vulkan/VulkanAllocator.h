#pragma once
#include <vulkan/VulkanProofExternalLibs/vk_mem_alloc.h>
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
		void AllocateImage(VkImageCreateInfo bufferInfo, VmaMemoryUsage usage, VulkanImageAlloc& image);

		void DestroyBuffer(VulkanBuffer buffer);
		void DestroyImage(VulkanImageAlloc image);

		template<typename T>
		T* MapMemory(VmaAllocation allocation)
		{
			T* mappedMemory;
			vmaMapMemory(VulkanAllocator::GetVmaAllocator(), allocation, (void**)&mappedMemory);
			return mappedMemory;
		}
		static GPUMemoryStats GetStats();
		static void DumpStats();
		void UnmapMemory(VmaAllocation allocation);
	private:
		static void Init(class VulkanGraphicsContext* context);
		static void ShutDown();
		friend class VulkanGraphicsContext;
		std::string m_DebugName ;
	};
}