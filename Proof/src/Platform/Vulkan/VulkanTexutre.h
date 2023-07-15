#pragma once
#include <vulkan/vulkan.h>
#include "VulkanUtils/VulkanBufferBase.h"
#include "Proof/Renderer/Texture.h"
#include "Proof/Core/Buffer.h"

#include "Proof/Renderer/Shader.h"
namespace Proof
{
	struct VulkanImageExcessData {
		VkSampler ImageSampler;
		VkImageView ImageView;
		VkImage Image;
	};
	namespace Utils {
		inline void InsertImageMemoryBarrier(VkCommandBuffer commandBuffer, VkImage image, VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask, VkImageLayout oldLayout, VkImageLayout newLayout, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkImageSubresourceRange subresourceRange)
		{
			VkImageMemoryBarrier imageBarrier{};
			imageBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			imageBarrier.srcAccessMask = srcAccessMask;
			imageBarrier.dstAccessMask = dstAccessMask;
			imageBarrier.oldLayout = oldLayout;
			imageBarrier.newLayout = newLayout;
			imageBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			imageBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			imageBarrier.image = image;
			imageBarrier.subresourceRange = subresourceRange;

			vkCmdPipelineBarrier(commandBuffer, srcStageMask, dstStageMask, 0, 0, nullptr, 0, nullptr, 1, &imageBarrier);
		}
		inline void SetImageLayout(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout,  VkImageSubresourceRange subresourceRange)
		{
			VkImageMemoryBarrier imageBarrier{};
			imageBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			imageBarrier.oldLayout = oldLayout;
			imageBarrier.newLayout = newLayout;
			imageBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			imageBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			imageBarrier.image = image;
			imageBarrier.subresourceRange = subresourceRange;
			imageBarrier.srcAccessMask = 0; // Specify the appropriate source access mask
			imageBarrier.dstAccessMask = 0; // Specify the appropriate destination access mask

			vkCmdPipelineBarrier(
				commandBuffer,
				VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
				VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
				0,
				0, nullptr,
				0, nullptr,
				1, &imageBarrier
			);
		}

		inline VkFilter VulkanSamplerFilter(TextureFilter filter)
		{
			switch (filter)
			{
				case Proof::TextureFilter::Linear:
					return VK_FILTER_LINEAR;
				case Proof::TextureFilter::Nearest:
					return VK_FILTER_NEAREST;
				case Proof::TextureFilter::Cubic:
					return VK_FILTER_CUBIC_IMG;
					break;
			}
			PF_CORE_ASSERT(false, "Not supported");
			return (VkFilter)0;
		}

		inline VkSamplerAddressMode VulkanSamplerWrap(TextureWrap wrap)
		{
			switch (wrap)
			{
				case Proof::TextureWrap::Repeat:
					return VK_SAMPLER_ADDRESS_MODE_REPEAT;
					break;
				case Proof::TextureWrap::MirroredRepeat:
					return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
					break;
				case Proof::TextureWrap::ClampEdge:
					return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
					break;
				case Proof::TextureWrap::MirrorClampEdge:
					return VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE;
					break;
				case Proof::TextureWrap::ClampBorder:
					return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
					break;
			}
			PF_CORE_ASSERT(false, "Not supported");
			return (VkSamplerAddressMode)0;
		}
	}
	class VulkanTexture2D : public Texture2D{
	public:

		VulkanTexture2D(const TextureConfiguration& config, const void* data);
		VulkanTexture2D(const std::filesystem::path& path, const TextureConfiguration& config);
		virtual ~VulkanTexture2D();

		virtual Count<Image2D> GetImage() { return m_Image; };

		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual void Resize(Vector2U size) override { Resize(size.X, size.Y); }
		
		virtual uint32_t GetWidth()const override { return m_Config.Width; };
		virtual uint32_t GetHeight() const override { return m_Config.Height; };
		virtual Vector2U GetSize()const override { return {m_Config.Width, m_Config.Height }; }
		
		virtual float GetAspectRatio()const override { return (float)GetWidth() / (float)GetHeight(); };
		uint32_t GetMipLevelCount() { return Utils::GetMipLevelCount(m_Config.Width, m_Config.Height); }

		const std::filesystem::path& GetPath()const override { return m_Path; };
		const TextureConfiguration& GetSpecification()const override { return m_Config; };

		const VkDescriptorImageInfo& GetDescriptorInfoVulkan()const { return *(VkDescriptorImageInfo*)GetResourceDescriptorInfo(); };
		virtual ResourceDescriptorInfo GetResourceDescriptorInfo()const override;

		//bool Loaded()const { return m_ImageData; }
		virtual uint64_t GetHash();
		void GenerateMips();
		virtual void SetData(const void* data);

	private:
		void Build();
		void Release();
		std::filesystem::path m_Path;
		TextureConfiguration m_Config;
		Count<Image2D> m_Image;
		Buffer m_ImageData;
	};

	#if 0
	class VulkanTextureCube : public TextureCube{
	public:
		VulkanTextureCube(const std::filesystem::path& Path, uint32_t dimension = 512, bool generateMips = false);
		VulkanTextureCube(uint32_t dimension = 512, bool generateMips = false);
		VulkanTextureCube(Count<Texture2D> texture,uint32_t dimension = 512, bool generateMips = false);
		VulkanTextureCube(Count<TextureCube> map, Count<class Shader> shader, uint32_t dimension=64, bool generateMips=false);
		
		static Count<TextureCube> GeneratePreFilterMap(Count<TextureCube>map, uint32_t dimension = 128, uint32_t numSamples = 1024);
		virtual ~VulkanTextureCube();
		virtual Count<Image2D> GetImage()const ;
		VkDescriptorImageInfo& GetImageBufferInfo();
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
		const VkDescriptorImageInfo& GetDescriptorInfoVulkan()const { return m_ImageDescriptorInfo; };
		virtual ResourceDescriptorInfo GetResourceDescriptorInfo()const { return (ResourceDescriptorInfo)&m_ImageDescriptorInfo; }
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
		VkDescriptorImageInfo m_ImageDescriptorInfo;
		Count<Image2D> m_Image;
		uint32_t m_MipLevels =1;
	};
	#endif
}
