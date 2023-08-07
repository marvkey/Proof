#pragma once
#include "Proof/Core/Core.h"
#include <vulkan/vulkan.h>
#include "VulkanUtils/VulkanBufferBase.h"
#include "Proof/Renderer/FrameBuffer.h"
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
		std::vector<Count<Image>> RefImages;
	};
	class VulkanFrameBuffer : public FrameBuffer {
	public:
		VulkanFrameBuffer(const FrameBufferConfig& attachments);
		virtual ~VulkanFrameBuffer();
		VkFramebuffer GetFrameBuffer(uint32_t swapchainImageIndex);
		const FrameBufferConfig& GetConfig() const  {
			return m_Config;
		}
		const FrameBufferConfig& GetConfig() {
			return m_Config;
		}
		// index is the image attachment
		virtual Count<Image> GetColorAttachmentImage(uint32_t swapchainImageIndex, uint32_t index);
		virtual Count<Image> GetDepthImage(uint32_t swapchainImageIndex);

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
