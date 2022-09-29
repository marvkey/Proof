#pragma once
#include "Proof/Renderer/Texture.h"
#include <vulkan/vulkan.h>
#include "VulkanUtils/VulkanBufferBase.h"
namespace Proof
{
	class VulkanTexture2D : public Texture2D {
	public:

		VulkanTexture2D(const std::string& Path); // Transperant if the image is Png IT has an alpha channel
		~VulkanTexture2D();
		const VkImageView GetImageView() {
			return m_ImageView;
		}

		const VkSampler GetTextureSampler() {
			return m_TextureSampler;
		}

		uint32_t GetID() {
			return 0;
		}
	private:
		VulkanImage m_Image;
		VkImageView m_ImageView;
		VkSampler m_TextureSampler;
		int m_Width, m_Height, m_Channel;
	};
}
