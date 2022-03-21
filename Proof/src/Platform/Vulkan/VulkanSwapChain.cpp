
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
        CreateSwapChain();
        CreateImageViews();
        CreateRenderPass();
        CreateDepthResources();
        CreateFramebuffers();
        CreateSyncObjects();
    }

    VulkanSwapChain::~VulkanSwapChain() {
        for (auto imageView : m_SwapChainImageViews) {
            vkDestroyImageView(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), imageView, nullptr);
        }
        m_SwapChainImageViews.clear();

        if (m_SwapChain != nullptr) {
            vkDestroySwapchainKHR(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), m_SwapChain, nullptr);
            m_SwapChain = nullptr;
        }

        for (int i = 0; i < m_DepthImages.size(); i++) {
            vkDestroyImageView(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), m_DepthImageViews[i], nullptr);
            vkDestroyImage(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), m_DepthImages[i], nullptr);
            vkFreeMemory(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), m_DepthImageMemorys[i], nullptr);
        }

        for (auto framebuffer : m_SwapChainFramebuffers) {
            vkDestroyFramebuffer(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), framebuffer, nullptr);
        }

        vkDestroyRenderPass(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), m_RenderPass, nullptr);

        // cleanup synchronization objects
        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroySemaphore(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), m_RenderFinishedSemaphores[i], nullptr);
            vkDestroySemaphore(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), m_ImageAvailableSemaphores[i], nullptr);
            vkDestroyFence(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), m_InFlightFences[i], nullptr);
        }
    }

    VkResult VulkanSwapChain::AcquireNextImage(uint32_t* imageIndex) {
        vkWaitForFences(
            Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(),
            1,
            &m_InFlightFences[m_CurrentFrame],
            VK_TRUE,
            std::numeric_limits<uint64_t>::max());

        VkResult result = vkAcquireNextImageKHR(
            Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(),
            m_SwapChain,
            std::numeric_limits<uint64_t>::max(),
            m_ImageAvailableSemaphores[m_CurrentFrame],  // must be a not signaled semaphore
            VK_NULL_HANDLE,
            imageIndex);

        return result;
    }

    VkResult VulkanSwapChain::SubmitCommandBuffers(
        const VkCommandBuffer* buffers, uint32_t* imageIndex) {
        if (m_ImagesInFlight[*imageIndex] != VK_NULL_HANDLE) {
            vkWaitForFences(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), 1, &m_ImagesInFlight[*imageIndex], VK_TRUE, UINT64_MAX);
        }
        m_ImagesInFlight[*imageIndex] = m_InFlightFences[m_CurrentFrame];

        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = { m_ImageAvailableSemaphores[m_CurrentFrame] };
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = buffers;

        VkSemaphore signalSemaphores[] = { m_RenderFinishedSemaphores[m_CurrentFrame] };
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        vkResetFences(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), 1, &m_InFlightFences[m_CurrentFrame]);
        if (vkQueueSubmit(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetGraphicsQueue(), 1, &submitInfo, m_InFlightFences[m_CurrentFrame]) !=
            VK_SUCCESS) {
            PF_CORE_ASSERT(false,"failed to submit draw command buffer!");
        }

        VkPresentInfoKHR presentInfo = {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = { m_SwapChain };
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;

        presentInfo.pImageIndices = imageIndex;

        auto result = vkQueuePresentKHR(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetPresentQueue(), &presentInfo);

        m_CurrentFrame = (m_CurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

        return result;
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

            if (vkCreateFramebuffer(
                Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(),
                &framebufferInfo,
                nullptr,
                &m_SwapChainFramebuffers[i]) != VK_SUCCESS) {
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
        m_ImageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        m_RenderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        m_InFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
        m_ImagesInFlight.resize(GetImageCount(), VK_NULL_HANDLE);

        VkSemaphoreCreateInfo semaphoreInfo = {};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo = {};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            if (vkCreateSemaphore(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), &semaphoreInfo, nullptr, &m_ImageAvailableSemaphores[i]) !=
                VK_SUCCESS ||
                vkCreateSemaphore(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), &semaphoreInfo, nullptr, &m_RenderFinishedSemaphores[i]) !=
                VK_SUCCESS ||
                vkCreateFence(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), &fenceInfo, nullptr, &m_InFlightFences[i]) != VK_SUCCESS) {
                PF_CORE_ASSERT(false, "failed to create synchronization objects for a frame!");
            }
        }
    }

    VkSurfaceFormatKHR VulkanSwapChain::ChooseSwapSurfaceFormat(
        const std::vector<VkSurfaceFormatKHR>& availableFormats) {
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

    VkPresentModeKHR VulkanSwapChain::ChooseSwapPresentMode(
const std::vector<VkPresentModeKHR>& availablePresentModes) {

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

}  
