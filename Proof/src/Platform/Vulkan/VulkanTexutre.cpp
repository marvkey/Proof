#include "Proofprch.h"
#include "VulkanTexutre.h"
#include "VulkanGraphicsContext.h"
#include "VulkanRenderer/VulkanRenderer.h"
#include "VulkanUtils/VulkanConvert.h"
#include "VulkanVertexArray.h"
#include "VulkanRenderPass.h"
#include "Bitmap.h"
#include "VulkanFrameBuffer.h"
#include "Proof/Renderer/MeshWorkShop.h"
#include "VulkanRenderer/VulkanRenderer.h"
#include "Proof/Renderer/Texture.h"
#include "Proof/Renderer/Renderer2D.h"
#include "VulkanGraphicsPipeline.h"
#include "VulkanComputePass.h"
#include "VulkanComputePipeline.h"
#include "Proof/Renderer/Renderer.h"
#include "Proof/Renderer/CommandBuffer.h"
#include "Proof/Renderer/GraphicsContext.h"
#include "VulkanCommandBuffer.h"
#include "VulkanImage.h"
#include "VulkanAllocator.h"
#include "Vulkan.h"
#include  "Proof/Utils/FileSystem.h"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include"../vendor/stb_image.h"

namespace Proof
{
	static bool IsImageFormatFloat(ImageFormat format)
	{
		switch (format)
		{
			case ImageFormat::R16F:
			case ImageFormat::R32F:
			case ImageFormat::R64F:
			case ImageFormat::RG16F:
			case ImageFormat::RG32F:
			case ImageFormat::RG64F:
			case ImageFormat::RGB16F:
			case ImageFormat::RGB32F:
			case ImageFormat::RGB64F:
			case ImageFormat::RGBA16F:
			case ImageFormat::RGBA32F:
			case ImageFormat::RGBA64F:
				return true;
			default: return false;
		}
		PF_CORE_ASSERT(false);
	}
	namespace Utils {
		static void ValidateConfiguration(TextureConfiguration& config)
		{

			if (config.Height == 0) config.Height = 1;
			if (config.Width == 0) config.Width = 1;

			auto graphicsContext = VulkanRenderer::GetGraphicsContext();
			VkPhysicalDeviceProperties deviceProperties;
			vkGetPhysicalDeviceProperties(graphicsContext->GetGPU(), &deviceProperties);

			if (config.Width > deviceProperties.limits.maxImageDimension2D)
			{
				PF_ENGINE_WARN("Texture is too wide, making it smaller to be supported");
				config.Width = deviceProperties.limits.maxImageDimension2D;
			}

			if (config.Height > deviceProperties.limits.maxImageDimension2D)
			{
				PF_ENGINE_WARN("Texture is too tall, making it smaller to be supported");
				config.Height = deviceProperties.limits.maxImageDimension2D;
			}

		}
	}

	void VulkanTexture2D::GenerateMips()
	{
		m_Config.GenerateMips = true;
		uint32_t mipCount = Utils::GetMipLevelCount(m_Config.Width, m_Config.Height);
		VkImage image = m_Image.As<VulkanImage2D>()->GetinfoRef().ImageAlloc.Image;
		Renderer::SubmitCommand([&](CommandBuffer* cmd) {

			VkCommandBuffer cmdBuffer = cmd->As<VulkanCommandBuffer>()->GetCommandBuffer(Renderer::GetCurrentFrame().FrameinFlight);
			VkImageMemoryBarrier barrier{};
			barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			barrier.image = image;
			barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			barrier.subresourceRange.baseArrayLayer = 0;
			barrier.subresourceRange.layerCount = 1;
			barrier.subresourceRange.levelCount = 1;

			int32_t mipWidth = m_Config.Width;
			int32_t mipHeight = m_Config.Height;

			for (uint32_t i = 1; i < mipCount; i++)
			{
				barrier.subresourceRange.baseMipLevel = i - 1;
				barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
				barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
				barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

				vkCmdPipelineBarrier(cmdBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
					0, nullptr, 0, nullptr, 1, &barrier);

				VkImageBlit blit{};
				blit.srcOffsets[0] = { 0, 0, 0 };
				blit.srcOffsets[1] = { mipWidth, mipHeight, 1 };
				blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				blit.srcSubresource.mipLevel = i - 1;
				blit.srcSubresource.baseArrayLayer = 0;
				blit.srcSubresource.layerCount = 1;
				blit.dstOffsets[0] = { 0, 0, 0 };
				blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
				blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				blit.dstSubresource.mipLevel = i;
				blit.dstSubresource.baseArrayLayer = 0;
				blit.dstSubresource.layerCount = 1;

				vkCmdBlitImage(cmdBuffer, image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
					1, &blit, VK_FILTER_LINEAR);

				barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
				barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
				barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

				vkCmdPipelineBarrier(cmdBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
					0, nullptr, 0, nullptr, 1, &barrier);

				if (mipWidth > 1) mipWidth /= 2;
				if (mipHeight > 1) mipHeight /= 2;
			}

			barrier.subresourceRange.baseMipLevel = mipCount - 1;
			barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			vkCmdPipelineBarrier(cmdBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
				0, nullptr, 0, nullptr, 1, &barrier);
		});
	}
	void VulkanTexture2D::SetData(const void* data)
	{
		Utils::ValidateConfiguration(m_Config);
		m_ImageData = TextureImporter::ToBufferFromMemory(data, m_Config.Format, m_Config.Width, m_Config.Height);
		if (!m_ImageData)
		{
			PF_EC_ERROR("Texture: {} could not load Data", m_Config.DebugName);
			m_ImageData = TextureImporter::ToBufferFromFile("Assets/Textures/NullTexture.png", m_Config.Format, m_Config.Width, m_Config.Height);
		}
		ImageConfiguration imageConfig;
		imageConfig.Format = m_Config.Format;
		imageConfig.Width = m_Config.Width;
		imageConfig.Height = m_Config.Height;
		imageConfig.Mips = m_Config.GenerateMips ? Utils::GetMipLevelCount(m_Config.Width, m_Config.Height) : 1;
		imageConfig.DebugName = m_Config.DebugName + " Image";
		m_Image = Image2D::Create(imageConfig);
		// render thread
		Build();
	}


	VulkanTexture2D::VulkanTexture2D(const TextureConfiguration& config, const void* data)
		:m_Config(config)

	{
		Utils::ValidateConfiguration(m_Config);
		m_ImageData = TextureImporter::ToBufferFromMemory(data, m_Config.Format, m_Config.Width, m_Config.Height);
		if (!m_ImageData)
		{
			PF_EC_ERROR("Texture: {} could not load Data", m_Config.DebugName);
			m_ImageData = TextureImporter::ToBufferFromFile("Assets/Textures/NullTexture.png", m_Config.Format, m_Config.Width, m_Config.Height);
		}
		ImageConfiguration imageConfig;
		imageConfig.Format = m_Config.Format;
		imageConfig.Width = m_Config.Width;
		imageConfig.Height = m_Config.Height;
		imageConfig.Mips = m_Config.GenerateMips ? Utils::GetMipLevelCount(m_Config.Width, m_Config.Height) : 1;
		imageConfig.DebugName = m_Config.DebugName + " ImageTexture";
		m_Image = Image2D::Create(imageConfig);
		// render thread
		Build();
	}
	VulkanTexture2D::VulkanTexture2D(const std::filesystem::path& path, const TextureConfiguration& config)
		:m_Path(path), m_Config(config)
	{
		Utils::ValidateConfiguration(m_Config);
		m_ImageData = TextureImporter::ToBufferFromFile(path, m_Config.Format, m_Config.Width, m_Config.Height);
		if (!m_ImageData)
		{
			PF_EC_ERROR("Texture: {} could not Image Path: {}", m_Config.DebugName, path.string());
			m_ImageData = TextureImporter::ToBufferFromFile("Assets/Textures/NullTexture.png", m_Config.Format, m_Config.Width, m_Config.Height);
		}
		ImageConfiguration imageConfig;
		imageConfig.Format = m_Config.Format;
		imageConfig.Width = m_Config.Width;
		imageConfig.Height = m_Config.Height;
		imageConfig.Mips = m_Config.GenerateMips ? Utils::GetMipLevelCount(m_Config.Width, m_Config.Height) : 1;
		imageConfig.DebugName = m_Config.DebugName + " ImageTexture";
		m_Image = Image2D::Create(imageConfig);
		//render Thread
		Build();
	}
	VulkanTexture2D::VulkanTexture2D(const TextureConfiguration& config)
		:m_Config(config)

	{
		ImageConfiguration imageConfig;
		imageConfig.Format = m_Config.Format;
		imageConfig.Width = m_Config.Width;
		imageConfig.Height = m_Config.Height;
		imageConfig.Mips = m_Config.GenerateMips ? Utils::GetMipLevelCount(m_Config.Width, m_Config.Height) : 1;
		imageConfig.DebugName = m_Config.DebugName + " ImageTexture";
		m_Image = Image2D::Create(imageConfig);
		//render Thread
		Build();
	}
	ResourceDescriptorInfo VulkanTexture2D::GetResourceDescriptorInfo() const
	{
		return m_Image.As<VulkanImage2D>()->GetResourceDescriptorInfo();
	}
	uint64_t VulkanTexture2D::GetHash()
	{
		return (uint64_t)m_Image.As<VulkanImage2D>()->GetDescriptorInfoVulkan().imageView;
	}
	void VulkanTexture2D::Build()
	{
		auto device = VulkanRenderer::GetGraphicsContext()->GetDevice();
		auto graphicsContext = VulkanRenderer::GetGraphicsContext();

		Count<VulkanImage2D> vk_Image = m_Image.As<VulkanImage2D>();
		uint32_t mipCount = m_Config.GenerateMips ? GetMipLevelCount() : 1;

		vk_Image->Release();

		ImageConfiguration& imageConfig = vk_Image->GetSpecificationRef();
		imageConfig.Format = m_Config.Format;
		imageConfig.Width = m_Config.Width;
		imageConfig.Height = m_Config.Height;
		imageConfig.Mips = mipCount;
		if (!m_ImageData || m_Config.Storage)
			imageConfig.Usage = ImageUsage::Storage;
		vk_Image->Build();
		auto& imageInfo = vk_Image->GetinfoRef();

		if (!m_ImageData)
		{
			Renderer::SubmitCommand([&](CommandBuffer* cmd) {
				VkCommandBuffer cmdBuffer = cmd->As<VulkanCommandBuffer>()->GetCommandBuffer(Renderer::GetCurrentFrame().FrameinFlight);

				VkImageSubresourceRange subresourceRange = {};
				subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				subresourceRange.layerCount = mipCount;
				subresourceRange.baseMipLevel = 0;
				subresourceRange.levelCount = GetMipLevelCount();
				subresourceRange.baseArrayLayer = 0;

				Utils::SetImageLayout(cmdBuffer, imageInfo.ImageAlloc.Image, VK_IMAGE_LAYOUT_UNDEFINED, vk_Image->GetDescriptorInfoVulkan().imageLayout, subresourceRange);

			});
		}
		else
		{

			size_t uploadSize = m_ImageData.GetSize();
			VulkanBuffer stagingBuffer;
			VulkanAllocator statingBufferAllocator("VulkanTexture2DStagingBuffer");

			{
				VkBufferCreateInfo stagingBufferInfo = {};
				stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
				stagingBufferInfo.pNext = nullptr;
				stagingBufferInfo.size = uploadSize;
				stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;


				statingBufferAllocator.AllocateBuffer(stagingBufferInfo, VMA_MEMORY_USAGE_CPU_ONLY, stagingBuffer);

				uint8_t* stagingData = statingBufferAllocator.MapMemory<uint8_t>(stagingBuffer.Allocation);
				memcpy(stagingData, (void*)m_ImageData.Get(), uploadSize);
				statingBufferAllocator.UnmapMemory(stagingBuffer.Allocation);

			}
			Renderer::SubmitCommand([&](CommandBuffer* cmd) {
				VkCommandBuffer cmdBuffer = cmd->As<VulkanCommandBuffer>()->GetCommandBuffer(Renderer::GetCurrentFrame().FrameinFlight);
				VkImageSubresourceRange subresourceRange = {};
				subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				subresourceRange.baseMipLevel = 0;
				subresourceRange.levelCount = 1;
				subresourceRange.layerCount = 1;

				VkImageMemoryBarrier imageMemoryBarrier{};
				imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
				imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				imageMemoryBarrier.image = imageInfo.ImageAlloc.Image;
				imageMemoryBarrier.subresourceRange = subresourceRange;
				imageMemoryBarrier.srcAccessMask = 0;
				imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

				vkCmdPipelineBarrier(
					cmdBuffer,
					VK_PIPELINE_STAGE_HOST_BIT,
					VK_PIPELINE_STAGE_TRANSFER_BIT,
					0,
					0, nullptr,
					0, nullptr,
					1,
					&imageMemoryBarrier
				);

				VkBufferImageCopy bufferCopyRegion = {};
				bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				bufferCopyRegion.imageSubresource.mipLevel = 0;
				bufferCopyRegion.imageSubresource.baseArrayLayer = 0;
				bufferCopyRegion.imageSubresource.layerCount = 1;
				bufferCopyRegion.imageExtent.width = m_Config.Width;
				bufferCopyRegion.imageExtent.height = m_Config.Height;
				bufferCopyRegion.imageExtent.depth = 1;
				bufferCopyRegion.bufferOffset = 0;

				vkCmdCopyBufferToImage(cmdBuffer,
					stagingBuffer.Buffer,
					imageInfo.ImageAlloc.Image,
					VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
					1,
					&bufferCopyRegion);

				if (mipCount > 1)
				{

					Utils::InsertImageMemoryBarrier(cmdBuffer, imageInfo.ImageAlloc.Image,
						VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT,
						VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
						VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
						subresourceRange);
				}
				else
				{
					Utils::InsertImageMemoryBarrier(cmdBuffer, imageInfo.ImageAlloc.Image,
						VK_ACCESS_TRANSFER_READ_BIT, VK_ACCESS_SHADER_READ_BIT,
						VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, vk_Image->GetDescriptorInfoVulkan().imageLayout,
						VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
						subresourceRange);
				}
			});
			statingBufferAllocator.DestroyBuffer(stagingBuffer);
		}

		// sampler
		{

			vk_Image->GetinfoRef().Sampler = nullptr;
			graphicsContext->DeleteSampler(vk_Image->GetSamplerHash());

			VkSamplerCreateInfo samplerCreateInfo = {};
			samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
			samplerCreateInfo.maxAnisotropy = 1.0f;
			samplerCreateInfo.magFilter = Utils::VulkanSamplerFilter(m_Config.Filter);
			samplerCreateInfo.minFilter = Utils::VulkanSamplerFilter(m_Config.Filter);
			//samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
			samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
			samplerCreateInfo.addressModeU = Utils::VulkanSamplerWrap(m_Config.Wrap);
			samplerCreateInfo.addressModeV = Utils::VulkanSamplerWrap(m_Config.Wrap);
			samplerCreateInfo.addressModeW = Utils::VulkanSamplerWrap(m_Config.Wrap);
			samplerCreateInfo.mipLodBias = 0.0f;
			samplerCreateInfo.compareOp = VK_COMPARE_OP_NEVER;
			samplerCreateInfo.minLod = 0.0f;
			samplerCreateInfo.maxLod = mipCount;
			/**
			 *
			 * anisotrotic filtering.
			 *
			 */
			samplerCreateInfo.anisotropyEnable = false;
			samplerCreateInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_WHITE;

			auto [sampler, hash] = graphicsContext->GetOrCreateSampler(samplerCreateInfo);
			vk_Image->GetinfoRef().Sampler = sampler;
			vk_Image->SetSamplerHash(hash);
			vk_Image->UpdateDescriptor();

		}

		if (!m_Config.Storage)
		{

			VkImageViewCreateInfo imageViewCreateInfo = {};
			imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			imageViewCreateInfo.format = Utils::ProofFormatToVulkanFormat(m_Config.Format);
			imageViewCreateInfo.subresourceRange = {};
			imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
			imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
			imageViewCreateInfo.subresourceRange.levelCount = mipCount;
			imageViewCreateInfo.subresourceRange.layerCount = 1;
			imageViewCreateInfo.image = imageInfo.ImageAlloc.Image;
			if (imageInfo.ImageView)
				vkDestroyImageView(device, imageInfo.ImageView, nullptr);

			vkCreateImageView(graphicsContext->GetDevice(), &imageViewCreateInfo, nullptr, &vk_Image->GetinfoRef().ImageView);

			graphicsContext->SetDebugUtilsObjectName(VK_OBJECT_TYPE_IMAGE_VIEW, std::format("{} Image View", m_Config.DebugName), vk_Image->GetinfoRef().ImageView);
			vk_Image->UpdateDescriptor();
		}
		if (m_ImageData && m_Config.GenerateMips && mipCount > 1)
			GenerateMips();
		m_ImageData.Release();
	}

	VulkanTexture2D::~VulkanTexture2D() 
	{
		Release();
	}

	void VulkanTexture2D::Resize(uint32_t width, uint32_t height)
	{
		if (m_Config.Width != width && m_Config.Height != height)
			return;
		m_Config.Width = width;
		m_Config.Height = height;

		Build();
	}

	void VulkanTexture2D::Resize(uint32_t width, uint32_t height, const void* data)
	{
		Release();
		m_Config.Width = width;
		m_Config.Height = height;
		SetData(data);
		//Utils::ValidateConfiguration(m_Config);
		//m_ImageData = TextureImporter::ToBufferFromMemory(data, m_Config.Format, m_Config.Width, m_Config.Height);
		//Build();
	}

	void VulkanTexture2D::Release() {
		m_ImageData.Release();
	}

	VulkanTextureCube::VulkanTextureCube(const TextureConfiguration& config, const std::filesystem::path& path)
		:m_Config(config), m_Path(path)
	{
		uint32_t mipCount = m_Config.GenerateMips ? GetMipLevelCount() : 1;

		TextureConfiguration textureConfig;
		textureConfig.DebugName = "VulkanCubemap texture";
		textureConfig.Format = ImageFormat::RGBA32F;
		textureConfig.GenerateMips = false;
		textureConfig.Height = m_Config.Height;
		textureConfig.Width = m_Config.Width;
		textureConfig.Storage = true;
		m_Texture = Texture2D::Create(textureConfig, path).As<VulkanTexture2D>();

		ImageConfiguration imageConfig;
		imageConfig.DebugName = fmt::format("{} TextureCubeImage",FileSystem::GetFileName(path));
		imageConfig.Format = config.Format;
		imageConfig.Height = config.Height;
		imageConfig.Width = config.Width;
		imageConfig.Layers = 6;
		imageConfig.Mips = mipCount;
		if (m_Config.Storage)
			imageConfig.Usage = ImageUsage::Storage;
		else
			imageConfig.Usage = ImageUsage::Attachment;
		imageConfig.Transfer = true;
		m_Image = Image2D::Create(imageConfig);
		Build();

	}
	VulkanTextureCube::VulkanTextureCube(const void* data, const TextureConfiguration& config)
		:m_Config(config)

	{
		uint32_t mipCount = m_Config.GenerateMips ? GetMipLevelCount() : 1;

		TextureConfiguration textureConfig;
		textureConfig.DebugName = "VulkanCubemap texture";
		textureConfig.Format = m_Config.Format;
		textureConfig.GenerateMips = false;
		textureConfig.Height = m_Config.Height;
		textureConfig.Width = m_Config.Width;
		textureConfig.Storage = true;
		m_Texture = Texture2D::Create(data, textureConfig).As<VulkanTexture2D>();

		ImageConfiguration imageConfig;
		imageConfig.DebugName = fmt::format("{} TextureCubeImage", config.DebugName);
		imageConfig.Format = config.Format;
		imageConfig.Height = config.Height;
		imageConfig.Width = config.Width;
		imageConfig.Layers = 6;
		imageConfig.Mips = mipCount;

		if (m_Config.Storage)
			imageConfig.Usage = ImageUsage::Storage;
		else
			imageConfig.Usage = ImageUsage::Attachment;
		imageConfig.Transfer = true;
		m_Image = Image2D::Create(imageConfig);
		Build();
	}
	VulkanTextureCube::VulkanTextureCube(const TextureConfiguration& config)
		:m_Config(config)
	{
		uint32_t mipCount = m_Config.GenerateMips ? GetMipLevelCount() : 1;

		ImageConfiguration imageConfig;
		imageConfig.DebugName = fmt::format("{} TextureCubeImage", config.DebugName);
		imageConfig.DebugName = "TextureCubeImage";
		imageConfig.Format = config.Format;
		imageConfig.Height = config.Height;
		imageConfig.Width = config.Width;
		imageConfig.Layers = 6;
		imageConfig.Mips = mipCount;
		if (m_Config.Storage)
			imageConfig.Usage = ImageUsage::Storage;
		else
			imageConfig.Usage = ImageUsage::Attachment;
		imageConfig.Transfer = true;
		m_Image = Image2D::Create(imageConfig);
		Build();
	}
	VulkanTextureCube::VulkanTextureCube(const TextureConfiguration& config, Count<Texture2D> texture)
		:m_Config(config)
	{
		uint32_t mipCount = m_Config.GenerateMips ? GetMipLevelCount() : 1;

		m_Texture = texture.As<VulkanTexture2D>();

		ImageConfiguration imageConfig;
		imageConfig.DebugName = "TextureCubeImage";
		imageConfig.DebugName = fmt::format("{} TextureCubeImage", config.DebugName);
		imageConfig.Format = config.Format;
		imageConfig.Height = config.Height;
		imageConfig.Width = config.Width;
		imageConfig.Layers = 6;
		imageConfig.Mips = mipCount;

		if (m_Config.Storage)
			imageConfig.Usage = ImageUsage::Storage;
		else
			imageConfig.Usage = ImageUsage::Attachment;
		imageConfig.Transfer = true;
		m_Image = Image2D::Create(imageConfig);
		Build();
	}
	void VulkanTextureCube::GenerateMips()
	{
		m_Config.GenerateMips = true;

		uint32_t mipCount = Utils::GetMipLevelCount(m_Config.Width, m_Config.Height);
		m_Image.As<VulkanImage2D>()->GetSpecificationRef().Mips = mipCount;
		const uint32_t faces = 6;

		Renderer::SubmitCommand([&](CommandBuffer* cmd) {
			VkImage image = m_Image.As<VulkanImage2D>()->GetinfoRef().ImageAlloc.Image;
			VkDescriptorImageInfo  vk_ImageDescriporInfo =m_Image.As<VulkanImage2D>()->GetDescriptorInfoVulkan();
			VkCommandBuffer cmdBuffer = cmd->As<VulkanCommandBuffer>()->GetCommandBuffer(Renderer::GetCurrentFrame().FrameinFlight);
			
			VkImageSubresourceRange subresourceRange{};
			subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			subresourceRange.layerCount = faces;
			subresourceRange.baseArrayLayer = 0;
			subresourceRange.levelCount = mipCount;
			subresourceRange.baseMipLevel = 0;

			Utils::SetImageLayout(cmdBuffer, image, vk_ImageDescriporInfo.imageLayout, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, subresourceRange);
			for (uint32_t face = 0; face < faces; face++)
			{
				VkImageMemoryBarrier barrier{};
				barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
				barrier.image = image;
				barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				barrier.subresourceRange.baseArrayLayer = face;
				barrier.subresourceRange.layerCount = 1;
				barrier.subresourceRange.levelCount = 1;

				int32_t mipWidth = m_Config.Width;
				int32_t mipHeight = m_Config.Height;

				for (uint32_t mip = 1; mip < mipCount; mip++)
				{
					barrier.subresourceRange.baseMipLevel = mip - 1;
					barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
					barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
					barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
					barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

					vkCmdPipelineBarrier(cmdBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
						0, nullptr, 0, nullptr, 1, &barrier);

					VkImageBlit blit{};
					blit.srcOffsets[0] = { 0, 0, 0 };
					blit.srcOffsets[1] = { mipWidth, mipHeight, 1 };
					blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
					blit.srcSubresource.mipLevel = mip - 1;
					blit.srcSubresource.baseArrayLayer = face;
					blit.srcSubresource.layerCount = 1;
					blit.dstOffsets[0] = { 0, 0, 0 };
					blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
					blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
					blit.dstSubresource.mipLevel = mip;
					blit.dstSubresource.baseArrayLayer = face;
					blit.dstSubresource.layerCount = 1;

					vkCmdBlitImage(cmdBuffer, image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
						1, &blit, VK_FILTER_LINEAR);

					barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
					barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
					barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
					barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

					vkCmdPipelineBarrier(cmdBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
						0, nullptr, 0, nullptr, 1, &barrier);

					if (mipWidth > 1) mipWidth /= 2;
					if (mipHeight > 1) mipHeight /= 2;
				}

				barrier.subresourceRange.baseMipLevel = mipCount - 1;
				barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
				barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

				vkCmdPipelineBarrier(cmdBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
					0, nullptr, 0, nullptr, 1, &barrier);
			}

			Utils::SetImageLayout(cmdBuffer, image, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, vk_ImageDescriporInfo.imageLayout, subresourceRange);
		});
	}
	void VulkanTextureCube::Build()
	{
		auto device = VulkanRenderer::GetGraphicsContext()->GetDevice();
		auto graphicsContext = VulkanRenderer::GetGraphicsContext();

		Count<VulkanImage2D> vk_Image = m_Image.As<VulkanImage2D>();
		uint32_t mipCount = m_Config.GenerateMips ? GetMipLevelCount() : 1;

		vk_Image->Release();
		ImageConfiguration& imageConfig = vk_Image->GetSpecificationRef();
		imageConfig.Format = m_Config.Format;
		imageConfig.Width = m_Config.Width;
		imageConfig.Height = m_Config.Height;
		imageConfig.Mips = mipCount;
		imageConfig.Layers = 6;
		//if (!m_Texture || m_Config.Storage)
		//	imageConfig.Usage = ImageUsage::Storage;

		//need this to generate irradiance map
		if(m_Config.Storage)
			imageConfig.Usage = ImageUsage::Storage;
		vk_Image->Build();

		VulkanImageInfo& imageInfoRef = vk_Image->GetinfoRef();
		if (!m_Texture)
		{

			VkImageViewCreateInfo imageViewCreateInfo = {};
			imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
			imageViewCreateInfo.format = Utils::ProofFormatToVulkanFormat(m_Config.Format);
			imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
			imageViewCreateInfo.subresourceRange.levelCount = mipCount;
			imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
			imageViewCreateInfo.subresourceRange.layerCount = 6;
			imageViewCreateInfo.image = imageInfoRef.ImageAlloc.Image;
			imageViewCreateInfo.components = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
			if (imageInfoRef.ImageView)
				vkDestroyImageView(device, imageInfoRef.ImageView, nullptr);

			imageInfoRef.ImageView = nullptr;
			vkCreateImageView(graphicsContext->GetDevice(), &imageViewCreateInfo, nullptr, &vk_Image->GetinfoRef().ImageView);

			graphicsContext->SetDebugUtilsObjectName(VK_OBJECT_TYPE_IMAGE_VIEW, std::format("{} Image View", m_Config.DebugName), vk_Image->GetinfoRef().ImageView);
			
			// sampler
			{

				vk_Image->GetinfoRef().Sampler = nullptr;
				graphicsContext->DeleteSampler(vk_Image->GetSamplerHash());

				VkSamplerCreateInfo samplerCreateInfo = {};
				samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
				samplerCreateInfo.maxAnisotropy = 1.0f;
				samplerCreateInfo.magFilter = Utils::VulkanSamplerFilter(m_Config.Filter);
				samplerCreateInfo.minFilter = Utils::VulkanSamplerFilter(m_Config.Filter);
				samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
				samplerCreateInfo.addressModeU = Utils::VulkanSamplerWrap(m_Config.Wrap);
				samplerCreateInfo.addressModeV = Utils::VulkanSamplerWrap(m_Config.Wrap);
				samplerCreateInfo.addressModeW = Utils::VulkanSamplerWrap(m_Config.Wrap);
				samplerCreateInfo.mipLodBias = 0.0f;
				samplerCreateInfo.compareOp = VK_COMPARE_OP_NEVER;
				samplerCreateInfo.minLod = 0.0f;
				samplerCreateInfo.maxLod = mipCount;
				/**
				 *
				 * anisotrotic filtering.
				 *
				 */
				samplerCreateInfo.anisotropyEnable = false;
				samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;

				auto [sampler, hash] = graphicsContext->GetOrCreateSampler(samplerCreateInfo);
				vk_Image->GetinfoRef().Sampler = sampler;
				vk_Image->SetSamplerHash(hash);
				vk_Image->UpdateDescriptor();

			}


			Renderer::SubmitCommand([&](CommandBuffer* cmd) {
				VkCommandBuffer cmdBuffer = cmd->As<VulkanCommandBuffer>()->GetCommandBuffer(Renderer::GetCurrentFrame().FrameinFlight);

				VkImageSubresourceRange subresourceRange = {};
				subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				subresourceRange.baseArrayLayer = 0;
				subresourceRange.layerCount = 6;
				subresourceRange.levelCount = mipCount;
				subresourceRange.baseMipLevel = 0;
				Utils::SetImageLayout(cmdBuffer, imageInfoRef.ImageAlloc.Image, VK_IMAGE_LAYOUT_UNDEFINED, vk_Image->GetDescriptorInfoVulkan().imageLayout, subresourceRange);

			});
			vk_Image->UpdateDescriptor();
		}
		else
		{

		// sampler
			{

				vk_Image->GetinfoRef().Sampler = nullptr;
				graphicsContext->DeleteSampler(vk_Image->GetSamplerHash());

				VkSamplerCreateInfo samplerCreateInfo = {};
				samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
				samplerCreateInfo.maxAnisotropy = 1.0f;
				samplerCreateInfo.magFilter = Utils::VulkanSamplerFilter(m_Config.Filter);
				samplerCreateInfo.minFilter = Utils::VulkanSamplerFilter(m_Config.Filter);
				samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
				samplerCreateInfo.addressModeU = Utils::VulkanSamplerWrap(m_Config.Wrap);
				samplerCreateInfo.addressModeV = Utils::VulkanSamplerWrap(m_Config.Wrap);
				samplerCreateInfo.addressModeW = Utils::VulkanSamplerWrap(m_Config.Wrap);
				samplerCreateInfo.mipLodBias = 0.0f;
				samplerCreateInfo.compareOp = VK_COMPARE_OP_NEVER;
				samplerCreateInfo.minLod = 0.0f;
				samplerCreateInfo.maxLod = mipCount;
				/**
				 *
				 * anisotrotic filtering.
				 *
				 */
				samplerCreateInfo.anisotropyEnable = false;
				samplerCreateInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;

				auto [sampler, hash] = graphicsContext->GetOrCreateSampler(samplerCreateInfo);
				vk_Image->GetinfoRef().Sampler = sampler;
				vk_Image->SetSamplerHash(hash);
				vk_Image->UpdateDescriptor();

			}

			{
				VkImageViewCreateInfo imageViewCreateInfo = {};
				imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
				imageViewCreateInfo.format = Utils::ProofFormatToVulkanFormat(m_Config.Format);
				imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				imageViewCreateInfo.subresourceRange.levelCount = mipCount;
				imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
				imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
				imageViewCreateInfo.subresourceRange.layerCount = 6;
				imageViewCreateInfo.image = imageInfoRef.ImageAlloc.Image;
				if (imageInfoRef.ImageView)
					vkDestroyImageView(device, imageInfoRef.ImageView, nullptr);

				vkCreateImageView(graphicsContext->GetDevice(), &imageViewCreateInfo, nullptr, &vk_Image->GetinfoRef().ImageView);

				graphicsContext->SetDebugUtilsObjectName(VK_OBJECT_TYPE_IMAGE_VIEW, std::format("{} Image View", m_Config.DebugName), vk_Image->GetinfoRef().ImageView);
				vk_Image->UpdateDescriptor();
			}

			ComputePipelineConfig computePipelineConfig;
			computePipelineConfig.DebugName = "EquirectangularToCubemap Pipeline";
			computePipelineConfig.Shader = Renderer::GetShader("EquirectangularToCubemap");

			Count<ComputePipeline> computePipeline = ComputePipeline::Create(computePipelineConfig);


			ComputePassConfiguration computePassConfig;
			computePassConfig.DebugName = "EquirectangularToCubemap Pass";
			computePassConfig.Pipeline = computePipeline;

			auto computePass = ComputePass::Create(computePassConfig);

			computePass->SetInput("u_EquirectangularMap", m_Texture);
			Count<TextureCube> cube = this;
			computePass->SetInput("u_CubeMap", cube);
			struct pushData
			{
				Vector2U imageSize;
				Vector2U cubeSize;
			};

			// make sure cube does not delete this due to refercne cout
			Renderer::SubmitDatafree([cube = cube]()
			{
				auto id = cube->GetHeight();
			});
			Renderer::SubmitCommand([&](CommandBuffer* buffer) {

				Count<RenderCommandBuffer>renderCommandBuffer = RenderCommandBuffer::Create(buffer);
				Renderer::BeginComputePass(renderCommandBuffer, computePass);

				pushData pushData;
				pushData.imageSize = { m_Texture->GetSize() };
				pushData.cubeSize = GetSize();
				computePass->PushData("pc", &pushData);
				computePass->Dispatch(m_Config.Width / 32, m_Config.Height / 32, 6);
				Renderer::EndComputePass(computePass);

			});
			if (m_Texture && m_Config.GenerateMips && mipCount > 1)
				GenerateMips();
			m_Texture = nullptr;
		}
	}
	void VulkanTextureCube::Release()
	{
		m_Texture = nullptr;
		m_Image = nullptr;
	}
	ResourceDescriptorInfo VulkanTextureCube::GetResourceDescriptorInfo()const
	{
		return m_Image->GetResourceDescriptorInfo();
	}
	void VulkanTextureCube::Resize(uint32_t width, uint32_t height)
	{
		m_Config.Width = width;
		m_Config.Height = height;
		Build();
	}
	VulkanTextureCube::~VulkanTextureCube()
	{
		Release();
	}

}