#pragma once
#include "vulkan/vulkan.h"
#include <vulkan/VulkanProofExternalLibs/vk_mem_alloc.h>

namespace Proof{
	enum class VulkanMemmoryUsage
	{
		GpuOnly, // data is set once and will rarely ever be updatedd
		CpuToGpU // data is going to be possibly updated quite frequently
	};

	namespace Utils
	{
		static VmaMemoryUsage ProofVulkanMemmoryUsageToVMAMemoryUsage(VulkanMemmoryUsage usage)
		{
			switch (usage)
			{
				case Proof::VulkanMemmoryUsage::GpuOnly:
					return VMA_MEMORY_USAGE_GPU_ONLY;
					break;
				case Proof::VulkanMemmoryUsage::CpuToGpU:
					return VMA_MEMORY_USAGE_CPU_TO_GPU;
					break;
			}
			PF_CORE_ASSERT(false);
		}
	}
	struct VulkanBuffer {
		VkBuffer Buffer = nullptr;
		VmaAllocation Allocation = nullptr;
	};

	struct VulkanImageAlloc {
		VkImage Image = nullptr;
		VmaAllocation Allocation = nullptr;
	};
}
