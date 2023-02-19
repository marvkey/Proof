#include "Proofprch.h"
#include "Proof/Core/Core.h"
#include "VulkanRenderPass.h"
#include "VulkanRenderer/VulkanRenderer.h"
#include "VulkanFrameBuffer.h"
#include "VulkanFrameBuffer.h"
#include "VulkanUtils/VulkanConvert.h"
namespace Proof
{
    VulkanRenderPass::VulkanRenderPass(const RenderPassConfig& config)
        :
        m_Config(config)
    {
        Init();
        CreateRenderPass();
    }
   
    void VulkanRenderPass::Init()
    {
        //https://developer.samsung.com/galaxy-gamedev/resources/articles/renderpasses.html
        auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();
        auto swapChain = graphicsContext->GetSwapChain()->As<VulkanSwapChain>();

        for (auto& imageConfig : m_Config.Attachments.Attachments)
        {
            if (Utils::IsColorFormat(imageConfig.Format))
            {
                AddColorAttachment(imageConfig);
                continue;
            }
            if (Utils::IsDepthFormat(imageConfig.Format))
            {
                if (m_DepthFormat != ImageFormat::None)
                {
                    PF_CORE_ASSERT(false,
                        fmt::format("{} Cannot have to multiple depth format in renderPass", m_Config.DebugName).c_str());
                    continue;
                }
                SetDepthAttachment(imageConfig);
                continue;
            }

            PF_CORE_ASSERT(false,fmt::format("{} {} does not image format", m_Config.DebugName, EnumReflection::EnumString(imageConfig.Format)).c_str());
        }
    }
    void VulkanRenderPass::CreateRenderPass()
    {
        std::vector<VkAttachmentDescription> attachmentDescriptions;

        // compiler deleteing this for some reason before the functions is finsed 
        static std::vector<VkAttachmentReference> colorAttachmentRefs;
        colorAttachmentRefs.clear();
        for (auto& colorAttachment : m_ColorAttachments)
        {
            attachmentDescriptions.emplace_back(colorAttachment.AttchmentDescription);

            colorAttachmentRefs.emplace_back(colorAttachment.AttachmentRef);
        }

        // has depth
        if (m_DepthFormat != ImageFormat::None)
        {
            attachmentDescriptions.emplace_back(m_DepthAttachment.AttchmentDescription);
        }

        VkSubpassDescription subpass;
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

        VkSubpassDependency dependency = {};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.srcAccessMask = 0;
        dependency.dstSubpass = 0;
        if (m_ColorAttachments.size() > 0)
        {
            dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
            dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;

            dependency.dstAccessMask |= VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            subpass.colorAttachmentCount = colorAttachmentRefs.size();
            subpass.pColorAttachments = colorAttachmentRefs.data();
        }
        else
        {
            subpass.colorAttachmentCount = 0;
            subpass.pColorAttachments = nullptr;
        }

            // has depth
        if (m_DepthFormat != ImageFormat::None)
        {
            dependency.dstAccessMask |= VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
            subpass.pDepthStencilAttachment = &m_DepthAttachment.AttachmentRef;
        }
        else
        {
            subpass.pDepthStencilAttachment = nullptr;
        }
        subpass.inputAttachmentCount = 0;
        subpass.pInputAttachments = nullptr;
        subpass.pPreserveAttachments = nullptr;
        subpass.preserveAttachmentCount =0;
        subpass.pResolveAttachments = 0;
        subpass.flags = 0;
        // create render pass
        VkRenderPassCreateInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = attachmentDescriptions.size();
        renderPassInfo.pAttachments = attachmentDescriptions.data();
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;
        /*
            Bit mask that specifies which view rendering is broadcast to
            0011 = Broadcast to first and second view (layer)
        */

        /*
        
        */
        // default broadcast to all layers
        //https://anishbhobe.site/post/vulkan-render-to-cubemap-using-multiview/#3-attaching-the-cubemap-to-the-framebuffer
        const uint32_t viewMask = 0b11111111;
        //const uint32_t viewMask = 0;
        /*
            Bit mask that specifies correlation between views
            An implementation may use this for optimizations (concurrent render)
        */
        const uint32_t correlationMask = 0;

        VkRenderPassMultiviewCreateInfo renderPassMultiviewCI{};
        renderPassMultiviewCI.sType = VK_STRUCTURE_TYPE_RENDER_PASS_MULTIVIEW_CREATE_INFO;
        renderPassMultiviewCI.subpassCount = 1;
        renderPassMultiviewCI.pViewMasks = &viewMask;
        renderPassMultiviewCI.correlationMaskCount = 0;
        renderPassMultiviewCI.pCorrelationMasks = nullptr;
        renderPassMultiviewCI.pNext = nullptr;
        renderPassMultiviewCI.pViewOffsets = NULL;
        if (m_Config.MultiView)
        {
            renderPassInfo.pNext = &renderPassMultiviewCI;
        }
        if (vkCreateRenderPass(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), &renderPassInfo, nullptr, &m_RenderPass) != VK_SUCCESS)
        {
            PF_CORE_ASSERT(false, "failed to create render pass!");
        }
    }
    void VulkanRenderPass::Release()
    {
        m_DepthAttachment = {};
        m_ColorAttachments = {};
        m_DepthFormat = ImageFormat::None;

        Renderer::SubmitDatafree([renderPass = m_RenderPass] {
            auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();

            vkDestroyRenderPass(graphicsContext->GetDevice(), renderPass, nullptr);
        });

        m_RenderPass = nullptr;
    }
    VulkanRenderPass::~VulkanRenderPass() {
        Release();
    }
    void VulkanRenderPass::SetDepthAttachment(const RenderPassImageConfig& config)
    {
        VkAttachmentDescription depthAttachment{};
        depthAttachment.format = Utils::ProofFormatToVulkanFormat(config.Format);
        depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentReference depthAttachmentRef{};
        depthAttachmentRef.attachment = m_ColorAttachments.size();
        depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        m_DepthAttachment = { depthAttachment,depthAttachmentRef };

        m_DepthFormat = config.Format;
    }
    void VulkanRenderPass::AddColorAttachment(const RenderPassImageConfig& config)
    {
        uint32_t attachIndex = m_ColorAttachments.size();
        if (m_DepthFormat != ImageFormat::None)
        {
            attachIndex += 1;
        }
        VkAttachmentReference attachmentRef = {};
        attachmentRef.attachment = attachIndex;

        VkAttachmentDescription attachment = {};

        if (config.PresentKHr == true)
        {
            attachment.format = Utils::ProofFormatToVulkanFormat(config.Format);
            attachment.samples = VK_SAMPLE_COUNT_1_BIT;
            attachment.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            attachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

            attachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        }
        else
        {
            attachment.format = Utils::ProofFormatToVulkanFormat(config.Format);
            attachment.samples = VK_SAMPLE_COUNT_1_BIT;
            attachment.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            attachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            attachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

            attachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        }

        m_ColorAttachments.emplace_back(VulkanRenderPassAttach{ attachment,attachmentRef });
    }

    void VulkanRenderPass::BeginRenderPass(Count<class RenderCommandBuffer> command, Count<class FrameBuffer> frameBuffer)
    {
        PF_CORE_ASSERT(m_RenderPassEnabled == false, fmt::format("cannot start {} render pass when previous render pass is not closed",m_Config.DebugName).c_str());
        m_CommandBuffer = command;
        m_CurrentFrameBuffer = frameBuffer;
        m_RenderPassEnabled = true;

        const FrameBufferConfig& config = frameBuffer->GetConfig();
        VkClearValue colorValue{ config.ClearColor.X, config.ClearColor.Y, config.ClearColor.Z, config.ClearColor.W };
        std::vector< VkClearValue> clearValues;
  
        // setting frameBuffer clear values
        {
            clearValues.resize(config.Attachments.Attachments.size());
            std::fill(clearValues.begin(), clearValues.end(), VkClearValue{ colorValue.color });//https://stackoverflow.com/questions/8848575/fastest-way-to-reset-every-value-of-stdvectorint-to-0
            // depth image always he lst one bind
            if (frameBuffer->HasDepthImage())
            {
                auto& val = clearValues.back();
                val.depthStencil = { config.DepthClearValue,config.StencilClearValue };
            }
        }
        // starting the render pass
        {
            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = m_RenderPass;
            // teh frameBuffer we are writing
            renderPassInfo.framebuffer = frameBuffer->As<VulkanFrameBuffer>()->GetFrameBuffer();

            // the area shader loads and 
            renderPassInfo.renderArea.offset = { 0,0 };
            // for high displays swap chain extent could be higher than windows extent
            renderPassInfo.renderArea.extent.width = config.Size.X;
            renderPassInfo.renderArea.extent.height = config.Size.Y;

            renderPassInfo.clearValueCount = (uint32_t)clearValues.size();
            renderPassInfo.pClearValues = clearValues.data();

            vkCmdBeginRenderPass(command->As<VulkanRenderCommandBuffer>()->GetCommandBuffer(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        }


        if (config.ClearFrameBufferOnLoad == false)return;
        std::vector< VkClearAttachment> clears;
        std::vector< VkClearRect> reactClear;
        uint32_t iterate = 0;
        for (auto& attach : config.Attachments.Attachments)
        {
            VkClearAttachment clearAttach;
            if (attach.ClearOnLoad && Utils::IsColorFormat(attach.Format))
            {
                clearAttach.colorAttachment = iterate;
                clearAttach.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                clearAttach.clearValue = colorValue;
                iterate++;
                VkClearRect clearRect = {};
                clearRect.layerCount = 1;
                clearRect.rect.offset = { 0, 0 };
                clearRect.rect.extent.width = config.Size.X;
                clearRect.rect.extent.height = config.Size.Y;
                reactClear.emplace_back(clearRect);
            }
            // basically no need to check if it is depth
            // it would be a depth or stencil format 
            // if it not a color formats
            else if(config.ClearDepthOnLoad)
            {
                clearAttach.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
                clearAttach.clearValue.depthStencil = { config.DepthClearValue,config.StencilClearValue};

                VkClearRect clearRect = {};
                clearRect.layerCount = 1;
                clearRect.rect.offset = { 0, 0 };
                clearRect.rect.extent.width = config.Size.X;
                clearRect.rect.extent.height = config.Size.Y;
                reactClear.emplace_back(clearRect);
            }
    
            clears.emplace_back(clearAttach);
        }
        vkCmdClearAttachments(command->As<VulkanRenderCommandBuffer>()->GetCommandBuffer(),
            clears.size(),
            clears.data(),
            reactClear.size(),
            reactClear.data());
    }
    void VulkanRenderPass::RecordRenderPass(Count<class GraphicsPipeline>pipline, std::function<void(Count<RenderCommandBuffer> commandBuffer)> func) {
        auto vulkanPipeline =pipline->As<VulkanGraphicsPipeline>();
        vkCmdBindPipeline(m_CommandBuffer->As<VulkanRenderCommandBuffer>()->GetCommandBuffer(),
            VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanPipeline->GetPipline());

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)m_CurrentFrameBuffer->GetConfig().Size.X;
        viewport.height = (float)m_CurrentFrameBuffer->GetConfig().Size.Y;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(m_CommandBuffer->As<VulkanRenderCommandBuffer>()->GetCommandBuffer(), 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = { 0, 0 };
        scissor.extent = { (uint32_t)viewport.width,(uint32_t)viewport.height };
        vkCmdSetScissor(m_CommandBuffer->As<VulkanRenderCommandBuffer>()->GetCommandBuffer(), 0, 1, &scissor);
        if(vulkanPipeline->m_LineWidth !=1.0f)
            vkCmdSetLineWidth(m_CommandBuffer->As<VulkanRenderCommandBuffer>()->GetCommandBuffer(), vulkanPipeline->m_LineWidth);

        func(m_CommandBuffer);
    }
    void VulkanRenderPass::RecordRenderPass(Count<class GraphicsPipeline>pipline, VkViewport viewport, VkRect2D scissor, std::function<void(Count<RenderCommandBuffer> commandBuffer)> func) {
        auto vulkanPipeline = pipline->As<VulkanGraphicsPipeline>();
        vkCmdBindPipeline(m_CommandBuffer->As<VulkanRenderCommandBuffer>()->GetCommandBuffer(),
            VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanPipeline->GetPipline());

        
        vkCmdSetViewport(m_CommandBuffer->As<VulkanRenderCommandBuffer>()->GetCommandBuffer(), 0, 1, &viewport);

     
        vkCmdSetScissor(m_CommandBuffer->As<VulkanRenderCommandBuffer>()->GetCommandBuffer(), 0, 1, &scissor);
        if (vulkanPipeline->m_LineWidth != 1.0f)
            vkCmdSetLineWidth(m_CommandBuffer->As<VulkanRenderCommandBuffer>()->GetCommandBuffer(), vulkanPipeline->m_LineWidth);

        func(m_CommandBuffer);
    }

    void VulkanRenderPass::EndRenderPass() {
        PF_CORE_ASSERT(m_RenderPassEnabled == true, "cannot End render pass when render pass is not started");
        vkCmdEndRenderPass(m_CommandBuffer->As<VulkanRenderCommandBuffer>()->GetCommandBuffer());
        m_CommandBuffer = nullptr;
        m_RenderPassEnabled = false;
    }
}
