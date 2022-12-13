#pragma once
#include "Proof/Renderer/Texture.h"
#include <vulkan/vulkan.h>
#include "VulkanUtils/VulkanBufferBase.h"
#include "Proof/Renderer/Texture.h"

namespace Proof
{
	class VulkanTexture2D : public Texture2D {
	public:

		VulkanTexture2D(const std::string& Path, TextureType textureType = TextureType::None); // Transperant if the image is Png IT has an alpha channel
		VulkanTexture2D(uint32_t width, uint32_t height, ImageFormat format, const void* data);
		~VulkanTexture2D();
		const VkImageView GetImageView() {
			return m_ImageView;
		}
	
		virtual std::string GetPath() { return m_Path; };

		const VkSampler GetTextureSampler() {
			return m_Sampler;
		}

		void* GetID();

		uint32_t GetWidth() const { return m_Width; }
		uint32_t GetHeight() const { return m_Height; }
		void SetData(const void* data);
	private:
		void AllocateMemory(uint64_t size);
		void Release();
		TextureType m_Type = TextureType::None;
		VkImageView m_ImageView = nullptr;
		VkSampler m_Sampler = nullptr;
		VulkanImage m_Image;
		size_t m_AlignedSize = 0;
		int m_Width, m_Height, m_Channel;

		std::string m_Path;
		ImageFormat m_Format = ImageFormat::None;
		VkDescriptorSet m_Set{ VK_NULL_HANDLE };
	};
}
