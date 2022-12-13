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
		VulkanScreenFrameBuffer(Vector2 imageSize, Count<class VulkanRenderPass> renderPass = nullptr, bool includeDepth = true):
			m_ImageSize{ imageSize }, m_RenderPass{ renderPass }, m_Depth{includeDepth}
		{
			CreateImageViews();
			CreateDepthResources();
			CreateFramebuffers();
		}
		virtual ~VulkanScreenFrameBuffer();
		VkFramebuffer GetFrameBuffer(uint32_t imageIndex = Renderer::GetCurrentFrame().ImageIndex) {
			return m_Framebuffers[imageIndex];
		}
		void Resize(uint32_t width, uint32_t height, Count<class VulkanRenderPass> renderPass = nullptr);
		void Resize(uint32_t width, uint32_t height) {
			Resize(width, height, m_RenderPass);
		}
		void Resize(Vector2 imageSize, Count<class VulkanRenderPass> renderPass);
		void Resize(Vector2 imageSize) {
			Resize(imageSize.X, imageSize.Y, m_RenderPass);
		}

		virtual uint32_t GetFrameWidth() {return m_ImageSize.X;};
		virtual uint32_t GetFrameHeight() {return m_ImageSize.Y;}
		
		Vector2 m_ImageSize;
		Count<class VulkanRenderPass> m_RenderPass;
		std::vector<VkFramebuffer> m_Framebuffers;
		std::vector<VkImageView> m_DepthImageViews;
		
		std::vector< VulkanImage> m_DepthImages;
		std::vector<VulkanImage> m_Images;
		std::vector<VkImageView> m_ImageViews;

		std::vector<VkSampler> m_ImageSampler;
		bool m_Depth;

		void* GetTexture();
	private:
		void Release();
		void Recreate() {
			*this = VulkanScreenFrameBuffer(m_ImageSize, m_RenderPass, m_Depth);
		}
		void CreateImageViews();
		void CreateDepthResources();
		void CreateFramebuffers();
		friend class VulkanRenderer;
	};
}
