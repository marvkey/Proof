#include "Proofprch.h"
#include "VulkanRenderer.h"
#include "Proof/Renderer/Shader.h"

#include <vulkan/vulkan.h>
#include "VulkanCommandBuffer.h"
#include "VulkanCommandBuffer.h"
#include "VulkanRenderMaterial.h"
#include "VulkanComputePass.h"
#include "VulkanRenderPass.h"
#include "VulkanGraphicsContext.h"
#include "VulkanDevice.h"
#include "VulkanSwapChain.h"
#include "Proof/Core/Application.h"
#include "VulkanTexutre.h"
#include "VulkanComputePass.h"
#include "VulkanImage.h"
#include "VulkanComputePipeline.h"
#include "VulkanUtils/VulkanConvert.h"
namespace Proof
{
	std::map < uint32_t, std::tuple < Count<TextureCube>, Count<Texture2D>,Count<ComputePass>>> keepFromDelete;

	void VulkanRenderer::Init() 
	{
		
	}
	void VulkanRenderer::ShutDown()
	{
	}

	void VulkanRenderer::BeginFrame() 
	{	
	}
	void VulkanRenderer::EndFrame() 
	{
	}
	
	Count<class VulkanGraphicsContext> VulkanRenderer::GetGraphicsContext()
	{
		return VulkanGraphicsContext::Get();
	}

	void VulkanRenderer::SubmitCommandBuffer(Count<RenderCommandBuffer> commandBuffer) {
		commandBuffer.As<VulkanRenderCommandBuffer>()->Submit();
	}

	void VulkanRenderer::BeginCommandBuffer(Count<class RenderCommandBuffer> commandBuffer)
	{
		commandBuffer.As<VulkanRenderCommandBuffer>()->BeginRecord();
	}
	void VulkanRenderer::EndCommandBuffer(Count<class RenderCommandBuffer> commandBuffer)
	{
		commandBuffer.As<VulkanRenderCommandBuffer>()->EndRecord();
	}

	void VulkanRenderer::ClearImage(Count<RenderCommandBuffer> commandBuffer, Count<class Image2D> image)
	{
		Renderer::Submit([commandBuffer, image = image.As<VulkanImage2D>()]
		{
				
			const auto vulkanCommandBuffer = commandBuffer.As<VulkanRenderCommandBuffer>()->GetActiveCommandBuffer();

			const auto oldImageLayout = image->m_DescriptorImageInfo.imageLayout;
		
			
			VkImageAspectFlags aspectMask;
			if (Utils::IsDepthFormat(image->m_Specification.Format))
			{
				aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
				if (Utils::ContainStencilFormat(image->m_Specification.Format))
					aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
			}
			else
			{
				aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

			}

			VkImageSubresourceRange subresourceRange{};
			subresourceRange.aspectMask = aspectMask;
			subresourceRange.baseMipLevel = 0;
			subresourceRange.levelCount = image->GetSpecification().Mips;
			subresourceRange.layerCount = image->GetSpecification().Layers;
			{
				VkImageMemoryBarrier imageMemoryBarrier{};
				imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
				imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
				imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				imageMemoryBarrier.oldLayout = oldImageLayout;
				imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
				imageMemoryBarrier.image = image->GetinfoRef().ImageAlloc.Image;

				imageMemoryBarrier.subresourceRange = subresourceRange;

				vkCmdPipelineBarrier(vulkanCommandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
					0, nullptr,
					0, nullptr,
					1, &imageMemoryBarrier);
			}
			VkClearColorValue clearColor{ 0.f, 0.f, 0.f, 0.f };
			vkCmdClearColorImage(vulkanCommandBuffer, image->GetinfoRef().ImageAlloc.Image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &clearColor, 1, &subresourceRange);
			{
				VkImageMemoryBarrier imageMemoryBarrier{};
				imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
				imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
				imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
				imageMemoryBarrier.newLayout = oldImageLayout;
				imageMemoryBarrier.image = image->GetinfoRef().ImageAlloc.Image;
				imageMemoryBarrier.subresourceRange = subresourceRange;


				vkCmdPipelineBarrier(vulkanCommandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
					0, nullptr,
					0, nullptr,
					1, &imageMemoryBarrier);
			}
		});
	}

	void VulkanRenderer::CopyImage(Count<RenderCommandBuffer> commandBuffer, Count<Image2D> sourceImage, Count<Image2D> destinationImage)
	{
		Renderer::Submit([commandBuffer, src = sourceImage.As<VulkanImage2D>(), dst = destinationImage.As<VulkanImage2D>()]
			{
				const auto vulkanCommandBuffer = commandBuffer.As<VulkanRenderCommandBuffer>()->GetActiveCommandBuffer();

				VkDevice device = VulkanRenderer::GetGraphicsContext()->GetDevice()->GetVulkanDevice();

				VkImageAspectFlagBits aspectMask = Utils::IsDepthFormat( dst->GetSpecification().Format) ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;
				VkImage srcImage = src->GetinfoRef().ImageAlloc.Image;
				VkImage dstImage = dst->GetinfoRef().ImageAlloc.Image;
				glm::uvec2 srcSize = { src->GetSize().X,src->GetSize().Y };
				glm::uvec2 dstSize = { dst->GetSize().X,dst->GetSize().Y };

				VkImageCopy region;
				region.srcOffset = { 0, 0, 0 };
				region.dstOffset = { 0, 0, 0 };
				region.extent = { srcSize.x, srcSize.y, 1 };
				region.srcSubresource.aspectMask = aspectMask;
				region.srcSubresource.baseArrayLayer = 0;
				region.srcSubresource.mipLevel = 0;
				region.srcSubresource.layerCount = 1;
				region.dstSubresource = region.srcSubresource;

				VkImageLayout srcImageLayout = src->GetDescriptorInfoVulkan().imageLayout;
				VkImageLayout dstImageLayout = dst->GetDescriptorInfoVulkan().imageLayout;

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
			});

	}

	void VulkanRenderer::PushSetCubeMapImage(Count<class TextureCube> cube, Count<class Texture2D> texture)
	{
		Count<ComputePipeline> computePipeline;
		Count<ComputePass> computePass;

		ComputePipelineConfig computePipelineConfig;
		computePipelineConfig.DebugName = "EquirectangularToCubemap Pipeline";
		computePipelineConfig.Shader = Renderer::GetShader("EquirectangularToCubemap");
		computePipeline = ComputePipeline::Create(computePipelineConfig);

		ComputePassConfiguration computePassConfig;
		computePassConfig.DebugName = "EquirectangularToCubemap Pass";
		computePassConfig.Pipeline = computePipeline;
		computePass = ComputePass::Create(computePassConfig);


		computePass->SetInput("u_EquirectangularMap", texture);
		computePass->SetInput("u_CubeMap", cube);
		// make sure cube does not delete this due to refercne cout

		Count<RenderCommandBuffer>renderCommandBuffer = Renderer::GetRendererCommandBuffer();
		Renderer::BeginComputePass(renderCommandBuffer, computePass);
		computePass->Dispatch(cube->GetWidth() / 32, cube->GetHeight() / 32, 6);
		Renderer::EndComputePass(computePass);

#if 0
		// doesnt get destroyed
		Renderer::SubmitResourceFree([computePipeline, computePass] 
			{
				auto pipe = computePipeline;
				auto pass = computePass;
			});
#else
		keepFromDelete[keepFromDelete.size()] = { cube,texture,computePass };
#endif


	}
	void VulkanRenderer::DrawArrays(Count<class RenderCommandBuffer> commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
	{
		Renderer::Submit([commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance]()
		{
			vkCmdDraw(commandBuffer.As<VulkanRenderCommandBuffer>()->GetActiveCommandBuffer(), vertexCount, instanceCount, firstVertex, firstInstance);
		});
	}
	void VulkanRenderer::DrawElementIndexed(Count<class RenderCommandBuffer> commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstInstance)
	{
		Renderer::Submit([commandBuffer, indexCount,instanceCount, firstInstance]()
		{
			vkCmdDrawIndexed(commandBuffer.As<VulkanRenderCommandBuffer>()->GetActiveCommandBuffer(), indexCount, instanceCount, 0, 0, firstInstance);
		});

	}
	void VulkanRenderer::DrawElementIndexed(Count<class RenderCommandBuffer> commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance)
	{
		Renderer::Submit([commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance]()
		{
			vkCmdDrawIndexed(commandBuffer.As<VulkanRenderCommandBuffer>()->GetActiveCommandBuffer(), indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
		});
	}

	void VulkanRenderer::BeginRenderPass(Count<class RenderCommandBuffer> commandBuffer, Count<class RenderPass> renderPass, bool explicitClear)
	{
		renderPass.As<VulkanRenderPass>()->BeginRenderPass(commandBuffer, explicitClear);
	}

	void VulkanRenderer::BeginRenderMaterialRenderPass(Count<class RenderCommandBuffer> commandBuffer, Count<class RenderPass> renderPass, bool explicitClear)
	{
		renderPass.As<VulkanRenderPass>()->BeginRenderMaterialRenderPass(commandBuffer, explicitClear);
	}

	void VulkanRenderer::EndRenderPass(Count<class RenderPass> renderPass)
	{
		renderPass.As<VulkanRenderPass>()->EndRenderPass();
	}

	void VulkanRenderer::RenderPassPushRenderMaterial(Count<class RenderPass> renderPass, Count<class RenderMaterial> renderMaterial)
	{
		renderPass.As<VulkanRenderPass>()->RenderPassPushRenderMaterial(renderMaterial);
	}

	void VulkanRenderer::BeginComputePass(Count<RenderCommandBuffer> commandBuffer, Count<ComputePass> computePass)
	{
		computePass.As<VulkanComputePass>()->BeginComputePass(commandBuffer);
	}

	void VulkanRenderer::BeginRenderMaterialComputePass(Count<RenderCommandBuffer> commandBuffer, Count<ComputePass> computePass)
	{
		computePass.As<VulkanComputePass>()->BeginRenderMaterialComputePass(commandBuffer);
	}

	void VulkanRenderer::EndComputePass(Count<ComputePass> computePass)
	{
		computePass.As<VulkanComputePass>()->EndComputePass();
	}

	void VulkanRenderer::ComputePassPushRenderMaterial(Count<class ComputePass> computePass, Count<class RenderMaterial> renderMaterial)
	{
		computePass.As<VulkanComputePass>()->ComputePassPushRenderMaterial(renderMaterial);
	}

}