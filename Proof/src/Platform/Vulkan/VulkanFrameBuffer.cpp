#include "Proofprch.h"
#include "VulkanFrameBuffer.h"
#include "Proof/Renderer/Renderer.h"
#include "VulkanGraphicsContext.h"
#include "VulkanSwapChain.h"
#include "VulkanRenderPass.h"
#include "VulkanRenderer/VulkanRenderer.h"
#include "VulkanSwapChain.h"
#include <algorithm>
#include "VulkanUtils/VulkanConvert.h"
#include "VulkanTexutre.h"
#include <vector>
namespace Proof
{
    static bool hasStencilComponent(VkFormat format) {
        return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT
            || format == VK_FORMAT_D16_UNORM_S8_UINT;
    }
    VulkanFrameBuffer::VulkanFrameBuffer(const FrameBufferConfig& attachments)
        :
        m_Config(attachments)
    {

        Init();
    }
    VulkanFrameBuffer::~VulkanFrameBuffer()
    {
        Release();
    }
    void VulkanFrameBuffer::Init()
    {
        SetUpAttachments();
        CreateFramebuffer();
    }
    void VulkanFrameBuffer::SetUpAttachments()
    {
        auto graphicsContext = Renderer::GetGraphicsContext().As<VulkanGraphicsContext>();
        for (auto& frameImageConfig : m_Config.Attachments.Attachments)
        {
            if (Utils::IsDepthFormat(frameImageConfig.Format))
            {
                // depth alrread has been set
                if (m_DepthFormat != ImageFormat::None)
                {
                    PF_CORE_ASSERT(false, fmt::format("{} frameBuffer has multiple depth format passed in image Foramt ", m_Config.DebugName).c_str());
                }
                SetDepth(frameImageConfig);
            }
            else if (Utils::IsColorFormat(frameImageConfig.Format))
                AddImage(frameImageConfig);

        }
    }
    void VulkanFrameBuffer::SetDepth(const FrameBufferImageConfig& imageAttach)
    {
        auto graphicsContext = Renderer::GetGraphicsContext().As<VulkanGraphicsContext>();
        auto swapchain = graphicsContext->GetSwapChain();
        VkFormat depthFormat = Utils::ProofFormatToVulkanFormat(imageAttach.Format);
        bool hasImage = true ? imageAttach.GetImage().HasImage() == true || imageAttach.GetImagelayout().HasImages() == true : false;

        m_DepthImage = VulkanFrameBufferImages(hasImage);
        m_DepthFormat = imageAttach.Format;
        m_DepthImage.Images.resize(swapchain->GetImageCount());
        m_DepthImage.ImageSampler.resize(swapchain->GetImageCount());
        m_DepthImage.ImageViews.resize(swapchain->GetImageCount());
        for (int i = 0; i < swapchain->GetImageCount(); i++)
        {
            VkImageCreateInfo imageInfo{};
            imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
            imageInfo.imageType = VK_IMAGE_TYPE_2D;
            imageInfo.extent.width = m_Config.Size.X;
            imageInfo.extent.height = m_Config.Size.Y;
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
                vmaallocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
                graphicsContext->CreateVmaImage(imageInfo, vmaallocInfo,m_DepthImage.Images[i]);
            });

            VkImageViewCreateInfo viewInfo{};
            viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            viewInfo.image = m_DepthImage.Images[i].Image;
            viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            viewInfo.format = depthFormat;
            viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
            viewInfo.subresourceRange.baseMipLevel = 0;
            viewInfo.subresourceRange.levelCount = 1;
            viewInfo.subresourceRange.baseArrayLayer = 0;
            viewInfo.subresourceRange.layerCount = 1;

            if (vkCreateImageView(graphicsContext->GetDevice(), &viewInfo, nullptr, &m_DepthImage.ImageViews[i]) != VK_SUCCESS)
            {
                PF_CORE_ASSERT(false, "failed to create texture image view!");
            }
            Renderer::Submit([&](CommandBuffer* cmdBuffer) {

                VkImageMemoryBarrier barrier{};
                barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
                barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
                barrier.newLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
                barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
                barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
                barrier.image = m_DepthImage.Images[i].Image;
                barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                barrier.subresourceRange.baseMipLevel = 0;
                barrier.subresourceRange.levelCount = 1;
                barrier.subresourceRange.baseArrayLayer = 0;
                barrier.subresourceRange.layerCount = 1;

                VkPipelineStageFlags sourceStage;
                VkPipelineStageFlags destinationStage;
                barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

                if (hasStencilComponent(depthFormat))
                {
                    barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
                }
                barrier.srcAccessMask = 0;
                barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

                sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
                destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;

                vkCmdPipelineBarrier(
                    cmdBuffer->As<VulkanCommandBuffer>()->GetCommandBuffer(),
                    sourceStage, destinationStage,
                    0,
                    0, nullptr,
                    0, nullptr,
                    1, &barrier);
            });
        }
    }
    void VulkanFrameBuffer::AddImage(const FrameBufferImageConfig& imageAttach)
    {
        auto graphicsContext = Renderer::GetGraphicsContext().As<VulkanGraphicsContext>();
        auto swapchain = graphicsContext->GetSwapChain();
        uint32_t imageCount = swapchain->GetImageCount();
        auto imageFormat = Utils::ProofFormatToVulkanFormat(imageAttach.Format);

        bool hasImage = true ? imageAttach.GetImage().HasImage() == true || imageAttach.GetImagelayout().HasImages() == true : false;
        VulkanFrameBufferImages image(hasImage);
        image.Images.resize(imageCount);
        image.ImageViews.resize(imageCount);
        image.ImageSampler.resize(imageCount);
        for (int i = 0; i < imageCount; i++)
        {
            if (imageAttach.GetImagelayout().HasImages())
            {
                if (imageAttach.GetImagelayout().Images.size() >= i)
                {
                    VulkanImage& vkImage = (VulkanImage&)imageAttach.GetImagelayout().Images[i];
                    image.Images[i].Image = vkImage.GetImage();
                }
                else
                {
                    VulkanImage& vkImage = (VulkanImage&)imageAttach.GetImagelayout().Images.back();
                    image.Images[i].Image = vkImage.GetImage();
                }
                continue;
            }
            if (imageAttach.GetImage().HasImage())
            {
                VulkanImage& vkImage = (VulkanImage&)imageAttach.GetImage();
                image.Images[i].Image = vkImage.GetImage();
                //image.ImageSampler[i] = vkImage.GetImageSampler();
                continue;
            }
            if (image.Images[i].Image != nullptr)
                continue;
            VkImageCreateInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
            info.imageType = VK_IMAGE_TYPE_2D;
            info.format = imageFormat;
            info.extent.width = m_Config.Size.X;
            info.extent.height = m_Config.Size.Y;
            info.extent.depth = 1;
            info.mipLevels = 1;
            info.arrayLayers = 1;
            info.samples = VK_SAMPLE_COUNT_1_BIT;
            info.tiling = VK_IMAGE_TILING_OPTIMAL;
            info.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
            info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            Renderer::Submit([&](CommandBuffer* cmdBuffer) {
                VmaAllocationCreateInfo vmaallocInfo = {};
                vmaallocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

                graphicsContext->CreateVmaImage(info, vmaallocInfo, image.Images[i]);
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
                imageMemoryBarrier.image = image.Images[i].Image;
                imageMemoryBarrier.subresourceRange = VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };

                vkCmdPipelineBarrier(
                    cmdBuffer->As<VulkanCommandBuffer>()->GetCommandBuffer(),
                    VK_PIPELINE_STAGE_TRANSFER_BIT,
                    VK_PIPELINE_STAGE_TRANSFER_BIT,
                    0,
                    0, nullptr,
                    0, nullptr,
                    1, &imageMemoryBarrier);

            });
        }
        for (size_t i = 0; i < image.ImageViews.size(); i++)
        {
            if (imageAttach.GetImagelayout().HasImages())
            {
                if (imageAttach.GetImagelayout().Images.size() >= i)
                {
                    VulkanImage& vkImage = (VulkanImage&)imageAttach.GetImagelayout().Images[i];
                    image.ImageViews[i] = vkImage.GetImageView();
                    image.ImageSampler[i] = vkImage.GetImageSampler();
                }
                else
                {
                    VulkanImage& vkImage = (VulkanImage&)imageAttach.GetImagelayout().Images.back();
                    image.ImageViews[i] = vkImage.GetImageView();
                    image.ImageSampler[i] = vkImage.GetImageSampler();
                }
                continue;
            }
            if (imageAttach.GetImage().HasImage())
            {
                VulkanImage& vkImage = (VulkanImage&)imageAttach.GetImage();
                image.ImageViews[i] = vkImage.GetImageView();
                image.ImageSampler[i] = vkImage.GetImageSampler();
                continue;
            }
            VkImageViewCreateInfo viewInfo{};
            viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            viewInfo.image = image.Images[i].Image;
            viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            viewInfo.format = imageFormat;
            viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            viewInfo.subresourceRange.baseMipLevel = 0;
            viewInfo.subresourceRange.levelCount = 1;
            viewInfo.subresourceRange.baseArrayLayer = 0;
            viewInfo.subresourceRange.layerCount = 1;

            if (vkCreateImageView(Renderer::GetGraphicsContext().As<VulkanGraphicsContext>()->GetDevice(), &viewInfo, nullptr, &image.ImageViews[i]) !=
                VK_SUCCESS)
            {
                PF_CORE_ASSERT(false, "failed to create texture image view!");
            }
        }

        for (size_t i = 0; i < image.ImageSampler.size(); i++)
        {
            if (image.ImageSampler[i] != nullptr)
                continue;
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
            if (vkCreateSampler(graphicsContext->GetDevice(), &info, nullptr, &image.ImageSampler[i]) !=
                VK_SUCCESS)
            {
                PF_CORE_ASSERT(false, "failed to create texture image sampler");
            }
        }
        m_ColorImages.emplace_back(image);
    }
    void VulkanFrameBuffer::CreateFramebuffer()
    {

        const auto& device = Renderer::GetGraphicsContext().As<VulkanGraphicsContext>()->GetDevice();
        auto graphicsContext = Renderer::GetGraphicsContext().As<VulkanGraphicsContext>();

        auto swapchain = graphicsContext->GetSwapChain();

        m_Framebuffers.resize(swapchain->GetImageCount());
        VulkanRenderPass renderPass(RenderPassConfig("compatible renderPass", m_Config));
        
        for (size_t i = 0; i < swapchain->GetImageCount(); i++)
        {
            std::vector<VkImageView> attachments;

            for (auto& coloredImage : m_ColorImages)
            {
                attachments.emplace_back(coloredImage.ImageViews[i]);
            }
            // has depth
            if (m_DepthFormat != ImageFormat::None)
            {
                attachments.emplace_back(m_DepthImage.ImageViews[i]);
            }

            
            VkFramebufferCreateInfo framebufferInfo = {};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            // this is just to check if compatible wit renderPass

            // i dont wanna do this but it needs a compatible render to create for some reason
            framebufferInfo.renderPass = renderPass.GetRenderPass();
            framebufferInfo.attachmentCount = attachments.size();
            framebufferInfo.pAttachments = attachments.data();
            framebufferInfo.width = m_Config.Size.X;
            framebufferInfo.height = m_Config.Size.Y;
            framebufferInfo.layers = 1;
            framebufferInfo.pNext = nullptr;
            if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &m_Framebuffers[i]))
            {
                PF_CORE_ASSERT(false, fmt::format("failed to create {} Framebuffer", m_Config.DebugName));
            }
        }
    }

    VkFramebuffer VulkanFrameBuffer::GetFrameBuffer(uint32_t index)
    {
        return m_Framebuffers[index];
    }
    Image VulkanFrameBuffer::GetColorAttachmentImage(uint32_t index, uint32_t imageIndex)
    {
        auto graphicsContext = Renderer::GetGraphicsContext().As<VulkanGraphicsContext>();
        auto swapchain = graphicsContext->GetSwapChain();
        VkDescriptorSet set = graphicsContext->GetGlobalPool()->AddTexture(
            m_ColorImages[index].ImageSampler[imageIndex],
            m_ColorImages[index].ImageViews[imageIndex]);

        return VulkanImage(set, m_Config.Attachments.Attachments[index].Format, m_Config.Size, VulkanImageExcessData{ m_ColorImages[index].ImageSampler[imageIndex],
            m_ColorImages[index].ImageViews[imageIndex],m_ColorImages[index].Images[imageIndex].Image });
    }
    VulkanFrameBufferImages VulkanFrameBuffer::GetColorAttachmentFrameBufferImage(uint32_t index)
    {
        return m_ColorImages[index];
    }
    Image VulkanFrameBuffer::GetDepthImage(uint32_t imageIndex )
    {
        if(m_DepthFormat == ImageFormat::None)
            return Image();

        auto graphicsContext = Renderer::GetGraphicsContext().As<VulkanGraphicsContext>();
        auto swapchain = graphicsContext->GetSwapChain();
        VkDescriptorSet set = graphicsContext->GetGlobalPool()->AddTexture(
            m_DepthImage.ImageSampler[imageIndex],
            m_DepthImage.ImageViews[imageIndex]);

        return VulkanImage(set, m_DepthFormat, m_Config.Size, VulkanImageExcessData{ m_DepthImage.ImageSampler[imageIndex],
             m_DepthImage.ImageViews[imageIndex], m_DepthImage.Images[imageIndex].Image });
    }
    ImageLayouts VulkanFrameBuffer::GetColorAttachmentImageLayout(uint32_t index)
    {
        std::vector<Image> image;
        image.resize(m_Framebuffers.size());
        for (int i = 0; i < m_Framebuffers.size(); i++)
        {
            image.emplace_back(GetColorAttachmentImage(index,i));
        }
        return ImageLayouts(image);
    }
    ImageLayouts VulkanFrameBuffer::GetDepthImageLayout()
    {
        if (HasDepthImage() == false)
            return ImageLayouts();
        std::vector<Image> image;
        image.resize(m_Framebuffers.size());
        for (int i = 0; i < m_Framebuffers.size(); i++)
        {
            image.emplace_back(GetDepthImage(i));
        }
        return ImageLayouts(image);
    }
    bool VulkanFrameBuffer::HasDepthImage()
    {
        return m_DepthFormat != ImageFormat::None;
    }
    bool VulkanFrameBuffer::HasColorAttachment()
    {
        return m_ColorImages.size()>0;
    }
    void VulkanFrameBuffer::Resize(Vector2 imageSize)
    {
        Release();
        m_Config.Size = imageSize;
        Init();
    }
    void VulkanFrameBuffer::Release()
    {
        auto swapchain = Renderer::GetGraphicsContext().As<VulkanGraphicsContext>()->GetSwapChain()    ;

        for (size_t i = 0; i < swapchain->GetImageCount(); i++)
        {
            for (auto& coloredimage : m_ColorImages)
            {
                if (coloredimage.ImageAttached == true)
                    continue;
                Renderer::SubmitDatafree([images = coloredimage.Images[i], imageViews = coloredimage.ImageViews[i],imageSampler = coloredimage.ImageSampler[i]]()
                {
                    auto graphicsContext = Renderer::GetGraphicsContext().As<VulkanGraphicsContext>();
                    const auto& device = graphicsContext->GetDevice();
                    vkDestroyImageView(device, imageViews, nullptr);
                    vkDestroySampler(device, imageSampler, nullptr);
                    vmaDestroyImage(graphicsContext->GetVMA_Allocator(), images.Image, images.Allocation);
                });
            }

            if (HasDepthImage())
            {
                if (m_DepthImage.ImageAttached == true)
                    continue;
                Renderer::SubmitDatafree([images = m_DepthImage.Images[i], imageViews = m_DepthImage.ImageViews[i], imageSampler = m_DepthImage.ImageSampler[i]]()
                {
                    auto graphicsContext = Renderer::GetGraphicsContext().As<VulkanGraphicsContext>();
                    const auto& device = graphicsContext->GetDevice();
                    vkDestroyImageView(device, imageViews, nullptr);
                    vkDestroySampler(device, imageSampler, nullptr);
                    vmaDestroyImage(graphicsContext->GetVMA_Allocator(), images.Image, images.Allocation);
                });
            }

            Renderer::SubmitDatafree([buffer = m_Framebuffers[i]]() {
                const auto& device = Renderer::GetGraphicsContext().As<VulkanGraphicsContext>()->GetDevice();
                vkDestroyFramebuffer(device, buffer, nullptr);
            });
        }
        m_DepthFormat = ImageFormat::None;
        m_DepthImage = {};
        m_ColorImages.clear();
        m_Framebuffers.clear();
    }
}
