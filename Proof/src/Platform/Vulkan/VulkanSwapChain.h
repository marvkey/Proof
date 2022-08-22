#pragma once

// vulkan headers
#include <vulkan/vulkan.h>
// std lib headers
#include <string>
#include <vector>
#include "Proof/Renderer/Renderer.h"
namespace Proof
{
    class VulkanSwapChain {
    public:

        VulkanSwapChain(VkExtent2D windowExtent);
        ~VulkanSwapChain();

        VkFramebuffer GetFrameBuffer(int index)const { return m_SwapChainFramebuffers[index]; }
        VkRenderPass GetRenderPass()const { return m_RenderPass; }
        VkImageView GetImageView(int index) { return m_SwapChainImageViews[index]; }
        size_t GetImageCount()const { return m_SwapChainImages.size(); }
        VkFormat GetSwapChainImageFormat()const { return m_SwapChainImageFormat; }
        VkExtent2D GetSwapChainExtent()const { return m_SwapChainExtent; }
        uint32_t GetWidth() { return m_SwapChainExtent.width; }
        uint32_t GetHeight() { return m_SwapChainExtent.height; }


        float ExtentAspectRatio() {
            return static_cast<float>(m_SwapChainExtent.width) / static_cast<float>(m_SwapChainExtent.height);
        }
        VkFormat FindDepthFormat();

        void AcquireNextImage(uint32_t* imageIndex, uint32_t frameIndex= Renderer::GetCurrentFrame());
        void WaitAndResetFences(uint32_t frameIndex = Renderer::GetCurrentFrame());
        void WaitFences(uint32_t frameIndex = Renderer::GetCurrentFrame());
        void ResetFences(uint32_t frameIndex = Renderer::GetCurrentFrame());

        void SubmitCommandBuffers(VkCommandBuffer commnadBuffer, uint32_t* imageIndex, uint32_t frameIndex = Renderer::GetCurrentFrame());
        void Recreate();

        bool CompareSwapFormats(const VulkanSwapChain& swapChain) {
            return swapChain.m_SwapChainDepthFormat == m_SwapChainDepthFormat && swapChain.m_SwapChainImageFormat == m_SwapChainImageFormat;
        }

    private:
        void CreateSwapChain();
        void CreateImageViews();
        void CreateDepthResources();
        void CreateRenderPass();
        void CreateFramebuffers();
        void CreateSyncObjects();

        // Helper functions
        VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

        VkFormat m_SwapChainImageFormat;
        VkFormat m_SwapChainDepthFormat;
        VkExtent2D m_SwapChainExtent;

        std::vector<VkFramebuffer> m_SwapChainFramebuffers;
        VkRenderPass m_RenderPass;

        std::vector<VkImage> m_DepthImages;
        std::vector<VkDeviceMemory> m_DepthImageMemorys;
        std::vector<VkImageView> m_DepthImageViews;
        std::vector<VkImage> m_SwapChainImages;
        std::vector<VkImageView> m_SwapChainImageViews;

        VkExtent2D m_WindowExtent;

        VkSwapchainKHR m_SwapChain;

        std::vector<VkSemaphore> m_ImageAvailableSemaphores;
        std::vector<VkSemaphore> m_RenderFinishedSemaphores;
        std::vector<VkFence> m_InFlightFences;
        std::vector<VkFence> m_ImagesInFlight;
        friend class VulkanRenderer;
    };

}  
