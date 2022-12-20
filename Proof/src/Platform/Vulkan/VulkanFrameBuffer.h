#pragma once
#include "Proof/Core/Core.h"
#include <vulkan/vulkan.h>
#include "VulkanUtils/VulkanBufferBase.h"
#include "Proof/Renderer/ScreenFrameBuffer.h"
#include "Proof/Renderer/Renderer.h"
namespace Proof
{

	class VulkanScreenFrameBuffer: public ScreenFrameBuffer {
	public:
		VulkanScreenFrameBuffer(Vector2 imageSize, Count<class RenderPass> renderPass = nullptr, bool screenPresent = false):
			m_ImageSize{ imageSize }, m_RenderPass{ renderPass }, m_ScreenPresent{ screenPresent }
		{
			
			Init();
		}
		virtual ~VulkanScreenFrameBuffer() {
			Release();
		}
		VkFramebuffer GetFrameBuffer(uint32_t imageIndex = Renderer::GetCurrentFrame().ImageIndex) {
			return m_Framebuffers[imageIndex];
		}
		void Resize(uint32_t width, uint32_t height, Count<class RenderPass> renderPass =nullptr);
		void Resize(Vector2 imageSize, Count<class RenderPass> renderPass = nullptr);
		

		virtual uint32_t GetFrameWidth() {return m_ImageSize.X;};
		virtual uint32_t GetFrameHeight() {return m_ImageSize.Y;}
		
		Vector2 m_ImageSize;
		Count<class RenderPass> m_RenderPass;
		std::vector<VkFramebuffer> m_Framebuffers;
		std::vector<VkImageView> m_DepthImageViews;
		
		std::vector< VulkanImage> m_DepthImages;
		std::vector<VulkanImage> m_Images;
		std::vector<VkImageView> m_ImageViews;

		std::vector<VkSampler> m_ImageSampler;

		void* GetTexture();
		bool IsScreenPresent() {
			return m_ScreenPresent;
		}
	private:
		void Init();
		void CreateScreenPresent();
		bool m_ScreenPresent;

		void Release();
		void CreateImageViews();
		void CreateDepthResources();
		void CreateFramebuffers();
		friend class VulkanRenderer;
		friend class VulkanSwapChain;
	};
}
