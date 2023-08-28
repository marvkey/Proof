#pragma once
#include "Proof/Core/Core.h"
#include "Image.h"
#include <glm/glm.hpp>
namespace Proof {
	struct FrameBufferImageConfig {
		FrameBufferImageConfig() = default;
		FrameBufferImageConfig(ImageFormat format)
			: Format(format) {}

		ImageFormat Format = ImageFormat::None;
		bool ClearOnLoad = true;
		
		ImageLayouts2D ExistingImage;
	};
	struct FrameBufferAttachments {
		FrameBufferAttachments() = default;
		FrameBufferAttachments(std::initializer_list<FrameBufferImageConfig> attachments)
			: Attachments(attachments) {}

		std::vector<FrameBufferImageConfig> Attachments;
	};
	// https://github.com/TheCherno/Hazel/blob/master/Hazel/src/Platform/OpenGL/OpenGLFramebuffer.cpp
	// this helped a lot
	struct FrameBufferConfig {
		std::string DebugName;
		uint32_t Width = 0;//if 0 will use swapchiann size
		uint32_t Height = 0;//if 0 will use swapchiann size
		uint32_t Samples = 1; // multisampling

		mutable Vector4 ClearColor = { 0,0,0,1.0f };
		FrameBufferAttachments Attachments;
		float DepthClearValue = 0.0f;
		uint32_t StencilClearValue = 0;

		// used for transfer 
		bool Transfer = false;

		// clear when getting drawn from a render pass
		bool ClearDepthOnLoad = true;
		bool ClearColorOnLoad = true;
	};
	class FrameBuffer : public RefCounted {
	public:
		static Count<FrameBuffer> Create(const FrameBufferConfig& config);
		// index is hte framebuffer image index
		// iamge is only goign to be type iamge2D and imageView
		virtual Count <Image> GetColorAttachmentImage(uint32_t swapchainImageIndex,uint32_t index) = 0;
		// iamge is only goign to be type iamge2D and imageView
		virtual Count <Image> GetDepthImage(uint32_t swapchainImageIndex) = 0;

		virtual ImageLayouts2D GetColorAttachmentImageLayout(uint32_t index) = 0;
		virtual ImageLayouts2D GetDepthImageLayout() = 0;
		virtual void Resize(const glm::uvec2 size) =0;
		virtual ~FrameBuffer() = default;

		virtual bool HasDepthImage() = 0;
		virtual bool HasColorAttachment() = 0;
		virtual const FrameBufferConfig& GetConfig() const = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;

	};
}

