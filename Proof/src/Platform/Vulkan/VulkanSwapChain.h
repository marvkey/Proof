#pragma once

// vulkan headers
#include <vulkan/vulkan.h>
// std lib headers
#include <string>
#include <vector>
#include "Proof/Renderer/Renderer.h"
#include "VulkanCommandBuffer.h"
#include "Proof/Core/Core.h"
namespace Proof
{
    class VulkanSwapChain {
    public:

        VulkanSwapChain(VkExtent2D windowExtent);
        ~VulkanSwapChain();
        friend class ImGuiLayer;
        VkFramebuffer GetFrameBuffer(int index)const { return m_SwapChainFramebuffers[index]; }
        VkRenderPass GetRenderPass()const { return m_RenderPass; }
        VkImageView GetImageView(int index) { return m_SwapChainImageViews[index]; }
        size_t GetImageCount()const { return m_ImageCount; }
        VkFormat GetImageFormat()const { return m_ImageFormat; }
        ScreenSize GetSwapChainExtent()const { return m_SwapChainExtent; }
        //uint32_t GetWidth() { return m_SwapChainExtent.width; }
        //uint32_t GetHeight() { return m_SwapChainExtent.height; }


        float ExtentAspectRatio() {
            return static_cast<float>(m_SwapChainExtent.X) / static_cast<float>(m_SwapChainExtent.Y);
        }
        VkFormat GetDepthFormat();

        void AcquireNextImage(uint32_t* imageIndex, uint32_t frameIndex= Renderer::GetCurrentFrame().FrameinFlight);
        void WaitAndResetFences(uint32_t frameIndex = Renderer::GetCurrentFrame().FrameinFlight);
        void WaitFences(uint32_t frameIndex = Renderer::GetCurrentFrame().FrameinFlight);
        void ResetFences(uint32_t frameIndex = Renderer::GetCurrentFrame().FrameinFlight);
        void Present();
        void SubmitCommandBuffers(std::vector<Count<VulkanCommandBuffer>> buffers, uint32_t* imageIndex);

        bool CompareSwapFormats(const VulkanSwapChain& swapChain) {
            return swapChain.m_SwapChainDepthFormat == m_SwapChainDepthFormat && swapChain.m_ImageFormat == m_ImageFormat;
        }
        VkSwapchainKHR GetSwapChain() {
            return m_SwapChain;
        }
        VkSurfaceFormatKHR GetSurfaceFormat() {
            return m_SurfaceFormat;
        }
        VkPresentModeKHR GetPresentMode() {
            return m_PresentMode;
        }
    private:
        VkSurfaceFormatKHR m_SurfaceFormat;
        VkPresentModeKHR m_PresentMode;
        uint32_t m_ImageCount;
        void CreateSwapChain();
        void CreateImageViews();
        void CreateDepthResources();
        void CreateRenderPass();
        void CreateFramebuffers();
        void CreateSyncObjects();
        void Recreate(VectorTemplate2<uint32_t> size);
        void CleanUp();
        // Helper functions
        VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

        VkFormat m_ImageFormat;
        VkFormat m_SwapChainDepthFormat;
        VkRenderPass m_RenderPass;
        ScreenSize m_WindowSize;
        ScreenSize m_SwapChainExtent;
        VkSwapchainKHR m_SwapChain;

        std::vector<VkImage>m_DepthImages;
        std::vector<VkImageView>m_DepthImageViews;
        std::vector<VkDeviceMemory>m_DepthImageMemorys;

        std::vector<VkFramebuffer> m_SwapChainFramebuffers;
        std::vector<VkImageView> m_SwapChainImageViews;
        std::vector<VkImage> m_SwapChainImages;
        std::vector<VkSemaphore> m_ImageAvailableSemaphores;
        std::vector<VkSemaphore> m_RenderFinishedSemaphores;
        std::vector<VkFence> m_InFlightFences;
        friend class VulkanRenderer;
    };

}  
