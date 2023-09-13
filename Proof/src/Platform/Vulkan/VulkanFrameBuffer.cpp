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
        if (m_Config.Height == 0 || m_Config.Width == 0)
        {
            auto size = VulkanRenderer::GetGraphicsContext()->GetSwapChain()->GetSwapChainExtent();
            m_Config.Width = size.X;
            m_Config.Height = size.Y;
        }
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(VulkanRenderer::GetGraphicsContext()->GetGPU(), &deviceProperties);

        // Get the maximum framebuffer dimensions
        uint32_t maxWidth = deviceProperties.limits.maxFramebufferWidth;
        uint32_t maxHeight = deviceProperties.limits.maxFramebufferHeight;

        // Check if the current dimensions exceed the maximum supported dimensions
        if (m_Config.Width > maxWidth || m_Config.Height> maxHeight)
        {
            // Adjust the dimensions to fit within the maximum supported dimensions
            m_Config.Width = std::min(m_Config.Width, maxWidth);
            m_Config.Height = std::min(m_Config.Height, maxHeight);
            PF_ENGINE_INFO("FrameBuffer {} Dimension Exceed device Property new Width:{} Height:{}", m_Config.DebugName, m_Config.Width, m_Config.Height);
        }
        SetUpAttachments();

        Build();
    }
    VulkanFrameBuffer::~VulkanFrameBuffer()
    {
        Release();
    }
    void VulkanFrameBuffer::Build()
    {
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
            imageConfig.DebugName = fmt::format("{} FrameBuffer DepthImage {}",m_Config.DebugName,i);
            imageConfig.Format = m_DepthFormat;
            imageConfig.Usage = ImageUsage::Attachment;
            imageConfig.Transfer = m_Config.Transfer;
            imageConfig.Width = m_Config.Width;
            imageConfig.Height = m_Config.Height;
            m_DepthImage.RefImages[i] = Count<VulkanImage2D>::Create(imageConfig,VK_SAMPLE_COUNT_1_BIT);
            {
             
            }

            Renderer::SubmitCommand([&](CommandBuffer* cmdBuffer) {
                /*
                VkImageMemoryBarrier barrier{};
                barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
                barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
                barrier.newLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
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
                    */
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
            imageConfig.DebugName = fmt::format("{} FrameBuffer ColorAttachment Index: {} ", m_Config.DebugName, i);
            imageConfig.Format = imageAttach.Format;
            imageConfig.Usage = ImageUsage::Attachment;
            imageConfig.Transfer = m_Config.Transfer;
            imageConfig.Width = m_Config.Width;
            imageConfig.Height = m_Config.Height;
            colorImage.RefImages[i] = Image2D::Create(imageConfig);
            Renderer::SubmitCommand([&](CommandBuffer* cmdBuffer) {
                /*
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
                    */
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
       
        {
            uint32_t attachmentIndex = 0;
            std::vector<VkAttachmentReference> colorAttachmentReferences;
            std::vector<VkAttachmentDescription> attachmentDescriptions;
            VkAttachmentReference depthAttachmentReference;
            bool depthImage = false;
            for (const auto& attachmentSpec : m_Config.Attachments.Attachments)
            {
                if (Utils::IsDepthFormat(attachmentSpec.Format))
                {


                    VkAttachmentDescription& attachmentDescription = attachmentDescriptions.emplace_back();
                    attachmentDescription.flags = 0;
                    attachmentDescription.format = Utils::ProofFormatToVulkanFormat(attachmentSpec.Format);
                    attachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
                    attachmentDescription.loadOp = m_Config.ClearDepthOnLoad ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD;
                    attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE; 
                    attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
                    attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
                    attachmentDescription.initialLayout = m_Config.ClearDepthOnLoad ? VK_IMAGE_LAYOUT_UNDEFINED : VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
                    //dont usupport seperate stencil and depth yet
                  //  if (Utils::ContainStencilFormat(attachmentSpec.Format) )
                    //{
                       // attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL; 
                        attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL; 
                        depthAttachmentReference = { attachmentIndex, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };
                   // }
                   // else
                   // {
                   //     attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL; 
                   //     attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL; 
                   //     depthAttachmentReference = { attachmentIndex, VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL };
                   // }
                    depthImage = true;
                }
                else
                {

                    VkAttachmentDescription& attachmentDescription = attachmentDescriptions.emplace_back();
                    attachmentDescription.flags = 0;
                    attachmentDescription.format = Utils::ProofFormatToVulkanFormat(attachmentSpec.Format);
                    attachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
                    attachmentDescription.loadOp = m_Config.ClearColorOnLoad ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD;;
                    attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE; 
                    attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
                    attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
                    attachmentDescription.initialLayout = m_Config.ClearColorOnLoad ? VK_IMAGE_LAYOUT_UNDEFINED : VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                    attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                    colorAttachmentReferences.emplace_back(VkAttachmentReference{ attachmentIndex, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL });
                }
                attachmentIndex++;
            }


            VkSubpassDescription subpassDescription = {};
            subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
            subpassDescription.colorAttachmentCount = uint32_t(colorAttachmentReferences.size());
            subpassDescription.pColorAttachments = colorAttachmentReferences.data();
            if (depthImage)
                subpassDescription.pDepthStencilAttachment = &depthAttachmentReference;


            std::vector<VkSubpassDependency> dependencies;

            if (colorAttachmentReferences.size())
            {
                {
                    VkSubpassDependency& depedency = dependencies.emplace_back();
                    depedency.srcSubpass = VK_SUBPASS_EXTERNAL;
                    depedency.dstSubpass = 0;
                    depedency.srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
                    depedency.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
                    depedency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
                    depedency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                    depedency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
                }
                {
                    VkSubpassDependency& depedency = dependencies.emplace_back();
                    depedency.srcSubpass = 0;
                    depedency.dstSubpass = VK_SUBPASS_EXTERNAL;
                    depedency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
                    depedency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                    depedency.dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
                    depedency.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
                    depedency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
                }
            }

            if (depthImage)
            {
                {
                    VkSubpassDependency& depedency = dependencies.emplace_back();
                    depedency.srcSubpass = VK_SUBPASS_EXTERNAL;
                    depedency.dstSubpass = 0;
                    depedency.srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
                    depedency.dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
                    depedency.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
                    depedency.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
                    depedency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
                }

                {
                    VkSubpassDependency& depedency = dependencies.emplace_back();
                    depedency.srcSubpass = 0;
                    depedency.dstSubpass = VK_SUBPASS_EXTERNAL;
                    depedency.srcStageMask = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
                    depedency.dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
                    depedency.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
                    depedency.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
                    depedency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
                }
            }

            VkRenderPassCreateInfo renderPassInfo = {};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
            renderPassInfo.attachmentCount = static_cast<uint32_t>(attachmentDescriptions.size());
            renderPassInfo.pAttachments = attachmentDescriptions.data();
            renderPassInfo.subpassCount = 1;
            renderPassInfo.pSubpasses = &subpassDescription;
            renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
            renderPassInfo.pDependencies = dependencies.data();

            VK_CHECK_RESULT(vkCreateRenderPass(device, &renderPassInfo, nullptr, &m_CompatibilityRenderPass));
            graphicsContext->SetDebugUtilsObjectName(VK_OBJECT_TYPE_RENDER_PASS, m_Config.DebugName + "Compatitbility render pass", m_CompatibilityRenderPass);
        }
        for (size_t i = 0; i < m_Framebuffers.size(); i++)
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
            framebufferInfo.renderPass = m_CompatibilityRenderPass;
            framebufferInfo.attachmentCount = attachments.size();
            framebufferInfo.pAttachments = attachments.data();
            framebufferInfo.width = m_Config.Width;
            framebufferInfo.height = m_Config.Height;
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
    void VulkanFrameBuffer::Resize(uint32_t width, uint32_t height)
    {
        if (m_Config.Width == width && m_Config.Height == height)
            return;
        
        Release();
        m_Config.Width = width;
        m_Config.Height = height;

        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(VulkanRenderer::GetGraphicsContext()->GetGPU(), &deviceProperties);

        // Get the maximum framebuffer dimensions
        uint32_t maxWidth = deviceProperties.limits.maxFramebufferWidth;
        uint32_t maxHeight = deviceProperties.limits.maxFramebufferHeight;

        // Check if the current dimensions exceed the maximum supported dimensions
        if (m_Config.Width > maxWidth || m_Config.Height > maxHeight)
        {
            // Adjust the dimensions to fit within the maximum supported dimensions
            m_Config.Width = std::min(m_Config.Width, maxWidth);
            m_Config.Height = std::min(m_Config.Height, maxHeight);
            PF_ENGINE_INFO("FrameBuffer {} Dimension Exceed device Property new Width:{} Height:{}", m_Config.DebugName, m_Config.Width, m_Config.Height);
        }
        for (auto& framebufferImageAttach : m_ColorImages)
        {
            for (auto& image : framebufferImageAttach.RefImages)
            {
                if (image->GetRendererResourceType() == RendererResourceType::Image2D)
                    image.As<VulkanImage2D>()->Resize(m_Config.Width, m_Config.Height);
            }
        }

        for (auto& dpethImage : m_DepthImage.RefImages)
        {
            if (dpethImage->GetRendererResourceType() == RendererResourceType::Image2D)
                dpethImage.As<VulkanImage2D>()->Resize(m_Config.Width, m_Config.Height);
        }
        Build();

        PF_ENGINE_TRACE("Resized {} FrameBuffer {} {}", m_Config.DebugName, m_Config.Width, m_Config.Height);
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

        // dont destroy images and depth images attached because we may need to resize
        for (size_t i = 0; i < m_Framebuffers.size(); i++)
        {
            Renderer::SubmitDatafree([buffer = m_Framebuffers[i]]() {
                const auto& device = VulkanRenderer::GetGraphicsContext()->GetDevice();
                vkDestroyFramebuffer(device, buffer, nullptr);
            });
        }
        Renderer::SubmitDatafree([renderPass = m_CompatibilityRenderPass]() {
            vkDestroyRenderPass(VulkanRenderer::GetGraphicsContext()->GetDevice(), renderPass, nullptr);
        });
        m_CompatibilityRenderPass = nullptr;
    }
}
