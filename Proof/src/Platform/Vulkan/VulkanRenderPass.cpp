#include "Proofprch.h"
#include "Proof/Core/Core.h"
#include "VulkanRenderPass.h"
#include "VulkanRenderer.h"
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
          // for comaptbile rener pass
        if (GetPipeline() != nullptr)
        {
            VulkanDescriptorManagerConfig descr;
            descr.DebugName = m_Config.DebugName + " Descriptor Manager";
            descr.Shader = GetPipeline()->GetShader().As<VulkanShader>();
            m_DescritptorSetManager = Count<VulkanDescriptorManager>::Create(descr);
        }

        SetTargetFrameBuffer(config.TargetFrameBuffer);

        //Build();
    }
   
  
    void VulkanRenderPass::Build()
    {
        #if 0
        //https://developer.samsung.com/galaxy-gamedev/resources/articles/renderpasses.html
        auto graphicsContext = VulkanRenderer::GetGraphicsContext();
        auto swapChain = graphicsContext->GetSwapChain().As<VulkanSwapChain>();

        for (auto& imageConfig :GetPipeline()->GetConfig().Attachments.Attachments)
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
            PF_CORE_ASSERT(false, fmt::format("{} {} does not image format", m_Config.DebugName, EnumReflection::EnumString(imageConfig.Format)).c_str());
        }



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

        if (m_ColorAttachments.size() > 0)
        {

            VkSubpassDescription subpass;

            subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

            VkSubpassDependency dependency = {};
            dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
            dependency.srcAccessMask = 0;
            dependency.dstSubpass = 0;


            dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
            dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;

            dependency.dstAccessMask |= VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            subpass.colorAttachmentCount = colorAttachmentRefs.size();
            subpass.pColorAttachments = colorAttachmentRefs.data();
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
            //const uint32_t viewMask = 0b11111111;
            //const uint32_t viewMask = 0b111111;
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
            renderPassMultiviewCI.dependencyCount = 0;
            if (GetPipeline()->GetConfig().Multiview)
            {
                renderPassInfo.pNext = &renderPassMultiviewCI;
            }
            if (vkCreateRenderPass(Renderer::GetGraphicsContext().As<VulkanGraphicsContext>()->GetDevice(), &renderPassInfo, nullptr, &m_RenderPass) != VK_SUCCESS)
            {
                PF_CORE_ASSERT(false, "failed to create render pass!");
            }
        }
        else if(m_DepthFormat != ImageFormat::None)
        {
            // for shadow maps tahts why if onoly depth buffer
            //https://github.com/SaschaWillems/Vulkan/blob/master/examples/shadowmappingcascade/shadowmappingcascade.cpp
            VkSubpassDescription subpass = {};
            subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
            subpass.colorAttachmentCount = 0;
            subpass.pDepthStencilAttachment = &m_DepthAttachment.AttachmentRef;
                // Use subpass dependencies for layout transitions
            std::array<VkSubpassDependency, 2> dependencies;

            dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
            dependencies[0].dstSubpass = 0;
            dependencies[0].srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            dependencies[0].dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
            dependencies[0].srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
            dependencies[0].dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
            dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

            dependencies[1].srcSubpass = 0;
            dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
            dependencies[1].srcStageMask = VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
            dependencies[1].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            dependencies[1].srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
            dependencies[1].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
            dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;


            VkRenderPassCreateInfo renderPassInfo = {};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
            renderPassInfo.attachmentCount = attachmentDescriptions.size();
            renderPassInfo.pAttachments = attachmentDescriptions.data();
            renderPassInfo.subpassCount = 1;
            renderPassInfo.pSubpasses = &subpass;
            renderPassInfo.dependencyCount = 2;
            renderPassInfo.pDependencies = dependencies.data();
            /*
                Bit mask that specifies which view rendering is broadcast to
                0011 = Broadcast to first and second view (layer)
            */

            /*

            */
            // default broadcast to all layers
            //https://anishbhobe.site/post/vulkan-render-to-cubemap-using-multiview/#3-attaching-the-cubemap-to-the-framebuffer
            //const uint32_t viewMask = 0b11111111;
            //const uint32_t viewMask = 0b111111;
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
            renderPassMultiviewCI.dependencyCount = 0;
            if (GetPipeline()->GetConfig().Multiview)
            {
                renderPassInfo.pNext = &renderPassMultiviewCI;
            }
            if (vkCreateRenderPass(Renderer::GetGraphicsContext().As<VulkanGraphicsContext>()->GetDevice(), &renderPassInfo, nullptr, &m_RenderPass) != VK_SUCCESS)
            {
                PF_CORE_ASSERT(false, "failed to create render pass!");
            }
        }
        #endif
        
      

        
    }
    void VulkanRenderPass::Release()
    {
        #if 0
        Renderer::SubmitDatafree([renderPass = m_RenderPass] {
            auto graphicsContext = VulkanRenderer::GetGraphicsContext();
        
            vkDestroyRenderPass(graphicsContext->GetDevice(), renderPass, nullptr);
        });
       m_RenderPass = nullptr;
       #endif
    }
   
    VulkanRenderPass::~VulkanRenderPass() 
    {
        Release();
    }

    VkRenderPass VulkanRenderPass::GetRenderPass()
    {
        return GetTargetFrameBuffer().As<VulkanFrameBuffer>()->GetRenderPass();
    }
    

    void VulkanRenderPass::SetDepthAttachment(const GraphicsPipelineImageConfig& config)
    {
        #if  0
        auto graphicsContext = VulkanRenderer::GetGraphicsContext();

        VkAttachmentDescription depthAttachment{};
        depthAttachment.format = Utils::ProofFormatToVulkanFormat(config.Format);
        depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;

        VkAttachmentReference depthAttachmentRef{};
        depthAttachmentRef.attachment = m_ColorAttachments.size();
        depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        m_DepthAttachment = { depthAttachment,depthAttachmentRef };

        m_DepthFormat = config.Format;
        #endif
    }
    void VulkanRenderPass::AddColorAttachment(const GraphicsPipelineImageConfig& config)
    {
        #if  0

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
            attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
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
        #endif

    }
    void VulkanRenderPass::BeginRenderPassBase(Count<class RenderCommandBuffer>command, Viewport vieport, ViewportScissor scisscor)
    {
        Count<VulkanRenderPass> instance = this;
        Renderer::Submit([instance, command, vieport, scisscor]()
        {
            PF_CORE_ASSERT(instance->m_RenderPassEnabled == false, fmt::format("cannot start {} render pass when previous render pass is not closed", instance->m_Config.DebugName).c_str());
            instance->m_CommandBuffer = command;
            instance->m_RenderPassEnabled = true;
            const FrameBufferConfig config = instance->GetTargetFrameBuffer()->GetConfig();
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
                if (instance->GetTargetFrameBuffer()->HasDepthImage())
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
                renderPassInfo.renderPass = instance->GetRenderPass();
                // teh frameBuffer we are writing
                renderPassInfo.framebuffer = instance->GetTargetFrameBuffer().As<VulkanFrameBuffer>()->GetFrameBuffer();

                // the area shader loads and 
                // for high displays swap chain extent could be higher than windows extent
                renderPassInfo.renderArea = vk_scissor;
                renderPassInfo.clearValueCount = (uint32_t)clearValues.size();
                renderPassInfo.pClearValues = clearValues.data();
                vkCmdBeginRenderPass(command.As<VulkanRenderCommandBuffer>()->GetActiveCommandBuffer(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
            }
        });
        
    }
    Count<Image> VulkanRenderPass::GetOutput(uint32_t imageIndex)
    {
        return GetTargetFrameBuffer()->GetOutput(imageIndex);
    }
    void VulkanRenderPass::BeginRenderMaterialRenderPass(Count<class RenderCommandBuffer> command, Viewport vieport, ViewportScissor scisscor, bool explicitClear)
    {
        BeginRenderPassBase(command, vieport, scisscor);

        Count<VulkanRenderPass> instance = this;
        Renderer::Submit([instance, command, vieport, scisscor,explicitClear]()
        {
            PF_CORE_ASSERT(instance->m_MaterialRenderPass == false, "cannot start material render pass if previous material render pass not disabled");

            instance->m_MaterialRenderPass = true;
            const auto vulkanPipeline = instance->GetPipeline().As<VulkanGraphicsPipeline>();
            vulkanPipeline->RT_Bind(instance->m_CommandBuffer);
            //Count<VulkanRenderPass> pass = this;
            instance->m_DescritptorSetManager->RT_Bind();

            auto& frameSet = instance->m_DescritptorSetManager->GetDescriptorSets()[Renderer::RT_GetCurrentFrameInFlight()];
            for (auto& [set, setInfo] : frameSet)
            {
                // set0 is for te material to bind to 
                // basically we have to define a set layout for each descriptor set 0-3
                // but some set may not have data and we do note creata a descriptor set for it
                // so we basically just seeing if thats teh case we dont bind it
                if (set == 0 || setInfo.Set == nullptr)continue;
                vkCmdBindDescriptorSets(
                    instance->m_CommandBuffer.As<VulkanRenderCommandBuffer>()->GetActiveCommandBuffer(),
                    VK_PIPELINE_BIND_POINT_GRAPHICS,
                    instance->GetPipeline().As<VulkanGraphicsPipeline>()->GetPipelineLayout(),
                    (int)set,
                    1,
                    &setInfo.Set,
                    0,
                    nullptr);
            }
            instance->SetDynamicStates(vieport, scisscor, explicitClear);
        });
    }
    void VulkanRenderPass::BeginRenderMaterialRenderPass(Count<class RenderCommandBuffer> command, bool explicitClear )
    {
        Viewport viewport;
        ViewportScissor scissor;
        viewport.X = 0.0f;
        viewport.Y = 0.0f;
        viewport.Width = (float)GetTargetFrameBuffer()->GetConfig().Width;
        viewport.Height = (float)GetTargetFrameBuffer()->GetConfig().Height;
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;

        scissor.Offset = { 0, 0 };
        scissor.Extent = { viewport.Width,viewport.Height };

        BeginRenderMaterialRenderPass(command, viewport, scissor, explicitClear);

    }
  void VulkanRenderPass::BeginRenderPass(Count<class RenderCommandBuffer> command,Viewport vieport, ViewportScissor scisscor, bool explicitClear)
    {
        BeginRenderPassBase(command, vieport, scisscor);

        Count<VulkanRenderPass> instance = this;
        Renderer::Submit([instance, command, vieport, scisscor, explicitClear]()
        {

            auto vulkanPipeline = instance->GetPipeline().As<VulkanGraphicsPipeline>();
            vulkanPipeline->RT_Bind(instance->m_CommandBuffer);
            instance->m_DescritptorSetManager->RT_Bind();

            auto& frameSet = instance->m_DescritptorSetManager->GetDescriptorSets()[Renderer::RT_GetCurrentFrameInFlight()];
            for (auto& [set, setInfo] : frameSet)
            {
                // set0 is for te material to bind to 
                // basically we have to define a set layout for each descriptor set 0-3
                // but some set may not have data and we do note creata a descriptor set for it
                // so we basically just seeing if thats teh case we dont bind it
                if (setInfo.Set == nullptr)continue;
                vkCmdBindDescriptorSets(
                    instance->m_CommandBuffer.As<VulkanRenderCommandBuffer>()->GetActiveCommandBuffer(),
                    VK_PIPELINE_BIND_POINT_GRAPHICS,
                    instance->GetPipeline().As<VulkanGraphicsPipeline>()->GetPipelineLayout(),
                    (int)set,
                    1,
                    &setInfo.Set,
                    0,
                    nullptr);
            }
            instance->SetDynamicStates(vieport, scisscor, explicitClear);
        });

    }
    void VulkanRenderPass::BeginRenderPass(Count<class RenderCommandBuffer> command, bool explicitClear)
    {   
       
        Viewport viewport;
        ViewportScissor scissor;
        viewport.X = 0.0f;
        viewport.Y = 0.0f;
        viewport.Width = (float)GetTargetFrameBuffer()->GetConfig().Width;
        viewport.Height = (float)GetTargetFrameBuffer()->GetConfig().Height;
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;


        scissor.Offset = { 0, 0 };
        scissor.Extent = { viewport.Width,viewport.Height };

        BeginRenderPass(command, viewport, scissor, explicitClear);
    }
    void VulkanRenderPass::RenderPassPushRenderMaterial(Count<class RenderMaterial> renderMaterial)
    {
        Count<VulkanRenderPass> instance = this;
        Renderer::Submit([instance, renderMaterial]()
        {
            PF_CORE_ASSERT(instance->m_RenderPassEnabled == true, "cannot Push material fi render pass not enabled");
            PF_CORE_ASSERT(instance->m_MaterialRenderPass == true, "cannot Push if not a material Render Pass");

            renderMaterial.As<VulkanRenderMaterial>()->RT_Bind(instance->m_CommandBuffer.As<VulkanRenderCommandBuffer>(), instance);
        });
    }
    
    void VulkanRenderPass::EndRenderPass() 
    {
        Count<VulkanRenderPass> instance = this;
        Renderer::Submit([instance]()
        {
            PF_CORE_ASSERT(instance->m_RenderPassEnabled == true, "cannot End render pass when render pass is not started");
            vkCmdEndRenderPass(instance->m_CommandBuffer.As<VulkanRenderCommandBuffer>()->GetActiveCommandBuffer());
            instance->m_CommandBuffer = nullptr;
            instance->m_RenderPassEnabled = false;
            instance->m_MaterialRenderPass = false;
        });
    }
    void VulkanRenderPass::SetDynamicStates(Viewport vieport, ViewportScissor scisscor, bool explicitClear)
    {
        PF_PROFILE_FUNC();
        PF_CORE_ASSERT(m_RenderPassEnabled == true, "cannot Set dynamic staes if render pass not started");

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

        if (explicitClear)
        {
            std::vector< VkClearAttachment> clears;
            std::vector< VkClearRect> reactClear;
            uint32_t iterate = 0;

            for (auto& attach : frameBufferConfig.Attachments.Attachments)
            {

                VkClearAttachment clearAttach;

                if (Utils::IsColorFormat(attach.Format))
                {
                    clearAttach.colorAttachment = iterate;
                    clearAttach.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                    clearAttach.clearValue = colorValue;

                    clears.push_back(clearAttach);

                    VkClearRect clearRect = {};
                    clearRect.layerCount = 1;
                    clearRect.rect = vk_scissor;
                    reactClear.push_back(clearRect);
                }
                else if (Utils::IsDepthFormat(attach.Format))
                {
                    VkClearAttachment clearAttach;
                //config.Attachments.Attachments[depthIdex];
                    clearAttach.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;


                    if (Utils::ContainStencilFormat(GetTargetFrameBuffer().As<VulkanFrameBuffer>()->GetDepthFormat()))
                        clearAttach.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;

                    clearAttach.clearValue.depthStencil = { frameBufferConfig.DepthClearValue,frameBufferConfig.StencilClearValue };
                   // clearAttach.clearValue = colorValue;
                    clears.push_back(clearAttach);

                    VkClearRect clearRect = {};
                    clearRect.layerCount = 1;
                    clearRect.rect = vk_scissor;
                    reactClear.push_back(clearRect);
                }
                iterate++;
            }
           
            vkCmdClearAttachments(m_CommandBuffer.As<VulkanRenderCommandBuffer>()->GetActiveCommandBuffer(),
                clears.size(),
                clears.data(),
                reactClear.size(),
                reactClear.data());
        }


        VkCommandBuffer cmdBuffer = m_CommandBuffer.As<VulkanRenderCommandBuffer>()->GetActiveCommandBuffer();
        vkCmdSetViewport(cmdBuffer, 0, 1, &vk_viewport);
        vkCmdSetScissor(cmdBuffer, 0, 1, &vk_scissor);

        auto vk_pipeline = GetPipeline().As<VulkanGraphicsPipeline>();

        if (vk_pipeline->GetConfig().EditLineWidth)
        {
            float lineWidth;
            if (vk_pipeline->GetLineWidthStack().size())
                lineWidth = vk_pipeline->GetLineWidthStack().back();
            else
                lineWidth = vk_pipeline->GetConfig().LineWidth;

            PF_CORE_ASSERT(false, "not found ");

            //vkCmdSetLineWidth(cmdBuffer, lineWidth);
        }

        if (vk_pipeline->GetConfig().EditDrawType)
        {
            DrawType drawType;
            if (vk_pipeline->GetDrawTypeStack().size())
                drawType = vk_pipeline->GetDrawTypeStack().back();
            else
                drawType = vk_pipeline->GetConfig().DrawMode;

            PF_CORE_ASSERT(false, "not found ");

            //vkCmdSetPrimitiveTopology(cmdBuffer, Utils::ProofTopologyToVulkanTopology(drawType));
        }

        if (vk_pipeline->GetConfig().EditCullMode)
        {
            CullMode mode;
            if (vk_pipeline->GetCullModeStack().size())
                mode = vk_pipeline->GetCullModeStack().back();
            else
                mode = vk_pipeline->GetConfig().CullMode;

            PF_CORE_ASSERT(false, "cull mode not found ");
            //vkCmdSetCullMode(cmdBuffer, Utils::ProofFormatToVulkanFormat(mode));
        }
    }
    void VulkanRenderPass::SetTargetFrameBuffer(Count<FrameBuffer> frame)
    {
        PF_CORE_ASSERT(m_Config.Pipeline->GetConfig().Attachments.GetAttachmentsImages() == frame->GetConfig().Attachments.GetAttachmentsImages(),
            fmt::format("{} renderPass and {} pipeline does not match {} framebuffer imageLayout", m_Config.DebugName, m_Config.Pipeline->GetConfig().DebugName, frame->GetConfig().DebugName).c_str());
        m_Config.TargetFrameBuffer = frame;
    }
    Count<class FrameBuffer> VulkanRenderPass::GetTargetFrameBuffer()
    {
        return m_Config.TargetFrameBuffer;
    }
    void VulkanRenderPass::SetInput(std::string_view name, Count<class StorageBuffer> buffer)
    {
        m_DescritptorSetManager->SetInput(name, buffer);
    }
    void VulkanRenderPass::SetInput(std::string_view name, Count<class ImageView> imageView)
    {
        m_DescritptorSetManager->SetInput(name, imageView);
    }
    void VulkanRenderPass::SetInput(std::string_view name, const std::vector< Count<class ImageView>>& imageViews)
    {
        m_DescritptorSetManager->SetInput(name, imageViews);
    }
    void VulkanRenderPass::SetInput(std::string_view name, Count<class Image2D>image)
    {
        m_DescritptorSetManager->SetInput(name, image);
    }
    void VulkanRenderPass::SetInput(std::string_view name, const std::vector< Count<class Image2D>>& images)
    {
        m_DescritptorSetManager->SetInput(name, images);
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
    void VulkanRenderPass::SetInput(std::string_view name, Count<class TextureCube> imageCube)
    {
        m_DescritptorSetManager->SetInput(name, imageCube);
    }
    void VulkanRenderPass::SetInput(std::string_view name, Count<class UniformBufferSet> buffer)
    {
        m_DescritptorSetManager->SetInput(name, buffer);
    }
    void VulkanRenderPass::SetInput(std::string_view name, Count<class StorageBufferSet> buffer)
    {
        m_DescritptorSetManager->SetInput(name, buffer);
    }
    void VulkanRenderPass::AddGlobalInput(Count<class GlobalBufferSet> globalInputs)
    {
        m_DescritptorSetManager->SetGoalballInputs(globalInputs);
    }
    void VulkanRenderPass::RT_PushData(std::string_view name, const void* data)
    {
        PF_CORE_ASSERT(m_RenderPassEnabled == true, "cannot push render pass if not render pass started");
     // PF_CORE_ASSERT(m_MaterialRenderPass == false, "cannot push Data render pass if material render pass");

        auto vkShader = GetPipeline()->GetShader().As<VulkanShader>();
        std::string str = std::string(name);
        PF_CORE_ASSERT(vkShader->GetPushConstants().contains(str));
        const auto& pushRange = vkShader->GetPushConstants().at(str);
        vkCmdPushConstants(m_CommandBuffer.As<VulkanRenderCommandBuffer>()->GetActiveCommandBuffer(), GetPipeline().As<VulkanGraphicsPipeline>()->GetPipelineLayout(),
            pushRange.stageFlags, pushRange.offset, pushRange.size, data);
    }
    void VulkanRenderPass::PushData(std::string_view name, const void* data)
    {
        std::string str = std::string(name);
        auto vkShader = m_Config.Pipeline->GetShader().As<VulkanShader>();
        PF_CORE_ASSERT(vkShader->GetPushConstants().contains(str));
        const auto& pushRange = vkShader->GetPushConstants().at(str);

        m_LocalStorage.Copy(data, pushRange.size);
        Count<VulkanRenderPass> instance = this;
        Renderer::Submit([instance, str]()
        {
            instance->RT_PushData(str, instance->m_LocalStorage.Data);
            instance->m_LocalStorage.Release();
        });
    }
}
