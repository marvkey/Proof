#include "Proofprch.h"
#include "VulkanFrameBuffer.h"
#include "VulkanGraphicsContext.h"
#include "VulkanRenderPass.h"
#include "VulkanRenderer.h"
#include "VulkanUtils/VulkanConvert.h"
#include "VulkanTexutre.h"
#include "VulkanDevice.h"
#include "VulkanImage.h"
#include "VulkanCommandBuffer.h"
#include "Proof/Core/Application.h"

#include <vector>

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
            m_Config.Width = Application::Get()->GetWindow()->GetWidth();
            m_Config.Height = Application::Get()->GetWindow()->GetHeight();
        }
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(VulkanRenderer::GetGraphicsContext()->GetDevice()->GetPhysicalDevice()->GetVulkanPhysicalDevice(), &deviceProperties);

        // Get the maximum framebuffer dimensions
        uint32_t maxWidth = deviceProperties.limits.maxFramebufferWidth;
        uint32_t maxHeight = deviceProperties.limits.maxFramebufferHeight;

        // Check if the current dimensions exceed the maximum supported dimensions
        if (m_Config.Width > maxWidth || m_Config.Height> maxHeight)
        {
            // Adjust the dimensions to fit within the maximum supported dimensions
            m_Config.Width = std::min(m_Config.Width, maxWidth);
            m_Config.Height = std::min(m_Config.Height, maxHeight);
            PF_ENGINE_ERROR("FrameBuffer {} Dimension Exceed device Property new Width:{} Height:{} automatically resized it to correct dimension", m_Config.DebugName, m_Config.Width, m_Config.Height);
        }
        SetUpAttachments();

        Build();
    }
    VulkanFrameBuffer::~VulkanFrameBuffer()
    {
        Release();
    }
    void VulkanFrameBuffer::RT_Build()
    {
        CreateFramebuffer();
        auto graphicsContext = Renderer::GetGraphicsContext().As <VulkanGraphicsContext>();
        PF_ENGINE_TRACE("FrameBuffer {} created imagesCount: {} ", m_Config.DebugName, m_Images.size());
    }
    void VulkanFrameBuffer::Build()
    {
        Count<VulkanFrameBuffer> instance = this;
        Renderer::Submit([instance]()
            {
                instance->RT_Build();
            });
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
                    PF_CORE_ASSERT(false, fmt::format("{} frameBuffer has multiple depth format passed in image Format ", m_Config.DebugName).c_str());
                }
                SetDepth(frameImageConfig);
            }
            else if (Utils::IsColorFormat(frameImageConfig.Format))
                AddImage(frameImageConfig);

        }
    }
    void VulkanFrameBuffer::SetDepth(const FrameBufferImageConfig& imageAttach)
    {
        m_DepthFormat = imageAttach.Format;

        VkFormat depthFormat = Utils::ProofFormatToVulkanFormat(imageAttach.Format);
        bool hasImage = true ? imageAttach.ExistingImage != nullptr : false;

        
        if (hasImage)
        {
            if (imageAttach.ExistingImage->GetRendererResourceType() == RendererResourceType::ImageView || imageAttach.ExistingImage->GetRendererResourceType() == RendererResourceType::Image2D)
                m_DepthImage = imageAttach.ExistingImage;
            else
                PF_CORE_ASSERT(false, "Framebuffer only takes an imageView and Image2D");
        }
        else
        {
            ImageConfiguration imageConfig;
            imageConfig.DebugName = fmt::format("{} FrameBuffer DepthImage", m_Config.DebugName);
            imageConfig.Format = m_DepthFormat;
            imageConfig.Usage = ImageUsage::Attachment;
            imageConfig.Transfer = m_Config.Transfer;
            imageConfig.Width = m_Config.Width;
            imageConfig.Height = m_Config.Height;

            m_DepthImage = Count<VulkanImage2D>::Create(imageConfig, VK_SAMPLE_COUNT_1_BIT);
            #if  0

            Renderer::SubmitCommand([&](CommandBuffer* cmdBuffer) {
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
            #endif 
            m_DepthImage.As<VulkanImage2D>()->UpdateDescriptor();
        }

        m_Images.emplace_back(m_DepthImage);
    }
    void VulkanFrameBuffer::AddImage(const FrameBufferImageConfig& imageAttach)
    {
        auto imageFormat = Utils::ProofFormatToVulkanFormat(imageAttach.Format);
        bool hasImage = true ? imageAttach.ExistingImage != nullptr : false;
        
        Count<Image> image;
        if (hasImage)
        {
            if (imageAttach.ExistingImage->GetRendererResourceType() == RendererResourceType::ImageView || imageAttach.ExistingImage->GetRendererResourceType() == RendererResourceType::Image2D)
                image = imageAttach.ExistingImage;
            else
                PF_CORE_ASSERT(false, "Framebuffer only takes an imageView and Image2D");
        }
        else
        {
            ImageConfiguration imageConfig;
            imageConfig.DebugName = fmt::format("{} FrameBuffer Image Index: {} ", m_Config.DebugName, m_Images.size());
            imageConfig.Format = imageAttach.Format;
            imageConfig.Usage = ImageUsage::Attachment;
            imageConfig.Transfer = m_Config.Transfer;
            imageConfig.Width = m_Config.Width;
            imageConfig.Height = m_Config.Height;

            image = Image2D::Create(imageConfig);

            #if 0
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
            #endif
            image.As<VulkanImage2D>()->UpdateDescriptor();

        }
        m_Images.emplace_back(image);
   
    }
    void VulkanFrameBuffer::CreateFramebuffer()
    {
        const auto device = VulkanRenderer::GetGraphicsContext()->GetDevice()->GetVulkanDevice();
        auto graphicsContext = VulkanRenderer::GetGraphicsContext();

        {
            uint32_t attachmentIndex = 0;
            std::vector<VkAttachmentReference> colorAttachmentReferences;
            std::vector<VkAttachmentDescription> attachmentDescriptions;
            VkAttachmentReference depthAttachmentReference;
            bool depthImage = false;
            bool depthFirst = false;
            for (const auto& attachmentSpec : m_Config.Attachments.Attachments)
            {
                if (Utils::IsDepthFormat(attachmentSpec.Format))
                {
                    if (attachmentIndex == 0)
                        depthFirst = true;

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
                    
                    VkSubpassDependency depedency;
                    depedency.srcSubpass = VK_SUBPASS_EXTERNAL;
                    depedency.dstSubpass = 0;
                    depedency.srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
                    depedency.dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
                    depedency.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
                    depedency.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
                    depedency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

                    if (depthFirst)
                    {
                        dependencies.insert(dependencies.begin(), depedency);
                    }
                    else
                    {
                        dependencies.emplace_back(depedency);
                    }
                }

                {
                    VkSubpassDependency depedency;
                    depedency.srcSubpass = 0;
                    depedency.dstSubpass = VK_SUBPASS_EXTERNAL;
                    depedency.srcStageMask = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
                    depedency.dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
                    depedency.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
                    depedency.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
                    depedency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

                    if (depthFirst)
                    {
                        dependencies.insert(dependencies.begin()+1, depedency);
                    }
                    else
                    {
                        dependencies.emplace_back(depedency);   
                    }
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
            VulkanUtils::SetDebugUtilsObjectName(device,VK_OBJECT_TYPE_RENDER_PASS, m_Config.DebugName + "Compatitbility render pass", m_CompatibilityRenderPass);
        }

        std::vector<VkImageView> attachments;
        {
            uint32_t index = 0;
            for (auto& coloredImage : m_Images )
            {
                VkDescriptorImageInfo* infoRef = nullptr;

                if (coloredImage->GetRendererResourceType() == RendererResourceType::Image2D)
                {
                    Count<Image2D> image2D = coloredImage.As<Image2D>();
                    if(image2D->GetSpecification().Layers > 1)
                        infoRef = (VkDescriptorImageInfo*)image2D->CreateOrGetImageMip(0,m_Config.ExistingImageLayers[index]).As<VulkanImageView>()->GetResourceDescriptorInfo();

                }
                if(infoRef == nullptr)
                    infoRef = (VkDescriptorImageInfo*)coloredImage->GetResourceDescriptorInfo();

                index++;
                attachments.emplace_back(infoRef->imageView);
            }
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
        if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &m_FrameBuffer))
        {
            PF_CORE_ASSERT(false, fmt::format("failed to create {} Framebuffer", m_Config.DebugName));
        }
    }

    VkFramebuffer VulkanFrameBuffer::GetFrameBuffer()
    {
        return m_FrameBuffer;
    }
    Count<Image> VulkanFrameBuffer::GetOutput(uint32_t imageIndex)
    {
        PF_CORE_ASSERT(imageIndex <= m_Images.size(),"FrameBuffer does not contain imageIndex");
        return m_Images[imageIndex];
    }

    Count<Image> VulkanFrameBuffer::GetDepthOutput()
    {
        return m_DepthImage;
    }

    bool VulkanFrameBuffer::HasDepthImage()
    {
        return m_DepthFormat != ImageFormat::None;
    }
    
    void VulkanFrameBuffer::Resize(uint32_t width, uint32_t height)
    {
        if (m_Config.Width == width && m_Config.Height == height)
            return;

#if 0
    #if 0
            Release();
            Count<VulkanFrameBuffer> instance = this;
            Renderer::Submit([instance, width, height]() mutable
                {
                    //instance->Release();

                    instance->m_Config.Width = width;
                    instance->m_Config.Height = height;

                    VkPhysicalDeviceProperties deviceProperties;
                    vkGetPhysicalDeviceProperties(VulkanRenderer::GetGraphicsContext()->GetDevice()->GetPhysicalDevice()->GetVulkanPhysicalDevice(), &deviceProperties);

                    // Get the maximum framebuffer dimensions
                    uint32_t maxWidth = deviceProperties.limits.maxFramebufferWidth;
                    uint32_t maxHeight = deviceProperties.limits.maxFramebufferHeight;

                    // Check if the current dimensions exceed the maximum supported dimensions
                    if (instance->m_Config.Width > maxWidth || instance->m_Config.Height > maxHeight)
                    {
                        // Adjust the dimensions to fit within the maximum supported dimensions
                        instance->m_Config.Width = std::min(instance->m_Config.Width, maxWidth);
                        instance->m_Config.Height = std::min(instance->m_Config.Height, maxHeight);
                        PF_ENGINE_INFO("FrameBuffer {} Dimension Exceed device Property new Width:{} Height:{}", instance->m_Config.DebugName, instance->m_Config.Width, instance->m_Config.Height);
                    }

                    for (auto& image : instance->m_Images)
                    {
                        if (image->GetRendererResourceType() == RendererResourceType::Image2D)
                        {
                            auto vkImage = image.As<VulkanImage2D>();
                            vkImage->Release();
                            vkImage->GetSpecificationRef().Width = instance->m_Config.Width;
                            vkImage->GetSpecificationRef().Height = instance->m_Config.Height;
                            vkImage->RT_Build();

                        }
                    }

                    //instance->RT_Build();
                    PF_ENGINE_TRACE("Resized {} FrameBuffer {} {}", instance->m_Config.DebugName, instance->m_Config.Width, instance->m_Config.Height);
                });
            for (auto& image : m_Images)
            {
                if (image->GetRendererResourceType() == RendererResourceType::Image2D)
                {
                    auto vkImage = image.As<VulkanImage2D>();
                    vkImage->CallOnResizeFunctions();
                }
            }
            Build();

    #else
            Release();

            m_Config.Width = width;
            m_Config.Height = height;

            VkPhysicalDeviceProperties deviceProperties;
            vkGetPhysicalDeviceProperties(VulkanRenderer::GetGraphicsContext()->GetDevice()->GetPhysicalDevice()->GetVulkanPhysicalDevice(), &deviceProperties);

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
            for (auto& image : m_Images)
            {
                if (image->GetRendererResourceType() == RendererResourceType::Image2D)
                {
                    auto vkImage = image.As<VulkanImage2D>();
                    vkImage->Release();
                    vkImage->GetSpecificationRef().Width = m_Config.Width;
                    vkImage->GetSpecificationRef().Height = m_Config.Height;
                    vkImage->Build();

                    vkImage->CallOnResizeFunctions();
                }
            }
            Build();
    #endif
#endif

#if 1
        Release();
        m_Config.Width = width;
        m_Config.Height = height;

        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(VulkanRenderer::GetGraphicsContext()->GetDevice()->GetPhysicalDevice()->GetVulkanPhysicalDevice(), &deviceProperties);

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
        for (auto& image : m_Images)
        {
            if (image->GetRendererResourceType() == RendererResourceType::Image2D)
                image.As<VulkanImage2D>()->Resize(m_Config.Width, m_Config.Height);
        }

        Build();

        PF_ENGINE_TRACE("Resized {} FrameBuffer {} {}", m_Config.DebugName, m_Config.Width, m_Config.Height);
#endif
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
        // dont destroy images and depth images attached because we may need to resize
        Renderer::SubmitResourceFree([buffer = m_FrameBuffer]() 
            {
            const auto& device = VulkanRenderer::GetGraphicsContext()->GetDevice()->GetVulkanDevice();
            vkDestroyFramebuffer(device, buffer, nullptr);
        });
        Renderer::SubmitResourceFree([renderPass = m_CompatibilityRenderPass]() 
        {
            vkDestroyRenderPass(VulkanRenderer::GetGraphicsContext()->GetDevice()->GetVulkanDevice(), renderPass, nullptr);
        });
        m_CompatibilityRenderPass = nullptr;
    }
}
