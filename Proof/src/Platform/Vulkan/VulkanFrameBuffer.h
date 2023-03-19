#pragma once
#include "Proof/Core/Core.h"
#include <vulkan/vulkan.h>
#include "VulkanUtils/VulkanBufferBase.h"
#include "Proof/Renderer/FrameBuffer.h"
#include "Proof/Renderer/Renderer.h"
namespace Proof
{

	struct VulkanFrameBufferImages {
		VulkanFrameBufferImages() 
		{
			Images = {};
			ImageViews = {};
			ImageSampler = {};
		}
		VulkanFrameBufferImages(bool imageAttach) :
			ImageAttached(imageAttach)
		{
			Images = {};
			ImageViews = {};
			ImageSampler = {};
		}

		VulkanFrameBufferImages(const VulkanFrameBufferImages& other):
			ImageAttached(other.ImageAttached)
		{
			Images = other.Images;
			ImageViews = other.ImageViews;
			ImageSampler = other.ImageSampler;
		}
		VulkanFrameBufferImages& operator=(const VulkanFrameBufferImages& right) {
			return *this;
		}
		std::vector<VulkanImageAlloc> Images;
		std::vector<VkImageView> ImageViews;
		std::vector<VkSampler> ImageSampler;

		// weathere fraemBuffer is responsible for deleting casue the framebufer never created this image
		 const bool ImageAttached = false;
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
		virtual Image GetColorAttachmentImage(uint32_t index, uint32_t imageIndex = Renderer::GetCurrentFrame().ImageIndex);
		virtual VulkanFrameBufferImages GetColorAttachmentFrameBufferImage(uint32_t index);
		virtual Image GetDepthImage(uint32_t imageIndex = Renderer::GetCurrentFrame().ImageIndex);

		// index is the image attachment
		virtual ImageLayouts GetColorAttachmentImageLayout(uint32_t index);
		virtual ImageLayouts GetDepthImageLayout();


		virtual bool HasDepthImage();
		virtual bool HasColorAttachment();
		virtual void Resize(Vector2 imageSize);

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

		std::vector<VkFramebuffer> m_Framebuffers;


		void CreateFramebuffer();
		void Release();
	};
	
}
