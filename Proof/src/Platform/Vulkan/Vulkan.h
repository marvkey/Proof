#pragma once
#include "Proof/Core/Core.h"
#include<vulkan/vulkan.h>
#define VK_CHECK_RESULT(X){ if((X != VK_SUCCESS)) {PF_ENGINE_ERROR("[Vulkan] Error: VkResult = {}", X);__debugbreak();  } }

namespace Proof
{
	namespace Utils {
           // Hash combine function
        template <typename T>
        inline void hash_combine(std::size_t& seed, const T& value)
        {
            seed ^= std::hash<T>{}(value)+0x9e3779b9 + (seed << 6) + (seed >> 2);
        }

		inline uint64_t GetHashSamplerInfo(const VkSamplerCreateInfo& createInfo)
		{
            std::size_t seed = 0;
            hash_combine(seed, createInfo.sType);
            if(createInfo.pNext != nullptr)
                hash_combine(seed, createInfo.pNext);

            hash_combine(seed, createInfo.flags);
            hash_combine(seed, createInfo.magFilter);
            hash_combine(seed, createInfo.minFilter);
            hash_combine(seed, createInfo.mipmapMode);
            hash_combine(seed, createInfo.addressModeU);
            hash_combine(seed, createInfo.addressModeV);
            hash_combine(seed, createInfo.addressModeW);
            hash_combine(seed, createInfo.mipLodBias);
            hash_combine(seed, createInfo.anisotropyEnable);
            hash_combine(seed, createInfo.maxAnisotropy);
            hash_combine(seed, createInfo.compareEnable);
            hash_combine(seed, createInfo.compareOp);
            hash_combine(seed, createInfo.minLod);
            hash_combine(seed, createInfo.maxLod);
            hash_combine(seed, createInfo.borderColor);
            hash_combine(seed, createInfo.unnormalizedCoordinates);
            return seed;
        }
     
	}
}
