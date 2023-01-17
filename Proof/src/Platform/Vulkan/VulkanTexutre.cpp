#include "Proofprch.h"
#include "VulkanTexutre.h"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include"../vendor/stb_image.h"
#include "Proof/Renderer/RendererBase.h"
#include "VulkanGraphicsContext.h"
#include "VulkanRenderer/VulkanRenderer.h"
#include "VulkanUtils/VulkanConvert.h"
#include "ktx/include/ktx.h"
#include "Bitmap.h"
namespace Proof
{
	static void setImageLayout(
		VkCommandBuffer cmdbuffer,
		VkImage image,
		VkImageLayout oldImageLayout,
		VkImageLayout newImageLayout,
		VkImageSubresourceRange subresourceRange,
		VkPipelineStageFlags srcStageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
		VkPipelineStageFlags dstStageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT)
	{
		// Create an image barrier object
		VkImageMemoryBarrier imageMemoryBarrier;
		imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		imageMemoryBarrier.oldLayout = oldImageLayout;
		imageMemoryBarrier.newLayout = newImageLayout;
		imageMemoryBarrier.image = image;
		imageMemoryBarrier.subresourceRange = subresourceRange;
		imageMemoryBarrier.pNext = nullptr;
		// Source layouts (old)
		// Source access mask controls actions that have to be finished on the old layout
		// before it will be transitioned to the new layout
		switch (oldImageLayout)
		{
			case VK_IMAGE_LAYOUT_UNDEFINED:
				// Image layout is undefined (or does not matter)
				// Only valid as initial layout
				// No flags required, listed only for completeness
				imageMemoryBarrier.srcAccessMask = 0;
				break;

			case VK_IMAGE_LAYOUT_PREINITIALIZED:
				// Image is preinitialized
				// Only valid as initial layout for linear images, preserves memory contents
				// Make sure host writes have been finished
				imageMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
				break;

			case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
				// Image is a color attachment
				// Make sure any writes to the color buffer have been finished
				imageMemoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				break;

			case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
				// Image is a depth/stencil attachment
				// Make sure any writes to the depth/stencil buffer have been finished
				imageMemoryBarrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
				break;

			case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
				// Image is a transfer source
				// Make sure any reads from the image have been finished
				imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
				break;

			case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
				// Image is a transfer destination
				// Make sure any writes to the image have been finished
				imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				break;

			case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
				// Image is read by a shader
				// Make sure any shader reads from the image have been finished
				imageMemoryBarrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
				break;
			default:
				// Other source layouts aren't handled (yet)
				break;
		}

		// Target layouts (new)
		// Destination access mask controls the dependency for the new image layout
		switch (newImageLayout)
		{
			case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
				// Image will be used as a transfer destination
				// Make sure any writes to the image have been finished
				imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				break;

			case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
				// Image will be used as a transfer source
				// Make sure any reads from the image have been finished
				imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
				break;

			case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
				// Image will be used as a color attachment
				// Make sure any writes to the color buffer have been finished
				imageMemoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				break;

			case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
				// Image layout will be used as a depth/stencil attachment
				// Make sure any writes to depth/stencil buffer have been finished
				imageMemoryBarrier.dstAccessMask = imageMemoryBarrier.dstAccessMask | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
				break;

			case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
				// Image will be read in a shader (sampler, input attachment)
				// Make sure any writes to the image have been finished
				if (imageMemoryBarrier.srcAccessMask == 0)
				{
					imageMemoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
				}
				imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
				break;
			default:
				// Other source layouts aren't handled (yet)
				break;
		}

		// Put barrier inside setup command buffer
		vkCmdPipelineBarrier(
			cmdbuffer,
			srcStageMask,
			dstStageMask,
			0,
			0, nullptr,
			0, nullptr,
			1, &imageMemoryBarrier);
	}

	// Fixed sub resource on first mip level and layer
	static void setImageLayout(
		VkCommandBuffer cmdbuffer,
		VkImage image,
		VkImageAspectFlags aspectMask,
		VkImageLayout oldImageLayout,
		VkImageLayout newImageLayout,
		VkPipelineStageFlags srcStageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
		VkPipelineStageFlags dstStageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT)
	{
		VkImageSubresourceRange subresourceRange = {};
		subresourceRange.aspectMask = aspectMask;
		subresourceRange.baseMipLevel = 0;
		subresourceRange.levelCount = 1;
		subresourceRange.layerCount = 1;
		setImageLayout(cmdbuffer, image, oldImageLayout, newImageLayout, subresourceRange, srcStageMask, dstStageMask);
	}

	VulkanTexture2D::VulkanTexture2D(const std::string& Path) {
		auto graphicsContext = RendererBase::GetGraphicsContext()->As<VulkanGraphicsContext>();
		int width, height, channels;
		m_Path = Path;
		void* data = nullptr;
		if (Path.empty()) {
			PF_INFO("No path passed into texture");
			return;
		}
		if (stbi_is_hdr(Path.c_str())) {
			data = stbi_loadf(Path.c_str(), &width, &height, &channels, 4);
			m_Format = ImageFormat::RGBA32F;
		}
		else {
			data = stbi_load(Path.c_str(), &width, &height, &channels, 4);
			m_Format = ImageFormat::RGBA;
		}
		if (data == nullptr) {
			PF_ENGINE_ERROR("Texture passed is empty {}", Path.c_str());
			return;
		}
		// check for dimension
		{
			VkImageFormatProperties info;
			vkGetPhysicalDeviceImageFormatProperties(graphicsContext->GetGPU(), Utils::ProofFormatToVulkanFormat(m_Format), VK_IMAGE_TYPE_2D, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, 0, &info);
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
			m_Channel = channels;
		}
		AllocateMemory(m_Width * m_Height * Utils::BytesPerPixel(m_Format));
		SetData(data);
		stbi_image_free(data);
	}
	VulkanTexture2D::VulkanTexture2D(uint32_t width, uint32_t height, ImageFormat format, const void* data)
		: m_Width(width), m_Height(height), m_Format(format) {
		auto graphicsContext = RendererBase::GetGraphicsContext()->As<VulkanGraphicsContext>();

		// check for dimension
		{
			VkImageFormatProperties info;
			vkGetPhysicalDeviceImageFormatProperties(graphicsContext->GetGPU(), Utils::ProofFormatToVulkanFormat(m_Format), VK_IMAGE_TYPE_2D, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, 0, &info);
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
		{
			SetData(data);
		}
	}
	VulkanTexture2D::~VulkanTexture2D() {
		Release();
	}

	void VulkanTexture2D::AllocateMemory(uint64_t size) {
		auto graphicsContext = RendererBase::GetGraphicsContext()->As<VulkanGraphicsContext>();
		auto device = graphicsContext->GetDevice();


		VkFormat vulkanFormat = Utils::ProofFormatToVulkanFormat(m_Format);

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

	Image VulkanTexture2D::GetImage()const
	{
		auto graphicsContext = RendererBase::GetGraphicsContext()->As<VulkanGraphicsContext>();

		if (m_Set == nullptr)
			m_Set = graphicsContext->GetGlobalPool()->AddTexture(m_Sampler, m_ImageView);
		return Image(m_Set,m_Format,{GetWidth(),GetHeight()});
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
		imageBufferInfo.sampler = m_Sampler;
		imageBufferInfo.imageView = m_ImageView;
		imageBufferInfo.imageLayout = imageLayout;
		return imageBufferInfo;
	}
	void VulkanTexture2D::Release() {
		//Renderer::SubmitDatafree([sampler = m_Sampler, imageView = m_ImageView, image = m_Image]()
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
	void float24to32(
		const int w,
		const int h,
		const float* img24,
		float* img32
	) {
		const int numPixels = w * h;

		for (int i = 0; i != numPixels; i++)
		{
			*img32++ = *img24++;
			*img32++ = *img24++;
			*img32++ = *img24++;
			*img32++ = 1.0f;
		}
	}
	glm::vec3 faceCoordsToXYZ(int i, int j, int faceID, int faceSize)
	{
		const float A = 2.0f * float(i) / faceSize;
		const float B = 2.0f * float(j) / faceSize;

		if (faceID == 0) return glm::vec3(-1.0f, A - 1.0f, B - 1.0f);
		if (faceID == 1) return glm::vec3(A - 1.0f, -1.0f, 1.0f - B);
		if (faceID == 2) return glm::vec3(1.0f, A - 1.0f, 1.0f - B);
		if (faceID == 3) return glm::vec3(1.0f - A, 1.0f, 1.0f - B);
		if (faceID == 4) return glm::vec3(B - 1.0f, A - 1.0f, 1.0f);
		if (faceID == 5) return glm::vec3(1.0f - B, A - 1.0f, -1.0f);

		return glm::vec3();
	}
	Bitmap convertEquirectangularMapToVerticalCross(const Bitmap& b)
	{
		if (b.type_ != eBitmapType_2D) return Bitmap();

		const int faceSize = b.w_ / 4;

		const int w = faceSize * 3;
		const int h = faceSize * 4;

		Bitmap result(w, h, b.comp_, b.fmt_);

		const glm::ivec2 kFaceOffsets[] =
		{
		  glm::ivec2(faceSize, faceSize * 3),
			glm::ivec2(0, faceSize),
			glm::ivec2(faceSize, faceSize),
			glm::ivec2(faceSize * 2, faceSize),
			glm::ivec2(faceSize, 0),
			glm::ivec2(faceSize, faceSize * 2)
		};

		const int clampW = b.w_ - 1;
		const int clampH = b.h_ - 1;

		for (int face = 0; face != 6; face++)
		{
			for (int i = 0; i != faceSize; i++)
			{
				for (int j = 0; j != faceSize; j++)
				{
					const glm::vec3 P = faceCoordsToXYZ(i, j, face, faceSize);
					const float R = std::hypot(P.x, P.y);
					const float theta = std::atan2(P.y, P.x);
					const float phi = std::atan2(P.z, R);
					//	float point source coordinates
					const float Uf = float(
						2.0f *
						faceSize *
						(theta + glm::pi<float>()) / glm::pi<float>()
						);
					const float Vf = float(
						2.0f *
						faceSize *
						(glm::pi<float>() / 2.0f - phi) / glm::pi<float>()
						);
							// 4-samples for bilinear interpolation
					const int U1 = std::clamp(int(std::floor(Uf)), 0, clampW);
					const int V1 = std::clamp(int(std::floor(Vf)), 0, clampH);
					const int U2 = std::clamp(U1 + 1, 0, clampW);
					const int V2 = std::clamp(V1 + 1, 0, clampH);
					// fractional part
					const float s = Uf - U1;
					const float t = Vf - V1;
					// fetch 4-samples
					const glm::vec4 A = b.getPixel(U1, V1);
					const glm::vec4 B = b.getPixel(U2, V1);
					const glm::vec4 C = b.getPixel(U1, V2);
					const glm::vec4 D = b.getPixel(U2, V2);
					// bilinear interpolation
					const glm::vec4 color = (
						A * (1 - s) * (1 - t) +
						B * (s) * (1 - t) +
						C * (1 - s) * t +
						D * (s) * (t)
						);
					result.setPixel(
						i + kFaceOffsets[face].x, j + kFaceOffsets[face].y, color
					);
				}
			};
		}

		return result;
	}

	Bitmap convertVerticalCrossToCubeMapFaces(const Bitmap& b)
	{
		const int faceWidth = b.w_ / 3;
		const int faceHeight = b.h_ / 4;

		Bitmap cubemap(faceWidth, faceHeight, 6, b.comp_, b.fmt_);
		cubemap.type_ = eBitmapType_Cube;

		const uint8_t* src = b.data_.data();
		uint8_t* dst = cubemap.data_.data();

		/*
				------
				| +Y |
		 ----------------
		 | -X | -Z | +X |
		 ----------------
				| -Y |
				------
				| +Z |
				------
		*/

		const int pixelSize = (
			cubemap.comp_ * Bitmap::getBytesPerComponent(cubemap.fmt_)
			);

		for (int face = 0; face != 6; ++face)
		{
			for (int j = 0; j != faceHeight; ++j)
			{
				for (int i = 0; i != faceWidth; ++i)
				{
					int x = 0;
					int y = 0;

					switch (face)
					{
						// POSITIVE_X
						case 0:
							x = i;
							y = faceHeight + j;
							break;

							// NEGATIVE_X
						case 1:
							x = 2 * faceWidth + i;
							y = 1 * faceHeight + j;
							break;

							// POSITIVE_Y
						case 2:
							x = 2 * faceWidth - (i + 1);
							y = 1 * faceHeight - (j + 1);
							break;

							// NEGATIVE_Y
						case 3:
							x = 2 * faceWidth - (i + 1);
							y = 3 * faceHeight - (j + 1);
							break;

							// POSITIVE_Z
						case 4:
							x = 2 * faceWidth - (i + 1);
							y = b.h_ - (j + 1);
							break;

							// NEGATIVE_Z
						case 5:
							x = faceWidth + i;
							y = faceHeight + j;
							break;
					}

					memcpy(dst, src + (y * b.w_ + x) * pixelSize, pixelSize);

					dst += pixelSize;
				}
			}
		}

		return cubemap;
	}
	VulkanCubeMap::VulkanCubeMap(const std::filesystem::path& path)
		:
		m_Path(path.string())

	{
		// basic part set up 
		//https://github.com/kidrigger/Blaze/blob/7e76de71e2e22f3b5e8c4c2c50c58e6d205646c6/Blaze/core/TextureCube.cpp

		int width, height, numChannels;
		float* data = stbi_loadf(m_Path.c_str(), &width, &height, &numChannels, 0);

		float* data_rgba = new float[size_t(width) * size_t(height) * 4];
		if (numChannels == 3)
		{
			for (size_t i = 0; i < size_t(width) * size_t(height); i++)
			{
				for (size_t c = 0; c < 3; c++)
				{
					data_rgba[4 * i + c] = data[3 * i + c];
				}
				data_rgba[4 * i + 3] = 1.0f;
			}
		}
		else
		{
			memcpy(data_rgba, data, size_t(width) * size_t(height) * 4 * 4);
		}
		stbi_image_free(data);
		auto graphicsContext = RendererBase::GetGraphicsContext()->As<VulkanGraphicsContext>();

		m_Format = ImageFormat::RGBA32F;
		uint32_t imageSize = width * height * 4 *sizeof(float);
		uint32_t miplevels = 1;
		//miplevels = static_cast<uint32_t>(floor(log2(Math::Max(width, height)))) + 1;

		uint32_t layerCount = 1;
		VulkanBuffer stagingBuffer;
	// Create the Upload Buffer
		{

			VkBufferCreateInfo stagingBufferInfo = {};
			stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			stagingBufferInfo.pNext = nullptr;

			stagingBufferInfo.size = imageSize;
			stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

			//let the VMA library know that this data should be on CPU RAM
			VmaAllocationCreateInfo vmaallocInfo = {};
			vmaallocInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;
			graphicsContext->CreateVmaBuffer(stagingBufferInfo, vmaallocInfo, stagingBuffer);
		}

		{
			void* stagingData;
			vmaMapMemory(graphicsContext->GetVMA_Allocator(), stagingBuffer.Allocation, &stagingData);

			
			memcpy(stagingData, data_rgba, imageSize);

			vmaUnmapMemory(graphicsContext->GetVMA_Allocator(), stagingBuffer.Allocation);
		}
		{
			VkImageCreateInfo imageInfo = {};
			imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
			imageInfo.imageType = VK_IMAGE_TYPE_2D;
			imageInfo.extent.width = width;
			imageInfo.extent.height = height;
			imageInfo.extent.depth = 1;
			imageInfo.mipLevels = miplevels;
			imageInfo.arrayLayers = layerCount;
			imageInfo.format = Utils::ProofFormatToVulkanFormat(m_Format);
			imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
			imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			imageInfo.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
			imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
			imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

			//info.samples;
			Renderer::Submit([&](CommandBuffer* cmd) {
				VmaAllocationCreateInfo vmaallocInfo = {};
					//let the VMA library know that this data should be GPU native
				vmaallocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

				graphicsContext->CreateVmaImage(imageInfo, vmaallocInfo, m_Image);
			});
		}

		Renderer::Submit([&](CommandBuffer* cmd) {
			VkPipelineStageFlags srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			VkPipelineStageFlags dstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;

			VkImageMemoryBarrier barrier = {};
			barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

			barrier.image = m_Image.Image;
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			barrier.subresourceRange.baseMipLevel = 0;
			barrier.subresourceRange.levelCount = miplevels;
			barrier.subresourceRange.baseArrayLayer = 0;
			barrier.subresourceRange.layerCount = layerCount;
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = 0;

			vkCmdPipelineBarrier((VkCommandBuffer)cmd->Get(), srcStage, dstStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
			VkBufferImageCopy region = {};
			region.bufferOffset = 0;
			region.bufferRowLength = 0;
			region.bufferImageHeight = 0;
			region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			region.imageSubresource.mipLevel = 0;
			region.imageSubresource.baseArrayLayer = 0;
			region.imageSubresource.layerCount = layerCount;

			region.imageOffset = { 0, 0 };
			region.imageExtent = { (uint32_t)width, (uint32_t)height, 1 };

			vkCmdCopyBufferToImage((VkCommandBuffer)cmd->Get(), stagingBuffer.Buffer, m_Image.Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1,
				&region);


			barrier.oldLayout = barrier.newLayout;
			barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			srcStage = dstStage;
			dstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;

			vkCmdPipelineBarrier((VkCommandBuffer)cmd->Get(), srcStage, dstStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

			// Mipmapping

			{
				int32_t mipwidth = static_cast<int32_t>(width);
				int32_t mipheight = static_cast<int32_t>(height);
				barrier.subresourceRange.levelCount = 1;

				for (uint32_t i = 1; i < miplevels; i++)
				{
					barrier.subresourceRange.baseMipLevel = i - 1;
					barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
					barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
					barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
					barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

					vkCmdPipelineBarrier((VkCommandBuffer)cmd->Get(), VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0,
						nullptr, 0, nullptr, 1, &barrier);

					VkImageBlit blit = {};
					blit.srcOffsets[0] = { 0, 0, 0 };
					blit.srcOffsets[1] = { mipwidth, mipheight, 1 };
					blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;;
					blit.srcSubresource.mipLevel = i - 1;
					blit.srcSubresource.baseArrayLayer = 0;
					blit.srcSubresource.layerCount = 1;
					blit.dstOffsets[0] = { 0, 0, 0 };
					blit.dstOffsets[1] = { mipwidth > 1 ? mipwidth / 2 : 1, mipheight > 1 ? mipheight / 2 : 1, 1 };
					blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;;
					blit.dstSubresource.mipLevel = i;
					blit.dstSubresource.baseArrayLayer = 0;
					blit.dstSubresource.layerCount = layerCount;

					vkCmdBlitImage((VkCommandBuffer)cmd->Get(), m_Image.Image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, m_Image.Image,
						VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit, VK_FILTER_LINEAR);

					barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
					barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
					barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
					barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

					vkCmdPipelineBarrier((VkCommandBuffer)cmd->Get(), VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
						0, 0, nullptr, 0, nullptr, 1, &barrier);

					mipwidth = Math::Max(mipwidth / 2, 1);
					mipheight = Math::Max(mipheight / 2, 1);
				}

				barrier.subresourceRange.baseMipLevel = miplevels - 1;
				barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
				barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

				vkCmdPipelineBarrier((VkCommandBuffer)cmd->Get(), VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0,
					nullptr, 0, nullptr, 1, &barrier);
			}
		});
		delete[] data_rgba;

		{
			VkImageViewCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.image = m_Image.Image;
			createInfo.viewType = layerCount > 1 ? VK_IMAGE_VIEW_TYPE_2D_ARRAY : VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = Utils::ProofFormatToVulkanFormat(m_Format);
			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = miplevels;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = layerCount;
			vkCreateImageView(graphicsContext->GetDevice(), &createInfo, nullptr, &m_ImageView);
			/*
			std::vector<VkImageView> views(layerCount);
			uint32_t index = 0;
			for (auto& view : views)
			{
				view = createImageView(context->get_device(), get_image(), VK_IMAGE_VIEW_TYPE_2D, format, aspect, miplevels, 1,
					index);
				index++;
			}
			*/
		}

		{
			VkSamplerCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
			createInfo.magFilter = VK_FILTER_LINEAR;
			createInfo.minFilter = VK_FILTER_LINEAR;
			createInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			createInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			createInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			createInfo.anisotropyEnable =  VK_TRUE;
			createInfo.maxAnisotropy = graphicsContext->GetGPUProperties().limits.maxSamplerAnisotropy;
			createInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
			createInfo.unnormalizedCoordinates = VK_FALSE;
			createInfo.compareEnable = VK_FALSE;
			createInfo.compareOp = VK_COMPARE_OP_ALWAYS;
			createInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
			createInfo.mipLodBias = 0.0f;
			createInfo.minLod = 0.0f;
			createInfo.maxLod = static_cast<float>(miplevels);

			vkCreateSampler(graphicsContext->GetDevice(), &createInfo, nullptr, &m_Sampler);
		}
	}
	/*
	VulkanCubeMap::VulkanCubeMap(const std::filesystem::path& path)
		:
		m_Path(path.string())
	{
		uint32_t mipLevels = 1;
		int width, height, bytesPerPixel;

		float* pixelData = stbi_loadf(m_Path.c_str(), &width, &height, &bytesPerPixel, STBI_rgb_alpha);
		m_Format = ImageFormat::RGBA32F;
		auto graphicsContext = RendererBase::GetGraphicsContext()->As<VulkanGraphicsContext>();

		   // Converts RGB -> RGBA
   // (Because Vulkan doesn't accept to use RGB format as sampler)
		std::vector<float> img32(width * height * 4);
		float24to32(width, height, pixelData, img32.data());
		stbi_image_free((void*)pixelData);

		Bitmap in(width, height, 4, eBitmapFormat_Float, img32.data());
		Bitmap out = convertEquirectangularMapToVerticalCross(in);
		 
		Bitmap cubemap = convertVerticalCrossToCubeMapFaces(out);

		uint8_t* cubeMapdata = cubemap.data_.data();

		uint32_t imageSize = (
			cubemap.w_ *
			cubemap.h_ *
			// rgba
			4 *
			Bitmap::getBytesPerComponent(cubemap.fmt_) *
			// faces
			6
			);
		// Create the Image
		{
			VkImageCreateInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
			info.imageType = VK_IMAGE_TYPE_2D;
			info.format = Utils::ProofFormatToVulkanFormat(m_Format);
			info.extent.width = cubemap.w_;
			info.extent.height = cubemap.h_;
			info.extent.depth = 1;

			info.mipLevels = mipLevels;
			// cubemap
			info.arrayLayers = 6;

			info.samples = VK_SAMPLE_COUNT_1_BIT;
			info.tiling = VK_IMAGE_TILING_OPTIMAL;
			info.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
			info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			info.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
			//info.samples;
			Renderer::Submit([&](CommandBuffer* cmd) {
				VmaAllocationCreateInfo vmaallocInfo = {};
					//let the VMA library know that this data should be GPU native
				vmaallocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

				graphicsContext->CreateVmaImage(info, vmaallocInfo, m_Image);
			});
		}
		{
			VkSamplerCreateInfo samplerInfo{};
			samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
			// Specifies how to interpolate texels that are magnified or minified.
			// Manification -> when oversampling.
			// Minification -> when undersampling.
			samplerInfo.magFilter = VK_FILTER_LINEAR;
			samplerInfo.minFilter = VK_FILTER_LINEAR;
			samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
			samplerInfo.anisotropyEnable = VK_TRUE;

			// Limits the amount of texel samples that can be used to calculate the
			// final color.
			// (A lower value results in better performance, but lower quality results)
			// (To find the best match, we will retrieve the Phyisical Device properties)
			VkPhysicalDeviceProperties properties{};
			vkGetPhysicalDeviceProperties(graphicsContext->GetGPU(), &properties);
			samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;

			// Specifies the color that is returned when sampling beyond the image with
			// clmap to border adressing mode.
			samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
			// Vk_TRUE: we can use the coords within [0, texWidth) and [0, texHeight)
			// Vk_FALSE: we can use the coords within [0, 1)
			samplerInfo.unnormalizedCoordinates = VK_FALSE;
			// These two options are used in SHADOW MAPS(percentage-closer filtering).
			samplerInfo.compareEnable = VK_FALSE;
			samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
			// Mipmapping fields:
			samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
			samplerInfo.mipLodBias = 0.0f;
			samplerInfo.minLod = 0.0f;

			if (mipLevels > 1)
				samplerInfo.maxLod = (float)mipLevels;
			else
				samplerInfo.maxLod = 0.0f;
			vkCreateSampler(graphicsContext->GetDevice(), &samplerInfo, nullptr, &m_Sampler);
		}
		{
			VkImageViewCreateInfo viewCreateInfo;
			viewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			viewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
			viewCreateInfo.format = Utils::ProofFormatToVulkanFormat(m_Format);

			viewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			viewCreateInfo.subresourceRange.baseMipLevel = 0;
			viewCreateInfo.subresourceRange.levelCount = mipLevels;
			viewCreateInfo.subresourceRange.baseArrayLayer = 0;

			viewCreateInfo.subresourceRange.layerCount = 6;
			viewCreateInfo.subresourceRange.levelCount = mipLevels;
			viewCreateInfo.image = m_Image.Image;

			viewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			viewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			viewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			viewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

			viewCreateInfo.pNext = nullptr;
			vkCreateImageView(graphicsContext->GetDevice(), &viewCreateInfo, nullptr, &m_ImageView);
		}

		VulkanBuffer stagingBuffer;
		// Create the Upload Buffer
		{

			VkBufferCreateInfo stagingBufferInfo = {};
			stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			stagingBufferInfo.pNext = nullptr;

			stagingBufferInfo.size = imageSize;
			stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

			//let the VMA library know that this data should be on CPU RAM
			VmaAllocationCreateInfo vmaallocInfo = {};
			vmaallocInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;
			graphicsContext->CreateVmaBuffer(stagingBufferInfo, vmaallocInfo, stagingBuffer);
		}

		{
			char* stagingData;
			vmaMapMemory(graphicsContext->GetVMA_Allocator(), stagingBuffer.Allocation, (void**)(&stagingData));

			memcpy(stagingData, cubeMapdata, imageSize);

			vmaUnmapMemory(graphicsContext->GetVMA_Allocator(), stagingBuffer.Allocation);
		}
		Renderer::Submit([&](CommandBuffer* cmd) {
			VkImageMemoryBarrier imgMemoryBarrier;

			imgMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			// We could use VK_IMAGE_LAYOUT_UNDEFINED if we don't care about the
			// existing contents of the image.
			imgMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			imgMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			// Since we are not using the barrier to transfer queue family ownership,
			// we'll ignore these two.
			imgMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			imgMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			imgMemoryBarrier.image = m_Image.Image;
			imgMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imgMemoryBarrier.subresourceRange.baseMipLevel = 0;
			// In this case, our image is not an array(it has 2D coords -> texel).
			imgMemoryBarrier.subresourceRange.baseArrayLayer = 0;

			imgMemoryBarrier.subresourceRange.layerCount = 6;

			imgMemoryBarrier.subresourceRange.levelCount = mipLevels;
			imgMemoryBarrier.srcAccessMask = 0;
			imgMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			//sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			//destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			// 
			//Barriers are primarily used for synchronization purposes, so you must
			//specify which types of operations that involve the resource must happen
			//before the barrier, and which operations that involve the resource must
			//wait on the barrier. We need to do that despite already using
			//vkQueueWaitIdle to manually synchronize. The right values depend on the
			//old and new layout.
			imgMemoryBarrier.srcAccessMask = 0;
			imgMemoryBarrier.dstAccessMask = 0;


			vkCmdPipelineBarrier(
				(VkCommandBuffer)cmd->Get(),
				// Pipeline stage in which operations occur that should happen 
				// before the barrier.
				VK_PIPELINE_STAGE_HOST_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
				// 0 or VK_DEPENDENCY_BY_REGION_BIT(per-region condition)
				0,
				// References arrays of pipeline barries of the three available
				// types: memory barriers, buffer memory barriers, and image memory
				// barriers.
				0,
				nullptr,
				0,
				nullptr,
				1,
				&imgMemoryBarrier
			);
		});

		Renderer::Submit([&](CommandBuffer* cmd) {
		
			VkBufferImageCopy region{};

		// Which parts of the buffer to copy.
			region.bufferOffset = 0;
			region.bufferRowLength = 0;
			region.bufferImageHeight = 0;

			// Which part of the image we want to copy the pixels(from the buffer..).
			region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			region.imageSubresource.mipLevel = 0;
			region.imageSubresource.baseArrayLayer = 0;

				region.imageSubresource.layerCount = 6;

			region.imageOffset = { 0, 0, 0 };
			region.imageExtent = {
			  (uint32_t) cubemap.w_,
			  (uint32_t)cubemap.h_,
			   1
			};

			vkCmdCopyBufferToImage(
				(VkCommandBuffer)cmd->Get(),
				stagingBuffer.Buffer,
				m_Image.Image,
				// Indicates wich layout the image is currently using. Here
				// we are asumming that the image has already been transitioned to
				// the layout that is optimal for copying pixels to.
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				// Count of VkBufferImageCopy(it can be an array of that).
				1,
				&region
			);
		});

		Renderer::Submit([&](CommandBuffer* cmd) {
			VkImageMemoryBarrier imgMemoryBarrier;

			imgMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			// We could use VK_IMAGE_LAYOUT_UNDEFINED if we don't care about the
			// existing contents of the image.
			imgMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			imgMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			// Since we are not using the barrier to transfer queue family ownership,
			// we'll ignore these two.
			imgMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			imgMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			imgMemoryBarrier.image = m_Image.Image;
			imgMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imgMemoryBarrier.subresourceRange.baseMipLevel = 0;
			// In this case, our image is not an array(it has 2D coords -> texel).
			imgMemoryBarrier.subresourceRange.baseArrayLayer = 0;

			imgMemoryBarrier.subresourceRange.layerCount = 6;

			imgMemoryBarrier.subresourceRange.levelCount = mipLevels;
			imgMemoryBarrier.srcAccessMask = 0;
			imgMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			//sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			//destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			// 
			//Barriers are primarily used for synchronization purposes, so you must
			//specify which types of operations that involve the resource must happen
			//before the barrier, and which operations that involve the resource must
			//wait on the barrier. We need to do that despite already using
			//vkQueueWaitIdle to manually synchronize. The right values depend on the
			//old and new layout.
			imgMemoryBarrier.srcAccessMask = 0;
			imgMemoryBarrier.dstAccessMask = 0;


			vkCmdPipelineBarrier(
				(VkCommandBuffer)cmd->Get(),
				// Pipeline stage in which operations occur that should happen 
				// before the barrier.
				VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
				// 0 or VK_DEPENDENCY_BY_REGION_BIT(per-region condition)
				0,
				// References arrays of pipeline barries of the three available
				// types: memory barriers, buffer memory barriers, and image memory
				// barriers.
				0,
				nullptr,
				0,
				nullptr,
				1,
				&imgMemoryBarrier
			);
		});

	}
	*/
	/*
	VulkanCubeMap::VulkanCubeMap(const std::filesystem::path& path)
		:
		m_Path(path.string())
	{
		//https://github.com/beaumanvienna/vulkan/blob/master/engine/platform/Vulkan/VKcubemap.cpp
		//https://www.reddit.com/r/vulkan/comments/uuqe7m/cube_map_with_6_different_images_not_using_ktx/
		//https://github.com/SaschaWillems/Vulkan/blob/master/base/VulkanTexture.cpp
		// //https://github.com/SaferGo/CroissantVulkanRenderer/tree/main/src/CroissantRenderer/Texture
		//https://github.com/KhronosGroup/glTF-IBL-Sampler/blob/master/lib/source/STBImage.cpp
		auto graphicsContext = RendererBase::GetGraphicsContext()->As<VulkanGraphicsContext>();
		m_Format = ImageFormat::RGBA32F;
		int width, height, bytesPerPixel;
		void* pixelData = nullptr;
		if (m_Path.empty())
		{
			PF_INFO("No path passed into texture");
			return;
		}
		if (stbi_is_hdr(m_Path.c_str()))
		{
			pixelData = stbi_loadf(m_Path.c_str(), &width, &height, &bytesPerPixel, STBI_rgb_alpha);
			m_Format = ImageFormat::RGBA32F;
		}
		else
		{
			pixelData = stbi_load(m_Path.c_str(), &width, &height, &bytesPerPixel, 4);
			m_Format = ImageFormat::RGBA;
		}
		if (pixelData == nullptr)
		{
			PF_ENGINE_ERROR("Texture passed is empty {}", m_Path.c_str());
			return;
		}
		// vulkan want sthe widht and heigt to be the same size
		// when it comes to cubemaps
		width,height = std::max(width, height);
		size_t textureByteSize = width * height * Utils::BytesPerPixel(m_Format);
		VulkanBuffer stagingBuffer;
		// Create the Upload Buffer
		{

			VkBufferCreateInfo stagingBufferInfo = {};
			stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			stagingBufferInfo.pNext = nullptr;

			stagingBufferInfo.size = textureByteSize;
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

			memcpy(stagingData, pixelData, textureByteSize);

			vmaUnmapMemory(graphicsContext->GetVMA_Allocator(), stagingBuffer.Allocation);
		}
		uint32_t mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(width, height)))) + 1;
		uint32_t textureDivided = textureByteSize / 6;
		std::vector<VkBufferImageCopy> bufferCopyRegions;
		for (uint32_t face= 0; face < 6; face++)
		{
			for (uint32_t level = 0; level < mipLevels; level++)
			{

				VkBufferImageCopy bufferCopyRegion = {};
				bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				bufferCopyRegion.imageSubresource.mipLevel = level;
				bufferCopyRegion.imageSubresource.baseArrayLayer = face;
				bufferCopyRegion.imageSubresource.layerCount = 1;
				bufferCopyRegion.imageExtent.width = width >> level;
				bufferCopyRegion.imageExtent.height = height >> level;
				//bufferCopyRegion.imageExtent.width = 1;
				//bufferCopyRegion.imageExtent.height = 1;
				bufferCopyRegion.imageExtent.depth = 1;
				//bufferCopyRegion.bufferOffset = textureDivided * face;
				bufferCopyRegion.bufferOffset = 0;
				bufferCopyRegions.push_back(bufferCopyRegion);
			}
		}

		// Create the Image
		{
			VkImageCreateInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
			info.imageType = VK_IMAGE_TYPE_2D;
			info.format = Utils::ProofFormatToVulkanFormat(m_Format);
			info.extent.width = width;
			info.extent.height = height;
			info.extent.depth = 1;

			info.mipLevels = mipLevels;
			// cubemap
			info.arrayLayers = 6;

			info.samples = VK_SAMPLE_COUNT_1_BIT;
			info.tiling = VK_IMAGE_TILING_OPTIMAL;
			info.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
			info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			info.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
			Renderer::Submit([&](CommandBuffer* cmd) {
				VmaAllocationCreateInfo vmaallocInfo = {};
					//let the VMA library know that this data should be GPU native
				vmaallocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

				graphicsContext->CreateVmaImage(info, vmaallocInfo, m_Image);
			});

			Renderer::Submit([&](CommandBuffer* cmd) {
				VkImageSubresourceRange subresourceRange = {};
				subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				subresourceRange.baseMipLevel = 0;
				subresourceRange.levelCount = mipLevels;
				subresourceRange.layerCount = 6;
				setImageLayout(
					(VkCommandBuffer)cmd->Get(),
					m_Image.Image,
					VK_IMAGE_LAYOUT_UNDEFINED,
					VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
					subresourceRange);

				vkCmdCopyBufferToImage(
					(VkCommandBuffer)cmd->Get(),
					stagingBuffer.Buffer,
					m_Image.Image,
					VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
					static_cast<uint32_t>(bufferCopyRegions.size()),
					bufferCopyRegions.data());

				setImageLayout(
					(VkCommandBuffer)cmd->Get(),
					m_Image.Image,
					VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
					VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
					subresourceRange);
			});
		}
		

		VkSamplerCreateInfo samplerCI{};
		samplerCI.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerCI.magFilter = VK_FILTER_LINEAR;
		samplerCI.minFilter = VK_FILTER_LINEAR;
		samplerCI.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerCI.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		samplerCI.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		samplerCI.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		samplerCI.minLod = 0.0f;
		samplerCI.maxLod = static_cast<float>(mipLevels);
		samplerCI.maxAnisotropy = 1.0f;
		samplerCI.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
		vkCreateSampler(graphicsContext->GetDevice(), &samplerCI, nullptr, &m_Sampler);

		VkImageViewCreateInfo viewCreateInfo;
		viewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
		viewCreateInfo.format = Utils::ProofFormatToVulkanFormat(m_Format);
		viewCreateInfo.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
		viewCreateInfo.subresourceRange.layerCount = 6;
		viewCreateInfo.subresourceRange.levelCount = mipLevels;
		viewCreateInfo.image = m_Image.Image;
		viewCreateInfo.components = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
		viewCreateInfo.pNext = nullptr;
		vkCreateImageView(graphicsContext->GetDevice(), &viewCreateInfo, nullptr, &m_ImageView);

		stbi_image_free(pixelData);
		vmaDestroyBuffer(graphicsContext->GetVMA_Allocator(), stagingBuffer.Buffer, stagingBuffer.Allocation);
	}
	*/
	Image VulkanCubeMap::GetImage()const
	{

		auto graphicsContext = RendererBase::GetGraphicsContext()->As<VulkanGraphicsContext>();

		if (m_Set == nullptr)
			m_Set = graphicsContext->GetGlobalPool()->AddTexture(m_Sampler, m_ImageView);
		return Image(m_Set, m_Format, m_Size);
	}
	VkDescriptorImageInfo VulkanCubeMap::GetImageBufferInfo(VkImageLayout imageLayout)
	{
		VkDescriptorImageInfo imageBufferInfo;
		imageBufferInfo.sampler = m_Sampler;
		imageBufferInfo.imageView = m_ImageView;
		imageBufferInfo.imageLayout = imageLayout;
		return imageBufferInfo;
	}
	void VulkanCubeMap::AllocateMemory(uint64_t size)
	{
		auto graphicsContext = RendererBase::GetGraphicsContext()->As<VulkanGraphicsContext>();
		auto device = graphicsContext->GetDevice();
		VkFormat vulkanFormat = Utils::ProofFormatToVulkanFormat(m_Format);
		// Create the Image
		{
			VkImageCreateInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
			info.imageType = VK_IMAGE_TYPE_2D;
			info.format = vulkanFormat;
			info.extent.width = m_Size.X;
			info.extent.height = m_Size.Y;
			info.extent.depth = 1;
			info.mipLevels = 1;
			info.arrayLayers = 1;
			info.samples = VK_SAMPLE_COUNT_1_BIT;
			info.tiling = VK_IMAGE_TILING_OPTIMAL;
			info.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
			info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			info.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
			Renderer::Submit([&](CommandBuffer* cmd) {
				VmaAllocationCreateInfo vmaallocInfo = {};
					//let the VMA library know that this data should be GPU native
				vmaallocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

				graphicsContext->CreateVmaImage(info, vmaallocInfo, m_Image);
			});
		}

			// Create the Image View:
		{
			// View
			//VkImageViewCreateInfo info{};
			//info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			//info.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
			//info.format = vulkanFormat;
			//info.subresourceRange = {};
			//info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			//info.subresourceRange.levelCount = numMips;
			//info.subresourceRange.layerCount = 6;
			//info.image = m_Image.Image;
			//vkCreateImageView(device, &info, nullptr, &m_ImageView);
		}

		// Create sampler:
		{
				// Sampler
			//VkSamplerCreateInfo samplerCI{};
			//samplerCI.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
			//samplerCI.magFilter = VK_FILTER_LINEAR;
			//samplerCI.minFilter = VK_FILTER_LINEAR;
			//samplerCI.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
			//samplerCI.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
			//samplerCI.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
			//samplerCI.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
			//samplerCI.minLod = 0.0f;
			//samplerCI.maxLod = static_cast<float>(numMips);
			//samplerCI.maxAnisotropy = 1.0f;
			//samplerCI.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
			//vkCreateSampler(device, &samplerCI, nullptr, &m_Sampler);
		}
	}
	void VulkanCubeMap::SetData(const void* data)
	{
		auto graphicsContext = RendererBase::GetGraphicsContext()->As<VulkanGraphicsContext>();

		size_t upload_size = m_Size.X * m_Size.Y * Utils::BytesPerPixel(m_Format);
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
		/*
		std::vector<VkBufferImageCopy> bufferCopyRegions;
		uint32_t mipLevels = 5;
		for (uint32_t face = 0; face < 6; face++)
		{
			for (uint32_t level = 0; level < mipLevels; level++)
			{

				VkBufferImageCopy bufferCopyRegion = {};
				bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				bufferCopyRegion.imageSubresource.mipLevel = level;
				bufferCopyRegion.imageSubresource.baseArrayLayer = face;
				bufferCopyRegion.imageSubresource.layerCount = 1;
				bufferCopyRegion.imageExtent.width = ktxTexture->baseWidth >> level;
				bufferCopyRegion.imageExtent.height = ktxTexture->baseHeight >> level;
				bufferCopyRegion.imageExtent.depth = 1;
				bufferCopyRegion.bufferOffset = offset;

				bufferCopyRegions.push_back(bufferCopyRegion);
			}
		}
		// copy to image
		Renderer::Submit([&](CommandBuffer* cmd) {


			VkImageSubresourceRange range;
			range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			range.baseMipLevel = 0;
			range.levelCount = numMips;
			range.baseArrayLayer = 0;
			range.layerCount = 6;
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
			region.imageSubresource.layerCount = 6;
			region.imageExtent.width = dim;
			region.imageExtent.height = dim;
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
			use_barrier.subresourceRange = range;
			vkCmdPipelineBarrier((VkCommandBuffer)cmd->Get(), VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, NULL, 0, NULL, 1, &use_barrier);
		});
		vmaDestroyBuffer(graphicsContext->GetVMA_Allocator(), stagingBuffer.Buffer, stagingBuffer.Allocation);
		*/
	}
	void VulkanCubeMap::Release()
	{
	}
}
