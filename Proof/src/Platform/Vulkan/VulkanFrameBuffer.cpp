#include "Proofprch.h"
#include "VulkanFrameBuffer.h"
#include "Proof/Renderer/Renderer.h"
#include "VulkanGraphicsContext.h"
#include "VulkanSwapChain.h"
#include "VulkanRenderPass.h"
#include "VulkanRenderer/VulkanRenderer.h"
#include "VulkanSwapChain.h"
#include <algorithm>
#include <vector>
namespace Proof
{
    uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
        auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();

        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(graphicsContext->GetGPU(), &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }

        throw std::runtime_error("failed to find suitable memory type!");
    }
    void VulkanScreenFrameBuffer::CreateImageViews() {
        auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();
        auto swapchain = graphicsContext->GetSwapChain();
        uint32_t imageCount = swapchain->GetImageCount();
        auto imageFormat = swapchain->As<VulkanSwapChain>()->GetImageFormat();
        m_Images.resize(imageCount);
        m_ImageViews.resize(imageCount);
        m_ImageSampler.resize(imageCount);

        for (int i = 0; i < m_Images.size(); i++) {
            VkImageCreateInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
            info.imageType = VK_IMAGE_TYPE_2D;
            info.format = imageFormat;
            info.extent.width = m_ImageSize.X;
            info.extent.height = m_ImageSize.Y;
            info.extent.depth = 1;
            info.mipLevels = 1;
            info.arrayLayers = 1;
            info.samples = VK_SAMPLE_COUNT_1_BIT;
            info.tiling = VK_IMAGE_TILING_OPTIMAL;
            info.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
            info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            Renderer::Submit([&](CommandBuffer* cmdBuffer) {
                VmaAllocationCreateInfo vmaallocInfo = {};
                vmaallocInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;

                graphicsContext->CreateVmaImage(info, vmaallocInfo, m_Images[i]);
            });

            Renderer::Submit([&](CommandBuffer* cmdBuffer) {
                VkImageMemoryBarrier imageMemoryBarrier{};
                imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
                imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
                imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
                imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
                imageMemoryBarrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
                imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
                imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                imageMemoryBarrier.image = m_Images[i].Image;
                imageMemoryBarrier.subresourceRange = VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };

                vkCmdPipelineBarrier(
                    (VkCommandBuffer)cmdBuffer->Get(),
                    VK_PIPELINE_STAGE_TRANSFER_BIT,
                    VK_PIPELINE_STAGE_TRANSFER_BIT,
                    0,
                    0, nullptr,
                    0, nullptr,
                    1, &imageMemoryBarrier);

            });

        }

        for (size_t i = 0; i < m_ImageViews.size(); i++) {
            VkImageViewCreateInfo viewInfo{};
            viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            viewInfo.image = m_Images[i].Image;
            viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            viewInfo.format = imageFormat;
            viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            viewInfo.subresourceRange.baseMipLevel = 0;
            viewInfo.subresourceRange.levelCount = 1;
            viewInfo.subresourceRange.baseArrayLayer = 0;
            viewInfo.subresourceRange.layerCount = 1;

            if (vkCreateImageView(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), &viewInfo, nullptr, &m_ImageViews[i]) !=
                VK_SUCCESS) {
                PF_CORE_ASSERT(false, "failed to create texture image view!");
            }
        }
      


        for (size_t i = 0; i < m_ImageViews.size(); i++) {
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
            VkResult err = vkCreateSampler(graphicsContext->GetDevice(), &info, nullptr, &m_ImageSampler[i]);
        }
    }
    bool hasStencilComponent(VkFormat format) {
        return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
    }
    void VulkanScreenFrameBuffer::CreateDepthResources() {
        auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();
        auto swapchain = graphicsContext->GetSwapChain();
        VkFormat depthFormat = swapchain->As<VulkanSwapChain>()->GetDepthFormat();

        m_DepthImages.resize(swapchain->GetImageCount());
        m_DepthImageViews.resize(swapchain->GetImageCount());
        for (int i = 0; i < m_DepthImages.size(); i++) {
            VkImageCreateInfo imageInfo{};
            imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
            imageInfo.imageType = VK_IMAGE_TYPE_2D;
            imageInfo.extent.width = m_ImageSize.X;
            imageInfo.extent.height = m_ImageSize.Y;
            imageInfo.extent.depth = 1;
            imageInfo.mipLevels = 1;
            imageInfo.arrayLayers = 1;
            imageInfo.format = depthFormat;
            imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
            imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
            imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
            imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            imageInfo.flags = 0;
            Renderer::Submit([&](CommandBuffer* cmdBuffer) {
                VmaAllocationCreateInfo vmaallocInfo = {};
                vmaallocInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;
                graphicsContext->CreateVmaImage(imageInfo, vmaallocInfo, m_DepthImages[i]);
            });

            VkImageViewCreateInfo viewInfo{};
            viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            viewInfo.image = m_DepthImages[i].Image;
            viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            viewInfo.format = depthFormat;
            viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
            viewInfo.subresourceRange.baseMipLevel = 0;
            viewInfo.subresourceRange.levelCount = 1;
            viewInfo.subresourceRange.baseArrayLayer = 0;
            viewInfo.subresourceRange.layerCount = 1;

            if (vkCreateImageView(graphicsContext->GetDevice(), &viewInfo, nullptr, &m_DepthImageViews[i]) != VK_SUCCESS) {
                PF_CORE_ASSERT(false, "failed to create texture image view!");
            }
            Renderer::Submit([&](CommandBuffer* cmdBuffer) {
            
                VkImageMemoryBarrier barrier{};
                barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
                barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
                barrier.newLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
                barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
                barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
                barrier.image = m_DepthImages[i].Image;
                barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                barrier.subresourceRange.baseMipLevel = 0;
                barrier.subresourceRange.levelCount = 1;
                barrier.subresourceRange.baseArrayLayer = 0;
                barrier.subresourceRange.layerCount = 1;

                VkPipelineStageFlags sourceStage;
                VkPipelineStageFlags destinationStage;
                barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

                if (hasStencilComponent(depthFormat)) {
                    barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
                }
                barrier.srcAccessMask = 0;
                barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

                sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
                destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;

                vkCmdPipelineBarrier(
                    (VkCommandBuffer)cmdBuffer->Get(),
                    sourceStage, destinationStage,
                    0,
                    0, nullptr,
                    0, nullptr,
                    1, &barrier);
            });
        }
    }
    void VulkanScreenFrameBuffer::CreateFramebuffers() {

        const auto& device = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice();
        auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();

        auto swapchain = graphicsContext->GetSwapChain();

        m_Framebuffers.resize(swapchain->GetImageCount());
        for (size_t i = 0; i < swapchain->GetImageCount(); i++) {
            std::array<VkImageView, 2> attachments;
            attachments = { m_ImageViews[i], m_DepthImageViews[i] };

            VkFramebufferCreateInfo framebufferInfo = {};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = m_RenderPass == nullptr ? swapchain->GetRenderPass()->As<VulkanRenderPass>()->GetRenderPass() : m_RenderPass->As<VulkanRenderPass>()->GetRenderPass();
            framebufferInfo.attachmentCount = 2;
            framebufferInfo.pAttachments = attachments.data();
            framebufferInfo.width = m_ImageSize.X;
            framebufferInfo.height = m_ImageSize.Y;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &m_Framebuffers[i]) != VK_SUCCESS) {
                PF_CORE_ASSERT(false, "failed to create framebuffer!");
            }
        }
          
    }

   
    void VulkanScreenFrameBuffer::Resize(uint32_t width, uint32_t height, Count<class RenderPass> renderPass) {
        Release();
        if (renderPass != nullptr)
            m_RenderPass = renderPass;

        m_ImageSize.X = width;
        m_ImageSize.Y = height;
        Init();
    }
    void VulkanScreenFrameBuffer::Resize(Vector2 imageSize, Count<class RenderPass> renderPass) {
        Resize(imageSize.X, imageSize.Y, renderPass);
    }
    
    void VulkanScreenFrameBuffer::Release() {
        for (int i = 0; i < m_Framebuffers.size(); i++) {
            Renderer::SubmitDatafree([buffer = m_Framebuffers[i],depthViews = m_DepthImageViews[i],
            imageViews = m_ImageViews[i], depthimage = m_DepthImages[i],images = m_Images[i]]() {
                auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();
                const auto& device = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice();
                vkDestroyFramebuffer(device, buffer, nullptr);
                vkDestroyImageView(device, depthViews, nullptr);
                vkDestroyImageView(device, imageViews, nullptr);
                vmaDestroyImage(graphicsContext->GetVMA_Allocator(), depthimage.Image, depthimage.Allocation);
           
                vmaDestroyImage(graphicsContext->GetVMA_Allocator(), images.Image, images.Allocation);
           
            });
        }
        m_Framebuffers.clear();
        m_DepthImageViews.clear();
        m_ImageViews.clear();
        m_DepthImages.clear();
        m_Images.clear();
         // remove from index if this mesh was in there before 
        auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();
        int index = -1;
        int iterator = 0;
        for (auto i : graphicsContext->GetSwapChain()->As<VulkanSwapChain>()->FrameBuffers) {
            if (i == this) {
                index = iterator;
                break;
            }
            iterator++;
        }
        if (index != -1) {
            graphicsContext->GetSwapChain()->As<VulkanSwapChain>()->FrameBuffers.erase(
                graphicsContext->GetSwapChain()->As<VulkanSwapChain>()->FrameBuffers.begin() + index);
        }
    }
    void* VulkanScreenFrameBuffer::GetTexture() {
        auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();
        return graphicsContext->GetGlobalPool()->AddTexture(m_ImageSampler[Renderer::GetCurrentFrame().ImageIndex], m_ImageViews[Renderer::GetCurrentFrame().ImageIndex]);
    }
    void VulkanScreenFrameBuffer::Init() {
        auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();
        if (m_ScreenPresent == true) {
            CreateScreenPresent();
            goto a;
        }
        CreateImageViews();
        CreateDepthResources();
        CreateFramebuffers();
        a:
        if (std::find(graphicsContext->GetSwapChain()->As<VulkanSwapChain>()->FrameBuffers.begin(), graphicsContext->GetSwapChain()->As<VulkanSwapChain>()->FrameBuffers.end(),
            this) != graphicsContext->GetSwapChain()->As<VulkanSwapChain>()->FrameBuffers.end())
            return;
        else
            graphicsContext->GetSwapChain()->As<VulkanSwapChain>()->FrameBuffers.emplace_back(this);
    }
    void VulkanScreenFrameBuffer::CreateScreenPresent() {
        auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();
        auto swapchain = graphicsContext->GetSwapChain();
        uint32_t imageCount = swapchain->GetImageCount();
        m_Images.resize(imageCount);
        m_ImageViews.resize(imageCount);
        m_ImageSampler.resize(imageCount);

        m_DepthImages.resize(swapchain->GetImageCount());
        m_DepthImageViews.resize(swapchain->GetImageCount());
        m_Framebuffers.resize(swapchain->GetImageCount());

        VkImageView attachment[1];
        VkFramebufferCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        info.renderPass = m_RenderPass == nullptr ? swapchain->GetRenderPass()->As<VulkanRenderPass>()->GetRenderPass() : m_RenderPass->As<VulkanRenderPass>()->GetRenderPass();
        info.attachmentCount = 1;
        info.pAttachments = attachment;
        info.width = CurrentWindow::GetWindow().GetWidth();
        info.height = CurrentWindow::GetWindow().GetHeight();
        info.layers = 1;
        for (uint32_t i = 0; i < graphicsContext->GetSwapChain()->GetImageCount(); i++) {
            attachment[0] = graphicsContext->GetSwapChain()->As<VulkanSwapChain>()->GetImageView(i);

            if (vkCreateFramebuffer(graphicsContext->GetDevice(), &info, nullptr, &m_Framebuffers[i]) != VK_SUCCESS) {
                PF_CORE_ASSERT(false, "failed to create framebuffer!");
            }
        }
    }
}
