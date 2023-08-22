#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Renderer/SwapChain.h"
#include "VulkanUtils/VulkanBufferBase.h"

#include <vulkan/vulkan.h>
#include <string>
#include <vector>
namespace Proof
{
    class VulkanSwapChain : public SwapChain {
    public:

        VulkanSwapChain(ScreenSize windowExtent, bool vsync);
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

        void AcquireNextImage(uint32_t* imageIndex, uint32_t frameInfllight);
        void WaitAndResetFences(uint32_t frameInfllight);
        void WaitFences(uint32_t frameInfllight);
        void ResetFences(uint32_t frameInfllight);
        void SubmitCommandBuffers(std::vector<Count<class RenderCommandBuffer>> buffers, uint32_t* imageIndex);
        void Present(uint32_t* imageIndex);
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
        virtual ImageLayouts2D GetImageLayout();
        virtual Count<Image2D>  GetImage(uint32_t imageIndex);

        VkCommandBuffer GetCommandBuffer(uint32_t frameInFlight)
        {
            return m_CommandBuffers[frameInFlight].CommandBuffer;
        }

        virtual void SetVsync(bool vsync);
        virtual bool GetVsync() { return m_Vsync; }
    private:
        bool m_Vsync;
        struct SwapchainCommandBuffer
        {
            VkCommandPool CommandPool = nullptr;
            VkCommandBuffer CommandBuffer = nullptr;
        };
        std::vector<SwapchainCommandBuffer> m_CommandBuffers;
        VkFormat FindDepthFormat();

        VkSurfaceFormatKHR m_SurfaceFormat;
        VkPresentModeKHR m_PresentMode;
        uint32_t m_ImageCount;
        void CreateSwapChain();
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
        std::vector<VmaAllocation> m_SwapChainImageAllocation;
        std::vector<VkSemaphore> m_ImageAvailableSemaphores;
        std::vector<VkSemaphore> m_RenderFinishedSemaphores;
        std::vector<VkFence> m_InFlightFences;

        std::vector<Count<class Image2D>> m_ImagesRefs;
        friend class VulkanRenderer;
        // temporary
        friend class VulkanFrameBuffer;
    };

}  
