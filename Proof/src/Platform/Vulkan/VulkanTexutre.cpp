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
#include "VulkanVertexArray.h"
#include "VulkanRenderPass.h"
#include "Bitmap.h"
#include "VulkanFrameBuffer.h"
#include "Proof/Renderer/MeshWorkShop.h"
#include "VulkanRendererAPI.h"
#include "Proof/Renderer/Texture.h"
#include "Proof/Renderer/Renderer2D.h"

/// tempiry for textures because teh 
#include "Imgui/imgui_impl_vulkan.h"
namespace Proof
{

	VulkanTexture2D::VulkanTexture2D(const std::string& Path) :
		m_Path(Path)

	{
		Init(m_Path);
	}
	void VulkanTexture2D::Init(const std::string& Path) {
		auto graphicsContext = RendererBase::GetGraphicsContext().As<VulkanGraphicsContext>();
		int width, height, channels;
		if (Path.empty())
		{
			PF_INFO("No path passed into texture");
			return;
		}
		stbi_set_flip_vertically_on_load(true);
		if (stbi_is_hdr(Path.c_str()))
		{
//https://github.com/marvkey/Proof/blob/main/Proof/src/Platform/Vulkan/VulkanTexutre.cpp (check cubemap sized)
//https://github.com/kidrigger/Blaze/blob/7e76de71e2e22f3b5e8c4c2c50c58e6d205646c6/Blaze/core/TextureCube.cpp
			m_Format = ImageFormat::RGBA32F;

			float* data = stbi_loadf(Path.c_str(), &width, &height, &channels, 0);
			if (data == nullptr)
			{
				PF_ENGINE_ERROR("Texture passed is empty {}", Path.c_str());
				return;
			}
			// check for dimension
			{
				VkImageFormatProperties info;
				vkGetPhysicalDeviceImageFormatProperties(graphicsContext->GetGPU(), Utils::ProofFormatToVulkanFormat(m_Format), VK_IMAGE_TYPE_2D, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, 0, &info);
				if (width > info.maxExtent.width)
				{
					PF_ENGINE_WARN("Image is to wide, made smaller to be support");
					width = info.maxExtent.width;
				}

				if (height > info.maxExtent.height)
				{
					PF_ENGINE_WARN("Image is to tall, made smaller to be support");
					height = info.maxExtent.height;
				}
				m_Width = width;
				m_Height = height;
				m_Channel = channels;
			}
			// prrety sure flaot is already 4 so it is the smae as width * height * 4 *4 im guessing i forgot
			float* data_rgba = new float[size_t(width) * size_t(height) * 4];
			if (channels == 3)
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

			// same s width * height * 16
			// sizeof(float) == 34
			uint32_t imageSize = width * height * 4 * sizeof(float);
			AllocateMemory(imageSize);
			SetData(data_rgba);
			delete[]data_rgba;
			return;
		}

		uint8_t* data = stbi_load(Path.c_str(), &width, &height, &channels, 4);
		m_Format = ImageFormat::RGBA;
		if (data == nullptr)
		{
			PF_EC_ERROR("Texture passed is NotValid {}", Path.c_str());
			return;
		}
		// check for dimension
		{
			VkImageFormatProperties info;
			vkGetPhysicalDeviceImageFormatProperties(graphicsContext->GetGPU(), Utils::ProofFormatToVulkanFormat(m_Format), VK_IMAGE_TYPE_2D, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, 0, &info);
			if (width > info.maxExtent.width)
			{
				PF_ENGINE_WARN("Image is to wide, made smaller to be support");
				width = info.maxExtent.width;
			}

			if (height > info.maxExtent.height)
			{
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
	void VulkanTexture2D::Recreate(const std::string& path) {
		Release();
		Init(path);
	}
	VulkanTexture2D::VulkanTexture2D(uint32_t width, uint32_t height, ImageFormat format, const void* data)
		: m_Width(width), m_Height(height), m_Format(format) {
		auto graphicsContext = RendererBase::GetGraphicsContext().As<VulkanGraphicsContext>();

		// check for dimension
		{
			VkImageFormatProperties info;
			vkGetPhysicalDeviceImageFormatProperties(graphicsContext->GetGPU(), Utils::ProofFormatToVulkanFormat(m_Format), VK_IMAGE_TYPE_2D, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, 0, &info);
			if (width > info.maxExtent.width)
			{
				PF_ENGINE_WARN("Image is to wide, made smaller to be support");
				width = info.maxExtent.width;
			}

			if (height > info.maxExtent.height)
			{
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


	VulkanTexture2D::VulkanTexture2D(TextureConfig config)
	{
		auto graphicsContext = RendererBase::GetGraphicsContext().As<VulkanGraphicsContext>();
		// check for dimension
		{
			VkImageFormatProperties info;
			vkGetPhysicalDeviceImageFormatProperties(graphicsContext->GetGPU(), Utils::ProofFormatToVulkanFormat(config.Format), VK_IMAGE_TYPE_2D, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, 0, &info);
			if (config.width > info.maxExtent.width)
			{
				PF_ENGINE_WARN("Image is to wide, made smaller to be support");
				config.width = info.maxExtent.width;
			}

			if (config.Height > info.maxExtent.height)
			{
				PF_ENGINE_WARN("Image is to tall, made smaller to be support");
				config.Height = info.maxExtent.height;
			}

		}
		m_Width = config.width;
		m_Height = config.Height;
		m_Format = config.Format;

		uint32_t blck = 0xFF000000;
		if (config.Usage & TextureUsage::Color)
		{
			uint32_t bit = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT
				| VK_IMAGE_USAGE_SAMPLED_BIT;
			if (config.Address == AdressType::ClampEdge)
				AllocateMemory(m_Width * m_Height * Utils::BytesPerPixel(m_Format), bit, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
			else
				AllocateMemory(m_Width * m_Height * Utils::BytesPerPixel(m_Format), bit);
			return;
		}
		if (config.Address == AdressType::ClampEdge)
			AllocateMemory(m_Width * m_Height * Utils::BytesPerPixel(m_Format), VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE);
		else
			AllocateMemory(m_Width * m_Height * Utils::BytesPerPixel(m_Format), VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT);

		SetData(&blck);
	}

	VulkanTexture2D::~VulkanTexture2D() {
		Release();
	}

	Count<Texture2D> VulkanTexture2D::GenerateBRDF(uint32_t dimension, uint32_t sampleCount)
	{


		TextureConfig config;
		config.Format = ImageFormat::RGBA16F;
		config.Address = AdressType::ClampEdge;
		config.Height = dimension;
		config.width = dimension;
		config.Usage = TextureUsage::Color;
		Count<Texture2D>brdfTexture = Texture2D::Create(config);

		FrameBufferConfig frameConfig;
		frameConfig.DebugName = "Texture-BRDF";
		frameConfig.Size.X = dimension;
		frameConfig.Size.Y = dimension;
		frameConfig.Attachments = { ImageFormat::RGBA16F };
		frameConfig.Attachments.Attachments[0].SetOverrideImage(brdfTexture->GetImage());

		Count<FrameBuffer> frameBuffer = FrameBuffer::Create(frameConfig);

		Count<PushConstant> pushConstatnt = PushConstant::Create(sizeof(uint32_t), 0, ShaderStage::Fragment);

		RenderPassConfig renderPassConfig("brdf Renderpass", frameBuffer->GetConfig());
		Count<RenderPass> renderPass = RenderPass::Create(renderPassConfig);

		Count<PipeLineLayout> PipelineLayout;
		PipelineLayout = PipeLineLayout::Create({}, pushConstatnt);

		GraphicsPipelineConfig pipelineConfig;
		pipelineConfig.DebugName = "BRDFMap Pipeline";
		pipelineConfig.Shader = Shader::GetOrCreate("BRDFMap", ProofCurrentDirectorySrc + "Proof/Renderer/Asset/Shader/PBR/PBRCubeMap/BRDF.shader");

		auto [quadVertxBuffer, quadindexBuffer] = Renderer2D::CreateQuad();

		pipelineConfig.VertexArray = VertexArray::Create({ sizeof(Vertex) });
		pipelineConfig.VertexArray->AddData(0, DataType::Vec3, offsetof(Vertex, Vertex::Vertices));
		pipelineConfig.VertexArray->AddData(1, DataType::Vec3, offsetof(Vertex, Vertex::Normal));
		pipelineConfig.VertexArray->AddData(2, DataType::Vec2, offsetof(Vertex, Vertex::TexCoords));
		pipelineConfig.VertexArray->AddData(3, DataType::Vec3, offsetof(Vertex, Vertex::Tangent));
		pipelineConfig.VertexArray->AddData(4, DataType::Vec3, offsetof(Vertex, Vertex::Bitangent));
		pipelineConfig.PipelineLayout = PipelineLayout;
		pipelineConfig.RenderPass = renderPass;

		Count<VertexBuffer> buffer;
		Count<GraphicsPipeline> RenderPipline = GraphicsPipeline::Create(pipelineConfig);

		Renderer::Submit([&](CommandBuffer* cmd) {
			Count<RenderCommandBuffer> renderCmd = RenderCommandBuffer::Create(cmd);

			VkViewport viewport = {};
			viewport.x = 0.0f;
			viewport.y = static_cast<float>(dimension);
			viewport.width = static_cast<float>(dimension);
			viewport.height = -static_cast<float>(dimension);
			viewport.minDepth = 0.0f;
			viewport.maxDepth = 1.0f;

			VkRect2D scissor = {};
			scissor.offset = { 0, 0 };
			scissor.extent = { dimension, dimension };

			uint32_t copy = sampleCount;
			pushConstatnt->PushData(renderCmd, PipelineLayout, &copy);

			Renderer::BeginRenderPass(renderCmd, renderPass, frameBuffer);
			renderPass.As<VulkanRenderPass>()->RecordRenderPass(RenderPipline, viewport, scissor, [&](Count <RenderCommandBuffer> commandBuffer) {
				quadVertxBuffer->Bind(renderCmd);
				quadindexBuffer->Bind(renderCmd);
				Renderer::DrawElementIndexed(renderCmd, quadindexBuffer->GetCount());
			});
			Renderer::EndRenderPass(renderPass);
		});
		return brdfTexture;
	}

	void VulkanTexture2D::AllocateMemory(uint64_t size, uint32_t bits, VkSamplerAddressMode mode) {
		auto graphicsContext = RendererBase::GetGraphicsContext().As<VulkanGraphicsContext>();
		auto device = graphicsContext->GetDevice();
		uint32_t layerCount = 1;

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
			info.mipLevels = m_MipLevels;
			info.arrayLayers = 1;
			info.samples = VK_SAMPLE_COUNT_1_BIT;
			info.tiling = VK_IMAGE_TILING_OPTIMAL;
			info.usage = bits;
			info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			VmaAllocationCreateInfo vmaallocInfo = {};
				//let the VMA library know that this data should be GPU native
			vmaallocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

			graphicsContext->CreateVmaImage(info, vmaallocInfo, m_Image);
			Renderer::Submit([&](CommandBuffer* cmd) {

				// tempoarary
				if (bits & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT)
				{
					VkPipelineStageFlags srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
					VkPipelineStageFlags dstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;

					VkImageMemoryBarrier barrier = {};
					barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
					barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
					barrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
					barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
					barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

					barrier.image = m_Image.Image;
					barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
					barrier.subresourceRange.baseMipLevel = 0;
					barrier.subresourceRange.levelCount = m_MipLevels;
					barrier.subresourceRange.baseArrayLayer = 0;
					barrier.subresourceRange.layerCount = layerCount;
					barrier.srcAccessMask = 0;
					barrier.dstAccessMask = 0;

					vkCmdPipelineBarrier(cmd->As<VulkanCommandBuffer>()->GetCommandBuffer(), srcStage, dstStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
				}
			});
		}

			// Create the Image View:
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
			createInfo.subresourceRange.levelCount = m_MipLevels;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = layerCount;
			vkCreateImageView(device, &createInfo, nullptr, &m_ImageView);
		}

		// Create sampler:
		{
			VkSamplerCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
			createInfo.magFilter = VK_FILTER_LINEAR;
			createInfo.minFilter = VK_FILTER_LINEAR;
			createInfo.addressModeU = mode;
			createInfo.addressModeV = mode;
			createInfo.addressModeW = mode;
			createInfo.anisotropyEnable = VK_TRUE;
			createInfo.maxAnisotropy = graphicsContext->GetGPUProperties().limits.maxSamplerAnisotropy;
			//createInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
			createInfo.unnormalizedCoordinates = VK_FALSE;
			createInfo.compareEnable = VK_FALSE;
			createInfo.compareOp = VK_COMPARE_OP_ALWAYS;
			createInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
			createInfo.mipLodBias = 0.0f;
			createInfo.minLod = 0.0f;
			createInfo.maxLod = static_cast<float>(m_MipLevels);
			vkCreateSampler(device, &createInfo, nullptr, &m_Sampler);
		}
		m_Set = (VkDescriptorSet) graphicsContext->GetGlobalPool()->AddTexture(m_Sampler, m_ImageView);
	}

	void* VulkanTexture2D::GetID()const {
		return m_Set;
	}

	Image VulkanTexture2D::GetImage()const
	{
		return VulkanImage(m_Set, m_Format, { (float)GetWidth(),(float)GetHeight() }, VulkanImageExcessData{ m_Sampler,m_ImageView,m_Image.Image });
	}

	void VulkanTexture2D::SetData(const void* data) {
		auto graphicsContext = RendererBase::GetGraphicsContext().As<VulkanGraphicsContext>();


		size_t upload_size = m_Width * m_Height * Utils::BytesPerPixel(m_Format);

		//if (Utils::BytesPerPixel(m_Format) == 16) {
		//	// i dont know why this is not working
		//	upload_size = m_Width * m_Height * 4 * sizeof(float);
		//}
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
			void* stagingData;
			vmaMapMemory(graphicsContext->GetVMA_Allocator(), stagingBuffer.Allocation, &stagingData);

			memcpy(stagingData, data, upload_size);

			vmaUnmapMemory(graphicsContext->GetVMA_Allocator(), stagingBuffer.Allocation);
		}
		// copy to image
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
			barrier.subresourceRange.levelCount = m_MipLevels;
			barrier.subresourceRange.baseArrayLayer = 0;
			barrier.subresourceRange.layerCount = 1;
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = 0;

			vkCmdPipelineBarrier(cmd->As<VulkanCommandBuffer>()->GetCommandBuffer(), srcStage, dstStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
			VkBufferImageCopy region = {};
			region.bufferOffset = 0;
			region.bufferRowLength = 0;
			region.bufferImageHeight = 0;
			region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			region.imageSubresource.mipLevel = 0;
			region.imageSubresource.baseArrayLayer = 0;
			region.imageSubresource.layerCount = 1;

			region.imageOffset = { 0, 0 };
			region.imageExtent = { (uint32_t)m_Width, (uint32_t)m_Height, 1 };

			vkCmdCopyBufferToImage(cmd->As<VulkanCommandBuffer>()->GetCommandBuffer(), stagingBuffer.Buffer, m_Image.Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1,
				&region);


			barrier.oldLayout = barrier.newLayout;
			barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			srcStage = dstStage;
			dstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;

			vkCmdPipelineBarrier(cmd->As<VulkanCommandBuffer>()->GetCommandBuffer(), srcStage, dstStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

			// Mipmapping

			{
				int32_t mipwidth = static_cast<int32_t>(m_Width);
				int32_t mipheight = static_cast<int32_t>(m_Height);
				barrier.subresourceRange.levelCount = 1;

				// if mipmap is 1 then we dont run it
				for (uint32_t i = 1; i < m_MipLevels; i++)
				{
					barrier.subresourceRange.baseMipLevel = i - 1;
					barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
					barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
					barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
					barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

					vkCmdPipelineBarrier(cmd->As<VulkanCommandBuffer>()->GetCommandBuffer(), VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0,
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
					blit.dstSubresource.layerCount = 1;

					vkCmdBlitImage(cmd->As<VulkanCommandBuffer>()->GetCommandBuffer(), m_Image.Image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, m_Image.Image,
						VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit, VK_FILTER_LINEAR);

					barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
					barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
					barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
					barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

					vkCmdPipelineBarrier(cmd->As<VulkanCommandBuffer>()->GetCommandBuffer(), VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
						0, 0, nullptr, 0, nullptr, 1, &barrier);

					mipwidth = Math::Max(mipwidth / 2, 1);
					mipheight = Math::Max(mipheight / 2, 1);
				}

				barrier.subresourceRange.baseMipLevel = m_MipLevels - 1;
				barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
				barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

				vkCmdPipelineBarrier(cmd->As<VulkanCommandBuffer>()->GetCommandBuffer(), VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0,
					nullptr, 0, nullptr, 1, &barrier);
			}
		});
		vmaDestroyBuffer(graphicsContext->GetVMA_Allocator(), stagingBuffer.Buffer, stagingBuffer.Allocation);
	}
	VkDescriptorImageInfo& VulkanTexture2D::GetImageBufferInfo() {
		m_ImageDescriptorInfo.sampler = m_Sampler;
		m_ImageDescriptorInfo.imageView = m_ImageView;
		m_ImageDescriptorInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		return m_ImageDescriptorInfo;
	}
	void VulkanTexture2D::Release() {
		Renderer::SubmitDatafree([sampler = m_Sampler, imageView = m_ImageView, image = m_Image]()
		{
			VkDevice device = Renderer::GetGraphicsContext().As<VulkanGraphicsContext>()->GetDevice();
			auto graphicsContext = RendererBase::GetGraphicsContext().As<VulkanGraphicsContext>();

			vkDestroySampler(device, sampler, nullptr);
			vkDestroyImageView(device, imageView, nullptr);
			vmaDestroyImage(graphicsContext->GetVMA_Allocator(), image.Image, image.Allocation);
		});
		m_Sampler = nullptr;
		m_ImageView = nullptr;
		m_Image.Image = nullptr;
		m_Set = nullptr;
		m_Image.Allocation = nullptr;
	}

	VulkanCubeMap::VulkanCubeMap(const std::filesystem::path& path, uint32_t dimension, bool generateMips)
		:
		m_Dimension(dimension),
		m_Path(path.string())
	{
		if (generateMips)
			m_MipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(dimension, dimension)))) + 1;

		AllocateMemory();
		Count<Texture2D> text = Texture2D::Create(path.string());
		SetData(text);
	}

	VulkanCubeMap::VulkanCubeMap(Count<Texture2D> texture, uint32_t dimension, bool generateMips)
		:
		m_Dimension(dimension)

	{
		if (generateMips)
			m_MipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(dimension, dimension)))) + 1;

		m_Format = texture.As<VulkanTexture2D>()->GetImage().Format;
		AllocateMemory();
		if (texture)
		{
			GenerateCubeMap(texture);
		}
	}

	VulkanCubeMap::VulkanCubeMap(Count<CubeMap> map, Count<Shader> shader, uint32_t dimension, bool generateMips)
		:
		m_Dimension(dimension)

	{
		if (generateMips)
			m_MipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(dimension, dimension)))) + 1;
		AllocateMemory();
		if (map)
			GenerateCubeMap(map.As<Texture>(), shader);


	}

	Count<CubeMap> VulkanCubeMap::GeneratePreFilterMap(Count<CubeMap> map, uint32_t dimension, uint32_t numSamples)
	{
		// https://github.com/kidrigger/Blaze/blob/7e76de71e2e22f3b5e8c4c2c50c58e6d205646c6/Blaze/util/Environment.cpp
		 // https://github.com/SaschaWillems/Vulkan-glTF-PBR/blob/master/src/main.cpp //(generate cuebmap)
		struct PCB
		{
			float Roughness = 0;
			uint32_t NumSamples = 128;
		};

		Count<PushConstant> PcbConstnat = PushConstant::Create(sizeof(PCB), 0, ShaderStage::Fragment);
		Count<CubeMap> preFilterMap = CubeMap::Create(nullptr, nullptr, dimension, true);


		auto format = ImageFormat::RGBA32F;
		TextureConfig conigTexture;
		conigTexture.Format = format;
		conigTexture.Usage = TextureUsage::Color;
		conigTexture.width = dimension;
		conigTexture.Height = dimension;
		Count<Texture2D> fbColorAttachment = Texture2D::Create(conigTexture);

		FrameBufferConfig frameConfig;
		frameConfig.DebugName = "Texture-Cube";
		frameConfig.Size.X = dimension;
		frameConfig.Size.Y = dimension;
		frameConfig.Attachments = { format };
		frameConfig.Attachments.Attachments[0].SetOverrideImage(fbColorAttachment->GetImage());

		Count<FrameBuffer> frameBuffer = FrameBuffer::Create(frameConfig);

		RenderPassConfig renderPassConfig("texture-cube-RenderPass", frameBuffer->GetConfig());
		renderPassConfig.MultiView = true;
		Count<RenderPass> renderPass = RenderPass::Create(renderPassConfig);

		std::unordered_map<DescriptorSets, Count<DescriptorSet>> Descriptors;

		auto descriptor = DescriptorSet::Builder(DescriptorSets::Zero)
			.AddBinding(0, DescriptorType::UniformBuffer, ShaderStage::Vertex)
			.AddBinding(1, DescriptorType::ImageSampler, ShaderStage::Fragment)
			.Build();
		Descriptors.insert({ DescriptorSets::Zero,descriptor });

		Count<PipeLineLayout> PipelineLayout;
		PipelineLayout = PipeLineLayout::Create(std::vector{ Descriptors[DescriptorSets::Zero] }, PcbConstnat);

		GraphicsPipelineConfig pipelineConfig;
		pipelineConfig.DebugName = "generate PRefiltered map";
		pipelineConfig.Shader = Shader::GetOrCreate("prefilterCubeMap",
			{ {ShaderStage::Vertex,ProofCurrentDirectorySrc +
			"Proof/Renderer/Asset/Shader/PBR/PBRCubeMap/prefilter.vs"},
			{ShaderStage::Fragment,ProofCurrentDirectorySrc +
			"Proof/Renderer/Asset/Shader/PBR/PBRCubeMap/prefilter.fs"} });

		pipelineConfig.VertexArray = VertexArray::Create({ sizeof(Vertex) });
		pipelineConfig.VertexArray->AddData(0, DataType::Vec3, offsetof(Vertex, Vertex::Vertices));
		pipelineConfig.VertexArray->AddData(1, DataType::Vec3, offsetof(Vertex, Vertex::Normal));
		pipelineConfig.VertexArray->AddData(2, DataType::Vec2, offsetof(Vertex, Vertex::TexCoords));
		pipelineConfig.VertexArray->AddData(3, DataType::Vec3, offsetof(Vertex, Vertex::Tangent));
		pipelineConfig.VertexArray->AddData(4, DataType::Vec3, offsetof(Vertex, Vertex::Bitangent));
		pipelineConfig.PipelineLayout = PipelineLayout;
		pipelineConfig.RenderPass = renderPass;
		Count<GraphicsPipeline> RenderPipline = GraphicsPipeline::Create(pipelineConfig);
		glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
		glm::mat4 captureViews[6] =
		{
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
		};
		struct UboData {
			/// Projection matrix common to each face of the cubemap.
			glm::mat4 projection;

			/// View matrix to look at the direction of each cubemap face.
			glm::mat4 view;
		};
		UboData uboData = {
			glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f),
			captureViews[0]
		};
		Count<UniformBuffer> ubuffer = UniformBuffer::Create(&uboData, sizeof(uboData), DescriptorSets::Zero, 0);
		auto cube = MeshWorkShop::GenerateCube();

		uint32_t totalMips = static_cast<uint32_t>(std::floor(std::log2(std::max(dimension, dimension)))) + 1;

				// cubemap is incolor attahcment oopitmal format we just need to change it first to VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
		// so we cna render this my solution no toutorial had this 
		// we need to have a bettter system of rendering
		Renderer::Submit([&](CommandBuffer* cmd) {

			VkPipelineStageFlags srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			VkPipelineStageFlags dstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			VkImageMemoryBarrier barrier = {};
			barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			barrier.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

			barrier.image = preFilterMap.As<VulkanCubeMap>()->GetImageAlloc().Image;
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			barrier.subresourceRange.baseMipLevel = 0;
			barrier.subresourceRange.levelCount = totalMips;
			barrier.subresourceRange.baseArrayLayer = 0;
			barrier.subresourceRange.layerCount = 6;
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = 0;

			vkCmdPipelineBarrier(cmd->As<VulkanCommandBuffer>()->GetCommandBuffer(), srcStage, dstStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
		});
		PCB pcb;
		uint32_t mipsize = dimension;
		Renderer::Submit([&](CommandBuffer* cmd) {
			for (uint32_t miplevel = 0; miplevel < totalMips; miplevel++)
			{
				VkViewport viewport = {};
				viewport.x = 0.0f;
				viewport.y = static_cast<float>(mipsize);
				viewport.width = static_cast<float>(mipsize);
				viewport.height = -static_cast<float>(mipsize);
				viewport.minDepth = 0.0f;
				viewport.maxDepth = 1.0f;

				VkRect2D scissor = {};
				scissor.offset = { 0, 0 };
				scissor.extent = { mipsize, mipsize };

				for (uint32_t face = 0; face < 6; face++)
				{
					{
						fbColorAttachment = Texture2D::Create(conigTexture);

						frameConfig.DebugName = "Texture-Cube";
						frameConfig.Size.X = dimension;
						frameConfig.Size.Y = dimension;
						frameConfig.Attachments = { format };
						frameConfig.Attachments.Attachments[0].SetOverrideImage(fbColorAttachment->GetImage());

						frameBuffer = FrameBuffer::Create(frameConfig);

						renderPass = RenderPass::Create(renderPassConfig);
					}
					uboData.view = captureViews[face];
					ubuffer->SetData(&uboData, sizeof(uboData));
					pcb.Roughness = (float)miplevel / (float)(totalMips - 1);
					pcb.NumSamples = numSamples;
					Count<RenderCommandBuffer> renderCmd = RenderCommandBuffer::Create(cmd);
					PcbConstnat->PushData(renderCmd, PipelineLayout, &pcb);

					Renderer::BeginRenderPass(renderCmd, renderPass, frameBuffer);
					renderPass.As<VulkanRenderPass>()->RecordRenderPass(RenderPipline, viewport, scissor, [&](Count <RenderCommandBuffer> commandBuffer) {

						auto descriptor0 = Descriptors[DescriptorSets::Zero];
						descriptor0->WriteBuffer(0, ubuffer);
						descriptor0->WriteImage(1, map);
						descriptor0->Bind(renderCmd, PipelineLayout);
						for (const auto& subMesh : cube->GetMeshSource()->GetSubMeshes())
						{
							subMesh.VertexBuffer->Bind(renderCmd);
							subMesh.IndexBuffer->Bind(renderCmd);
							Renderer::DrawElementIndexed(renderCmd, subMesh.IndexBuffer->GetCount());
						}

					});
					Renderer::EndRenderPass(renderPass);

					{
						VkImageMemoryBarrier barrier = {};
						barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
						barrier.oldLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
						//barrier.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
						barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
						barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
						barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
						 //VkImage image = frameBuffer.As<VulkanFrameBuffer>()->GetColorAttachmentFrameBufferImage(0).Images[Renderer::GetCurrentFrame().ImageIndex].Image;

						barrier.image = fbColorAttachment.As<VulkanTexture2D>()->GetImageAlloc().Image;
						barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
						barrier.subresourceRange.baseMipLevel = 0;
						barrier.subresourceRange.levelCount = 1;
						barrier.subresourceRange.baseArrayLayer = 0;
						barrier.subresourceRange.layerCount = 1;
						barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
						barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

						vkCmdPipelineBarrier(cmd->As<VulkanCommandBuffer>()->GetCommandBuffer(), VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
							VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);
					}
					{
						VkImageCopy copyRegion = {};

						copyRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
						copyRegion.srcSubresource.baseArrayLayer = 0;
						copyRegion.srcSubresource.mipLevel = 0;
						copyRegion.srcSubresource.layerCount = 1;
						copyRegion.srcOffset = { 0, 0, 0 };

						copyRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
						copyRegion.dstSubresource.mipLevel = miplevel;
						copyRegion.dstSubresource.baseArrayLayer = face;
						copyRegion.dstSubresource.layerCount = 1;
						copyRegion.dstOffset = { 0, 0, 0 };

						copyRegion.extent.width = static_cast<uint32_t>(mipsize);
						copyRegion.extent.height = static_cast<uint32_t>(mipsize);

						copyRegion.extent.depth = 1;

						//VkImage image = frameBuffer.As<VulkanFrameBuffer>()->GetColorAttachmentFrameBufferImage(0).Images[Renderer::GetCurrentFrame().ImageIndex].Image;
						VkImage image = fbColorAttachment.As<VulkanTexture2D>()->GetImageAlloc().Image;
						vkCmdCopyImage(cmd->As<VulkanCommandBuffer>()->GetCommandBuffer(), image,
							VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
							preFilterMap.As<VulkanCubeMap>()->GetImageAlloc().Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyRegion);
					}
				}
				mipsize = mipsize / 2;
			}
		});
		Renderer::Submit([&](CommandBuffer* cmd) {

			VkImageMemoryBarrier barrier = {};
			barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

			barrier.image = preFilterMap.As<VulkanCubeMap>()->GetImageAlloc().Image;
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			barrier.subresourceRange.baseMipLevel = 0;
			barrier.subresourceRange.levelCount = totalMips;
			barrier.subresourceRange.baseArrayLayer = 0;
			barrier.subresourceRange.layerCount = 6;
			barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			vkCmdPipelineBarrier(cmd->As<VulkanCommandBuffer>()->GetCommandBuffer(),
				VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);
		});
		return preFilterMap;
	}

	VulkanCubeMap::~VulkanCubeMap()
	{
		Release();
	}

	Image VulkanCubeMap::GetImage()const
	{
		return VulkanImage(m_Set, m_Format, { (float)m_Dimension,(float)m_Dimension }, VulkanImageExcessData{ m_Sampler,m_ImageView,m_Image.Image });
	}
	VkDescriptorImageInfo& VulkanCubeMap::GetImageBufferInfo()
	{
		m_ImageDescriptorInfo.sampler = m_Sampler;
		m_ImageDescriptorInfo.imageView = m_ImageView;
		m_ImageDescriptorInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		return m_ImageDescriptorInfo;
	}


	void VulkanCubeMap::AllocateMemory()
	{
		//https://github.com/kidrigger/Blaze/blob/7e76de71e2e22f3b5e8c4c2c50c58e6d205646c6/Blaze/core/TextureCube.cpp
		// (Goto) same file to view creating a framebuffer with data
		// it would generate this texture for us then we convert to cubemap
		const uint32_t size = Utils::BytesPerPixel(m_Format) * 6 * m_Dimension * m_Dimension;
		const uint32_t layerSize = Utils::BytesPerPixel(m_Format) * m_Dimension * m_Dimension;

		auto graphicsContext = RendererBase::GetGraphicsContext().As<VulkanGraphicsContext>();

		VkFormat format = Utils::ProofFormatToVulkanFormat(m_Format);
		VkImageCreateInfo imageInfo = {};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = m_Dimension;
		imageInfo.extent.height = m_Dimension;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = m_MipLevels;
		imageInfo.arrayLayers = 6;
		imageInfo.format = format;
		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		Renderer::Submit([&](CommandBuffer* cmd) {
			VmaAllocationCreateInfo vmaallocInfo = {};
				//let the VMA library know that this data should be GPU native
			vmaallocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

			graphicsContext->CreateVmaImage(imageInfo, vmaallocInfo, m_Image);
		});

		Renderer::Submit([&](CommandBuffer* cmd) {

			VkPipelineStageFlags srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			VkPipelineStageFlags dstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			VkImageMemoryBarrier barrier = {};
			barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			barrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

			barrier.image = m_Image.Image;
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			barrier.subresourceRange.baseMipLevel = 0;
			barrier.subresourceRange.levelCount = m_MipLevels;
			barrier.subresourceRange.baseArrayLayer = 0;
			barrier.subresourceRange.layerCount = 6;
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = 0;

			vkCmdPipelineBarrier(cmd->As<VulkanCommandBuffer>()->GetCommandBuffer(), srcStage, dstStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
		});
		{
			VkImageViewCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.image = m_Image.Image;
			createInfo.viewType = VK_IMAGE_VIEW_TYPE_CUBE;
			createInfo.format = format;
			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = m_MipLevels;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 6;
			vkCreateImageView(graphicsContext->GetDevice(), &createInfo, nullptr, &m_ImageView);
		}

		{
			VkSamplerCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
			createInfo.magFilter = VK_FILTER_LINEAR;
			createInfo.minFilter = VK_FILTER_LINEAR;
			createInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
			createInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
			createInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
			createInfo.anisotropyEnable = VK_TRUE;
			createInfo.maxAnisotropy = graphicsContext->GetGPUProperties().limits.maxSamplerAnisotropy;
			createInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
			createInfo.unnormalizedCoordinates = VK_FALSE;
			createInfo.compareEnable = VK_FALSE;
			createInfo.compareOp = VK_COMPARE_OP_ALWAYS;
			createInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
			createInfo.mipLodBias = 0.0f;
			createInfo.minLod = 0.0f;
			createInfo.maxLod = static_cast<float>(m_MipLevels);
			vkCreateSampler(graphicsContext->GetDevice(), &createInfo, nullptr, &m_Sampler);
		}

		m_Set = graphicsContext->GetGlobalPool()->AddTexture(m_Sampler, m_ImageView);

	}
	void VulkanCubeMap::SetData(const void* data)
	{
		Count<Texture2D> texture = Texture2D::Create(m_Dimension, m_Dimension, m_Format, data);
		GenerateCubeMap(texture);
	}
	void VulkanCubeMap::SetData(const void* data[6])
	{
		//https://github.com/kidrigger/Blaze/blob/7e76de71e2e22f3b5e8c4c2c50c58e6d205646c6/Blaze/core/TextureCube.cpp

		PF_CORE_ASSERT(false, "Has not been implmented");
	}
	void VulkanCubeMap::SetData(Count<Texture2D> texture)
	{
		GenerateCubeMap(texture);
	}
	void VulkanCubeMap::SetData(Count<Texture2D> textures[6])
	{
		PF_CORE_ASSERT(false, "Has not been implmented");
		//https://github.com/kidrigger/Blaze/blob/7e76de71e2e22f3b5e8c4c2c50c58e6d205646c6/Blaze/core/TextureCube.cpp
		// go to file to see implementation
	}
	void VulkanCubeMap::GenerateCubeMap(Count<Texture> texture, Count< Shader> shader)
	{
		auto graphicsContext = RendererBase::GetGraphicsContext().As<VulkanGraphicsContext>();

		FrameBufferConfig frameConfig;
		frameConfig.DebugName = "Texture-Cube";
		frameConfig.Size.X = m_Dimension;
		frameConfig.Size.Y = m_Dimension;
		frameConfig.Attachments = { m_Format };
		frameConfig.Attachments.Attachments[0].SetOverrideImage(GetImage());

		Count<FrameBuffer> frameBuffer = FrameBuffer::Create(frameConfig);

		RenderPassConfig renderPassConfig("texture-cube-RenderPass", frameBuffer->GetConfig());
		renderPassConfig.MultiView = true;
		Count<RenderPass> renderPass = RenderPass::Create(renderPassConfig);

		std::unordered_map<DescriptorSets, Count<DescriptorSet>> Descriptors;

		auto descriptor = DescriptorSet::Builder(DescriptorSets::Zero)
			.AddBinding(0, DescriptorType::UniformBuffer, ShaderStage::Vertex)
			.AddBinding(1, DescriptorType::ImageSampler, ShaderStage::Fragment)
			.Build();
		Descriptors.insert({ DescriptorSets::Zero,descriptor });

		Count<PipeLineLayout> PipelineLayout;
		PipelineLayout = PipeLineLayout::Create(std::vector{ Descriptors[DescriptorSets::Zero] });

		GraphicsPipelineConfig pipelineConfig;
		pipelineConfig.DebugName = "generate cubemap create";
		pipelineConfig.Shader = shader;

		pipelineConfig.VertexArray = VertexArray::Create({ sizeof(Vertex) });
		pipelineConfig.VertexArray->AddData(0, DataType::Vec3, offsetof(Vertex, Vertex::Vertices));
		pipelineConfig.VertexArray->AddData(1, DataType::Vec3, offsetof(Vertex, Vertex::Normal));
		pipelineConfig.VertexArray->AddData(2, DataType::Vec2, offsetof(Vertex, Vertex::TexCoords));
		pipelineConfig.VertexArray->AddData(3, DataType::Vec3, offsetof(Vertex, Vertex::Tangent));
		pipelineConfig.VertexArray->AddData(4, DataType::Vec3, offsetof(Vertex, Vertex::Bitangent));
		pipelineConfig.PipelineLayout = PipelineLayout;
		pipelineConfig.RenderPass = renderPass;
		Count<GraphicsPipeline> RenderPipline = GraphicsPipeline::Create(pipelineConfig);

		struct UboData {
			/// Projection matrix common to each face of the cubemap.
			alignas(16) glm::mat4 projection;

			/// View matrix to look at the direction of each cubemap face.
			alignas(16) glm::mat4 view[6];
		};
		UboData uboData = {
			glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f),
			{
				glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
				glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
				glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
				glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
				glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
				glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
			}
		};
		Count<UniformBuffer> ubuffer = UniformBuffer::Create(&uboData, sizeof(uboData), DescriptorSets::Zero, 0);
		auto cube = MeshWorkShop::GenerateCube();

		Renderer::Submit([&](CommandBuffer* buffer) {
			Count<RenderCommandBuffer> renderCmd = RenderCommandBuffer::Create(buffer);

			Renderer::BeginRenderPass(renderCmd, renderPass, frameBuffer);
			Renderer::RecordRenderPass(renderPass, RenderPipline, [&](Count <RenderCommandBuffer> commandBuffer) {

				auto descriptor0 = Descriptors[DescriptorSets::Zero];
				descriptor0->WriteBuffer(0, ubuffer);
				if (texture.As<Texture2D>())
					descriptor0->WriteImage(1, texture.As<Texture2D>());
				else
					descriptor0->WriteImage(1, texture.As<CubeMap>());
				descriptor0->Bind(renderCmd, PipelineLayout);
				for (const auto& subMesh : cube->GetMeshSource()->GetSubMeshes())
				{
					subMesh.VertexBuffer->Bind(renderCmd);
					subMesh.IndexBuffer->Bind(renderCmd);
					Renderer::DrawElementIndexed(renderCmd, subMesh.IndexBuffer->GetCount());
				}

			});
			Renderer::EndRenderPass(renderPass);
		});
		Renderer::Submit([&](CommandBuffer* buffer) {
			if (m_MipLevels == 1)
			{

				VkImageMemoryBarrier barrier = {};
				barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
				barrier.oldLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				barrier.image = m_Image.Image;
				barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				barrier.subresourceRange.baseMipLevel = 0;
				barrier.subresourceRange.levelCount = m_MipLevels;
				barrier.subresourceRange.baseArrayLayer = 0;
				barrier.subresourceRange.layerCount = 6;
				barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
				vkCmdPipelineBarrier(buffer->As<VulkanCommandBuffer>()->GetCommandBuffer(), VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);
			}
			if (m_MipLevels == 1)
				return;
			// Mipmapping

			for (uint32_t face = 0; face < 6; face++)
			{
				VkImageMemoryBarrier barrier{};
				barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
				barrier.image = m_Image.Image;
				barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
				barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				barrier.subresourceRange.baseArrayLayer = face;
				barrier.subresourceRange.layerCount = 1;
				barrier.subresourceRange.levelCount = 1;

				int32_t mipWidth = static_cast<int32_t>(m_Dimension);
				int32_t mipHeight = static_cast<int32_t>(m_Dimension);

				// if mipmap is 1 then we dont run it
				for (uint32_t i = 1; i < m_MipLevels; i++)
				{
					barrier.subresourceRange.baseMipLevel = i - 1;
					barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
					barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
					barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
					barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

					vkCmdPipelineBarrier(buffer->As<VulkanCommandBuffer>()->GetCommandBuffer(),
						VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
						0, nullptr,
						0, nullptr,
						1, &barrier);

					VkImageBlit blit{};
					blit.srcOffsets[0] = { 0, 0, 0 };
					blit.srcOffsets[1] = { mipWidth, mipHeight, 1 };
					blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
					blit.srcSubresource.mipLevel = i - 1;
					blit.srcSubresource.baseArrayLayer = face;
					blit.srcSubresource.layerCount = 1;
					blit.dstOffsets[0] = { 0, 0, 0 };
					blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
					blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
					blit.dstSubresource.mipLevel = i;
					blit.dstSubresource.baseArrayLayer = face;
					blit.dstSubresource.layerCount = 1;

					vkCmdBlitImage(buffer->As<VulkanCommandBuffer>()->GetCommandBuffer(),
						m_Image.Image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
						m_Image.Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
						1, &blit,
						VK_FILTER_LINEAR);

					barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
					barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
					barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
					barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

					vkCmdPipelineBarrier(buffer->As<VulkanCommandBuffer>()->GetCommandBuffer(),
						VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
						0, nullptr,
						0, nullptr,
						1, &barrier);

					if (mipWidth > 1) mipWidth /= 2;
					if (mipHeight > 1) mipHeight /= 2;
				}

				barrier.subresourceRange.baseMipLevel = m_MipLevels - 1;
				barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
				barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

				vkCmdPipelineBarrier(buffer->As<VulkanCommandBuffer>()->GetCommandBuffer(),
					VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
					0, nullptr,
					0, nullptr,
					1, &barrier);
			}

		});
	}
	void VulkanCubeMap::Release()
	{
		Renderer::SubmitDatafree([sampler = m_Sampler, imageView = m_ImageView, image = m_Image]()
		{
			VkDevice device = Renderer::GetGraphicsContext().As<VulkanGraphicsContext>()->GetDevice();
			auto graphicsContext = RendererBase::GetGraphicsContext().As<VulkanGraphicsContext>();

			vkDestroySampler(device, sampler, nullptr);
			vkDestroyImageView(device, imageView, nullptr);
			vmaDestroyImage(graphicsContext->GetVMA_Allocator(), image.Image, image.Allocation);
		});
		m_Sampler = nullptr;
		m_ImageView = nullptr;
		m_Image.Image = nullptr;
		m_Image.Allocation = nullptr;
	}
}