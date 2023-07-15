#include "Proofprch.h"
#include "Proof/Core/Core.h"
#include "VulkanRenderPass.h"
#include "VulkanRenderer/VulkanRenderer.h"
#include "VulkanFrameBuffer.h"
#include "VulkanFrameBuffer.h"
#include "VulkanUtils/VulkanConvert.h"
#include "VulkanCommandBuffer.h"
#include "VulkanSwapChain.h"
#include "VulkanGraphicsPipeline.h"
#include "VulkanShader.h"
#include "VulkanGraphicsContext.h"
#include "VulkanRenderMaterial.h"
#include <iostream>
#include <string_view>
#include <string>
namespace Proof
{
    //https://developer.samsung.com/galaxy-gamedev/resources/articles/renderpasses.html
    /*
 * pCreateInfo
 * sType: VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO
 * pNext: NULL
 * flags: 0
 * attachmentCount: 2
 * pAttachments:
 *   pAttachments[0]
 *     flags: 0
 *     format: VK_IMAGE_FORMAT_B8G8R8A8_UNORM
 *     samples: 1
 *     loadOp: VK_ATTACHMENT_LOAD_OP_DONT_CARE
 *     storeOp: VK_ATTACHMENT_STORE_OP_STORE
 *     stencilLoadOp: VK_ATTACHMENT_LOAD_OP_DONT_CARE
 *     stencilStoreOp: VK_ATTACHMENT_STORE_OP_DONT_CARE
 *     initialLayout: VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
 *     finalLayout: VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
 *   pAttachments[1]
 *     flags: 0
 *     format: VK_IMAGE_FORMAT_D16_UNORM
 *     samples: 1
 *     loadOp: VK_ATTACHMENT_LOAD_OP_CLEAR
 *     storeOp: VK_ATTACHMENT_STORE_OP_DONT_CARE
 *     stencilLoadOp: VK_ATTACHMENT_LOAD_OP_DONT_CARE
 *     stencilStoreOp: VK_ATTACHMENT_STORE_OP_DONT_CARE
 *     initialLayout: VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
 *     finalLayout: VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
 * subpassCount: 1
 * pSubpasses:
 *   pSubpasses[0]
 *     flags: 0
 *     pipelineBindPoint: VK_PIPELINE_BIND_POINT_GRAPHICS
 *     inputAttachmentCount: 0
 *     pInputAttachments: NULL
 *     colorAttachmentCount: 1
 *     pColorAttachments:
 *       pColorAttachments[0]
 *         attachment: 0
 *         layout: VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
 *     pResolveAttachments: NULL
 *     pDepthStencilAttachment:
 *       *pDepthStencilAttachment
 *         attachment: 1
 *         layout: VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
 *     preserveAttachmentCount: 0
 *     pPreserveAttachments: NULL
 * dependencyCount: 0
 * pDependencies: NULL
 */

    VulkanRenderPass::VulkanRenderPass(const RenderPassConfig& config)
        :
        m_Config(config)
    {
        //PF_CORE_ASSERT(m_Config.TargetBuffer == nullptr, "Render pass needs a frameBuffer");
        Init();
        CreateRenderPass();
    }
   
    void VulkanRenderPass::Init()
    {
        //https://developer.samsung.com/galaxy-gamedev/resources/articles/renderpasses.html
        auto graphicsContext = VulkanRenderer::GetGraphicsContext();
        auto swapChain = graphicsContext->GetSwapChain().As<VulkanSwapChain>();

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
        subpass.preserveAttachmentCount = 0;
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
        if (vkCreateRenderPass(VulkanRenderer::GetGraphicsContext()->GetDevice(), &renderPassInfo, nullptr, &m_RenderPass) != VK_SUCCESS)
        {
            PF_CORE_ASSERT(false, "failed to create render pass!");
        }

        {
            // for comaptbile rener pass
            if (GetPipeline() != nullptr)
            {
                VulkanDescriptorManagerConfig descr;
                descr.DebugName = m_Config.DebugName + " Descriptor Manager";
                descr.Shader = GetPipeline()->GetShader().As<VulkanShader>();
                m_DescritptorSetManager = Count<VulkanDescriptorManager>::Create(descr);
            }

        }
    }
    void VulkanRenderPass::Release()
    {
        m_DepthAttachment = {};
        m_ColorAttachments = {};
        m_DepthFormat = ImageFormat::None;

        Renderer::SubmitDatafree([renderPass = m_RenderPass] {
            auto graphicsContext = VulkanRenderer::GetGraphicsContext();

            vkDestroyRenderPass(graphicsContext->GetDevice(), renderPass, nullptr);
        });

        m_RenderPass = nullptr;
    }
    VulkanRenderPass::~VulkanRenderPass() {
        Release();
    }
    

    void VulkanRenderPass::SetDepthAttachment(const RenderPassImageConfig& config)
    {
        auto graphicsContext = VulkanRenderer::GetGraphicsContext();

        VkAttachmentDescription depthAttachment{};
        depthAttachment.format = Utils::ProofFormatToVulkanFormat(config.Format);
        depthAttachment.samples = graphicsContext->GetSampleCount();
        depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE; // we want to control these ourself
        depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
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
            attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            attachment.initialLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            attachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

            attachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        }

        m_ColorAttachments.emplace_back(VulkanRenderPassAttach{ attachment,attachmentRef });
    }
    void VulkanRenderPass::BeginRenderPassBase(Count<class RenderCommandBuffer>command, Viewport vieport, ViewportScissor scisscor)
    {
        PF_CORE_ASSERT(m_RenderPassEnabled == false, fmt::format("cannot start {} render pass when previous render pass is not closed", m_Config.DebugName).c_str());
        m_CommandBuffer = command;
        m_RenderPassEnabled = true;
        const FrameBufferConfig config = GetTargetFrameBuffer()->GetConfig();
        VkClearValue colorValue{ config.ClearColor.X, config.ClearColor.Y, config.ClearColor.Z, config.ClearColor.W };
        std::vector< VkClearValue> clearValues;
        VkViewport vk_viewport;
        VkRect2D vk_scissor;
        vk_viewport.x = vieport.X;
        vk_viewport.y = vieport.Y;
        vk_viewport.width = (float)vieport.Width;
        vk_viewport.height = (float)vieport.Height;
        vk_viewport.minDepth = vieport.MinDepth;
        vk_viewport.maxDepth = vieport.MaxDepth;

        vk_scissor.offset = { (int)scisscor.Offset.X, (int)scisscor.Offset.Y };
        vk_scissor.extent = { (uint32_t)scisscor.Extent.X,(uint32_t)scisscor.Extent.Y };
        // setting frameBuffer clear values
        {
            clearValues.resize(config.Attachments.Attachments.size());
            std::fill(clearValues.begin(), clearValues.end(), VkClearValue{ colorValue.color });//https://stackoverflow.com/questions/8848575/fastest-way-to-reset-every-value-of-stdvectorint-to-0
            // depth image always he lst one bind
            if (GetTargetFrameBuffer()->HasDepthImage())
            {
                auto& val = clearValues.back();
                val = {};
                val.depthStencil = { config.DepthClearValue,config.StencilClearValue };
            }
        }
        // starting the render pass
        {
            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = m_RenderPass;
            // teh frameBuffer we are writing
            renderPassInfo.framebuffer = GetTargetFrameBuffer().As<VulkanFrameBuffer>()->GetFrameBuffer();

            // the area shader loads and 
            // for high displays swap chain extent could be higher than windows extent
            renderPassInfo.renderArea = vk_scissor;
            renderPassInfo.clearValueCount = (uint32_t)clearValues.size();
            renderPassInfo.pClearValues = clearValues.data();
//vkCmdSetViewport(m_CommandBuffer.As<VulkanRenderCommandBuffer>()->GetCommandBuffer(), 0, 1, &vk_viewport);

           // vkCmdSetScissor(m_CommandBuffer.As<VulkanRenderCommandBuffer>()->GetCommandBuffer(), 0, 1, &vk_scissor);
            vkCmdBeginRenderPass(command.As<VulkanRenderCommandBuffer>()->GetCommandBuffer(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        }
    }
    
    void VulkanRenderPass::BeginRenderPass(Count<class RenderCommandBuffer> command, Count<class RenderMaterial>material, Viewport vieport, ViewportScissor scisscor, bool explicitClear)
    {
        BeginRenderPassBase(command, vieport, scisscor);
        const auto vulkanPipeline = GetPipeline().As<VulkanGraphicsPipeline>();
        vulkanPipeline->Bind( m_CommandBuffer);
        material.As<VulkanRenderMaterial>()->Bind(command.As<VulkanRenderCommandBuffer>(),this);
        m_DescritptorSetManager->Bind();

        auto& frameSet = m_DescritptorSetManager->GetDescriptorSets()[Renderer::GetCurrentFrame().FrameinFlight];
        for (auto& [set, setInfo] : frameSet)
        {
            // set0 is for te material to bind to 
            // basically we have to define a set layout for each descriptor set 0-3
            // but some set may not have data and we do note creata a descriptor set for it
            // so we basically just seeing if thats teh case we dont bind it
            if (set == 0 || setInfo.Set == nullptr)continue;
            vkCmdBindDescriptorSets(
                m_CommandBuffer.As<VulkanRenderCommandBuffer>()->GetCommandBuffer(),
                VK_PIPELINE_BIND_POINT_GRAPHICS,
                GetPipeline().As<VulkanGraphicsPipeline>()->GetPipelineLayout(),
                (int)set,
                1,
                &setInfo.Set,
                0,
                nullptr);
        }

        VkViewport vk_viewport;
        VkRect2D vk_scissor;
        vk_viewport.x = vieport.X;
        vk_viewport.y = vieport.Y;
        vk_viewport.width = (float)vieport.Width;
        vk_viewport.height = (float)vieport.Height;
        vk_viewport.minDepth = vieport.MinDepth;
        vk_viewport.maxDepth = vieport.MaxDepth;

        vk_scissor.offset = { (int)scisscor.Offset.X, (int)scisscor.Offset.Y };
        vk_scissor.extent = { (uint32_t)scisscor.Extent.X,(uint32_t)scisscor.Extent.Y };
        const FrameBufferConfig frameBufferConfig = GetTargetFrameBuffer()->GetConfig();
        VkClearValue colorValue{ frameBufferConfig.ClearColor.X, frameBufferConfig.ClearColor.Y, frameBufferConfig.ClearColor.Z, frameBufferConfig.ClearColor.W };

        vkCmdSetViewport(m_CommandBuffer.As<VulkanRenderCommandBuffer>()->GetCommandBuffer(), 0, 1, &vk_viewport);
        vkCmdSetScissor(m_CommandBuffer.As<VulkanRenderCommandBuffer>()->GetCommandBuffer(), 0, 1, &vk_scissor);
        if (frameBufferConfig.ClearFrameBufferOnLoad || explicitClear)
        {
            std::vector< VkClearAttachment> clears;
            std::vector< VkClearRect> reactClear;
            reactClear.resize(frameBufferConfig.Attachments.Attachments.size());
            clears.resize(frameBufferConfig.Attachments.Attachments.size());
            uint32_t iterate = 0;

            // has depth, in framebuffer creat depth buffer is always the last attachment
            int depthIndex = -1;
            for (auto& attach : frameBufferConfig.Attachments.Attachments)
            {
                VkClearAttachment clearAttach;
                if ((attach.ClearOnLoad || explicitClear) && Utils::IsColorFormat(attach.Format))
                {
                    clearAttach.colorAttachment = iterate;
                    clearAttach.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                    clearAttach.clearValue = colorValue;
                    VkClearRect clearRect = {};
                    clearRect.layerCount = 1;
                    clearRect.rect = vk_scissor;
                    reactClear[iterate] = clearRect;
                    clears[iterate] = clearAttach;

                }
                else if ((frameBufferConfig.ClearDepthOnLoad || explicitClear) && GetTargetFrameBuffer()->HasDepthImage())
                {
                    // basically no need to check if it is depth
                    // it would be a depth or stencil format 
                    // if it not a color formats
                    depthIndex = iterate;
                }
                iterate++;

            }
            if (depthIndex != -1)
            {
                VkClearAttachment clearAttach;
                //config.Attachments.Attachments[depthIdex];
                clearAttach.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
                if (Utils::ContainStencilFormat(m_DepthFormat))
                    clearAttach.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
                clearAttach.clearValue.depthStencil = { frameBufferConfig.DepthClearValue,frameBufferConfig.StencilClearValue };
               // clearAttach.clearValue = colorValue;
                clears[clears.size() - 1] = clearAttach;

                VkClearRect clearRect = {};
                clearRect.layerCount = 1;
                clearRect.rect = vk_scissor;
                reactClear[reactClear.size() - 1] = clearRect;
            }
            vkCmdClearAttachments(command.As<VulkanRenderCommandBuffer>()->GetCommandBuffer(),
                clears.size(),
                clears.data(),
                reactClear.size(),
                reactClear.data());
        }
        /*
        const VkClearAttachment clearAttachements[2] = {
              {
                VK_IMAGE_ASPECT_COLOR_BIT,
                0,
                { 0.0f,0.0f,0.0f,1.0f },
              },
              {
                VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT,
                0,
                { 1.0f,0},
              }
        };

        const VkClearRect rects[1] = {
          {
            {
              {0,0},
              {(uint32_t)vieport.Width,(uint32_t)vieport.Height},
            },
            0,1
          }
        };

        vkCmdClearAttachments(m_CommandBuffer.As<VulkanRenderCommandBuffer>()->GetCommandBuffer(),
            2, // attachmentCount
            clearAttachements,
            1, // rectCount
            rects);
            */
    }

    void VulkanRenderPass::BeginRenderPass(Count<class RenderCommandBuffer> command, Count<class RenderMaterial> material, bool explicitClear )
    {
        Viewport viewport;
        ViewportScissor scissor;
        viewport.X = 0.0f;
        viewport.Y = 0.0f;
        viewport.Width = (float)GetTargetFrameBuffer()->GetConfig().Size.X;
        viewport.Height = (float)GetTargetFrameBuffer()->GetConfig().Size.Y;
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;

        scissor.Offset = { 0, 0 };
        scissor.Extent = { viewport.Width,viewport.Height };

        BeginRenderPass(command, material,viewport, scissor, explicitClear);

    }
  void VulkanRenderPass::BeginRenderPass(Count<class RenderCommandBuffer> command,Viewport vieport, ViewportScissor scisscor, bool explicitClear)
    {
        BeginRenderPassBase(command, vieport, scisscor);

        auto vulkanPipeline = GetPipeline().As<VulkanGraphicsPipeline>();
        vulkanPipeline->Bind(m_CommandBuffer);
        m_DescritptorSetManager->Bind();

        auto& frameSet = m_DescritptorSetManager->GetDescriptorSets()[Renderer::GetCurrentFrame().FrameinFlight];
        for (auto& [set, setInfo] : frameSet)
        {
            // set0 is for te material to bind to 
            // basically we have to define a set layout for each descriptor set 0-3
            // but some set may not have data and we do note creata a descriptor set for it
            // so we basically just seeing if thats teh case we dont bind it
            if (setInfo.Set == nullptr)continue;
            vkCmdBindDescriptorSets(
                m_CommandBuffer.As<VulkanRenderCommandBuffer>()->GetCommandBuffer(),
                VK_PIPELINE_BIND_POINT_GRAPHICS,
                GetPipeline().As<VulkanGraphicsPipeline>()->GetPipelineLayout(),
                (int)set,
                1,
                &setInfo.Set,
                0,
                nullptr);
        }
        VkViewport vk_viewport;
        VkRect2D vk_scissor;
        vk_viewport.x = vieport.X;
        vk_viewport.y = vieport.Y;
        vk_viewport.width = (float)vieport.Width;
        vk_viewport.height = (float)vieport.Height;
        vk_viewport.minDepth = vieport.MinDepth;
        vk_viewport.maxDepth = vieport.MaxDepth;

        vk_scissor.offset = { (int)scisscor.Offset.X, (int)scisscor.Offset.Y };
        vk_scissor.extent = { (uint32_t)scisscor.Extent.X,(uint32_t)scisscor.Extent.Y };
        const FrameBufferConfig frameBufferConfig = GetTargetFrameBuffer()->GetConfig();
        VkClearValue colorValue{ frameBufferConfig.ClearColor.X, frameBufferConfig.ClearColor.Y, frameBufferConfig.ClearColor.Z, frameBufferConfig.ClearColor.W };

        vkCmdSetViewport(m_CommandBuffer.As<VulkanRenderCommandBuffer>()->GetCommandBuffer(), 0, 1, &vk_viewport);
        vkCmdSetScissor(m_CommandBuffer.As<VulkanRenderCommandBuffer>()->GetCommandBuffer(), 0, 1, &vk_scissor);
        if (frameBufferConfig.ClearFrameBufferOnLoad || explicitClear)
        {
            std::vector< VkClearAttachment> clears;
            std::vector< VkClearRect> reactClear;
            reactClear.resize(frameBufferConfig.Attachments.Attachments.size());
            clears.resize(frameBufferConfig.Attachments.Attachments.size());
            uint32_t iterate = 0;

            // has depth, in framebuffer creat depth buffer is always the last attachment
            int depthIndex = -1;
            for (auto& attach : frameBufferConfig.Attachments.Attachments)
            {
                VkClearAttachment clearAttach;
                if ( (attach.ClearOnLoad || explicitClear )&& Utils::IsColorFormat(attach.Format))
                {
                    clearAttach.colorAttachment = iterate;
                    clearAttach.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                    clearAttach.clearValue = colorValue;
                    VkClearRect clearRect = {};
                    clearRect.layerCount = 1;
                    clearRect.rect = vk_scissor;
                    reactClear[iterate] = clearRect;
                    clears[iterate] = clearAttach;

                }
                else if ((frameBufferConfig.ClearDepthOnLoad || explicitClear) && GetTargetFrameBuffer()->HasDepthImage())
                {
                    // basically no need to check if it is depth
                    // it would be a depth or stencil format 
                    // if it not a color formats
                    depthIndex = iterate;
                }
                iterate++;

            }
            if (depthIndex != -1)
            {
                VkClearAttachment clearAttach;
                     //config.Attachments.Attachments[depthIdex];
                clearAttach.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
                if (Utils::ContainStencilFormat(m_DepthFormat))
                    clearAttach.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
                clearAttach.clearValue.depthStencil = { frameBufferConfig.DepthClearValue,frameBufferConfig.StencilClearValue };
               // clearAttach.clearValue = colorValue;
                clears[clears.size() - 1] = clearAttach;

                VkClearRect clearRect = {};
                clearRect.layerCount = 1;
                clearRect.rect = vk_scissor;
                reactClear[reactClear.size() - 1] = clearRect;
            }
            vkCmdClearAttachments(command.As<VulkanRenderCommandBuffer>()->GetCommandBuffer(),
                clears.size(),
                clears.data(),
                reactClear.size(),
                reactClear.data());
        }
  

    }
    void VulkanRenderPass::BeginRenderPass(Count<class RenderCommandBuffer> command, bool explicitClear)
    {
        Viewport viewport;
        ViewportScissor scissor;
        viewport.X = 0.0f;
        viewport.Y = 0.0f;
        viewport.Width = (float)GetTargetFrameBuffer()->GetConfig().Size.X ;
        viewport.Height = (float)GetTargetFrameBuffer()->GetConfig().Size.Y ;
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;


        scissor.Offset = { 0, 0 };
        scissor.Extent = { viewport.Width,viewport.Height };

        BeginRenderPass(command, viewport, scissor,explicitClear);
    }
    
    void VulkanRenderPass::EndRenderPass() {
        PF_CORE_ASSERT(m_RenderPassEnabled == true, "cannot End render pass when render pass is not started");
        vkCmdEndRenderPass(m_CommandBuffer.As<VulkanRenderCommandBuffer>()->GetCommandBuffer());
        m_CommandBuffer = nullptr;
        m_RenderPassEnabled = false;
    }

    void VulkanRenderPass::SetInput(std::string_view name, Count<class StorageBuffer> buffer)
    {
        m_DescritptorSetManager->SetInput(name, buffer);
    }
    void VulkanRenderPass::PushData(std::string_view name, const void* data)
    {
        auto vkShader = GetPipeline()->GetShader().As<VulkanShader>();
        std::string str = std::string(name);
        PF_CORE_ASSERT(vkShader->GetPushConstants().contains(str));
        const auto& pushRange = vkShader->GetPushConstants().at(str);
        vkCmdPushConstants(m_CommandBuffer.As<VulkanRenderCommandBuffer>()->GetCommandBuffer(), GetPipeline().As<VulkanGraphicsPipeline>()->GetPipelineLayout(),
            pushRange.stageFlags, pushRange.offset, pushRange.size, data);
    }
    Count<class FrameBuffer> VulkanRenderPass::GetTargetFrameBuffer()
    {
        return m_Config.Pipeline->GetTargetBuffer();
    }
    void VulkanRenderPass::SetInput(std::string_view name, Count<class UniformBuffer> buffer)
    {
        m_DescritptorSetManager->SetInput(name, buffer);
    }
    void VulkanRenderPass::SetInput(std::string_view name, Count<class Texture2D> image)
    {
        m_DescritptorSetManager->SetInput(name, image);
    }
    void VulkanRenderPass::SetInput(std::string_view name, const std::vector<Count<class Texture2D>>& images)
    {
        m_DescritptorSetManager->SetInput(name, images);
    }
}
