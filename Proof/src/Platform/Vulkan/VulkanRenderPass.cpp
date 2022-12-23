#include "Proofprch.h"
#include "VulkanRenderPass.h"
#include "VulkanRenderer/VulkanRenderer.h"
#include "VulkanFrameBuffer.h"
namespace Proof
{
	VulkanRenderPass::VulkanRenderPass(RenderPassType type) {
        auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();
        auto swapChain = graphicsContext->GetSwapChain()->As<VulkanSwapChain>();
        if (type == RenderPassType::World) {
            VkAttachmentDescription depthAttachment{};
            depthAttachment.format = swapChain->GetDepthFormat();
            depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
            depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

            VkAttachmentReference depthAttachmentRef{};
            depthAttachmentRef.attachment = 1;
            depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

            VkAttachmentDescription colorAttachment = {};
            colorAttachment.format = swapChain->GetImageFormat();
            colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
            colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
            colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            colorAttachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

            VkAttachmentReference colorAttachmentRef = {};
            colorAttachmentRef.attachment = 0;
            colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

            VkSubpassDescription subpass = {};
            subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
            subpass.colorAttachmentCount = 1;
            subpass.pColorAttachments = &colorAttachmentRef;
            subpass.pDepthStencilAttachment = &depthAttachmentRef;

            VkSubpassDependency dependency = {};
            dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
            dependency.srcAccessMask = 0;
            dependency.srcStageMask =
                VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
            dependency.dstSubpass = 0;
            dependency.dstStageMask =
                VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
            dependency.dstAccessMask =
                VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

            std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };
            VkRenderPassCreateInfo renderPassInfo = {};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
            renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
            renderPassInfo.pAttachments = attachments.data();
            renderPassInfo.subpassCount = 1;
            renderPassInfo.pSubpasses = &subpass;
            renderPassInfo.dependencyCount = 1;
            renderPassInfo.pDependencies = &dependency;

            if (vkCreateRenderPass(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), &renderPassInfo, nullptr, &m_RenderPass) != VK_SUCCESS) {
                PF_CORE_ASSERT(false, "failed to create render pass!");
            }
        }
        else {
            VkAttachmentDescription attachment = {};
            attachment.format = swapChain->GetImageFormat();
            attachment.samples = VK_SAMPLE_COUNT_1_BIT;
            attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR ;
            attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
            attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
            VkAttachmentReference color_attachment = {};
            color_attachment.attachment = 0;
            color_attachment.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
            VkSubpassDescription subpass = {};
            subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
            subpass.colorAttachmentCount = 1;
            subpass.pColorAttachments = &color_attachment;
            VkSubpassDependency dependency = {};
            dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
            dependency.dstSubpass = 0;
            dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            dependency.srcAccessMask = 0;
            dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            VkRenderPassCreateInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
            info.attachmentCount = 1;
            info.pAttachments = &attachment;
            info.subpassCount = 1;
            info.pSubpasses = &subpass;
            info.dependencyCount = 1;
            info.pDependencies = &dependency;

            if (vkCreateRenderPass(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), &info, nullptr, &m_RenderPass) != VK_SUCCESS) {
                PF_CORE_ASSERT(false, "failed to create render pass!");
            }
        }
	}

	VulkanRenderPass::VulkanRenderPass(const VulkanRenderPassInfo& info) {
        m_PipeLine = info.PipeLine;
        VkRenderPassCreateInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = info.Attachments.size();
        renderPassInfo.pAttachments = info.Attachments.data();
        renderPassInfo.subpassCount = info.SubPass.size();
        renderPassInfo.pSubpasses = info.SubPass.data();
        renderPassInfo.dependencyCount = info.SubPassDepedency.size();
        renderPassInfo.pDependencies = info.SubPassDepedency.data();

        if (vkCreateRenderPass(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), &renderPassInfo, nullptr, &m_RenderPass) != VK_SUCCESS) {
            PF_CORE_ASSERT(false, "failed to create render pass!");
        }
	}
    VulkanRenderPass::~VulkanRenderPass() {
    }
    void VulkanRenderPass::BeginRenderPass(Count<class CommandBuffer> command, Count<class ScreenFrameBuffer>frameBuffer, bool viewScreen , const glm::vec4& Color, float Depth, uint32_t stencil) {
       
        PF_CORE_ASSERT(m_RenderPassEnabled == false, "cannot start render pass when previous render pass is not closed");
        m_CommandBuffer = command;
        m_CommandBuffer->As<VulkanCommandBuffer>()->BeginRecord(GetPipeLine(), Renderer::GetCurrentFrame().FrameinFlight, viewScreen);
        m_FrameBuffer = frameBuffer;

        auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();
        auto swapchain = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetSwapChain();
        if (frameBuffer->As<VulkanScreenFrameBuffer>()->IsScreenPresent() == true) {
            VkClearValue value{ Color.x, Color.y, Color.z, Color.a };
            VkRenderPassBeginInfo renderPassInfo = {};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = m_RenderPass;   
            renderPassInfo.framebuffer = frameBuffer->As<VulkanScreenFrameBuffer>()->GetFrameBuffer();
            renderPassInfo.renderArea.offset = { 0, 0 };
            renderPassInfo.renderArea.extent = VkExtent2D{ (uint32_t)frameBuffer->As<VulkanScreenFrameBuffer>()->m_ImageSize.X, (uint32_t)frameBuffer->As<VulkanScreenFrameBuffer>()->m_ImageSize.Y };
            renderPassInfo.clearValueCount = 1;
            renderPassInfo.pClearValues = &value;
            vkCmdBeginRenderPass(command->As<VulkanCommandBuffer>()->GetCommandBuffer(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
            return;
        }
        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = { Color.x, Color.y, Color.z, Color.a };
        // color of screen
        // teh reason we are not settign [0].depthStencil is because 
        //we set color atachmetna as index 0 and depth as index 1 in 
        // the render pass
        clearValues[1].depthStencil = { Depth,stencil };


        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = m_RenderPass;
        // teh frameBuffer we are writing
        renderPassInfo.framebuffer = frameBuffer->As<VulkanScreenFrameBuffer>()->GetFrameBuffer(Renderer::GetCurrentFrame().ImageIndex);

        // the area shader loads and 
        renderPassInfo.renderArea.offset = { 0,0 };
        // for high displays swap chain extent could be higher than windows extent
        renderPassInfo.renderArea.extent = VkExtent2D{ (uint32_t)frameBuffer->As<VulkanScreenFrameBuffer>()->m_ImageSize.X, (uint32_t)frameBuffer->As<VulkanScreenFrameBuffer>()->m_ImageSize.Y };


        renderPassInfo.clearValueCount = (uint32_t)clearValues.size();
        renderPassInfo.pClearValues = clearValues.data();
        vkCmdBeginRenderPass(command->As<VulkanCommandBuffer>()->GetCommandBuffer(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    }
    void VulkanRenderPass::RecordRenderPass(std::function<void(Count<CommandBuffer> commandBuffer)> func) {
        m_CommandBuffer->As<VulkanCommandBuffer>()->Bind();
        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)m_FrameBuffer->GetFrameWidth();
        viewport.height = (float)m_FrameBuffer->GetFrameHeight();
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(m_CommandBuffer->As<VulkanCommandBuffer>()->GetCommandBuffer(), 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = { 0, 0 };
        scissor.extent = VkExtent2D{ (uint32_t)m_FrameBuffer->As<VulkanScreenFrameBuffer>()->m_ImageSize.X, (uint32_t)m_FrameBuffer->As<VulkanScreenFrameBuffer>()->m_ImageSize.Y };;
        vkCmdSetScissor(m_CommandBuffer->As<VulkanCommandBuffer>()->GetCommandBuffer(), 0, 1, &scissor);
        
        func(m_CommandBuffer);
    }

    void VulkanRenderPass::EndRenderPass() {
        PF_CORE_ASSERT(m_RenderPassEnabled == false, "cannot End render pass when render pass is not started");
        vkCmdEndRenderPass(m_CommandBuffer->As<VulkanCommandBuffer>()->GetCommandBuffer());
        m_CommandBuffer->As<VulkanCommandBuffer>()->EndRecord();
        m_CommandBuffer = nullptr;
    }
}
