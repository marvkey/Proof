
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
#include "VulkanRenderPass.h"
#include "VulkanFrameBuffer.h"
#include "VulkanTexutre.h"
#include "VulkanUtils/VulkanConvert.h"
namespace Proof
{

    VulkanSwapChain::VulkanSwapChain(ScreenSize extent)
        : m_WindowSize{ extent } {
        Init();
       
    }
    void VulkanSwapChain::Init() {
        CreateSwapChain();
        CreateSyncObjects();
        CreateImageViews();
    }

    VulkanSwapChain::~VulkanSwapChain() {
        CleanUp();
    }

    ImageFormat VulkanSwapChain::GetImageFormat()
    {
        return Utils::VulkanFormatToProofFormat(m_ImageFormat);
    }

    ImageFormat VulkanSwapChain::GetDepthFormat()
    {
        return Utils::VulkanFormatToProofFormat(m_SwapChainDepthFormat);
    }

    void VulkanSwapChain::AcquireNextImage(uint32_t* imageIndex, uint32_t frameIndex) {
            const auto& graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();
            vkAcquireNextImageKHR(graphicsContext->GetDevice(), m_SwapChain, UINT64_MAX, m_ImageAvailableSemaphores[frameIndex], VK_NULL_HANDLE, imageIndex);

    }

    void VulkanSwapChain::SubmitCommandBuffers(std::vector<Count<RenderCommandBuffer>> buffers, uint32_t* imageIndex) {
        auto graphicsContext= Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();
        
        std::vector<VkCommandBuffer> submitCommandBuffers;
        for (auto& buffer : buffers) {
            submitCommandBuffers.emplace_back(buffer->As<VulkanRenderCommandBuffer>()->m_CommandBuffers[Renderer::GetCurrentFrame().FrameinFlight]);
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

    ImageLayouts VulkanSwapChain::GetImageLayout()
    {
        std::vector<VulkanImage> images;
        images.resize(m_ImageCount);
        int index = 0;
        for (auto& image : images)
        {
            image = VulkanImage(nullptr, GetImageFormat(), {(float) m_SwapChainExtent.X, (float)m_SwapChainExtent.Y }, { nullptr,m_SwapChainImageViews[index] });
            index++;
        }
        std::vector<Image> imagesCast(m_ImageCount);
        for (uint32_t i = 0; i < imagesCast.size(); i++)
        {
            imagesCast[i] = images[i];
        }
        return ImageLayouts(imagesCast);
    }

    void VulkanSwapChain::CreateSwapChain() {
        auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();

        SwapChainSupportDetails swapChainSupport = graphicsContext->GetSwapChainSupport();

        m_SurfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
        m_PresentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
        VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);

        m_ImageCount = Renderer::GetConfig().MaxImageCount;
        /*
        m_ImageCount = swapChainSupport.capabilities.minImageCount + 1;
        if (swapChainSupport.capabilities.maxImageCount > 0 && m_ImageCount > swapChainSupport.capabilities.maxImageCount) {
            m_ImageCount = swapChainSupport.capabilities.maxImageCount;
        }
        */
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
        m_SwapChainDepthFormat = FindDepthFormat();
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
            viewInfo.subresourceRange.layerCount = 1;

            if (vkCreateImageView(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), &viewInfo, nullptr, &m_SwapChainImageViews[i]) !=
                VK_SUCCESS) {
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

    void VulkanSwapChain::Resize(ScreenSize size) {
        const auto& device = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice();
        m_WindowSize = size;
        vkDeviceWaitIdle(device);
        {
            
            for (size_t i = 0; i < m_SwapChainImageViews.size(); i++) {
                vkDestroyImageView(device, m_SwapChainImageViews[i], nullptr);
                vkDestroyImage(device, m_SwapChainImages[i], nullptr);
            }
            m_SwapChainImageViews.clear();
            vkDestroySwapchainKHR(device, m_SwapChain, nullptr);
        }
        CreateSwapChain();
        CreateImageViews();
        //for (auto i : FrameBuffers)
        //    i->Init();
    }

    void VulkanSwapChain::CleanUp() {
        auto device = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice();

        {
            for (size_t i = 0; i < m_SwapChainImageViews.size(); i++) {
                Renderer::SubmitDatafree([imageViews = m_SwapChainImageViews[i],images = m_SwapChainImages[i], device = device] {
                    vkDestroyImageView(device,imageViews , nullptr);
                    //vkDestroyImage(device, images, nullptr);
                });

            }
            Renderer::SubmitDatafree([swapchain = m_SwapChain,device = device] {
                vkDestroySwapchainKHR(device, swapchain, nullptr);
            });

            m_SwapChainImageViews.clear();
            m_SwapChainImages.clear();
            m_SwapChain = nullptr;
        }


        for (uint32_t i = 0; i < Renderer::GetConfig().FramesFlight; i++) {
            Renderer::SubmitDatafree([finishSemaphore = m_RenderFinishedSemaphores[i], imageAvailableSemphore = m_ImageAvailableSemaphores[i], fences = m_InFlightFences[i], device = device]{
                vkDestroySemaphore(device, finishSemaphore, nullptr);
                vkDestroySemaphore(device, imageAvailableSemphore, nullptr);
                vkDestroyFence(device, fences, nullptr);
            });

        }
        m_RenderFinishedSemaphores.clear();
        m_ImageAvailableSemaphores.clear();
        m_InFlightFences.clear();
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
            actualExtent.width = Math::Max(
                capabilities.minImageExtent.width,
                Math::Min(capabilities.maxImageExtent.width, actualExtent.width));
            actualExtent.height = Math::Max(
                capabilities.minImageExtent.height,
                Math::Min(capabilities.maxImageExtent.height, actualExtent.height));

            return actualExtent;
        }
    }

   

    void VulkanSwapChain::WaitAndResetFences(uint32_t frameIndex) {
        WaitFences(frameIndex);
        ResetFences(frameIndex);
    }

    void VulkanSwapChain::WaitFences(uint32_t frameIndex) {
        vkWaitForFences(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), 1, &m_InFlightFences[frameIndex], VK_TRUE, UINT64_MAX);    // wait indefinitely instead of periodically checking
    }

    void VulkanSwapChain::ResetFences(uint32_t frameIndex) {
        vkResetFences(Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->GetDevice(), 1, &m_InFlightFences[frameIndex]);
    }

    VkFormat VulkanSwapChain::FindDepthFormat() {
        return 
           Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>()->FindSupportedFormat(
            { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
            VK_IMAGE_TILING_OPTIMAL,
            VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
    }
    
}  
