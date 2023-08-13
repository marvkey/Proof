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
#include "VulkanImage.h"
#include <vector>

#include "VulkanCommandBuffer.h"
namespace Proof
{
    static bool hasStencilComponent(VkFormat format) {
        return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT
            || format == VK_FORMAT_D16_UNORM_S8_UINT ;
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
        auto graphicsContext = Renderer::GetGraphicsContext().As <VulkanGraphicsContext>();

        if (std::find(graphicsContext->GetSwapChain().As<VulkanSwapChain>()->FrameBuffers.begin(), graphicsContext->GetSwapChain().As<VulkanSwapChain>()->FrameBuffers.end(),
            this) != graphicsContext->GetSwapChain().As<VulkanSwapChain>()->FrameBuffers.end())
            return;
        else
            graphicsContext->GetSwapChain().As<VulkanSwapChain>()->FrameBuffers.emplace_back(this);

        PF_ENGINE_TRACE("FrameBuffer {} created with Color: {} ImageAttachment and Depth:{} Attachment", m_Config.DebugName, m_ColorImages.size(), 
            0 ? m_DepthImage.RefImages.size() ==0 : 1);
    }
    void VulkanFrameBuffer::SetUpAttachments()
    {
        auto graphicsContext = VulkanRenderer::GetGraphicsContext();
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
        auto graphicsContext = VulkanRenderer::GetGraphicsContext();
        auto swapchain = graphicsContext->GetSwapChain();
        m_DepthFormat = imageAttach.Format;

        VkFormat depthFormat = Utils::ProofFormatToVulkanFormat(imageAttach.Format);
        bool hasImage = true ? imageAttach.ExistingImage.HasImages() : false;
        m_DepthImage.RefImages.resize(swapchain->GetImageCount());

        if (hasImage)
        {
            PF_CORE_ASSERT(imageAttach.ExistingImage.Images.size() == swapchain->GetImageCount(), "Not equal to image count ");
            for (int i = 0; i < imageAttach.ExistingImage.Images.size(); i++)
            {
                auto image = imageAttach.ExistingImage.Images[i];
                if (image->GetRendererResourceType() == RendererResourceType::ImageView || image->GetRendererResourceType() == RendererResourceType::Image2D)
                    m_DepthImage.RefImages[i] = image;
                else
                    PF_CORE_ASSERT(false, "Framebuffer only takes an imageView and Image2D");
            }
            return;
        }
        m_DepthFormat = imageAttach.Format;

        for (int i = 0; i < swapchain->GetImageCount(); i++)
        {

            ImageConfiguration imageConfig;
            imageConfig.DebugName = fmt::format("{} DepthImage ",m_Config.DebugName);
            imageConfig.Format = m_DepthFormat;
            imageConfig.Usage = ImageUsage::Attachment;
            imageConfig.Width = m_Config.Size.X;
            imageConfig.Height = m_Config.Size.Y;
            m_DepthImage.RefImages[i] = Count<VulkanImage2D>::Create(imageConfig,VK_SAMPLE_COUNT_1_BIT);
            {

             
            }
            Renderer::SubmitCommand([&](CommandBuffer* cmdBuffer) {

                VkImageMemoryBarrier barrier{};
                barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
                barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
                barrier.newLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
                barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
                barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
                barrier.image = m_DepthImage.RefImages[i].As<VulkanImage2D>()->GetinfoRef().ImageAlloc.Image;
                barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                barrier.subresourceRange.baseMipLevel = 0;
                barrier.subresourceRange.levelCount = 1;
                barrier.subresourceRange.baseArrayLayer = 0;
                barrier.subresourceRange.layerCount = 1;

                VkPipelineStageFlags sourceStage;
                VkPipelineStageFlags destinationStage;
                barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

                if (Utils::ContainStencilFormat(m_DepthFormat))
                {
                    barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
                }
                barrier.srcAccessMask = 0;
                barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

                sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
                destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;

                vkCmdPipelineBarrier(
                    cmdBuffer->As<VulkanCommandBuffer>()->GetCommandBuffer(Renderer::GetCurrentFrame().FrameinFlight),
                    sourceStage, destinationStage,
                    0,
                    0, nullptr,
                    0, nullptr,
                    1, &barrier);
                /*
                VkClearColorValue clearColor = {};
                clearColor.float32[0] = 0.0f; // Red
                clearColor.float32[1] = 0.0f; // Green
                clearColor.float32[2] = 0.0f; // Blue
                clearColor.float32[3] = 1.0f; // Alpha
                // Specify the range of the image to clear
                VkImageSubresourceRange subresourceRange = {};
                subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT; // Set the aspect mask for color image
                subresourceRange.baseMipLevel = 0;                       // Set the base mip level
                subresourceRange.levelCount = 1;                         // Set the number of mip levels
                subresourceRange.baseArrayLayer = 0;                     // Set the base array layer
                subresourceRange.layerCount = 1;                         // Set the number of layers

                // Clear the image to the specified color
                vkCmdClearColorImage(cmdBuffer->As<VulkanCommandBuffer>()->GetCommandBuffer(), m_DepthImage.RefImages[i].As<VulkanImage2D>()->GetinfoRef().ImageAlloc.Image, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, 
                    &clearColor, 1, &subresourceRange);
                    */

            });
            m_DepthImage.RefImages[i].As<VulkanImage2D>()->UpdateDescriptor();
        }
    }
    void VulkanFrameBuffer::AddImage(const FrameBufferImageConfig& imageAttach)
    {
        auto graphicsContext = VulkanRenderer::GetGraphicsContext();
        auto swapchain = graphicsContext->GetSwapChain();
        uint32_t imageCount = swapchain->GetImageCount();
        auto imageFormat = Utils::ProofFormatToVulkanFormat(imageAttach.Format);
        bool hasImage = true ? imageAttach.ExistingImage.HasImages() : false;
        
        VulkanFrameBufferImages colorImage;
        colorImage.RefImages.resize(imageCount);
        if (hasImage)
        {

            PF_CORE_ASSERT(imageAttach.ExistingImage.Images.size() == swapchain->GetImageCount(), "Not equal to image count ");
            for (int i = 0; i < imageAttach.ExistingImage.Images.size(); i++)
            {
                auto image = imageAttach.ExistingImage.Images[i];
                if (image->GetRendererResourceType() == RendererResourceType::ImageView || image->GetRendererResourceType() == RendererResourceType::Image2D)
                    colorImage.RefImages[i] = imageAttach.ExistingImage.Images[i];
                else
                    PF_CORE_ASSERT(false, "Framebuffer only takes an imageView and Image2D");
            }
            
            m_ColorImages.emplace_back(colorImage);
            return;
        }
        for (int i = 0; i < imageCount; i++)
        {
            ImageConfiguration imageConfig;
            imageConfig.DebugName = fmt::format("{} ColorAttachment Index: {} ", m_Config.DebugName, m_ColorImages.size());
            imageConfig.Format = imageAttach.Format;
            imageConfig.Usage = ImageUsage::Attachment;
            imageConfig.Width = m_Config.Size.X;
            imageConfig.Height = m_Config.Size.Y;
            colorImage.RefImages[i] = Image2D::Create(imageConfig);
            //m_DepthImage.RefImages[i] = Count<VulkanImage2D>::Create(imageConfig,VK_SAMPLE_COUNT_8_BIT);

            Renderer::SubmitCommand([&](CommandBuffer* cmdBuffer) {
                VkImageMemoryBarrier imageMemoryBarrier{};
                imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
                imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
                imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
                imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
                imageMemoryBarrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
                imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
                imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;//VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
                imageMemoryBarrier.image = colorImage.RefImages[i].As<VulkanImage2D>()->GetinfoRef().ImageAlloc.Image;
                imageMemoryBarrier.subresourceRange = VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };

                vkCmdPipelineBarrier(
                    cmdBuffer->As<VulkanCommandBuffer>()->GetCommandBuffer(Renderer::GetCurrentFrame().FrameinFlight),
                    VK_PIPELINE_STAGE_TRANSFER_BIT,
                    VK_PIPELINE_STAGE_TRANSFER_BIT,
                    0,
                    0, nullptr,
                    0, nullptr,
                    1, &imageMemoryBarrier);
                /*
                VkClearColorValue clearColor = {};
                clearColor.float32[0] = 0.0f; // Red
                clearColor.float32[1] = 0.0f; // Green
                clearColor.float32[2] = 0.0f; // Blue
                clearColor.float32[3] = 1.0f; // Alpha
                // Specify the range of the image to clear
                VkImageSubresourceRange subresourceRange = {};
                subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT; // Set the aspect mask for color image
                subresourceRange.baseMipLevel = 0;                       // Set the base mip level
                subresourceRange.levelCount = 1;                         // Set the number of mip levels
                subresourceRange.baseArrayLayer = 0;                     // Set the base array layer
                subresourceRange.layerCount = 1;                         // Set the number of layers
                
                // Clear the image to the specified color
                vkCmdClearColorImage(cmdBuffer->As<VulkanCommandBuffer>()->GetCommandBuffer(), colorImage.RefImages[i].As<VulkanImage2D>()->GetinfoRef().ImageAlloc.Image, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                    &clearColor, 1, &subresourceRange);
                */

            });
            colorImage.RefImages[i].As<VulkanImage2D>()->UpdateDescriptor();
        }
        m_ColorImages.emplace_back(colorImage);

        
   
    }
    void VulkanFrameBuffer::CreateFramebuffer()
    {

        const auto& device = VulkanRenderer::GetGraphicsContext()->GetDevice();
        auto graphicsContext = VulkanRenderer::GetGraphicsContext();

        auto swapchain = graphicsContext->GetSwapChain();

        m_Framebuffers.resize(swapchain->GetImageCount());
        bool multiView = false;

        // for texturecube
        if (m_ColorImages.size() > 0)
        {
            if (m_ColorImages[0].RefImages[0].As<Image2D>())
            {
                if (m_ColorImages[0].RefImages[0].As<Image2D>()->GetSpecification().Layers == 6)
                    multiView = true;

            }
        }
        RenderPassConfig renderPassCofnig("compatible renderPass",m_Config);
        renderPassCofnig.DebugName = "compatible renderPass";
        renderPassCofnig.Pipeline = nullptr;
        renderPassCofnig.MultiView = multiView;
        VulkanRenderPass renderPass(renderPassCofnig);
        
        for (size_t i = 0; i < swapchain->GetImageCount(); i++)
        {

            std::vector<VkImageView> attachments;

            for (auto& coloredImage : m_ColorImages)
            {
                VkDescriptorImageInfo* infoRef = (VkDescriptorImageInfo* )coloredImage.RefImages[i]->GetResourceDescriptorInfo();
                attachments.emplace_back(infoRef->imageView);
            }
            // has depth
            if (m_DepthFormat != ImageFormat::None)
            {
                VkDescriptorImageInfo* infoRef = (VkDescriptorImageInfo*)m_DepthImage.RefImages[i]->GetResourceDescriptorInfo();
                attachments.emplace_back(infoRef->imageView);
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
    Count<Image> VulkanFrameBuffer::GetColorAttachmentImage(uint32_t swapchainImageIndex, uint32_t index)
    {
        return m_ColorImages[index].RefImages[swapchainImageIndex];
    }

    Count<Image> VulkanFrameBuffer::GetDepthImage(uint32_t swapchainImageIndex)
    {
        if(m_DepthFormat == ImageFormat::None)
            return nullptr;
        return m_DepthImage.RefImages[swapchainImageIndex];
    }
    ImageLayouts2D VulkanFrameBuffer::GetColorAttachmentImageLayout(uint32_t index)
    {
        ImageLayouts2D layout;
        layout.Images.resize(Renderer::GetConfig().MaxImageCount);
        int i =  0;
        for (auto image : m_ColorImages[index].RefImages)
        {
            if (image->GetRendererResourceType() == RendererResourceType::ImageView)
                layout.Images[i] = image.As<ImageView>();
            else
                layout.Images[i] = image.As<Image>();

            i++;
        }
        return layout;
    }
    ImageLayouts2D VulkanFrameBuffer::GetDepthImageLayout()
    {
        if (HasDepthImage() == false)
            return ImageLayouts2D();
        ImageLayouts2D layout;
        layout.Images.resize(Renderer::GetConfig().MaxImageCount);
        int index = 0;
        for (auto image : m_DepthImage.RefImages)
        {
            if (image->GetRendererResourceType() == RendererResourceType::ImageView)
                layout.Images[index] = image.As<ImageView>();
            else
                layout.Images[index] = image.As<Image>();

            index++;
        }
        return layout;
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
    void VulkanFrameBuffer::Copy(Count<FrameBuffer> copyFrameBuffer)
    {
        /*

        Count<VulkanFrameBuffer> copyVulkanFrameBuffer = copyFrameBuffer.As<VulkanFrameBuffer>();
        {
                // prepare for copy
            for (int imageIndex = 0; imageIndex < copyVulkanFrameBuffer->m_ColorImages.size(); imageIndex++)
            {
                Renderer::SubmitCommand([&](CommandBuffer* cmd) {
                    //Count<RenderCommandBuffer> renderCmd = RenderCommandBuffer::Create(cmd);
                    VkImage image = copyVulkanFrameBuffer->m_ColorImages[imageIndex].Images[Renderer::GetCurrentFrame().ImageIndex].Image;

                    VkImageMemoryBarrier barrier = {};
                    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
                    barrier.oldLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                    barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
                    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
                    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

                    barrier.image = image;
                    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                    barrier.subresourceRange.baseMipLevel = 0;
                    barrier.subresourceRange.levelCount = 1;
                    barrier.subresourceRange.baseArrayLayer = 0;
                    barrier.subresourceRange.layerCount = 1;
                    barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                    barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

                    vkCmdPipelineBarrier(cmd->As<VulkanCommandBuffer>()->GetCommandBuffer(), VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                        VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);
                });
            }

            if (copyFrameBuffer->HasDepthImage())
            {
                //VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
                Renderer::SubmitCommand([&](CommandBuffer* cmd) {
                    VkImage image = copyVulkanFrameBuffer->m_DepthImage.Images[Renderer::GetCurrentFrame().ImageIndex].Image;

                    VkImageMemoryBarrier barrier = {};
                    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
                    barrier.oldLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
                    barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
                    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
                    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

                    barrier.image = image;
                    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                    barrier.subresourceRange.baseMipLevel = 0;
                    barrier.subresourceRange.levelCount = 1;
                    barrier.subresourceRange.baseArrayLayer = 0;
                    barrier.subresourceRange.layerCount = 1;
                    barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                    barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

                    vkCmdPipelineBarrier(cmd->As<VulkanCommandBuffer>()->GetCommandBuffer(), VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                        VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);
                });
            }
        }

        //copy
        {

        }
        */
    }
    void VulkanFrameBuffer::Release()
    {
        auto swapchain = VulkanRenderer::GetGraphicsContext()->GetSwapChain()    ;

        for (size_t i = 0; i < swapchain->GetImageCount(); i++)
        {
            for (auto& coloredimage : m_ColorImages)
            {
                coloredimage.RefImages.clear();
                /*
                Renderer::SubmitDatafree([images = coloredimage.Images[i], imageViews = coloredimage.ImageViews[i],imageSampler = coloredimage.ImageSampler[i]]()
                {
                    auto graphicsContext = VulkanRenderer::GetGraphicsContext();
                    const auto& device = graphicsContext->GetDevice();
                    vkDestroyImageView(device, imageViews, nullptr);
                    vkDestroySampler(device, imageSampler, nullptr);
                    vmaDestroyImage(graphicsContext->GetVMA_Allocator(), images.Image, images.Allocation);
                });
                */
            }

            if (HasDepthImage())
            {
                if (!HasDepthImage())
                    continue;
                m_DepthImage.RefImages.clear();
                /*
                Renderer::SubmitDatafree([images = m_DepthImage.Images[i], imageViews = m_DepthImage.ImageViews[i], imageSampler = m_DepthImage.ImageSampler[i]]()
                {
                    auto graphicsContext = VulkanRenderer::GetGraphicsContext();
                    const auto& device = graphicsContext->GetDevice();
                    vkDestroyImageView(device, imageViews, nullptr);
                    vkDestroySampler(device, imageSampler, nullptr);
                    vmaDestroyImage(graphicsContext->GetVMA_Allocator(), images.Image, images.Allocation);
                });
                */
            }

            Renderer::SubmitDatafree([buffer = m_Framebuffers[i]]() {
                const auto& device = VulkanRenderer::GetGraphicsContext()->GetDevice();
                vkDestroyFramebuffer(device, buffer, nullptr);
            });
        }
        m_DepthFormat = ImageFormat::None;
        m_DepthImage = {};
        m_ColorImages.clear();
        m_Framebuffers.clear();

         // remove from index if this mesh was in there before 
        auto graphicsContext = Renderer::GetGraphicsContext().As <VulkanGraphicsContext>();
        int index = -1;
        int iterator = 0;
        for (auto i : graphicsContext->GetSwapChain().As<VulkanSwapChain>()->FrameBuffers)
        {
            if (i == this)
            {
                index = iterator;
                break;
            }
            iterator++;
        }
        if (index != -1)
        {
            graphicsContext->GetSwapChain().As<VulkanSwapChain>()->FrameBuffers.erase(
                graphicsContext->GetSwapChain().As<VulkanSwapChain>()->FrameBuffers.begin() + index);
        }
    }
}
