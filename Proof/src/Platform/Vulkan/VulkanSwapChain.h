#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Renderer/SwapChain.h"
#include "VulkanUtils/VulkanBufferBase.h"

#include <vulkan/vulkan.h>
#include <string>
#include <vector>
namespace Proof
{
	class VulkanSwapChain : public SwapChain
	{
	public:

		VulkanSwapChain(const class Window* window);
		void InitSurface();
		void Create(uint32_t width, uint32_t height, bool vsync);
		void Destroy();

		void Resize(uint32_t width, uint32_t height);
		virtual void Resize(glm::uvec2 size) { Resize(size.x, size.y); }

		void BeginFrame();
		void EndFrame();
		void Present();

		RendererConfig GetRenderConfig();
		virtual uint32_t GetFrameIndex() { return m_CurrentBufferIndex; }
		virtual uint32_t GetImageIndex() { return m_CurrentImageIndex; }

		size_t GetImageCount() const { return m_ImageCount; }

		uint32_t GetWidth() const { return m_Width; }
		uint32_t GetHeight() const { return m_Height; }

		VkRenderPass GetRenderPass() { return m_RenderPass; }

		VkFramebuffer GetCurrentFramebuffer() { return GetFramebuffer(m_CurrentImageIndex); }
		VkCommandBuffer GetCurrentDrawCommandBuffer() { return GetDrawCommandBuffer(m_CurrentBufferIndex); }

		VkFormat GetColorFormatVulkan() { return m_ColorFormat; }
		VkFormat GetDepthFormatVulkan() { return m_DepthFormat; }
		ImageFormat GetColorFormat();
		ImageFormat GetDepthFormat();

		VkFramebuffer GetFramebuffer(uint32_t index)
		{
			PF_CORE_ASSERT(index < m_Framebuffers.size());
			return m_Framebuffers[index];
		}
		VkCommandBuffer GetDrawCommandBuffer(uint32_t index)
		{
			PF_CORE_ASSERT (index < m_CommandBuffers.size());
			return m_CommandBuffers[index].CommandBuffer;
		}

		VkSemaphore GetRenderCompleteSemaphore() { return m_Semaphores.RenderComplete; }

		virtual glm::uvec2 GetSize() {return { m_Width,m_Height };}

		virtual void SetVsync(bool vsync);
		virtual bool GetVsync() { return m_VSync; }
	private:
		uint32_t AcquireNextImage();

		void FindImageFormatAndColorSpace();
		void Build();
	private:
		bool m_InFrame = false;
		VkInstance m_Instance = nullptr;
		bool m_VSync = false;

		VkFormat m_ColorFormat;
		VkFormat m_DepthFormat;
		VkColorSpaceKHR m_ColorSpace;

		VkSwapchainKHR m_SwapChain = nullptr;
		uint32_t m_ImageCount = 0;
		std::vector<VkImage> m_VulkanImages;

		struct SwapchainImage
		{
			VkImage Image = nullptr;
			VkImageView ImageView = nullptr;
		};
		std::vector<SwapchainImage> m_Images;

		struct
		{
			VkImage Image = nullptr;
			VmaAllocation MemoryAlloc = nullptr;
			VkImageView ImageView = nullptr;
		} m_DepthStencil;

		std::vector<VkFramebuffer> m_Framebuffers;

		struct SwapchainCommandBuffer
		{
			VkCommandPool CommandPool = nullptr;
			VkCommandBuffer CommandBuffer = nullptr;
		};
		std::vector<SwapchainCommandBuffer> m_CommandBuffers;

		struct
		{
			// Swap chain
			VkSemaphore PresentComplete = nullptr;
			// Command buffer
			VkSemaphore RenderComplete = nullptr;
		} m_Semaphores;
		VkSubmitInfo m_SubmitInfo;

		std::vector<VkFence> m_WaitFences;

		VkRenderPass m_RenderPass = nullptr;
		uint32_t m_CurrentBufferIndex = 0;
		uint32_t m_CurrentImageIndex = 0;

		uint32_t m_QueueNodeIndex = UINT32_MAX;
		uint32_t m_Width = 0, m_Height = 0;

		VkSurfaceKHR m_Surface;

		const Window* m_Window;
		friend class VulkanContext;
    };

}  
