#include "Proofprch.h"
#include "VulkanImage.h"

#include "VulkanGraphicsContext.h"
#include "VulkanRenderer/VulkanRenderer.h"
#include "Proof/Renderer/Renderer.h"
#include "VulkanUtils/VulkanConvert.h"
#include "VulkanCommandBuffer.h"
#include "VulkanImage.h"
#include "VulkanUtils/VulkanConvert.h"
#include "VulkanImguiLayer.h"
namespace Proof {
	

	namespace Utils {
		// dont use this becaue framebuffer sizes are different
		static void ValidateConfiguration(ImageConfiguration& config)
		{
			if (config.Height == 0)config.Height = 1;
			if (config.Width == 0)config.Width = 1;
			auto graphicsContext = VulkanRenderer::GetGraphicsContext();

			VkImageFormatProperties info;
			vkGetPhysicalDeviceImageFormatProperties(graphicsContext->GetGPU(), Utils::ProofFormatToVulkanFormat(config.Format), VK_IMAGE_TYPE_2D, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_SAMPLED_BIT, 0, &info);
			if (config.Width > info.maxExtent.width)
			{
				PF_ENGINE_WARN("Image is to wide, made smaller to be support");
				config.Height = info.maxExtent.width;
			}

			if (config.Height > info.maxExtent.height)
			{
				PF_ENGINE_WARN("Image is to tall, made smaller to be support");
				config.Height = info.maxExtent.height;
			}

		}
	}
	VulkanImage2D::VulkanImage2D(const ImageConfiguration& imageSpec)
		:
		m_Specification(imageSpec)
	{
		PF_CORE_ASSERT(m_Specification.Height > 0 && m_Specification.Width > 0);
		Build();
	}
	VulkanImage2D::VulkanImage2D(const ImageConfiguration& imageSpec, VulkanImageInfo info, uint64_t samplerHash)
	{
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
	void VulkanImage2D::Resize(uint32_t width, uint32_t height)
	{
		Release();
		Build();
		for (auto& [layer, layerImage] : m_ImageViews)
		{
			for (auto& [mip, image] : layerImage)
			{
				// recreate
				image.As<VulkanImageView>()->Init();
			}
		}
	}
	void VulkanImage2D::Build()
	{
		auto graphicsContext = VulkanRenderer::GetGraphicsContext();
		auto device = VulkanRenderer::GetGraphicsContext()->GetDevice();
		VkImageUsageFlags usage = VK_IMAGE_USAGE_SAMPLED_BIT;
		if (m_Specification.Usage == ImageUsage::Attachment)
		{
			if (Utils::IsDepthFormat(m_Specification.Format))
				usage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
			else
				usage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
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
		if(m_Specification.Layers  >1)
			imageCreateInfo.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
		imageCreateInfo.format = vulkanFormat;
		imageCreateInfo.extent.width = m_Specification.Width;
		imageCreateInfo.extent.height = m_Specification.Height;
		imageCreateInfo.extent.depth = 1;
		imageCreateInfo.mipLevels = m_Specification.Mips;
		imageCreateInfo.arrayLayers = m_Specification.Layers;
		imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageCreateInfo.tiling = m_Specification.Usage == ImageUsage::HostRead ? VK_IMAGE_TILING_LINEAR : VK_IMAGE_TILING_OPTIMAL;
		imageCreateInfo.usage = usage;
		imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VmaAllocationCreateInfo vmaallocInfo = {};
		vmaallocInfo.usage = memoryUsage;
		graphicsContext->CreateVmaImage(imageCreateInfo, vmaallocInfo, m_Info.ImageAlloc);

		graphicsContext->SetDebugUtilsObjectName(VK_OBJECT_TYPE_IMAGE, m_Specification.DebugName, m_Info.ImageAlloc.Image);

		

		VkImageViewCreateInfo imageViewCreateInfo = {};
		imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewCreateInfo.viewType = m_Specification.Layers > 1 ?  VK_IMAGE_VIEW_TYPE_2D_ARRAY : VK_IMAGE_VIEW_TYPE_2D;
		imageViewCreateInfo.format = vulkanFormat;
		imageViewCreateInfo.subresourceRange = {};
		imageViewCreateInfo.subresourceRange.aspectMask = aspectMask;
		imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
		imageViewCreateInfo.subresourceRange.levelCount = m_Specification.Mips;
		imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
		imageViewCreateInfo.subresourceRange.layerCount = m_Specification.Layers;
		imageViewCreateInfo.image = m_Info.ImageAlloc.Image;
		vkCreateImageView(graphicsContext->GetDevice(), &imageViewCreateInfo, nullptr, &m_Info.ImageView);
		graphicsContext->SetDebugUtilsObjectName(VK_OBJECT_TYPE_IMAGE_VIEW,std::format("{} Image View", m_Specification.DebugName), m_Info.ImageView);

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
			Renderer::SubmitCommand([&](CommandBuffer* cmd)
			{
				VkPipelineStageFlags srcStage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
				VkPipelineStageFlags dstStage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
				VkImageSubresourceRange subResourceRange = {};
				subResourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
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

				vkCmdPipelineBarrier(cmd->As<VulkanCommandBuffer>()->GetCommandBuffer(), srcStage, dstStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

			});

		}
		else if (m_Specification.Usage == ImageUsage::HostRead)
		{
			Renderer::SubmitCommand([&](CommandBuffer* cmd)
			{
				//  Transition image to TransferDst 

				VkPipelineStageFlags srcStage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
				VkPipelineStageFlags dstStage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;

				VkImageSubresourceRange subResourceRange = {};
				subResourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
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

				vkCmdPipelineBarrier(cmd->As<VulkanCommandBuffer>()->GetCommandBuffer(), srcStage, dstStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

			});
		}

		UpdateDescriptor();
	}

	Count<ImageView> VulkanImage2D::GetImageView(Count<Image2D> image,uint32_t layer, uint32_t mip)
	{
		PF_CORE_ASSERT(m_Specification.Layers > layer);
		PF_CORE_ASSERT(m_Specification.Mips > mip);
		ImageViewConfiguration spec;
		spec.Image = image;
		spec.Layer = layer;
		spec.Mip = mip;
		spec.DebugName = std::format("{} ImageView Layer {} Mip {}", m_Specification.DebugName, layer, mip);
		if (m_ImageViews.contains(layer))
		{
			auto& imageLayers = m_ImageViews[layer];
			if (imageLayers.contains(mip))
				return imageLayers[mip];
			imageLayers[mip] = ImageView::Create(spec);
			return imageLayers[mip];
		}
		m_ImageViews[layer][mip] = ImageView::Create(spec);
		return m_ImageViews[layer][mip];
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
		// make this into a callback
		if (Application::Get()->GetImguiLayer() != nullptr)
		{
			Application::Get()->GetImguiLayer().AsRaw<VulkanImguiLayer>()->UpdateImageDescriptor(this);
		}
	}

	void VulkanImage2D::Release()
	{
		if (m_SwapchainImage)
		{
			const VulkanImageInfo info = m_Info;
			Renderer::SubmitDatafree([info , samplerHash = m_SamplerHash]()
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

		const VulkanImageInfo& info = m_Info;
		Renderer::SubmitDatafree([info ]()
		{
			auto vulkanDevice = VulkanRenderer::GetGraphicsContext()->GetDevice();
			auto graphics = VulkanRenderer::GetGraphicsContext();
			vkDestroyImageView(vulkanDevice, info.ImageView, nullptr);
			vmaDestroyImage(VulkanRenderer::GetGraphicsContext()->GetVMA_Allocator(), info.ImageAlloc.Image, info.ImageAlloc.Allocation);
		
		});
		if (Application::Get()->GetImguiLayer() != nullptr)
		{
			Application::Get()->GetImguiLayer().As<VulkanImguiLayer>()->RemoveImageDescriptor(this);
		}
		m_Info.ImageAlloc.Image = nullptr;
		m_Info.ImageAlloc.Allocation = nullptr;
		m_Info.Sampler = nullptr;
		m_Info.ImageView = nullptr;
		auto graphics = VulkanRenderer::GetGraphicsContext();
		graphics->DeleteSampler(m_SamplerHash);
	}

	void VulkanImage2D::CopyToHost(Buffer& data)
	{
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
	}
	
	VulkanImageView::VulkanImageView(const ImageViewConfiguration& spec)
		:m_Specification(spec)
	{
		Init();
	}

	VulkanImageView::~VulkanImageView()
	{
		Release();
	}

	void VulkanImageView::Init()
	{   
		Count<VulkanImage2D> image = m_Specification.Image.As<VulkanImage2D>();
		auto device = VulkanRenderer::GetGraphicsContext()->GetDevice();
		VkImageAspectFlags aspectMask = Utils::IsDepthFormat(image->GetSpecification().Format) ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;
		if (Utils::ContainStencilFormat(image->GetSpecification().Format))
			aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
		VkFormat vulkanFormat = Utils::ProofFormatToVulkanFormat(image->GetSpecification().Format);

		VkImageViewCreateInfo imageViewCreateInfo = {};
		imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		//tempory for prefilter map
		imageViewCreateInfo.viewType = m_Specification.LayerCount == 6 ? VK_IMAGE_VIEW_TYPE_CUBE : VK_IMAGE_VIEW_TYPE_2D;
		imageViewCreateInfo.format = vulkanFormat;
		imageViewCreateInfo.subresourceRange = {};
		imageViewCreateInfo.subresourceRange.aspectMask = aspectMask;
		imageViewCreateInfo.subresourceRange.baseMipLevel = m_Specification.Mip;
		imageViewCreateInfo.subresourceRange.levelCount = m_Specification.MipCount;
		imageViewCreateInfo.subresourceRange.baseArrayLayer = m_Specification.Layer;
		imageViewCreateInfo.subresourceRange.layerCount = m_Specification.LayerCount;
		imageViewCreateInfo.image = image->Getinfo().ImageAlloc.Image;
		vkCreateImageView(device, &imageViewCreateInfo, nullptr, &m_ImageView);

		VulkanRenderer::GetGraphicsContext()->SetDebugUtilsObjectName(VK_OBJECT_TYPE_IMAGE_VIEW, std::format("{} Image View Layer:{} LayerCount:{} Mip:{} MipCount:{}", image->GetSpecification().DebugName, 
			m_Specification.Layer, m_Specification.LayerCount, m_Specification.Mip, m_Specification.MipCount), m_ImageView);
		UpdateDescriptor();
	}
	void VulkanImageView::Release()
	{
		if (m_ImageView == nullptr)return;
		Renderer::SubmitDatafree([imageView = m_ImageView]()
		{
			auto vulkanDevice = VulkanRenderer::GetGraphicsContext()->GetDevice();
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
	void GetMaxImageDimensions(VkPhysicalDevice physicalDevice, uint32_t& maxWidth, uint32_t& maxHeight) {
		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);

		maxWidth = deviceProperties.limits.maxImageDimension2D;
		maxHeight = deviceProperties.limits.maxImageDimension2D;
		// You can also check other limit members like maxImageDimension1D, maxImageDimension3D, etc.
	}
	
}

