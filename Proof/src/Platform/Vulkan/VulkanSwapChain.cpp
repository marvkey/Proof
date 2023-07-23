
// std
#include "Proofprch.h"

#include "VulkanSwapChain.h"
#include "Proof/Renderer/Renderer.h"
#include "VulkanGraphicsContext.h"
#include "Vulkan.h"
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
#include "VulkanRenderer/VulkanRenderer.h"
#include "VulkanImage.h"
#define VMA_IMPLEMENTATION

#include <vulkan/VulkanProofExternalLibs/vk_mem_alloc.h>

// images for vulkan swapchaing
/*
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <vk_mem_alloc.h>

// Initialize VMA and create a VmaAllocator object
VmaAllocator allocator;
VmaAllocatorCreateInfo allocatorCreateInfo = {};
allocatorCreateInfo.physicalDevice = physicalDevice;
allocatorCreateInfo.device = device;
vmaCreateAllocator(&allocatorCreateInfo, &allocator);

// Retrieve the swapchain images from the Vulkan API
std::vector<VkImage> swapchainImages;
uint32_t swapchainImageCount;
vkGetSwapchainImagesKHR(device, swapchain, &swapchainImageCount, nullptr);
swapchainImages.resize(swapchainImageCount);
vkGetSwapchainImagesKHR(device, swapchain, &swapchainImageCount, swapchainImages.data());

// Allocate memory for each swapchain image
std::vector<VmaAllocation> imageAllocations(swapchainImageCount);
std::vector<VkDeviceMemory> imageMemory(swapchainImageCount);
for (uint32_t i = 0; i < swapchainImageCount; i++) {
    VkImage image = swapchainImages[i];

    // Create VmaAllocationCreateInfo and fill it with image info
    VmaAllocationCreateInfo allocationCreateInfo = {};
    allocationCreateInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

    // Allocate memory for the image
    vmaAllocateMemoryForImage(allocator, image, &allocationCreateInfo, &imageAllocations[i], nullptr);

    // Bind the allocated memory to the image
    vkBindImageMemory(device, image, imageAllocations[i], 0);
}

// Use the swapchain images and their associated memory in your Vulkan rendering pipeline

// Free the allocated memory when you're done using the images
for (uint32_t i = 0; i < swapchainImageCount; i++) {
    vmaFreeMemory(allocator, imageAllocations[i]);
}

// Destroy the VmaAllocator
vmaDestroyAllocator(allocator);*/


namespace Proof
{

    VulkanSwapChain::VulkanSwapChain(ScreenSize extent)
        : m_WindowSize{ extent } {
        Init();
       
    }
    void VulkanSwapChain::Init() {
        CreateSwapChain();
        CreateSyncObjects();
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
            const auto& graphicsContext = VulkanRenderer::GetGraphicsContext();
            vkAcquireNextImageKHR(graphicsContext->GetDevice(), m_SwapChain, UINT64_MAX, m_ImageAvailableSemaphores[frameIndex], VK_NULL_HANDLE, imageIndex);

    }

    void VulkanSwapChain::SubmitCommandBuffers(std::vector<Count<RenderCommandBuffer>> buffers, uint32_t* imageIndex) {
        
        auto graphicsContext= VulkanRenderer::GetGraphicsContext();
        
        std::vector<VkCommandBuffer> submitCommandBuffers;
        for (auto& buffer : buffers) {
            submitCommandBuffers.emplace_back(buffer.As<VulkanRenderCommandBuffer>()->m_CommandBuffers[Renderer::GetCurrentFrame().FrameinFlight]);
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

        //if (vkQueueSubmit(graphicsContext->GetGraphicsQueue(), 1, &submitInfo, m_InFlightFences[Renderer::GetCurrentFrame().FrameinFlight]) !=
        //    VK_SUCCESS) {
        //    PF_CORE_ASSERT(false, "failed to submit draw command buffer!");
        //}


        VK_CHECK_RESULT(vkQueueSubmit(graphicsContext->GetGraphicsQueue(), 1, &submitInfo, m_InFlightFences[Renderer::GetCurrentFrame().FrameinFlight]),
            "Failed to submit draw command Buffer");
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

    ImageLayouts2D VulkanSwapChain::GetImageLayout()
    {
        /*
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
        return ImageLayouts2D(imagesCast);
        */
        return ImageLayouts2D(m_ImagesRefs);
    }

    Count<Image2D> VulkanSwapChain::GetImage(uint32_t imageIndex)
    {
        return m_ImagesRefs[imageIndex];
    }

    void VulkanSwapChain::CreateSwapChain() {
        auto graphicsContext = VulkanRenderer::GetGraphicsContext();

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
        uint32_t queueFamilyIndices[] = { indices.graphicsAndComputeFamily.value(), indices.presentFamily.value()};

        if (indices.graphicsAndComputeFamily.value() != indices.presentFamily) {
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
        // Enable required extension features
        //VkPhysicalDeviceVulkan12Features physicalDeviceFeature{};
        //physicalDeviceFeature.shader
        //physicalDeviceMultiviewFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_FEATURES_KHR;
        //physicalDeviceMultiviewFeatures.multiview = VK_TRUE;
        //createInfo.pNext = &physicalDeviceMultiviewFeatures;
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
        {
            m_SwapChainImageViews.resize(m_SwapChainImages.size());
            for (size_t i = 0; i < m_SwapChainImages.size(); i++)
            {
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

                if (vkCreateImageView(VulkanRenderer::GetGraphicsContext()->GetDevice(), &viewInfo, nullptr, &m_SwapChainImageViews[i]) !=
                    VK_SUCCESS)
                {
                    PF_CORE_ASSERT(false, "failed to create texture image view!");
                }
            }
        }
        m_SwapChainImageAllocation.resize(m_SwapChainImages.size());
        for (uint32_t i = 0; i < m_SwapChainImages.size(); i++)
        {
            VkImageCreateInfo imageCreateInfo = {};
            imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
            imageCreateInfo.format = m_ImageFormat;
            imageCreateInfo.extent.width = GetSwapChainExtent().X;
            imageCreateInfo.extent.height = GetSwapChainExtent().Y;
            imageCreateInfo.extent.depth = 1;
            imageCreateInfo.arrayLayers = 1;
            imageCreateInfo.mipLevels = 1;
            imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
            imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
            imageCreateInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
            imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

            //VmaAllocationCreateInfo imageAllocCreateInfo = {};
            //imageAllocCreateInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
            //vmaCreateImage(graphicsContext->GetVMA_Allocator(), &imageCreateInfo, &imageAllocCreateInfo, &m_SwapChainImages[i], &m_SwapChainImageAllocation[i], nullptr);
            // Create VmaAllocationCreateInfo and fill it with image info
            VmaAllocationCreateInfo allocationCreateInfo = {};
            allocationCreateInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

            // Allocate memory for the image
            vmaAllocateMemoryForImage(graphicsContext->GetVMA_Allocator(), m_SwapChainImages[i], &allocationCreateInfo, &m_SwapChainImageAllocation[i], nullptr);
            // Bind the allocated memory to the image
            vkBindImageMemory(graphicsContext->GetDevice(), m_SwapChainImages[i], m_SwapChainImageAllocation[i]->GetMemory(), m_SwapChainImageAllocation[i]->GetOffset());
        }
        VkSamplerCreateInfo samplerCreateInfo = {};
        samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerCreateInfo.magFilter = VK_FILTER_LINEAR;      // Magnification filter (for oversampling)
        samplerCreateInfo.minFilter = VK_FILTER_LINEAR;      // Minification filter (for undersampling)
        samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;  // U coordinate wrap mode
        samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;  // V coordinate wrap mode
        samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;  // W coordinate wrap mode
        samplerCreateInfo.anisotropyEnable = VK_FALSE;       // Anisotropic filtering (if supported)
        samplerCreateInfo.maxAnisotropy = 1.0f;             // Anisotropy level (if enabled)
        samplerCreateInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;  // Border color (if address mode is VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER)
        samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;   // If set to VK_TRUE, the coordinates are not normalized to [0, 1]
        samplerCreateInfo.compareEnable = VK_FALSE;            // Enable comparison against a reference value (for shadow mapping)
        samplerCreateInfo.compareOp = VK_COMPARE_OP_ALWAYS;    // Comparison operator (ignored if compareEnable is VK_FALSE)
        samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;  // Mipmap filtering mode
        samplerCreateInfo.mipLodBias = 0.0f;                   // Mipmap LOD bias
        samplerCreateInfo.minLod = 0.0f;                       // Minimum level-of-detail
        samplerCreateInfo.maxLod = 1.0f;                       // Maximum level-of-detail
        m_ImagesRefs.resize(GetImageCount());
        for (int i = 0; i < m_ImagesRefs.size(); i++)
        {
            ImageConfiguration imageConfig;
            imageConfig.DebugName = fmt::format("Swapchain Image Index: {}", i);
            imageConfig.Format = GetImageFormat();
            imageConfig.Usage = ImageUsage::SwapChain;
            imageConfig.Width = GetSwapChainExtent().X;
            imageConfig.Height = GetSwapChainExtent().Y;

            VulkanImageInfo imagInfo;
            imagInfo.ImageAlloc.Image = m_SwapChainImages[i];
            imagInfo.ImageAlloc.Allocation =  nullptr;
            imagInfo.ImageView = m_SwapChainImageViews[i];
            auto [sampler, hash] = graphicsContext->GetOrCreateSampler(samplerCreateInfo);
            imagInfo.Sampler = sampler;

            m_ImagesRefs[i] = Count<VulkanImage2D>::Create(imageConfig, imagInfo,hash);
        }   
        PF_ENGINE_INFO("Swapchain created with Width: {} Height: {}", m_SwapChainExtent.X, m_SwapChainExtent.Y);
    }

    void VulkanSwapChain::CreateSyncObjects() {
        auto graphicsContext = VulkanRenderer::GetGraphicsContext();

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

        PF_ENGINE_INFO("Swapchain Sync objects created");

    }

    void VulkanSwapChain::Resize(ScreenSize size) {
        const auto& device = VulkanRenderer::GetGraphicsContext()->GetDevice();
        m_WindowSize = size;
        vkDeviceWaitIdle(device);
        {
            
            for (size_t i = 0; i < m_SwapChainImageViews.size(); i++) {
                //vkDestroyImageView(device, m_SwapChainImageViews[i], nullptr);
                //vmaFreeMemory()
            }
            m_ImagesRefs.clear();
            vkDestroySwapchainKHR(device, m_SwapChain, nullptr);
            m_SwapChainImageViews.clear();
            m_SwapChainImages.clear();
            m_SwapChainImageAllocation.clear();
            m_SwapChain = nullptr;
        }

        CreateSwapChain();
        //for (auto i : FrameBuffers)
        //{
        //    if(i->m_DepthImage)
        //    i->Release();
        //    i->Init();
        //}
    }

    void VulkanSwapChain::CleanUp() {
        auto device = VulkanRenderer::GetGraphicsContext()->GetDevice();

        {
            m_ImagesRefs.clear();

           // for (size_t i = 0; i < m_SwapChainImageViews.size(); i++) {
           //     Renderer::SubmitDatafree([imageViews = m_SwapChainImageViews[i],images = m_SwapChainImages[i],device = device] {
           //         vkDestroyImageView(device,imageViews , nullptr);
           //         //vkDestroyImage(device, images, nullptr);
           //     });
           //
           // }
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
        #if 1
        PF_ENGINE_INFO("Present mode: V-Sync");
        return VK_PRESENT_MODE_FIFO_KHR;
        #else 
        PF_ENGINE_INFO("Present mode: Immediate");
        return VK_PRESENT_MODE_IMMEDIATE_KHR;
        #endif
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
        if (vkGetFenceStatus(VulkanRenderer::GetGraphicsContext()->GetDevice(), m_InFlightFences[frameIndex]) == VK_NOT_READY)
        {
            vkWaitForFences(VulkanRenderer::GetGraphicsContext()->GetDevice(), 1, &m_InFlightFences[frameIndex], VK_TRUE, UINT64_MAX);    // wait indefinitely instead of periodically checking
        }
    }

    void VulkanSwapChain::ResetFences(uint32_t frameIndex) {
        vkResetFences(VulkanRenderer::GetGraphicsContext()->GetDevice(), 1, &m_InFlightFences[frameIndex]);
    }

    VkFormat VulkanSwapChain::FindDepthFormat() {
        return 
           VulkanRenderer::GetGraphicsContext()->FindSupportedFormat(
            { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
            VK_IMAGE_TILING_OPTIMAL,
            VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
    }
    
}  
