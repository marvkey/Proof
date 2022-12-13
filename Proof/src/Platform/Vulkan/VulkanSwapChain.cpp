
// std
#include "Proofprch.h"

#include "VulkanSwapChain.h"
#include "Proof/Renderer/Renderer.h"
#include "VulkanGraphicsContext.h"

#include <array>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <limits>
#include <set>
#include <stdexcept>
#include "Proof/Math/Math.h"

namespace Proof
{

    VulkanSwapChain::VulkanSwapChain(VkExtent2D extent)
        : m_WindowSize{ extent.width,extent.height } {
      //  vkDeviceWaitIdle(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice());

        CreateSwapChain();
        CreateSyncObjects();
        CreateImageViews();
        CreateRenderPass();
        CreateDepthResources();
        CreateFramebuffers();
    }

    VulkanSwapChain::~VulkanSwapChain() {
        CleanUp();
    }

    void VulkanSwapChain::AcquireNextImage(uint32_t* imageIndex, uint32_t frameIndex) {
            //vkAcquireNextImageKHR(
            //Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(),
            //m_SwapChain,
            //Math::GetMaxType<uint64_t>(),
            //m_ImageAvailableSemaphores[frameIndex],  // must be a not signaled semaphore
            //nullptr,
            //imageIndex);
            const auto& graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();
            vkAcquireNextImageKHR(graphicsContext->GetDevice(), m_SwapChain, UINT64_MAX, m_ImageAvailableSemaphores[frameIndex], VK_NULL_HANDLE, imageIndex);

    }

    void VulkanSwapChain::SubmitCommandBuffers(std::vector<Count<VulkanCommandBuffer>> buffers, uint32_t* imageIndex) {
        auto graphicsContext= Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();
        
        std::vector<VkCommandBuffer> submitCommandBuffers;
        for (auto& buffer : buffers) {
            submitCommandBuffers.emplace_back(buffer->m_CommandBuffer[Renderer::GetCurrentFrame().FrameinFlight]);
        }
        VkSemaphore waitSemaphores[] = { m_ImageAvailableSemaphores[Renderer::GetCurrentFrame().FrameinFlight] };
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        VkSemaphore signalSemaphores[] = { m_RenderFinishedSemaphores[Renderer::GetCurrentFrame().FrameinFlight] };

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = submitCommandBuffers.size();
        submitInfo.pCommandBuffers = submitCommandBuffers.data();

        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (vkQueueSubmit(graphicsContext->GetGraphicsQueue(), 1, &submitInfo, m_InFlightFences[Renderer::GetCurrentFrame().FrameinFlight]) !=
            VK_SUCCESS) {
            PF_CORE_ASSERT(false, "failed to submit draw command buffer!");
        }
        //WaitFences();
        VkSwapchainKHR swapChains[] = { m_SwapChain };
        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = imageIndex;
        
        vkQueuePresentKHR(graphicsContext->GetPresentQueue(), &presentInfo);
        vkDeviceWaitIdle(graphicsContext->GetDevice());
        for(int i =0; i<submitCommandBuffers.size(); i++)
            vkResetCommandBuffer(submitCommandBuffers[i], 0);
    }

    void VulkanSwapChain::CreateSwapChain() {
        auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();

        SwapChainSupportDetails swapChainSupport = graphicsContext->GetSwapChainSupport();

        m_SurfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
        m_PresentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
        VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);

        m_ImageCount = swapChainSupport.capabilities.minImageCount + 1;
        if (swapChainSupport.capabilities.maxImageCount > 0 && m_ImageCount > swapChainSupport.capabilities.maxImageCount) {
            m_ImageCount = swapChainSupport.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = graphicsContext->GetSurface();
        createInfo.minImageCount = m_ImageCount;
        createInfo.imageFormat = m_SurfaceFormat.format;
        createInfo.imageColorSpace = m_SurfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
       
        QueueFamilyIndices indices = graphicsContext->FindPhysicalQueueFamilies();
        uint32_t queueFamilyIndices[] = { indices.graphicsFamily, indices.presentFamily};

        if (indices.graphicsFamily != indices.presentFamily) {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        }

        createInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = m_PresentMode;
        createInfo.clipped = VK_TRUE;

        if (vkCreateSwapchainKHR(graphicsContext->GetDevice(), &createInfo, nullptr, &m_SwapChain) != VK_SUCCESS) {
            PF_CORE_ASSERT(false,"failed to create swap chain!");
        }

        uint32_t swapchainCount;
        vkGetSwapchainImagesKHR(graphicsContext->GetDevice(), m_SwapChain, &swapchainCount, nullptr);
        m_SwapChainImages.resize(swapchainCount);
        vkGetSwapchainImagesKHR(graphicsContext->GetDevice(), m_SwapChain, &swapchainCount, m_SwapChainImages.data());

        m_ImageFormat = m_SurfaceFormat.format;
        m_SwapChainExtent = { extent.width,extent.height };
    }

    void VulkanSwapChain::CreateImageViews() {
        auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();

        m_SwapChainImageViews.resize(m_SwapChainImages.size());
        for (size_t i = 0; i < m_SwapChainImages.size(); i++) {
            VkImageViewCreateInfo viewInfo{};
            viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            viewInfo.image = m_SwapChainImages[i];
            viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            viewInfo.format = m_ImageFormat;
            viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            viewInfo.subresourceRange.baseMipLevel = 0;
            viewInfo.subresourceRange.levelCount = 1;
            viewInfo.subresourceRange.baseArrayLayer = 0;
            viewInfo.subresourceRange.layerCount = 1;;

            if (vkCreateImageView(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), &viewInfo, nullptr, &m_SwapChainImageViews[i]) !=
                VK_SUCCESS) {
                PF_CORE_ASSERT(false, "failed to create texture image view!");
            }
        }
    }

    void VulkanSwapChain::CreateRenderPass() {
        VkAttachmentDescription depthAttachment{};
        depthAttachment.format = GetDepthFormat();
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
        colorAttachment.format = GetImageFormat();
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
       // colorAttachment.finalLayout =  VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
       
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

    void VulkanSwapChain::CreateFramebuffers() {
        const auto& device = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice();
        VkExtent2D swapChainExtent = { GetSwapChainExtent().X,GetSwapChainExtent().Y };
        m_SwapChainFramebuffers.resize(GetImageCount());
        for (size_t i = 0; i < GetImageCount(); i++) {
            std::array<VkImageView, 2> attachments = { m_SwapChainImageViews[i], m_DepthImageViews[i] };

            VkFramebufferCreateInfo framebufferInfo = {};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = m_RenderPass;
            framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
            framebufferInfo.pAttachments = attachments.data();
            framebufferInfo.width = swapChainExtent.width;
            framebufferInfo.height = swapChainExtent.height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(device,&framebufferInfo,nullptr,&m_SwapChainFramebuffers[i]) != VK_SUCCESS) {
                PF_CORE_ASSERT(false, "failed to create framebuffer!");
            }
        }
    }

    void VulkanSwapChain::CreateDepthResources() {
        auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();
        VkFormat depthFormat = GetDepthFormat();
        VkExtent2D swapChainExtent = { GetSwapChainExtent().X , GetSwapChainExtent().Y };

        m_DepthImages.resize(GetImageCount());
        m_DepthImageMemorys.resize(GetImageCount());
        m_DepthImageViews.resize(GetImageCount());

        for (int i = 0; i < m_DepthImages.size(); i++) {
            VkImageCreateInfo imageInfo{};
            imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
            imageInfo.imageType = VK_IMAGE_TYPE_2D;
            imageInfo.extent.width = swapChainExtent.width;
            imageInfo.extent.height = swapChainExtent.height;
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

            graphicsContext->CreateImageWithInfo(
                imageInfo,
                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                m_DepthImages[i],
                m_DepthImageMemorys[i]);

            VkImageViewCreateInfo viewInfo{};
            viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            viewInfo.image = m_DepthImages[i];
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
        }
    }

    void VulkanSwapChain::CreateSyncObjects() {
        auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();

        uint32_t framesInFlight = Renderer::GetConfig().FramesFlight;
        m_ImageAvailableSemaphores.resize(framesInFlight);
        m_RenderFinishedSemaphores.resize(framesInFlight);
        m_InFlightFences.resize(framesInFlight);

        VkSemaphoreCreateInfo semaphoreInfo = {};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo = {};

        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        //we want to create the fence with the Create Signaled flag, so we can wait on it before using it on a GPU command (for the first frame)
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (uint32_t i = 0; i < framesInFlight; i++) {
            if (vkCreateSemaphore(graphicsContext->GetDevice(), &semaphoreInfo, nullptr, &m_ImageAvailableSemaphores[i]) != VK_SUCCESS)
                PF_CORE_ASSERT(false, "failed to create semapthore!");
            
            if (vkCreateSemaphore(graphicsContext->GetDevice(), &semaphoreInfo, nullptr, &m_RenderFinishedSemaphores[i]) != VK_SUCCESS)
                PF_CORE_ASSERT(false, "failed to create semapthore!");

            if(vkCreateFence(graphicsContext->GetDevice(), &fenceInfo, nullptr, &m_InFlightFences[i]) != VK_SUCCESS) 
                PF_CORE_ASSERT(false, "failed to create synchronization objects for a frame!");
        }
    }

    void VulkanSwapChain::Recreate(VectorTemplate2<uint32_t> size) {
        const auto& device = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice();
        vkDeviceWaitIdle(device);
        {
            for (size_t i = 0; i < m_SwapChainFramebuffers.size(); i++) {
                vkDestroyFramebuffer(device, m_SwapChainFramebuffers[i], nullptr);
            }
            
            for (size_t i = 0; i < m_SwapChainImageViews.size(); i++) {
                vkDestroyImageView(device, m_SwapChainImageViews[i], nullptr);
            }

            vkDestroySwapchainKHR(device, m_SwapChain, nullptr);
        }
        CreateSwapChain();
        CreateImageViews();
        CreateFramebuffers();
    }

    void VulkanSwapChain::CleanUp() {
        const auto& device = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice();

        {
            for (size_t i = 0; i < m_SwapChainFramebuffers.size(); i++) {
                vkDestroyFramebuffer(device, m_SwapChainFramebuffers[i], nullptr);
            }
            
            for (size_t i = 0; i < m_SwapChainImageViews.size(); i++) {
                vkDestroyImageView(device, m_SwapChainImageViews[i], nullptr);
            }

            vkDestroySwapchainKHR(device, m_SwapChain, nullptr);
        }
        for (auto imageView : m_SwapChainImageViews) {
            vkDestroyImageView(device, imageView, nullptr);
        }
        m_SwapChainImageViews.clear();

        if (m_SwapChain != nullptr) {
            vkDestroySwapchainKHR(device, m_SwapChain, nullptr);
            m_SwapChain = nullptr;
        }

        for (int i = 0; i < m_DepthImages.size(); i++) {
            vkDestroyImageView(device, m_DepthImageViews[i], nullptr);
            vkDestroyImage(device, m_DepthImages[i], nullptr);
            vkFreeMemory(device, m_DepthImageMemorys[i], nullptr);
        }
        
        for (auto framebuffer : m_SwapChainFramebuffers) {
            vkDestroyFramebuffer(device, framebuffer, nullptr);
        }

        vkDestroyRenderPass(device, m_RenderPass, nullptr);
        for (uint32_t i = 0; i < Renderer::GetConfig().FramesFlight; i++) {
            vkDestroySemaphore(device, m_RenderFinishedSemaphores[i], nullptr);
            vkDestroySemaphore(device, m_ImageAvailableSemaphores[i], nullptr);
            vkDestroyFence(device, m_InFlightFences[i], nullptr);
        }
       //  cleanup synchronization objects
        for (int i = 0; i < m_SwapChainFramebuffers.size(); i++) {
            vkDestroyFramebuffer(device, m_SwapChainFramebuffers[i], nullptr);
        
            vkDestroyImageView(device, m_SwapChainImageViews[i], nullptr);
        }
    }

  

    
    
    VkSurfaceFormatKHR VulkanSwapChain::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
        return availableFormats[0];

        for (const auto& availableFormat : availableFormats) {
            //VK_FORMAT_B8G8R8A8_UNORM 
            //VK_FORMAT_B8G8R8A8_SRGB USES GAMMA CORRECTION
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
                availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return availableFormat;
            }
        }

        return availableFormats[0];
    }

    VkPresentModeKHR VulkanSwapChain::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {

        for (const auto& availablePresentMode : availablePresentModes) {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                PF_ENGINE_INFO("Present mode: Mailbox");
                return availablePresentMode;
            }
        }

        // for (const auto &availablePresentMode : availablePresentModes) {
        //   if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
     //           PF_ENGINE_INFO("Present mode: Immediate");
        // 
        //     return availablePresentMode;
        //   }
        // }
        PF_ENGINE_INFO("Present mode: V-Sync");
        return VK_PRESENT_MODE_FIFO_KHR;
    }

   

    VkExtent2D VulkanSwapChain::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
            return capabilities.currentExtent;
        }
        else {
            VkExtent2D actualExtent = { m_WindowSize.X,m_WindowSize.Y };
            actualExtent.width = std::max(
                capabilities.minImageExtent.width,
                std::min(capabilities.maxImageExtent.width, actualExtent.width));
            actualExtent.height = std::max(
                capabilities.minImageExtent.height,
                std::min(capabilities.maxImageExtent.height, actualExtent.height));

            return actualExtent;
        }
    }

   

    void VulkanSwapChain::WaitAndResetFences(uint32_t frameIndex) {
        WaitFences(frameIndex);
        ResetFences(frameIndex);
    }

    void VulkanSwapChain::WaitFences(uint32_t frameIndex) {
        vkWaitForFences(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), 1, &m_InFlightFences[frameIndex], VK_TRUE, UINT64_MAX);    // wait indefinitely instead of periodically checking
        //vkWaitForFences(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), 1, &m_InFlightFences[frameIndex], VK_TRUE, Math::GetMaxType<uint64_t>());
    }

    void VulkanSwapChain::ResetFences(uint32_t frameIndex) {
        vkResetFences(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), 1, &m_InFlightFences[frameIndex]);
    }

    void VulkanSwapChain::Present() {
        auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();

        VkSemaphore waitSemaphores[] = { m_ImageAvailableSemaphores[Renderer::GetCurrentFrame().FrameinFlight] };
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        VkSemaphore signalSemaphores[] = { m_RenderFinishedSemaphores[Renderer::GetCurrentFrame().FrameinFlight] };
        uint32_t currentFrame = Renderer::GetCurrentFrame().FrameinFlight;
        VkSwapchainKHR swapChains[] = { m_SwapChain };
        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &currentFrame;

        vkQueuePresentKHR(graphicsContext->GetPresentQueue(), &presentInfo);
        //vkDeviceWaitIdle(graphicsContext->GetDevice());
        //for (int i = 0; i < submitCommandBuffers.size(); i++)
        //    vkResetCommandBuffer(submitCommandBuffers[i], 0);

    }

    VkFormat VulkanSwapChain::GetDepthFormat() {
        return Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->FindSupportedFormat(
            { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
            VK_IMAGE_TILING_OPTIMAL,
            VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
    }
    
}  
