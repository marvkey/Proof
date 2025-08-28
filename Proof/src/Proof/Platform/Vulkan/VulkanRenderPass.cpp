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
#include "VulkanSwapChain.h"
#include "Proof/Core/Application.h"

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

     //   SetTargetFrameBuffer(config.TargetFrameBuffer);

        //Build();
    }
   
  
    void VulkanRenderPass::Build()
    {
       
        
      

        
    }
    void VulkanRenderPass::Release()
    {
     
    }
   
    VulkanRenderPass::VulkanRenderPass(const std::string& debugName, Count<GraphicsPipeline> pipeline)
    {
        m_Config.DebugName = debugName;
        m_Config.Pipeline = pipeline;
        m_Config.TargetFrameBuffer = FrameBuffer::CreateSwapChainFrameBuffer(debugName);
        m_SwapChain = true;

        VulkanDescriptorManagerConfig descr;
        descr.DebugName = m_Config.DebugName + " Descriptor Manager";
        descr.Shader = GetPipeline()->GetShader().As<VulkanShader>();
        m_DescritptorSetManager = Count<VulkanDescriptorManager>::Create(descr);
    }

    VulkanRenderPass::~VulkanRenderPass()
    {
        Release();
    }

    VkRenderPass VulkanRenderPass::GetRenderPass()
    {
        
        if (m_SwapChain)
            return Application::Get()->GetWindow()->GetSwapChain().As<VulkanSwapChain>()->GetRenderPass();

        return GetTargetFrameBuffer().As<VulkanFrameBuffer>()->GetRenderPass();
    }
    

    void VulkanRenderPass::SetDepthAttachment(const GraphicsPipelineImageConfig& config)
    {
    }
    void VulkanRenderPass::AddColorAttachment(const GraphicsPipelineImageConfig& config)
    {
    }
    void VulkanRenderPass::BeginRenderPassBase(Count<class RenderCommandBuffer>command, Viewport vieport, ViewportScissor scisscor)
    {
        Count<VulkanRenderPass> instance = this;
        Renderer::Submit([instance, command, vieport, scisscor]()
        {
         //   PF_PROFILE_FUNC(fmt::format("BeginRenderPassBase ()").c_str());
            PF_PROFILE_SCOPE_DYNAMIC(fmt::format("BeginRenderPassBase {} ", instance->m_Config.DebugName.c_str()).c_str());

            PF_CORE_ASSERT(instance->m_RenderPassEnabled == false, fmt::format("cannot start {} render pass when previous render pass is not closed", instance->m_Config.DebugName).c_str());
            instance->m_CommandBuffer = command;
            instance->m_RenderPassEnabled = true;
            const FrameBufferConfig config = instance->GetTargetFrameBuffer()->GetConfig();
            VkClearValue colorValue{ config.ClearColor.x, config.ClearColor.y, config.ClearColor.z, config.ClearColor.w };
            std::vector< VkClearValue> clearValues;

           /*
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
            */
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
                if(instance->m_SwapChain)
                    renderPassInfo.framebuffer = Application::Get()->GetWindow()->GetSwapChain().As<VulkanSwapChain>()->GetCurrentFramebuffer();
                else
                    renderPassInfo.framebuffer = instance->GetTargetFrameBuffer().As<VulkanFrameBuffer>()->GetFrameBuffer();

                // the area shader loads and 
                // for high displays swap chain extent could be higher than windows extent
                renderPassInfo.renderArea.offset.x = 0;
                renderPassInfo.renderArea.offset.y = 0;
                renderPassInfo.renderArea.extent.width = instance->GetTargetFrameBuffer()->GetWidth();
                renderPassInfo.renderArea.extent.height = instance->GetTargetFrameBuffer()->GetHeight();
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
            PF_PROFILE_SCOPE_DYNAMIC(fmt::format("BeginMaterialRenderPass {} ", instance->m_Config.DebugName.c_str()).c_str());

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
        if (m_SwapChain)
        {
            viewport.X = 0.0f;
            viewport.Y = (float)GetTargetFrameBuffer()->GetHeight();
            viewport.Width = (float)GetTargetFrameBuffer()->GetWidth();
            viewport.Height = -(float)GetTargetFrameBuffer()->GetHeight();
            viewport.MinDepth = 0.0f;
            viewport.MaxDepth = 1.0f;
        }
        else
        {

            viewport.X = 0.0f;
            viewport.Y = 0.0f;
            viewport.Width = (float)GetTargetFrameBuffer()->GetWidth();
            viewport.Height = (float)GetTargetFrameBuffer()->GetHeight();
            viewport.MinDepth = 0.0f;
            viewport.MaxDepth = 1.0f;
        }

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
            PF_PROFILE_SCOPE_DYNAMIC(fmt::format("BeginRenderPass ({})", instance->GetConfig().DebugName.c_str()).c_str());
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
        if (m_SwapChain)
        {
            viewport.X = 0.0f;
            viewport.Y = (float)GetTargetFrameBuffer()->GetHeight();
            viewport.Width = (float)GetTargetFrameBuffer()->GetWidth();
            viewport.Height = -(float)GetTargetFrameBuffer()->GetHeight();
            viewport.MinDepth = 0.0f;
            viewport.MaxDepth = 1.0f;
        }
        else
        {

            viewport.X = 0.0f;
            viewport.Y = 0.0f;
            viewport.Width = (float)GetTargetFrameBuffer()->GetWidth();
            viewport.Height = (float)GetTargetFrameBuffer()->GetHeight();
            viewport.MinDepth = 0.0f;
            viewport.MaxDepth = 1.0f;
        }


        scissor.Offset = { 0, 0 };
        scissor.Extent = { viewport.Width,viewport.Height };

        BeginRenderPass(command, viewport, scissor, explicitClear);
    }
    void VulkanRenderPass::RenderPassPushRenderMaterial(Count<class RenderMaterial> renderMaterial, bool onlyFragPushConstant )
    {
        Count<VulkanRenderPass> instance = this;
        Renderer::Submit([instance, renderMaterial, onlyFragPushConstant]()
        {
            PF_PROFILE_SCOPE_DYNAMIC(fmt::format("RenderPassPushRenderMaterial {} ", instance->m_Config.DebugName.c_str()).c_str());

            PF_CORE_ASSERT(instance->m_RenderPassEnabled == true, "cannot Push material fi render pass not enabled");
            PF_CORE_ASSERT(instance->m_MaterialRenderPass == true, "cannot Push if not a material Render Pass");

            renderMaterial.As<VulkanRenderMaterial>()->RT_Bind(instance->m_CommandBuffer.As<VulkanRenderCommandBuffer>(), instance, onlyFragPushConstant);
        });
    }
    
    void VulkanRenderPass::EndRenderPass() 
    {
        Count<VulkanRenderPass> instance = this;
        Renderer::Submit([instance]()
        {
            PF_PROFILE_SCOPE_DYNAMIC(fmt::format("EndRenderPass {} ", instance->m_Config.DebugName.c_str()).c_str());

            PF_CORE_ASSERT(instance->m_RenderPassEnabled == true, "cannot End render pass when render pass is not started");
            VkCommandBuffer cmdBuffer = instance->m_CommandBuffer.As<VulkanRenderCommandBuffer>()->GetActiveCommandBuffer();
            vkCmdEndRenderPass(cmdBuffer);
            instance->m_CommandBuffer = nullptr;
            instance->m_RenderPassEnabled = false;
            instance->m_MaterialRenderPass = false;
        });
    }
    void VulkanRenderPass::SetDynamicStates(Viewport vieport, ViewportScissor scisscor, bool explicitClear)
    {
        PF_PROFILE_FUNC();
        PF_CORE_ASSERT(m_RenderPassEnabled == true, "cannot Set dynamic staes if render pass not started");
        /*
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
        */

        const uint32_t width = GetTargetFrameBuffer()->GetWidth();
        const uint32_t height = GetTargetFrameBuffer()->GetHeight();
        const FrameBufferConfig frameBufferConfig = GetTargetFrameBuffer()->GetConfig();
        VkClearValue colorValue{ frameBufferConfig.ClearColor.x, frameBufferConfig.ClearColor.y, frameBufferConfig.ClearColor.z, frameBufferConfig.ClearColor.w };

        VkViewport vk_viewport = {};
        vk_viewport.minDepth = 0.0f;
        vk_viewport.maxDepth = 1.0f;

        if (m_SwapChain)
        {
            vk_viewport.x = 0.0f;
            vk_viewport.y = (float)height;
            vk_viewport.width = (float)width;
            vk_viewport.height = -(float)height;
        }
        else
        {
            vk_viewport.x = 0.0f;
            vk_viewport.y = 0.0f;
            vk_viewport.width = (float)width;
            vk_viewport.height = (float)height;
        }

        VkRect2D vk_scissor = {};
        vk_scissor.extent.width = width;
        vk_scissor.extent.height = height;
        vk_scissor.offset.x = 0;
        vk_scissor.offset.y = 0;
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
                lineWidth = vk_pipeline->GetLineWidthStack().top();
            else
                lineWidth = vk_pipeline->GetConfig().LineWidth;


            vkCmdSetLineWidth(cmdBuffer, lineWidth);
        }

        if (vk_pipeline->GetConfig().EditDrawType)
        {
            DrawType drawType;
            if (vk_pipeline->GetDrawTypeStack().size())
                drawType = vk_pipeline->GetDrawTypeStack().top();
            else
                drawType = vk_pipeline->GetConfig().DrawMode;

            PF_CORE_ASSERT(false, "not found ");

            vkCmdSetPrimitiveTopology(cmdBuffer, Utils::ProofTopologyToVulkanTopology(drawType));
        }

        if (vk_pipeline->GetConfig().EditCullMode)
        {
            CullMode mode;
            if (vk_pipeline->GetCullModeStack().size())
                mode = vk_pipeline->GetCullModeStack().top();
            else
                mode = vk_pipeline->GetConfig().CullMode;

            PF_CORE_ASSERT(false, "cull mode not found ");
            vkCmdSetCullMode(cmdBuffer, Utils::ProofFormatToVulkanFormat(mode));
        }

        if (vk_pipeline->GetConfig().EditDepthTest)
        {
            bool test;
            if (vk_pipeline->GetDepthTestStack().size())
                test = vk_pipeline->GetDepthTestStack().top();
            else
                test = vk_pipeline->GetConfig().DepthTest;

            vkCmdSetDepthTestEnable(cmdBuffer, test == true ? VK_TRUE : VK_FALSE);
        }
    }
    void VulkanRenderPass::SetTargetFrameBuffer(Count<FrameBuffer> frame)
    {
        if (m_SwapChain)
            return;
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
    void VulkanRenderPass::SetInput(std::string_view name, Count<class Image>image)
    {
        m_DescritptorSetManager->SetInput(name, image);
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

        Buffer buffer;
        buffer.Allocate(pushRange.size);
        buffer.Copy(data, buffer.Size);

        Count<VulkanRenderPass> instance = this;
        Renderer::Submit([instance, str, buffer]() mutable
        {
            instance->RT_PushData(str, buffer.Data);
            buffer.Release();
        });
    }
}
