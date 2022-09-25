#pragma once
#include<vulkan/vulkan.h>
namespace Proof
{
	enum class VulkanDataFormat {
		Int = VK_FORMAT_R32_SINT,
		Uint32_t = VK_FORMAT_R32_UINT,
		Uint64_t = VK_FORMAT_R64_UINT,
		Float = VK_FORMAT_R32_SFLOAT,
		Double = VK_FORMAT_R64_SFLOAT,
		Char = VK_FORMAT_R4G4_UNORM_PACK8,

		//signed int vec2
		Ivec2 = VK_FORMAT_R32G32_SINT,
		//signed int vec3
		Ivec3 = VK_FORMAT_R32G32B32_SINT,
		//signed int vec4
		Ivec4 = VK_FORMAT_R32G32B32A32_SINT,


		//vector 2 with float
		Vec2 = VK_FORMAT_R32G32_SFLOAT,
		// vector 3 with float
		Vec3 = VK_FORMAT_R32G32B32_SFLOAT,
		//vector 4 with float
		Vec4 = VK_FORMAT_R32G32B32A32_SFLOAT,

		//vector 2 with double
		DVec2 = VK_FORMAT_R64G64_SFLOAT,
		// vector 3 with double
		DVec3 = VK_FORMAT_R64G64B64_SFLOAT,
		//vector 4 with double
		DVec4 = VK_FORMAT_R64G64B64A64_SFLOAT,

	};
}
