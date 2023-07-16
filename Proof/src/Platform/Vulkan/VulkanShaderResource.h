#pragma once
#include "Proof/Core/Core.h"
#include "vulkan/vulkan.h"
namespace Proof
{
	struct vulkanSahderFlags
	{
		enum Enum {
			Vertex = BIT(0),
			Fragment = BIT(1),
			Compute = BIT(2),
		};
	};
	enum class RendererResourceType
	{
		None,
		Texture2D,
		CubeTexture,
		UniformBuffer,
		StorageBuffer
	};

	enum class DescriptorResourceType
	{
		None =0,
		ImageSampler,
		UniformBuffer,
		StorageBuffer,
	};
	struct RenderResourceData {
		std::string Name;
		DescriptorResourceType Type;
		int Stage;
		uint32_t DescriptorCount =1; // (array size)
	};
	struct ShaderDescriptorSet
	{
		//https://www.youtube.com/watch?v=M_UI8tIsoYk&t=8848s (1:47:14)
		std::unordered_map<uint32_t, RenderResourceData> UniformBuffers;
		std::unordered_map<uint32_t, RenderResourceData> StorageBuffers;
		std::unordered_map<uint32_t, RenderResourceData> ImageSamplers;
		std::unordered_map<uint32_t, RenderResourceData> StorageImages;	
		std::unordered_map<uint32_t, RenderResourceData> SeperateTextures;
		std::unordered_map<uint32_t, RenderResourceData> SeperateSamplers;

		std::unordered_map<std::string, VkWriteDescriptorSet> WriteDesriptorSet;
		operator bool() const {
			return !(UniformBuffers.empty() && StorageBuffers.empty() && ImageSamplers.empty() && StorageImages.empty() && SeperateTextures.empty()
				&& SeperateSamplers.empty() && StorageImages.empty());
		}
	};
}
