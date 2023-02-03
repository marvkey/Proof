#pragma once
#include <vulkan/vulkan.h>
#include "VulkanUtils/VulkanBufferBase.h"
#include "Proof/Renderer/Texture.h"

namespace Proof
{
	struct VulkanImageExcessData {
		VkSampler ImageSampler;
		VkImageView ImageView;
	};
	struct VulkanImage : public Image {
		VulkanImage(const void* image, ImageFormat format, Vector2 size, VulkanImageExcessData data):
			Image(image,format,size)
		{
			m_ExcessData = data;
		}

		VulkanImage() = default;
		VkImageView GetImageView() {
			if (m_ExcessData.has_value())
			{
				return m_ExcessData._Cast<VulkanImageExcessData>()->ImageView;
			}
		}
		VkSampler GetImageSampler() {
			if (m_ExcessData.has_value())
			{
				return m_ExcessData._Cast<VulkanImageExcessData>()->ImageSampler;
			}
		}
	};
	class VulkanTexture : public Texture {
	public:
		virtual ~VulkanTexture() {};

		virtual VkImageView GetImageView() = 0;
		virtual VkSampler GetSampler() = 0;
	};
	class VulkanTexture2D : public Texture2D, VulkanTexture {
	public:

		VulkanTexture2D(const std::string& Path); // Transperant if the image is Png IT has an alpha channel
		VulkanTexture2D(uint32_t width, uint32_t height, ImageFormat format, const void* data);
		~VulkanTexture2D();
		VkSampler GetSampler() {
			return m_Sampler;
		}
		virtual VkImageView GetImageView() {
			return m_ImageView;
		}
	
		virtual std::string GetPath() { return m_Path; };

		const VkSampler GetTextureSampler() {
			return m_Sampler;
		}

		void* GetID ()const;
		virtual Image GetImage()const;

		uint32_t GetWidth() const { return m_Width; }
		uint32_t GetHeight() const { return m_Height; }
		void SetData(const void* data);
		VkDescriptorImageInfo GetImageBufferInfo(VkImageLayout imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	private:
		void AllocateMemory(uint64_t size);
		void Release();
		uint32_t m_MipLevels = 1;
		VkImageView m_ImageView = nullptr;
		VkSampler m_Sampler = nullptr;
		VulkanImageAlloc m_Image;
		int m_Width, m_Height, m_Channel;

		std::string m_Path;
		ImageFormat m_Format = ImageFormat::None;
		mutable VkDescriptorSet m_Set{ VK_NULL_HANDLE };
	};


	class VulkanCubeMap : public CubeMap, VulkanTexture {
	public:
		VulkanCubeMap(const std::filesystem::path& Path);
		virtual std::string GetPath() { return m_Path; };
		virtual Image GetImage()const ;
		VkDescriptorImageInfo GetImageBufferInfo(VkImageLayout imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		VkImageView GetImageView() {
			return m_ImageView;
		}
		virtual VkSampler GetSampler() {
			return m_Sampler;
		}
		
	private:
		void GenerateCubeMap();
		ImageFormat m_Format = ImageFormat::None;
		ScreenSize m_Size;
		int m_Channel;
		VkImageView m_ImageView = nullptr;
		VkSampler m_Sampler = nullptr;
		VulkanImageAlloc m_Image;
		void AllocateMemory(uint64_t size);
		void SetData(const void* data);
		void Release();
		mutable VkDescriptorSet m_Set{ VK_NULL_HANDLE };
		std::string m_Path;
		uint32_t m_MipLevels =1;
	};
}
