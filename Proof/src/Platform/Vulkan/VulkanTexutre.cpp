#include "Proofprch.h"
#include "VulkanTexutre.h"
#define STB_IMAGE_IMPLEMENTATION
#include"../vendor/stb_image.h"
#include "Proof/Renderer/Renderer.h"
#include "VulkanGraphicsContext.h"
#include "VulkanRenderer/VulkanRenderer.h"

namespace Proof
{
	VulkanTexture2D::VulkanTexture2D(const std::string& Path) {
		auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();

		unsigned char* data = stbi_load(Path.c_str(), &m_Width, &m_Height, &m_Channel, STBI_rgb_alpha);
		if (!data) {
			PF_CORE_ASSERT(false);
			return;
		}
		void* pixel_ptr = data;
		VkDeviceSize imageSize = m_Width * m_Height * 4;

		//the format R8G8B8A8 matches exactly with the pixels loaded from stb_image lib
		VkFormat image_format = VK_FORMAT_R8G8B8A8_SRGB;

		VkBufferCreateInfo stagingBufferInfo = {};
		stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		stagingBufferInfo.pNext = nullptr;

		stagingBufferInfo.size = imageSize;
		stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

		//let the VMA library know that this data should be on CPU RAM
		VmaAllocationCreateInfo vmaallocInfo = {};
		vmaallocInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;

		VulkanBuffer stagingBuffer;

		graphicsContext->CreateVmaBuffer(stagingBufferInfo, vmaallocInfo, stagingBuffer);
		void* stagingData;
		vmaMapMemory(graphicsContext->GetVMA_Allocator(), stagingBuffer.Allocation, &stagingData);

		memcpy(stagingData, pixel_ptr, imageSize);

		vmaUnmapMemory(graphicsContext->GetVMA_Allocator(), stagingBuffer.Allocation);
		stbi_image_free(data);
		
		VkExtent3D imageExtent;
		imageExtent.width = static_cast<uint32_t>(m_Width);
		imageExtent.height = static_cast<uint32_t>(m_Height);
		imageExtent.depth = 1;


		VulkanRenderer::Submit([&](VkCommandBuffer& cmd) {

			VkImageCreateInfo dimg_info = graphicsContext->ImageCreateInfo(image_format, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, imageExtent);

			VmaAllocationCreateInfo dimg_allocinfo = {};
			dimg_allocinfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

			graphicsContext->CreateVmaImage(dimg_info, dimg_allocinfo, m_Image);
			VkImageSubresourceRange range;
			range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			range.baseMipLevel = 0;
			range.levelCount = 1;
			range.baseArrayLayer = 0;
			range.layerCount = 1;

			VkImageMemoryBarrier imageBarrier_toTransfer = {};
			imageBarrier_toTransfer.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;

			imageBarrier_toTransfer.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			//imageBarrier_toTransfer.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			imageBarrier_toTransfer.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageBarrier_toTransfer.image = m_Image.Image;
			imageBarrier_toTransfer.subresourceRange = range;

			imageBarrier_toTransfer.srcAccessMask = 0;
			imageBarrier_toTransfer.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			//barrier the image into the transfer-receive layout
			vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageBarrier_toTransfer);
			
			VkBufferImageCopy copyRegion = {};
			copyRegion.bufferOffset = 0;
			copyRegion.bufferRowLength = 0;
			copyRegion.bufferImageHeight = 0;

			copyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			copyRegion.imageSubresource.mipLevel = 0;
			copyRegion.imageSubresource.baseArrayLayer = 0;
			copyRegion.imageSubresource.layerCount = 1;
			copyRegion.imageExtent = imageExtent;

			//copy the buffer into the image
			vkCmdCopyBufferToImage(cmd, stagingBuffer.Buffer, m_Image.Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyRegion);
		});
		vmaDestroyBuffer(graphicsContext->GetVMA_Allocator(), stagingBuffer.Buffer, stagingBuffer.Allocation);

		VkImageViewCreateInfo viewInfo{};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = m_Image.Image;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
		viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = 1;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;
		vkCreateImageView(graphicsContext->GetDevice(), &viewInfo, nullptr, &m_ImageView);

		VkSamplerCreateInfo samplerInfo{};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.minFilter = VK_FILTER_LINEAR;
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		//samplerInfo.anisotropyEnable = VK_TRUE;
		//samplerInfo.maxAnisotropy = graphicsContext->GetGPUProperties().limits.maxSamplerAnisotropy;
		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.mipLodBias = 0.0f;
		samplerInfo.minLod = 0.0f;
		samplerInfo.maxLod = 0.0f;
		samplerInfo.anisotropyEnable = VK_FALSE;
		samplerInfo.maxAnisotropy = 1.0f;

		vkCreateSampler(graphicsContext->GetDevice(), &samplerInfo, nullptr, &m_TextureSampler);
	}
	VulkanTexture2D::~VulkanTexture2D() {
		auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();

		vmaDestroyImage(graphicsContext->GetVMA_Allocator(), m_Image.Image, m_Image.Allocation);
	}
}
