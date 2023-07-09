#pragma once
#include "vulkan/vulkan.h"
#include <vulkan/VulkanProofExternalLibs/vk_mem_alloc.h>

namespace Proof{
	
	struct VulkanBuffer {
		VkBuffer Buffer = nullptr;
		VmaAllocation Allocation = nullptr;
	};

	struct VulkanImageAlloc {
		VkImage Image = nullptr;
		VmaAllocation Allocation = nullptr;
	};
}
