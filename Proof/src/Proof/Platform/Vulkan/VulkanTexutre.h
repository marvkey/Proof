#pragma once
#include "Proof/Renderer/Texture.h"
#include "Proof/Core/Buffer.h" 
#include "VulkanUtils/VulkanBufferBase.h"

#include <vulkan/vulkan.h>
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

		VulkanTexture2D(const TextureConfiguration& config, Buffer buffer);
		VulkanTexture2D(const std::filesystem::path& path, const TextureConfiguration& config);
		VulkanTexture2D(const TextureConfiguration& config);
		virtual ~VulkanTexture2D();

		virtual Count<Image2D> GetImage() { return m_Image; };

		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual void Resize(Vector2U size) override { Resize(size.X, size.Y); }
		virtual void Resize(uint32_t width, uint32_t height, Buffer buffer)override;

		virtual uint32_t GetWidth()const override { return m_Config.Width; };
		virtual uint32_t GetHeight() const override { return m_Config.Height; };
		virtual Vector2U GetSize()const override { return {m_Config.Width, m_Config.Height }; }
		
		virtual float GetAspectRatio()const override { return (float)GetWidth() / (float)GetHeight(); };
		uint32_t GetMipLevelCount() { 
			if (m_Config.GenerateMips)
				return Utils::GetMipLevelCount(m_Config.Width, m_Config.Height);
			else
				return 1;
		}

		const std::filesystem::path& GetPath()const override { return m_Path; };
		const TextureConfiguration& GetSpecification()const override { return m_Config; };

		const VkDescriptorImageInfo& GetDescriptorInfoVulkan()const { return *(VkDescriptorImageInfo*)GetResourceDescriptorInfo(); };
		virtual ResourceDescriptorInfo GetResourceDescriptorInfo()const override;

		//bool Loaded()const { return m_ImageData; }
		virtual uint64_t GetHash();
		void GenerateMips();
		void RT_GenerateMips();
		virtual void SetData(Buffer buffer);
		Count<ImageView> GetImageMip(uint32_t mip, uint32_t layer = 0);

	private:
		void Build();
		void RT_Build();
		void Release();
	private:
		//layer, mip
		std::map<uint32_t, std::map<uint32_t, Count<ImageView>>> m_ImageViews;
		std::filesystem::path m_Path;
		TextureConfiguration m_Config;
		Count<Image2D> m_Image;
		Buffer m_ImageData;
	};
	class VulkanTextureCube : public TextureCube {
	public:
		VulkanTextureCube(const TextureConfiguration& config, const std::filesystem::path& path);
		VulkanTextureCube(const void* data, const TextureConfiguration& config);
		VulkanTextureCube(const TextureConfiguration& config);
		VulkanTextureCube(const TextureConfiguration& config,Count<Texture2D> texture);
		~VulkanTextureCube();
		virtual Count<Image2D> GetImage()const { return m_Image; };
		const VkDescriptorImageInfo& GetDescriptorInfoVulkan()const { return *(VkDescriptorImageInfo*)GetResourceDescriptorInfo(); };
		virtual ResourceDescriptorInfo GetResourceDescriptorInfo()const ;

		virtual void Resize(uint32_t width, uint32_t height) ;
		virtual void Resize(Vector2U size)  { Resize(size.X, size.Y); }

		virtual uint32_t GetWidth()const { return m_Config.Width; };
		virtual uint32_t GetHeight() const { return m_Config.Height; };
		virtual Vector2U GetSize()const { return { m_Config.Width, m_Config.Height }; }

		virtual float GetAspectRatio()const { return (float)GetWidth() / (float)GetHeight(); };
		uint32_t GetMipLevelCount() {
			if (m_Config.GenerateMips)
				return Utils::GetMipLevelCount(m_Config.Width, m_Config.Height);
			else
				return 1;
		};
		void RT_GenerateMips();
		void GenerateMips();

	private:

	
		TextureConfiguration m_Config;
		std::filesystem::path m_Path;

		//texture we load
		Count<VulkanTexture2D> m_Texture;
		Count<Image2D> m_Image;
		void Build();
		void RT_Build();
		void Release();

	};
}