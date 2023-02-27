#pragma once
#include <vulkan/vulkan.h>
#include "VulkanUtils/VulkanBufferBase.h"
#include "Proof/Renderer/Texture.h"
#include "Proof/Renderer/Shader.h"
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
		virtual VkDescriptorImageInfo GetImageBufferInfo(VkImageLayout imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) =0;
		virtual VulkanImageAlloc GetImageAlloc() = 0;
	};
	class VulkanTexture2D : public Texture2D, VulkanTexture {
	public:

		VulkanTexture2D(const std::string& Path); // Transperant if the image is Png IT has an alpha channel
		VulkanTexture2D(uint32_t width, uint32_t height, ImageFormat format, const void* data);

		VulkanTexture2D(TextureConfig config);
		~VulkanTexture2D();
		static Count<Texture2D> GenerateBRDF(uint32_t dimension =512, uint32_t sampleCount=1024);
		VkSampler GetSampler() {
			return m_Sampler;
		}
		virtual VkImageView GetImageView() {
			return m_ImageView;
		}

		const VkSampler GetTextureSampler() {
			return m_Sampler;
		}
		VulkanImageAlloc GetImageAlloc() {
			return m_Image;
		}
		virtual std::string GetPath()const {
			return m_Path;
		}
		void* GetID ()const;
		virtual Image GetImage()const;

		uint32_t GetWidth() const { return m_Width; }
		uint32_t GetHeight() const { return m_Height; }
		void SetData(const void* data);
		VkDescriptorImageInfo GetImageBufferInfo(VkImageLayout imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	private:
		std::string m_Path;
		void AllocateMemory(uint64_t size, uint32_t bits = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, VkSamplerAddressMode mode= VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_REPEAT);
		void Release();
		uint32_t m_MipLevels = 1;
		VkImageView m_ImageView = nullptr;
		VkSampler m_Sampler = nullptr;
		VulkanImageAlloc m_Image;
		uint32_t m_Width, m_Height, m_Channel;

		ImageFormat m_Format = ImageFormat::None;
		mutable VkDescriptorSet m_Set{ VK_NULL_HANDLE };
	};


	class VulkanCubeMap : public CubeMap, VulkanTexture {
	public:
		VulkanCubeMap(const std::filesystem::path& Path, uint32_t dimension = 512, bool generateMips = false);
		VulkanCubeMap(uint32_t dimension = 512, bool generateMips = false);
		VulkanCubeMap(Count<Texture2D> texture,uint32_t dimension = 512, bool generateMips = false);
		VulkanCubeMap(Count<CubeMap> map, Count<class Shader> shader, uint32_t dimension=64, bool generateMips=false);
		
		static Count<CubeMap> GeneratePreFilterMap(Count<CubeMap>map, uint32_t dimension = 128, uint32_t numSamples = 1024);
		virtual ~VulkanCubeMap();
		virtual Image GetImage()const ;
		VkDescriptorImageInfo GetImageBufferInfo(VkImageLayout imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		VkImageView GetImageView() {
			return m_ImageView;
		}
		virtual VkSampler GetSampler() {
			return m_Sampler;
		}
		VulkanImageAlloc GetImageAlloc() {
			return m_Image;
		}
		virtual std::string GetPath()const {
			return m_Path;
		}
		void SetData(const void* data);
		void SetData(const void* data[6]);
		void SetData(Count<Texture2D> textures);
		void SetData(Count<Texture2D> textures[6]);
	private:
		uint32_t m_Dimension;
		std::string m_Path;
		// shader makes it esier for us to generate irradiance and other typees of maps
		void GenerateCubeMap(Count<Texture> textures,Count<class Shader> shader = Shader::GetOrCreate("Equirectangular to Cubemap",
			ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/PBR/PBRCubeMap/EquirectangularToCubemap.shader"));
		ImageFormat m_Format = ImageFormat::RGBA32F;
		VkImageView m_ImageView = nullptr;
		VkSampler m_Sampler = nullptr;
		VulkanImageAlloc m_Image;

		void AllocateMemory();
		void Release();
		mutable VkDescriptorSet m_Set{ VK_NULL_HANDLE };

		uint32_t m_MipLevels =1;
	};
}
