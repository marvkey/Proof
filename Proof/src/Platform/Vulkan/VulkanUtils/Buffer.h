#pragma once
#include "vulkan/vulkan.h"
namespace Proof{
	
	struct VulkanBuffer {
		VkBuffer Buffer;
		VkDeviceMemory BufferMemory;

		//static Copy(const void* data, uint32_t size);
	};
}
