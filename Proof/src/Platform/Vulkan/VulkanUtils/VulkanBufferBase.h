#pragma once
#include "vulkan/vulkan.h"
#include <vulkan/VulkanProofExternalLibs/vk_mem_alloc.h>

namespace Proof{
	
	struct VulkanBuffer {
		VkBuffer Buffer;
		VmaAllocation Allocation;
	};

	struct VulkanImageAlloc {
		VkImage Image;
		VmaAllocation Allocation;
	};
}
