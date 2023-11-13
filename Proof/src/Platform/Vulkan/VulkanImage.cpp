#include "Proofprch.h"
#include "VulkanImage.h"

#include "VulkanGraphicsContext.h"
#include "VulkanRenderer.h"
#include "Proof/Renderer/Renderer.h"
#include "VulkanUtils/VulkanConvert.h"
#include "VulkanCommandBuffer.h"
#include "VulkanUtils/VulkanConvert.h"
#include "VulkanImguiLayer.h"
#include "VulkanTexutre.h"
#include "Proof/Core/Application.h"
#include "VulkanAllocator.h"
#include "VulkanDevice.h"
namespace Proof {
	

	namespace Utils {
		// dont use this becaue framebuffer sizes are different
		static void ValidateConfiguration(ImageConfiguration& config)
		{
			if (config.Height == 0) config.Height = 1;
			if (config.Width == 0) config.Width = 1;

			auto graphicsContext = VulkanRenderer::GetGraphicsContext();
			const VkPhysicalDeviceProperties& deviceProperties = graphicsContext->GetDevice()->GetPhysicalDevice()->GetProperties();

			if (config.Width > deviceProperties.limits.maxImageDimension2D)
			{
				PF_ENGINE_WARN("Image is too wide, making it smaller to be supported");
				config.Width = deviceProperties.limits.maxImageDimension2D;
			}

			if (config.Height > deviceProperties.limits.maxImageDimension2D)
			{
				PF_ENGINE_WARN("Image is too tall, making it smaller to be supported");
				config.Height = deviceProperties.limits.maxImageDimension2D;
			}

		}

		static VkImageViewType ProofToVulkanFormat(ImageViewType view)
		{
			switch (view)
			{
				case Proof::ImageViewType::View1D:
					return VK_IMAGE_VIEW_TYPE_1D;
					break;
				case Proof::ImageViewType::View2D:
					return VK_IMAGE_VIEW_TYPE_2D;
					break;
				case Proof::ImageViewType::View3D:
					return VK_IMAGE_VIEW_TYPE_3D;
					break;
				case Proof::ImageViewType::ViewCube:
					return VK_IMAGE_VIEW_TYPE_CUBE;
					break;
				case Proof::ImageViewType::View1DArray:
					return VK_IMAGE_VIEW_TYPE_1D_ARRAY;
					break;
				case Proof::ImageViewType::View2DArray:
					return VK_IMAGE_VIEW_TYPE_2D_ARRAY;
					break;
				case Proof::ImageViewType::ViewCubeArray:
					return VK_IMAGE_VIEW_TYPE_CUBE_ARRAY;
					break;
				default:
					break;
			}
			PF_CORE_ASSERT(false);
			return VK_IMAGE_VIEW_TYPE_MAX_ENUM;
		}
	}
	VulkanImage2D::VulkanImage2D(const ImageConfiguration& imageSpec, VkSampleCountFlagBits sampleFlags )
		:
		m_Specification(imageSpec),
		m_SampleFlags(sampleFlags)
	{
		PF_CORE_ASSERT(m_Specification.Height > 0 && m_Specification.Width > 0);
		Utils::ValidateConfiguration(m_Specification);
		Build();
	}
	VulkanImage2D::VulkanImage2D(const ImageConfiguration& imageSpec, VulkanImageInfo info, uint64_t samplerHash)
	{
		m_SampleFlags = VK_SAMPLE_COUNT_1_BIT;
		m_Specification = imageSpec;
		m_Info = info;
		m_SamplerHash = samplerHash;
		m_SwapchainImage = true;
		UpdateDescriptor();
	}
	VulkanImage2D::~VulkanImage2D()
	{

		Release();
	}
	void VulkanImage2D::CallOnResizeFunctions()
	{
		for (auto& callback : m_ResizeCallbacks)
			callback(this);
	}

	void VulkanImage2D::AddResizeCallback(const Image2DResizeCallback& func)
	{
		m_ResizeCallbacks.push_back(func);
	}
	void VulkanImage2D::Resize(uint32_t width, uint32_t height)
	{
		if (m_Specification.Usage == ImageUsage::SwapChain)
		{
			PF_ENGINE_ERROR(" {} Cannot resize swaphcain image directly cannot resize",m_Specification.DebugName);
			return;
		}

		if (m_Specification.Usage == ImageUsage::Texture)
		{
			PF_ENGINE_ERROR("{} Texture images have to handle image resize manually cannot resize", m_Specification.DebugName);
			return;
		}
		if (m_Specification.Width == width && m_Specification.Height == height)
			return;
		uint32_t oldWIdht   = m_Specification.Width, oldHeight  = m_Specification.Height;

		m_Specification.Width = width;
		m_Specification.Height = height;
		Utils::ValidateConfiguration(m_Specification);

		uint32_t oldSamplerHash = m_SamplerHash;
		VulkanImageInfo oldImageInfo = m_Info;
		auto oldDescriptorInfo = m_DescriptorImageInfo;

		Build();
		Count<VulkanImage2D> instance = this;
		Renderer::Submit([instance,oldImageInfo,oldSamplerHash, oldWIdht, oldHeight, oldDescriptorInfo]()
		{
			VkCommandBuffer cmdBuffer = VulkanRenderer::GetGraphicsContext()->GetDevice()->GetCommandBuffer(true);

			VkImageAspectFlags aspectMask;
			if (Utils::IsDepthFormat(instance->m_Specification.Format))
			{
				aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
				if (Utils::ContainStencilFormat(instance->m_Specification.Format))
					aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
			}
			else
			{
				aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

			}
			VkCommandBuffer vulkanCommandBuffer = cmdBuffer;
			auto device = VulkanRenderer::GetGraphicsContext()->GetDevice();
			VkImage srcImage = oldImageInfo.ImageAlloc.Image;
			VkImage dstImage = instance->m_Info.ImageAlloc.Image;
			glm::uvec2 srcSize = { oldWIdht,oldHeight };
			glm::uvec2 dstSize = { instance->GetWidth(),instance->GetHeight()};

			VkImageCopy region;
			region.srcOffset = { 0, 0, 0 };
			region.dstOffset = { 0, 0, 0 };
			region.extent = { srcSize.x, srcSize.y, 1 };
			region.srcSubresource.aspectMask = aspectMask;
			region.srcSubresource.baseArrayLayer = 0;
			region.srcSubresource.mipLevel = 0;
			region.srcSubresource.layerCount = 1;
			region.dstSubresource = region.srcSubresource;

			// Adjust x-dimension offset and extent for the copy operation
			if (region.dstOffset.x + region.extent.width > dstSize.x)
			{
				region.extent.width = dstSize.x - region.dstOffset.x;
			}

			// Adjust y-dimension offset and extent for the copy operation
			if (region.dstOffset.y + region.extent.height > dstSize.y)
			{
				region.extent.height = dstSize.y - region.dstOffset.y;
			}
			VkImageLayout srcImageLayout = oldDescriptorInfo.imageLayout;
			VkImageLayout dstImageLayout = instance->m_DescriptorImageInfo.imageLayout;

			{
				VkImageMemoryBarrier imageMemoryBarrier{};
				imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
				imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
				imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				imageMemoryBarrier.oldLayout = srcImageLayout;
				imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
				imageMemoryBarrier.image = srcImage;

				imageMemoryBarrier.subresourceRange.aspectMask = aspectMask;
				imageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
				imageMemoryBarrier.subresourceRange.baseMipLevel = 0;
				imageMemoryBarrier.subresourceRange.layerCount = 1;
				imageMemoryBarrier.subresourceRange.levelCount = 1;

				vkCmdPipelineBarrier(vulkanCommandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
					0, nullptr,
					0, nullptr,
					1, &imageMemoryBarrier);
			}

			{
				VkImageMemoryBarrier imageMemoryBarrier{};
				imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
				imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
				imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				imageMemoryBarrier.oldLayout = dstImageLayout;
				imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
				imageMemoryBarrier.image = dstImage;

				imageMemoryBarrier.subresourceRange.aspectMask = aspectMask;
				imageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
				imageMemoryBarrier.subresourceRange.baseMipLevel = 0;
				imageMemoryBarrier.subresourceRange.layerCount = 1;
				imageMemoryBarrier.subresourceRange.levelCount = 1;

				vkCmdPipelineBarrier(vulkanCommandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
					0, nullptr,
					0, nullptr,
					1, &imageMemoryBarrier);
			}

			vkCmdCopyImage(vulkanCommandBuffer, srcImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
				dstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);


			{
				VkImageMemoryBarrier imageMemoryBarrier{};
				imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
				imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
				imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
				imageMemoryBarrier.newLayout = srcImageLayout;
				imageMemoryBarrier.image = srcImage;

				imageMemoryBarrier.subresourceRange.aspectMask = aspectMask;
				imageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
				imageMemoryBarrier.subresourceRange.baseMipLevel = 0;
				imageMemoryBarrier.subresourceRange.layerCount = 1;
				imageMemoryBarrier.subresourceRange.levelCount = 1;

				vkCmdPipelineBarrier(vulkanCommandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
					0, nullptr,
					0, nullptr,
					1, &imageMemoryBarrier);
			}

			{
				VkImageMemoryBarrier imageMemoryBarrier{};
				imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
				imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
				imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
				imageMemoryBarrier.newLayout = dstImageLayout;
				imageMemoryBarrier.image = dstImage;

				imageMemoryBarrier.subresourceRange.aspectMask = aspectMask;
				imageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
				imageMemoryBarrier.subresourceRange.baseMipLevel = 0;
				imageMemoryBarrier.subresourceRange.layerCount = 1;
				imageMemoryBarrier.subresourceRange.levelCount = 1;

				vkCmdPipelineBarrier(vulkanCommandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0,
					0, nullptr,
					0, nullptr,
					1, &imageMemoryBarrier);
			}

			VulkanRenderer::GetGraphicsContext()->GetDevice()->FlushCommandBuffer(cmdBuffer);

		});
	
		Renderer::SubmitResourceFree([info = oldImageInfo]()
		{
			auto vulkanDevice = VulkanRenderer::GetGraphicsContext()->GetDevice()->GetVulkanDevice();
			auto graphics = VulkanRenderer::GetGraphicsContext();
			vkDestroyImageView(vulkanDevice, info.ImageView, nullptr);

			VulkanAllocator allocator("VulkanImage2DRelease");
			allocator.DestroyImage(info.ImageAlloc);
		});
		auto graphics = VulkanRenderer::GetGraphicsContext();
		graphics->DeleteSampler(oldSamplerHash);

		
		for (auto& callback : m_ResizeCallbacks)
			callback(this);
		
		PF_ENGINE_TRACE("Resized image {} width {} height {}", m_Specification.DebugName, m_Specification.Width, m_Specification.Height);
	}
	void VulkanImage2D::Build()
	{
		Count<VulkanImage2D> instance = this;

		Renderer::Submit([instance]
		{
			instance->RT_Build();
		});
	}

	void VulkanImage2D::RT_Build()
	{
		auto graphicsContext = VulkanRenderer::GetGraphicsContext();
		auto device = VulkanRenderer::GetGraphicsContext()->GetDevice()->GetVulkanDevice();
		VkImageUsageFlags usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;//for copy image
		if (m_Specification.Usage == ImageUsage::Attachment)
		{
			if (Utils::IsDepthFormat(m_Specification.Format))
				usage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
			else
				usage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
		}

		if (m_Specification.Transfer || m_Specification.Usage == ImageUsage::Texture)
			usage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		if (m_Specification.Usage == ImageUsage::Storage)
			usage |= VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;

		VkImageAspectFlags aspectMask = Utils::IsDepthFormat(m_Specification.Format) ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;
		if (Utils::ContainStencilFormat(m_Specification.Format))
			aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;

		VkFormat vulkanFormat = Utils::ProofFormatToVulkanFormat(m_Specification.Format);
		VmaMemoryUsage memoryUsage = m_Specification.Usage == ImageUsage::HostRead ? VMA_MEMORY_USAGE_GPU_TO_CPU : VMA_MEMORY_USAGE_GPU_ONLY;
		VkImageCreateInfo imageCreateInfo = {};
		imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
		if (m_Specification.Layers == 6 && m_Specification.Height == m_Specification.Width)
			imageCreateInfo.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
		imageCreateInfo.format = vulkanFormat;
		imageCreateInfo.extent.width = m_Specification.Width;
		imageCreateInfo.extent.height = m_Specification.Height;
		imageCreateInfo.extent.depth = 1;
		imageCreateInfo.mipLevels = m_Specification.Mips;
		imageCreateInfo.arrayLayers = m_Specification.Layers;
		imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageCreateInfo.samples = m_SampleFlags;
		imageCreateInfo.tiling = m_Specification.Usage == ImageUsage::HostRead ? VK_IMAGE_TILING_LINEAR : VK_IMAGE_TILING_OPTIMAL;
		imageCreateInfo.usage = usage;
		imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VulkanAllocator allocator("VulkanImage2DBuild");
		allocator.AllocateImage(imageCreateInfo, memoryUsage, m_Info.ImageAlloc);

		VulkanUtils::SetDebugUtilsObjectName(device,VK_OBJECT_TYPE_IMAGE, m_Specification.DebugName, m_Info.ImageAlloc.Image);

		VkImageViewCreateInfo imageViewCreateInfo = {};
		imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewCreateInfo.viewType = m_Specification.Layers > 1 ? VK_IMAGE_VIEW_TYPE_2D_ARRAY : VK_IMAGE_VIEW_TYPE_2D;
		imageViewCreateInfo.format = vulkanFormat;
		imageViewCreateInfo.subresourceRange = {};
		imageViewCreateInfo.subresourceRange.aspectMask = aspectMask;
		imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
		imageViewCreateInfo.subresourceRange.levelCount = m_Specification.Mips;
		imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
		imageViewCreateInfo.subresourceRange.layerCount = m_Specification.Layers; 
		imageViewCreateInfo.image = m_Info.ImageAlloc.Image;
		vkCreateImageView(device, &imageViewCreateInfo, nullptr, &m_Info.ImageView);
		if (m_Info.ImageView)
			VulkanUtils::SetDebugUtilsObjectName(device,VK_OBJECT_TYPE_IMAGE_VIEW, std::format("{} Image View", m_Specification.DebugName), m_Info.ImageView);

		VkSamplerCreateInfo samplerCreateInfo = {};
		samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerCreateInfo.maxAnisotropy = 1.0f;

		if (Utils::IsIntegerBased(m_Specification.Format))
		{
			samplerCreateInfo.magFilter = VK_FILTER_NEAREST;
			samplerCreateInfo.minFilter = VK_FILTER_NEAREST;
			samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
		}
		else
		{
			samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
			samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
			samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		}
		samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		samplerCreateInfo.addressModeV = samplerCreateInfo.addressModeU;
		samplerCreateInfo.addressModeW = samplerCreateInfo.addressModeU;

		samplerCreateInfo.mipLodBias = 0.0f;
		samplerCreateInfo.minLod = 0.0f;
		samplerCreateInfo.maxLod = 100.0f;
		samplerCreateInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_WHITE;

		{
			auto [Sampler, hash] = graphicsContext->GetOrCreateSampler(samplerCreateInfo);
			m_SamplerHash = hash, m_Info.Sampler = Sampler;
		}
		//vkCreateSampler(graphicsContext->GetDevice(), &samplerCreateInfo, nullptr, &m_Info.Sampler);
	//	Utils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_SAMPLER, std::format("{} Sampler", m_Specification.DebugName), m_Info.Sampler);

		if (m_Specification.Usage == ImageUsage::Storage)
		{
			VkCommandBuffer cmdBuffer = VulkanRenderer::GetGraphicsContext()->GetDevice()->GetCommandBuffer(true);

			VkPipelineStageFlags srcStage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
			VkPipelineStageFlags dstStage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
			VkImageSubresourceRange subResourceRange = {};
			subResourceRange.aspectMask = aspectMask;
			subResourceRange.baseMipLevel = 0;
			subResourceRange.levelCount = m_Specification.Mips;
			subResourceRange.layerCount = m_Specification.Layers;

			VkImageMemoryBarrier barrier = {};
			barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			barrier.newLayout = VK_IMAGE_LAYOUT_GENERAL;
			barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.image = m_Info.ImageAlloc.Image;
			barrier.subresourceRange = subResourceRange;
			barrier.pNext = nullptr;

			vkCmdPipelineBarrier(cmdBuffer, srcStage, dstStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

			VulkanRenderer::GetGraphicsContext()->GetDevice()->FlushCommandBuffer(cmdBuffer);

		}
		else if (m_Specification.Usage == ImageUsage::HostRead)
		{
			VkCommandBuffer cmdBuffer = VulkanRenderer::GetGraphicsContext()->GetDevice()->GetCommandBuffer(true);

			//  Transition image to TransferDst 
			VkPipelineStageFlags srcStage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
			VkPipelineStageFlags dstStage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;

			VkImageSubresourceRange subResourceRange = {};
			subResourceRange.aspectMask = aspectMask;
			subResourceRange.baseMipLevel = 0;
			subResourceRange.levelCount = m_Specification.Mips;
			subResourceRange.layerCount = m_Specification.Layers;

			VkImageMemoryBarrier barrier = {};
			barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.image = m_Info.ImageAlloc.Image;
			barrier.subresourceRange = subResourceRange;
			barrier.pNext = nullptr;

			vkCmdPipelineBarrier(cmdBuffer, srcStage, dstStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

			VulkanRenderer::GetGraphicsContext()->GetDevice()->FlushCommandBuffer(cmdBuffer);

		}
		else if (m_Specification.Usage == ImageUsage::Attachment)
		{
			VkCommandBuffer cmdBuffer = VulkanRenderer::GetGraphicsContext()->GetDevice()->GetCommandBuffer(true);

				VkImageLayout newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				if (Utils::IsDepthFormat(m_Specification.Format))
				{
					newLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
				}

				VkCommandBuffer commandBuffer = cmdBuffer;
				//  Transition image to TransferDst 

				VkImageMemoryBarrier imageMemoryBarrier{};
				imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
				imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
				imageMemoryBarrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
				imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				imageMemoryBarrier.newLayout = newLayout;
				imageMemoryBarrier.image = m_Info.ImageAlloc.Image;
				VkImageSubresourceRange subResourceRange = {};
				subResourceRange.aspectMask = aspectMask;
				subResourceRange.baseMipLevel = 0;
				subResourceRange.levelCount = m_Specification.Mips;
				subResourceRange.layerCount = m_Specification.Layers;
				imageMemoryBarrier.subresourceRange = subResourceRange;

				vkCmdPipelineBarrier(
					commandBuffer,
					VK_PIPELINE_STAGE_TRANSFER_BIT,
					VK_PIPELINE_STAGE_TRANSFER_BIT,
					0,
					0, nullptr,
					0, nullptr,
					1, &imageMemoryBarrier);

			VulkanRenderer::GetGraphicsContext()->GetDevice()->FlushCommandBuffer(cmdBuffer);
		}

		UpdateDescriptor();
	}

	void VulkanImage2D::CopyFromHostBuffer(Buffer& buffer)
	{
		auto device = VulkanGraphicsContext::Get()->GetDevice();
		auto vulkanDevice = device->GetVulkanDevice();
		VulkanAllocator allocator("Image2D");

		uint64_t bufferSize = m_Specification.Width * m_Specification.Height * Utils::BytesPerPixel(m_Specification.Format);

		// Create staging buffer
		VkBufferCreateInfo bufferCreateInfo{};
		bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferCreateInfo.size = bufferSize;
		bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VulkanBuffer stagingBuffer;
		allocator.AllocateBuffer(bufferCreateInfo, VMA_MEMORY_USAGE_GPU_TO_CPU, stagingBuffer);

		uint32_t mipCount = 1;
		uint32_t mipWidth = m_Specification.Width, mipHeight = m_Specification.Height;

		VkCommandBuffer copyCmd = device->GetCommandBuffer(true);

		VkImageSubresourceRange subresourceRange = {};
		subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		subresourceRange.baseMipLevel = 0;
		subresourceRange.levelCount = mipCount;
		subresourceRange.layerCount = 1;

		Utils::InsertImageMemoryBarrier(copyCmd, m_Info.ImageAlloc.Image,
			VK_ACCESS_TRANSFER_READ_BIT, 0,
			m_DescriptorImageInfo.imageLayout, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
			VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
			subresourceRange);

		uint64_t mipDataOffset = 0;
		for (uint32_t mip = 0; mip < mipCount; mip++)
		{
			VkBufferImageCopy bufferCopyRegion = {};
			bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			bufferCopyRegion.imageSubresource.mipLevel = mip;
			bufferCopyRegion.imageSubresource.baseArrayLayer = 0;
			bufferCopyRegion.imageSubresource.layerCount = 1;
			bufferCopyRegion.imageExtent.width = mipWidth;
			bufferCopyRegion.imageExtent.height = mipHeight;
			bufferCopyRegion.imageExtent.depth = 1;
			bufferCopyRegion.bufferOffset = mipDataOffset;

			vkCmdCopyImageToBuffer(
				copyCmd,
				m_Info.ImageAlloc.Image,
				VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
				stagingBuffer.Buffer,
				1,
				&bufferCopyRegion);

			uint64_t mipDataSize = mipWidth * mipHeight * sizeof(float) * 4 * 6;
			mipDataOffset += mipDataSize;
			mipWidth /= 2;
			mipHeight /= 2;
		}

		Utils::InsertImageMemoryBarrier(copyCmd, m_Info.ImageAlloc.Image,
			VK_ACCESS_TRANSFER_READ_BIT, 0,
			VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, m_DescriptorImageInfo.imageLayout,
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
			subresourceRange);

		device->FlushCommandBuffer(copyCmd);

		// Copy data from staging buffer
		uint8_t* srcData = allocator.MapMemory<uint8_t>(stagingBuffer.Allocation);
		buffer.Allocate(bufferSize);
		memcpy(buffer.Data, srcData, bufferSize);
		allocator.UnmapMemory(stagingBuffer.Allocation);

		allocator.DestroyBuffer(stagingBuffer);
	}

	void VulkanImage2D::UpdateDescriptor()
	{
		if (Utils::IsDepthFormat(m_Specification.Format))
			m_DescriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
		else if (m_Specification.Usage == ImageUsage::Storage)
			m_DescriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
		else
			m_DescriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

		if(m_Specification.Usage  == ImageUsage::Storage)
			m_DescriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
		else if(m_Specification.Usage == ImageUsage::HostRead)
			m_DescriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

		if(m_Specification.Usage == ImageUsage::SwapChain)
			m_DescriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		if (m_SwapchainImage)
		{
			m_DescriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		}
		m_DescriptorImageInfo.imageView = m_Info.ImageView;
		m_DescriptorImageInfo.sampler = m_Info.Sampler;
	}

	void VulkanImage2D::Release()
	{
		if (m_SwapchainImage)
		{
			const VulkanImageInfo info = m_Info;
			Renderer::SubmitResourceFree([info , samplerHash = m_SamplerHash]()
			{
				auto graphics= VulkanRenderer::GetGraphicsContext();
				//vmaFreeMemory(graphics->GetVMA_Allocator(), info.ImageAlloc.Allocation);
				//vkDestroyImageView(graphics->GetDevice(), info.ImageView, nullptr);
				graphics->DeleteSampler(samplerHash);
			});
			m_SwapchainImage = false;
			m_Info.ImageAlloc.Image = nullptr;
			m_Info.ImageAlloc.Allocation = nullptr;
			m_Info.ImageView = nullptr;
			m_Info.Sampler = nullptr;
		}
		if (m_Info.ImageAlloc.Image == nullptr)return;

		Renderer::SubmitResourceFree([info  = m_Info]()
		{
			auto vulkanDevice = VulkanRenderer::GetGraphicsContext()->GetDevice()->GetVulkanDevice();
			auto graphics = VulkanRenderer::GetGraphicsContext();
			vkDestroyImageView(vulkanDevice, info.ImageView, nullptr);

			VulkanAllocator allocator("VulkanImage2DRelease");
			allocator.DestroyImage(info.ImageAlloc);
		});
		m_Info.ImageAlloc.Image = nullptr;
		m_Info.ImageAlloc.Allocation = nullptr;
		m_Info.Sampler = nullptr;
		m_Info.ImageView = nullptr;
		auto graphics = VulkanRenderer::GetGraphicsContext();
		graphics->DeleteSampler(m_SamplerHash);
	}

	void VulkanImage2D::CopyToHost(Buffer& data)
	{
		/*
		auto graphicsContext = VulkanRenderer::GetGraphicsContext();
		auto device = VulkanRenderer::GetGraphicsContext()->GetDevice();
		size_t bufferSize = m_Specification.Width * m_Specification.Height * Utils::BytesPerPixel(m_Specification.Format) * m_Specification.Layers;
		VkFormat format = Utils::ProofFormatToVulkanFormat(m_Specification.Format);
		VulkanBuffer stagingBuffer;
			// Create the Upload Buffer
		{

			VkBufferCreateInfo stagingBufferInfo = {};
			stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			stagingBufferInfo.pNext = nullptr;

			stagingBufferInfo.size = bufferSize;
			stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

			//let the VMA library know that this data should be on CPU RAM
			VmaAllocationCreateInfo vmaallocInfo = {};
			vmaallocInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;
			graphicsContext->CreateVmaBuffer(stagingBufferInfo, vmaallocInfo, stagingBuffer);

		}
		//upload to buffer
		{
			void* stagingData;
			vmaMapMemory(graphicsContext->GetVMA_Allocator(), stagingBuffer.Allocation, &stagingData);

			memcpy(stagingData, data.Get(), bufferSize);

			vmaUnmapMemory(graphicsContext->GetVMA_Allocator(), stagingBuffer.Allocation);
		}
		*/
	}
	
	VulkanImageView::VulkanImageView(const ImageViewConfiguration& spec)
		:m_Specification(spec)
	{
		WeakCount<VulkanImageView> instanceWeakCount = this;

		m_Specification.Image.As<VulkanImage2D>()->AddResizeCallback([instanceWeakCount](Count<Image2D> image) mutable
		{
			if (!instanceWeakCount)
				return;

			instanceWeakCount.Lock()->Release();
			instanceWeakCount.Lock()->Build();
		});
		Build();
	}
	void VulkanImageView::Build()
	{
		Count<VulkanImageView> instance = this;

		Renderer::Submit([instance]
			{
				instance->RT_Build();
			});
	}

	VulkanImageView::~VulkanImageView()
	{
		Release();
	}

	void VulkanImageView::RT_Build()
	{   
		Count<VulkanImage2D> image = m_Specification.Image.As<VulkanImage2D>();
		auto device = VulkanRenderer::GetGraphicsContext()->GetDevice()->GetVulkanDevice();
		VkImageAspectFlags aspectMask = Utils::IsDepthFormat(image->GetSpecification().Format) ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;
		if (Utils::ContainStencilFormat(image->GetSpecification().Format))
			aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
		VkFormat vulkanFormat = Utils::ProofFormatToVulkanFormat(image->GetSpecification().Format);

		VkImageViewCreateInfo imageViewCreateInfo = {};
		imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewCreateInfo.viewType = Utils::ProofToVulkanFormat(m_Specification.View);
		imageViewCreateInfo.format = vulkanFormat;
		imageViewCreateInfo.subresourceRange = {};
		imageViewCreateInfo.subresourceRange.aspectMask = aspectMask;
		imageViewCreateInfo.subresourceRange.baseMipLevel = m_Specification.Mip;
		imageViewCreateInfo.subresourceRange.levelCount = m_Specification.MipCount;
		imageViewCreateInfo.subresourceRange.baseArrayLayer = m_Specification.Layer;
		imageViewCreateInfo.subresourceRange.layerCount = m_Specification.LayerCount;
		imageViewCreateInfo.image = image->Getinfo().ImageAlloc.Image;
		vkCreateImageView(device, &imageViewCreateInfo, nullptr, &m_ImageView);

		VulkanUtils::SetDebugUtilsObjectName(device,VK_OBJECT_TYPE_IMAGE_VIEW, std::format("{} Image View Layer:{} LayerCount:{} Mip:{} MipCount:{}", image->GetSpecification().DebugName,
			m_Specification.Layer, m_Specification.LayerCount, m_Specification.Mip, m_Specification.MipCount), m_ImageView);
		UpdateDescriptor();
	}
	void VulkanImageView::Release()
	{
		if (m_ImageView == nullptr)return;
		Renderer::SubmitResourceFree([imageView = m_ImageView]()
		{
			auto vulkanDevice = VulkanRenderer::GetGraphicsContext()->GetDevice()->GetVulkanDevice();
			vkDestroyImageView(vulkanDevice, imageView, nullptr);

		});
		m_ImageView = nullptr;
	}
	void VulkanImageView::UpdateDescriptor()
	{
		auto textureSpec = m_Specification.Image->GetSpecification();
		if (Utils::IsDepthFormat(textureSpec.Format))
			m_DescriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
		else if (textureSpec.Usage == ImageUsage::Storage)
			m_DescriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
		else
			m_DescriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

		if (textureSpec.Usage == ImageUsage::Storage)
			m_DescriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
		else if (textureSpec.Usage == ImageUsage::HostRead)
			m_DescriptorImageInfo.imageLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

		m_DescriptorImageInfo.imageView = m_ImageView;
		m_DescriptorImageInfo.sampler = m_Specification.Image.As<VulkanImage2D>()->Getinfo().Sampler;

	}
	glm::uvec2 VulkanImageView::GetMipSize()
	{
		uint32_t width = GetWidth();
		uint32_t height = GetHeight();
		uint32_t mip = m_Specification.Mip;
		while (mip != 0)
		{
			width /= 2;
			height /= 2;
			mip--;
		}

		return { width, height };
	}

	void GetMaxImageDimensions(VkPhysicalDevice physicalDevice, uint32_t& maxWidth, uint32_t& maxHeight) {
		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);

		maxWidth = deviceProperties.limits.maxImageDimension2D;
		maxHeight = deviceProperties.limits.maxImageDimension2D;
		// You can also check other limit members like maxImageDimension1D, maxImageDimension3D, etc.
	}
	
}

