#include "Proofprch.h"
#include "VulkanTexutre.h"
#define STB_IMAGE_IMPLEMENTATION
#include"../vendor/stb_image.h"
#include "Proof/Renderer/RendererBase.h"
#include "VulkanGraphicsContext.h"
#include "VulkanRenderer/VulkanRenderer.h"
#include "ImGui/imgui_impl_vulkan.h"

namespace Proof
{
	namespace Utils
	{

		static uint32_t GetVulkanMemoryType(VkMemoryPropertyFlags properties, uint32_t type_bits) {
			VkPhysicalDeviceMemoryProperties prop;
			vkGetPhysicalDeviceMemoryProperties(RendererBase::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetGPU(), &prop);
			for (uint32_t i = 0; i < prop.memoryTypeCount; i++) {
				if ((prop.memoryTypes[i].propertyFlags & properties) == properties && type_bits & (1 << i))
					return i;
			}

			return 0xffffffff;
		}

		static uint32_t BytesPerPixel(ImageFormat format) {
			switch (format) {
				case ImageFormat::RGBA:    return 4;
				case ImageFormat::RGBA32F: return 16;
			}
			return 0;

		}

		static VkFormat WalnutFormatToVulkanFormat(ImageFormat format) {
			switch (format) {
				case ImageFormat::RGBA:    return VK_FORMAT_R8G8B8A8_SRGB;
				case ImageFormat::RGBA32F: return VK_FORMAT_R32G32B32A32_SFLOAT;
			}
			//VK_FORMAT_R8G8B8A8_SRGB
			//
			return (VkFormat)0;
		}

	}
	VulkanTexture2D::VulkanTexture2D(const std::string& Path, TextureType textureType) {
		m_Type = textureType;
		auto graphicsContext = RendererBase::GetGraphicsContext()->As<VulkanGraphicsContext>();
		int width, height, channels;
		m_Path = Path;
		uint8_t* data = nullptr;
		if (stbi_is_hdr(Path.c_str())) {
			data = (uint8_t*)stbi_loadf(Path.c_str(), &width, &height, &channels, 4);
			m_Format = ImageFormat::RGBA32F;
		}
		else {
			data = stbi_load(Path.c_str(), &width, &height, &channels, 4);
			m_Format = ImageFormat::RGBA;
		}
		// check for dimension
		{
			VkImageFormatProperties info;
			vkGetPhysicalDeviceImageFormatProperties(graphicsContext->GetGPU(), Utils::WalnutFormatToVulkanFormat(m_Format), VK_IMAGE_TYPE_2D, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, 0, &info);
			if (width > info.maxExtent.width) {
				PF_ENGINE_WARN("Image is to wide, made smaller to be support");
				width = info.maxExtent.width;
			}

			if (height > info.maxExtent.height) {
				PF_ENGINE_WARN("Image is to tall, made smaller to be support");
				height = info.maxExtent.height;
			}
			m_Width = width;
			m_Height = height;
		}
		AllocateMemory(m_Width * m_Height * Utils::BytesPerPixel(m_Format));
		SetData(data);
	}
	VulkanTexture2D::VulkanTexture2D(uint32_t width, uint32_t height, ImageFormat format, const void* data)
		: m_Width(width), m_Height(height), m_Format(format) {
		auto graphicsContext = RendererBase::GetGraphicsContext()->As<VulkanGraphicsContext>();

		// check for dimension
		{
			VkImageFormatProperties info;
			vkGetPhysicalDeviceImageFormatProperties(graphicsContext->GetGPU(), Utils::WalnutFormatToVulkanFormat(m_Format), VK_IMAGE_TYPE_2D, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, 0, &info);
			if (width > info.maxExtent.width) {
				PF_ENGINE_WARN("Image is to wide, made smaller to be support");
				width = info.maxExtent.width;
			}

			if (height > info.maxExtent.height) {
				PF_ENGINE_WARN("Image is to tall, made smaller to be support");
				height = info.maxExtent.height;
			}
			m_Width = width;
			m_Height = height;
		}
		AllocateMemory(m_Width * m_Height * Utils::BytesPerPixel(m_Format));
		if (data)
			SetData(data);
	}
	VulkanTexture2D::~VulkanTexture2D() {
		Release();
	}

	void VulkanTexture2D::AllocateMemory(uint64_t size) {
		auto graphicsContext = RendererBase::GetGraphicsContext()->As<VulkanGraphicsContext>();
		auto device = graphicsContext->GetDevice();

		VkResult err;

		VkFormat vulkanFormat = Utils::WalnutFormatToVulkanFormat(m_Format);

		// Create the Image
		{
			VkImageCreateInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
			info.imageType = VK_IMAGE_TYPE_2D;
			info.format = vulkanFormat;
			info.extent.width = m_Width;
			info.extent.height = m_Height;
			info.extent.depth = 1;
			info.mipLevels = 1;
			info.arrayLayers = 1;
			info.samples = VK_SAMPLE_COUNT_1_BIT;
			info.tiling = VK_IMAGE_TILING_OPTIMAL;
			info.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
			info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			Renderer::Submit([&](CommandBuffer* cmd) {
				VmaAllocationCreateInfo vmaallocInfo = {};
					//let the VMA library know that this data should be GPU native
				vmaallocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

				graphicsContext->CreateVmaImage(info, vmaallocInfo, m_Image);
			});
		}

			// Create the Image View:
		{
			VkImageViewCreateInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			info.image = m_Image.Image;
			info.viewType = VK_IMAGE_VIEW_TYPE_2D;
			info.format = vulkanFormat;
			info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			info.subresourceRange.levelCount = 1;
			info.subresourceRange.layerCount = 1;
			vkCreateImageView(device, &info, nullptr, &m_ImageView);
		}

		// Create sampler:
		{
			VkSamplerCreateInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
			info.magFilter = VK_FILTER_LINEAR;
			info.minFilter = VK_FILTER_LINEAR;
			info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
			info.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			info.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			info.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			info.minLod = -1000;
			info.maxLod = 1000;
			info.maxAnisotropy = 1.0f;
			vkCreateSampler(device, &info, nullptr, &m_Sampler);
		}

	}

	void* VulkanTexture2D::GetID()const {
		auto graphicsContext = RendererBase::GetGraphicsContext()->As<VulkanGraphicsContext>();

		if (m_Set != nullptr)
			return m_Set;
		m_Set = graphicsContext->GetGlobalPool()->AddTexture(m_Sampler, m_ImageView);
		return m_Set;
	}

	void VulkanTexture2D::SetData(const void* data) {
		auto graphicsContext = RendererBase::GetGraphicsContext()->As<VulkanGraphicsContext>();

		size_t upload_size = m_Width * m_Height * Utils::BytesPerPixel(m_Format);
		VulkanBuffer stagingBuffer;
		// Create the Upload Buffer
		{

			VkBufferCreateInfo stagingBufferInfo = {};
			stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			stagingBufferInfo.pNext = nullptr;

			stagingBufferInfo.size = upload_size;
			stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

			//let the VMA library know that this data should be on CPU RAM
			VmaAllocationCreateInfo vmaallocInfo = {};
			vmaallocInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;
			graphicsContext->CreateVmaBuffer(stagingBufferInfo, vmaallocInfo, stagingBuffer);

		}
		//upload to buffer
		{
			char* stagingData;
			vmaMapMemory(graphicsContext->GetVMA_Allocator(), stagingBuffer.Allocation, (void**)(&stagingData));

			memcpy(stagingData, data, upload_size);

			vmaUnmapMemory(graphicsContext->GetVMA_Allocator(), stagingBuffer.Allocation);
		}
		// copy to image
		Renderer::Submit([&](CommandBuffer* cmd) {
			VkImageSubresourceRange range;
			range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			range.baseMipLevel = 0;
			range.levelCount = 1;
			range.baseArrayLayer = 0;
			range.layerCount = 1;
			VkImageMemoryBarrier copy_barrier = {};
			copy_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			copy_barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			copy_barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			copy_barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			copy_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			copy_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			copy_barrier.image = m_Image.Image;
			copy_barrier.subresourceRange = range;
			copy_barrier.srcAccessMask = 0;

			vkCmdPipelineBarrier((VkCommandBuffer)cmd->Get(), VK_PIPELINE_STAGE_HOST_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, NULL, 0, NULL, 1, &copy_barrier);

			VkBufferImageCopy region = {};
			region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			region.imageSubresource.layerCount = 1;
			region.imageExtent.width = m_Width;
			region.imageExtent.height = m_Height;
			region.imageExtent.depth = 1;
			vkCmdCopyBufferToImage((VkCommandBuffer)cmd->Get(), stagingBuffer.Buffer, m_Image.Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

			VkImageMemoryBarrier use_barrier = {};
			use_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			use_barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			use_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
			use_barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			use_barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			use_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			use_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			use_barrier.image = m_Image.Image;
			use_barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			use_barrier.subresourceRange.levelCount = 1;
			use_barrier.subresourceRange.layerCount = 1;
			vkCmdPipelineBarrier((VkCommandBuffer)cmd->Get(), VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, NULL, 0, NULL, 1, &use_barrier);
		});
		vmaDestroyBuffer(graphicsContext->GetVMA_Allocator(), stagingBuffer.Buffer, stagingBuffer.Allocation);
	}
	VkDescriptorImageInfo VulkanTexture2D::GetImageBufferInfo(VkImageLayout imageLayout) {
		VkDescriptorImageInfo imageBufferInfo;
		imageBufferInfo.sampler = GetTextureSampler();
		imageBufferInfo.imageView = GetImageView();
		imageBufferInfo.imageLayout = imageLayout;
		return imageBufferInfo;
	}
	void VulkanTexture2D::Release() {
		//VulkanRenderer::SubmitDatafree([sampler = m_Sampler, imageView = m_ImageView, image = m_Image,
		//	memory = m_Memory, stagingBuffer = m_StagingBuffer, stagingBufferMemory = m_StagingBufferMemory]()
		//{
		//	VkDevice device = Application::GetDevice();
		//
		//	vkDestroySampler(device, sampler, nullptr);
		//	vkDestroyImageView(device, imageView, nullptr);
		//	vkDestroyImage(device, image, nullptr);
		//	vkFreeMemory(device, memory, nullptr);
		//	vkDestroyBuffer(device, stagingBuffer, nullptr);
		//	vkFreeMemory(device, stagingBufferMemory, nullptr);
		//});
		m_Sampler = nullptr;
		m_ImageView = nullptr;
		m_Image.Image = nullptr;
	}
}
