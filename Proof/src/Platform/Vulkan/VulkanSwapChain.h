#pragma once

// vulkan headers
#include <vulkan/vulkan.h>
// std lib headers
#include <string>
#include <vector>
#include "Proof/Renderer/Renderer.h"
#include "VulkanCommandBuffer.h"
#include "Proof/Core/Core.h"
#include "Proof/Renderer/SwapChain.h"
namespace Proof
{
    class VulkanSwapChain : public SwapChain {
    public:

        VulkanSwapChain(ScreenSize windowExtent);
        ~VulkanSwapChain();
        friend class ImGuiLayer;
        VkImageView GetImageView(int index) { return m_SwapChainImageViews[index]; }
        size_t GetImageCount()const { return m_ImageCount; }
        ImageFormat GetImageFormat();
        ImageFormat GetDepthFormat();
        ScreenSize GetSwapChainExtent()const { return m_SwapChainExtent; }
        ScreenSize GetWindowSize()const { return m_WindowSize; }

        float ExtentAspectRatio() {
            return static_cast<float>(m_SwapChainExtent.X) / static_cast<float>(m_SwapChainExtent.Y);
        }

        void AcquireNextImage(uint32_t* imageIndex, uint32_t frameIndex = Renderer::GetCurrentFrame().FrameinFlight);
        void WaitAndResetFences(uint32_t frameIndex = Renderer::GetCurrentFrame().FrameinFlight);
        void WaitFences(uint32_t frameIndex = Renderer::GetCurrentFrame().FrameinFlight);
        void ResetFences(uint32_t frameIndex = Renderer::GetCurrentFrame().FrameinFlight);
        void SubmitCommandBuffers(std::vector<Count<RenderCommandBuffer>> buffers, uint32_t* imageIndex);

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
        virtual ImageLayouts GetImageLayout();

    private:
        VkFormat FindDepthFormat();

        VkSurfaceFormatKHR m_SurfaceFormat;
        VkPresentModeKHR m_PresentMode;
        uint32_t m_ImageCount;
        void CreateSwapChain();
        void CreateImageViews();
        void CreateSyncObjects();
        void Resize(ScreenSize size);
        void CleanUp();

        void Init();
        // Helper functions
        VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

        VkFormat m_ImageFormat;
        VkFormat m_SwapChainDepthFormat;
        ScreenSize m_WindowSize;
        ScreenSize m_SwapChainExtent;
        VkSwapchainKHR m_SwapChain;

        std::vector<class VulkanFrameBuffer*> FrameBuffers;
        std::vector<class VulkanGraphicsPipeline*> GraphicsPipelines;
        std::vector<VkImageView> m_SwapChainImageViews;
        std::vector<VkImage> m_SwapChainImages;
        std::vector<VkSemaphore> m_ImageAvailableSemaphores;
        std::vector<VkSemaphore> m_RenderFinishedSemaphores;
        std::vector<VkFence> m_InFlightFences;
        friend class VulkanRenderer;
        // temporary
        friend class VulkanFrameBuffer;
    };

}  
