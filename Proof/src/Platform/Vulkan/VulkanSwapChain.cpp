
// std
#include "Proofprch.h"

#include <array>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <limits>
#include <set>
#include <stdexcept>
#include "VulkanSwapChain.h"
#include "Proof/Renderer/Renderer.h"
namespace Proof
{

    VulkanSwapChain::VulkanSwapChain(VkExtent2D extent)
        : m_WindowExtent{ extent } {
      //  vkDeviceWaitIdle(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice());

        CreateSwapChain();
        CreateImageViews();
        CreateRenderPass();
        CreateDepthResources();
        CreateFramebuffers();
        CreateSyncObjects();
    }

    VulkanSwapChain::~VulkanSwapChain() {
        const auto& device = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice();
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

        // cleanup synchronization objects
        vkDestroySemaphore(device, m_PresentSemaphore, nullptr);
        vkDestroySemaphore(device, m_RenderSemaphore, nullptr);
        for (int i = 0; i < m_SwapChainFramebuffers.size(); i++) {
            vkDestroyFramebuffer(device, m_SwapChainFramebuffers[i], nullptr);

            vkDestroyImageView(device, m_SwapChainImageViews[i], nullptr);
        }
    }

    VkResult VulkanSwapChain::AcquireNextImage(uint32_t* imageIndex, uint32_t frameIndex) {
      //  vkWaitForFences(
      //      Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(),
      //      1,
      //      &m_InFlightFences[m_CurrentFrame],
      //      VK_TRUE,
      //      std::numeric_limits<uint64_t>::max());
      //  
      //  VkResult result = vkAcquireNextImageKHR(
      //      Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(),
      //      m_SwapChain,
      //      std::numeric_limits<uint64_t>::max(),
      //      m_ImageAvailableSemaphores[m_CurrentFrame],  // must be a not signaled semaphore
      //      VK_NULL_HANDLE,
      //      imageIndex);

        return {};
    }

    VkResult VulkanSwapChain::SubmitCommandBuffers(const VkCommandBuffer* buffers, uint32_t* imageIndex) {
        /*
        auto currentDevice = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();
        VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        VkSubmitInfo submitInfo = {};
        submitInfo.pCommandBuffers = buffers;
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        submitInfo.pWaitDstStageMask = &waitStage;

        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = &m_PresentSemaphore;

        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = &m_RenderSemaphore;


        //submit command buffer to the queue and execute it.
        // _renderFence will now block until the graphic commands finish execution
        if (vkQueueSubmit(currentDevice->GetGraphicsQueue() , 1, &submitInfo, m_RenderFence) != VK_SUCCESS) {
            PF_CORE_ASSERT(false, "failed to submit que for command buffer!");

        };

        VkPresentInfoKHR presentInfo = {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
       
        presentInfo.pSwapchains = &m_SwapChain;
        presentInfo.swapchainCount = 1;
       
        presentInfo.pWaitSemaphores = &m_RenderSemaphore;
        presentInfo.waitSemaphoreCount = 1;
       
        presentInfo.pImageIndices = imageIndex;
       
        if (vkQueuePresentKHR(currentDevice->GetPresentQueue(), &presentInfo) != VK_SUCCESS)
            PF_CORE_ASSERT(false, "failed to submit que for command buffer!");

            */
        VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = &m_PresentSemaphore;
        submitInfo.pWaitDstStageMask = &waitStage;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = buffers;

        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = &m_RenderSemaphore;

        vkResetFences(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), 1, &m_RenderFence);
        if (vkQueueSubmit(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetGraphicsQueue(), 1, &submitInfo, m_RenderFence) !=
            VK_SUCCESS) {
            PF_CORE_ASSERT(false, "failed to submit draw command buffer!");
        }

        VkPresentInfoKHR presentInfo = {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = &m_RenderSemaphore;

        VkSwapchainKHR swapChains[] = { m_SwapChain };
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;

        presentInfo.pImageIndices = imageIndex;

        auto result = vkQueuePresentKHR(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetPresentQueue(), &presentInfo);

        return {};
    }

    void VulkanSwapChain::CreateSwapChain() {
        SwapChainSupportDetails swapChainSupport = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetSwapChainSupport();

        VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
        VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
        VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);

        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
        if (swapChainSupport.capabilities.maxImageCount > 0 &&
            imageCount > swapChainSupport.capabilities.maxImageCount) {
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetSurface();

        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        QueueFamilyIndices indices = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->FindPhysicalQueueFamilies();
        uint32_t queueFamilyIndices[] = { indices.graphicsFamily, indices.presentFamily };

        if (indices.graphicsFamily != indices.presentFamily) {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0;      // Optional
            createInfo.pQueueFamilyIndices = nullptr;  // Optional
        }

        createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;

        createInfo.oldSwapchain = VK_NULL_HANDLE;

        if (vkCreateSwapchainKHR(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), &createInfo, nullptr, &m_SwapChain) != VK_SUCCESS) {
            PF_CORE_ASSERT(false, "failed to create swap chain!");
        }

        // we only specified a minimum number of images in the swap chain, so the implementation is
        // allowed to create a swap chain with more. That's why we'll first query the final number of
        // images with vkGetSwapchainImagesKHR, then resize the container and finally call it again to
        // retrieve the handles.
        vkGetSwapchainImagesKHR(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), m_SwapChain, &imageCount, nullptr);
        m_SwapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), m_SwapChain, &imageCount, m_SwapChainImages.data());

        m_SwapChainImageFormat = surfaceFormat.format;
        m_SwapChainExtent = extent;
    }

    void VulkanSwapChain::CreateImageViews() {
        m_SwapChainImageViews.resize(m_SwapChainImages.size());
        for (size_t i = 0; i < m_SwapChainImages.size(); i++) {
            VkImageViewCreateInfo viewInfo{};
            viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            viewInfo.image = m_SwapChainImages[i];
            viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            viewInfo.format = m_SwapChainImageFormat;
            viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            viewInfo.subresourceRange.baseMipLevel = 0;
            viewInfo.subresourceRange.levelCount = 1;
            viewInfo.subresourceRange.baseArrayLayer = 0;
            viewInfo.subresourceRange.layerCount = 1;

            if (vkCreateImageView(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), &viewInfo, nullptr, &m_SwapChainImageViews[i]) !=
                VK_SUCCESS) {
                PF_CORE_ASSERT(false, "failed to create texture image view!");
            }
        }
    }

    void VulkanSwapChain::CreateRenderPass() {
        VkAttachmentDescription depthAttachment{};
        depthAttachment.format = FindDepthFormat();
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
        colorAttachment.format = GetSwapChainImageFormat();
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
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
        VkExtent2D swapChainExtent = GetSwapChainExtent();
        //create the framebuffers for the swapchain images. This will connect the render-pass to the images for rendering
        /** /
        VkFramebufferCreateInfo fb_info = {};
        fb_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        fb_info.pNext = nullptr;

        fb_info.renderPass = m_RenderPass;
        fb_info.attachmentCount = 1;
        fb_info.width = swapChainExtent.width;
        fb_info.height = swapChainExtent.height;
        fb_info.layers = 1;
        */
        //grab how many images we have in the swapchain

        /*
        const uint32_t swapchain_imagecount = m_SwapChainImages.size();
        m_SwapChainFramebuffers = std::vector<VkFramebuffer>(swapchain_imagecount);



        //create framebuffers for each of the swapchain image views
        for (int i = 0; i < swapchain_imagecount; i++) {
            VkFramebufferCreateInfo framebufferCreateInfo = {};

            framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferCreateInfo.renderPass = m_RenderPass;
            framebufferCreateInfo.attachmentCount = 1;
            framebufferCreateInfo.width = swapChainExtent.width;
            framebufferCreateInfo.height = swapChainExtent.height;
            framebufferCreateInfo.layers = 1;

            framebufferCreateInfo.pAttachments = &m_SwapChainImageViews[i];
            if (vkCreateFramebuffer(device,&framebufferCreateInfo,nullptr,&m_SwapChainFramebuffers[i]) != VK_SUCCESS) {
                PF_CORE_ASSERT(false, "failed to create framebuffer!");
            }
        }
        */
        m_SwapChainFramebuffers.resize(GetImageCount());
        for (size_t i = 0; i < GetImageCount(); i++) {
            std::array<VkImageView, 2> attachments = { m_SwapChainImageViews[i], m_DepthImageViews[i] };

            VkExtent2D swapChainExtent = GetSwapChainExtent();
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
        VkFormat depthFormat = FindDepthFormat();
        VkExtent2D swapChainExtent = GetSwapChainExtent();

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

            Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->CreateImageWithInfo(
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

            if (vkCreateImageView(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), &viewInfo, nullptr, &m_DepthImageViews[i]) != VK_SUCCESS) {
                PF_CORE_ASSERT(false, "failed to create texture image view!");
            }
        }
    }

    void VulkanSwapChain::CreateSyncObjects() {
        m_ImageAvailableSemaphores.resize(1);
        m_RenderFinishedSemaphores.resize(1);
        m_InFlightFences.resize(1);
        m_ImagesInFlight.resize(GetImageCount(), VK_NULL_HANDLE);

        VkSemaphoreCreateInfo semaphoreInfo = {};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo = {};

        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        //we want to create the fence with the Create Signaled flag, so we can wait on it before using it on a GPU command (for the first frame)
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        fenceInfo.pNext = nullptr;


        if (vkCreateSemaphore(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), &semaphoreInfo, nullptr, &m_PresentSemaphore) !=
            VK_SUCCESS ||
            vkCreateSemaphore(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), &semaphoreInfo, nullptr, &m_RenderSemaphore) !=
            VK_SUCCESS ||
            vkCreateFence(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), &fenceInfo, nullptr, &m_RenderFence) != VK_SUCCESS) {
            PF_CORE_ASSERT(false, "failed to create synchronization objects for a frame!");
        }
    }

    
    
    VkSurfaceFormatKHR VulkanSwapChain::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
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
            VkExtent2D actualExtent = m_WindowExtent;
            actualExtent.width = std::max(
                capabilities.minImageExtent.width,
                std::min(capabilities.maxImageExtent.width, actualExtent.width));
            actualExtent.height = std::max(
                capabilities.minImageExtent.height,
                std::min(capabilities.maxImageExtent.height, actualExtent.height));

            return actualExtent;
        }
    }

    VkFormat VulkanSwapChain::FindDepthFormat() {
        return Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->FindSupportedFormat(
            { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
            VK_IMAGE_TILING_OPTIMAL,
            VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
    }
    void VulkanSwapChain::Recreate() {
        // wait while the current swapchain is being used
      //  vkDeviceWaitIdle(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice());
        *this = VulkanSwapChain(GetSwapChainExtent());
    }
}  
