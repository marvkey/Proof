#pragma once
#include "Proof/Core/Core.h"
#include <vulkan/vulkan.h>
#include "VulkanUtils/VulkanBufferBase.h"
#include "Proof/Renderer/FrameBuffer.h"
#include "Proof/Renderer/Renderer.h"
#include "Proof/Renderer/Texture.h"
namespace Proof
{
	struct VulkanFrameBufferImages {

		VulkanFrameBufferImages()
		{
			RefImages = {};
		}

		VulkanFrameBufferImages(const VulkanFrameBufferImages& other)
		{
			RefImages = other.RefImages;
		}
		VulkanFrameBufferImages& operator=(const VulkanFrameBufferImages& right) {
			return *this;
		}
		std::vector<Count<Image2D>> RefImages;
	};
	class VulkanFrameBuffer : public FrameBuffer {
	public:
		VulkanFrameBuffer(const FrameBufferConfig& attachments);
		virtual ~VulkanFrameBuffer();
		VkFramebuffer GetFrameBuffer(uint32_t index = Renderer::GetCurrentFrame().ImageIndex);
		const FrameBufferConfig& GetConfig() const  {
			return m_Config;
		}

		// index is the image attachment
		virtual Count<Image2D> GetColorAttachmentImage(uint32_t index, uint32_t imageIndex = Renderer::GetCurrentFrame().ImageIndex);
		virtual Count<Image2D> GetDepthImage(uint32_t imageIndex = Renderer::GetCurrentFrame().ImageIndex);

		// index is the image attachment
		virtual ImageLayouts2D GetColorAttachmentImageLayout(uint32_t index);
		virtual ImageLayouts2D GetDepthImageLayout();

		virtual bool HasDepthImage();
		virtual bool HasColorAttachment();
		virtual void Resize(Vector2 imageSize);

		virtual void Copy(Count<FrameBuffer> framebUFFer);
	private:
		void Init();
		void SetUpAttachments();
		std::vector<VulkanFrameBufferImages> m_ColorImages;
		// can only have one depth image thats how vulkan works
		VulkanFrameBufferImages m_DepthImage;
		FrameBufferConfig m_Config;
		ImageFormat m_DepthFormat = ImageFormat::None;
		void SetDepth(const FrameBufferImageConfig& imageAttach);
		void AddImage(const FrameBufferImageConfig& imageAttach);

		void CreateFramebuffer();
		void Release();
		std::vector<VkFramebuffer> m_Framebuffers;
		friend class VulkanSwapChain;
	};
	
}
