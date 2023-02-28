#pragma once
#include "Proof/Core/Core.h"
#include "Texture.h"
#include "Renderer.h"
namespace Proof {
	struct FrameBufferImageConfig {
		FrameBufferImageConfig() = default;
		FrameBufferImageConfig(ImageFormat format)
			: Format(format) {}

		ImageFormat Format = ImageFormat::None;
		bool Blend = true;
		bool ClearOnLoad = true;
		
		// basicllya make the value of somethign this
		void SetOverrideImage(const Image& image) {
			if (m_ImageLayout.Images.empty()==false)
				PF_CORE_ASSERT(false, "Cannot set image and image layout");
			m_Image = image;
		}

		// tempoarary we will find a better way to andle this
		void SetOverrideLayout(const ImageLayouts& image) {
			if (m_Image.HasImage())
				PF_CORE_ASSERT(false, "Cannot set image and image layout");
			m_ImageLayout = image;
		}

		const Image& GetImage()const {
			return m_Image;
		}

		const ImageLayouts& GetImagelayout()const {
			return m_ImageLayout;
		}
	private:
		ImageLayouts m_ImageLayout;
		Image m_Image;
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
		Vector2 Size = { 100.f,100.f };
		float Samples = 1;
		mutable Vector4 ClearColor = { 0,0,0,1.0f };
		FrameBufferAttachments Attachments;
		mutable float DepthClearValue = 1.0f;
		uint32_t StencilClearValue = 0;
		mutable bool ClearDepthOnLoad = true;

		// bascially do we want to clear teh frame buffer when we load 
		// set to false if not 
		mutable bool ClearFrameBufferOnLoad = true;
	};
	class FrameBuffer {
	public:
		static Count<FrameBuffer> Create(const FrameBufferConfig& config);
		// index is hte framebuffer image index
		virtual Image GetColorAttachmentImage(uint32_t index, uint32_t imageIndex = Renderer::GetCurrentFrame().ImageIndex) = 0;
		virtual Image GetDepthImage(uint32_t imageIndex = Renderer::GetCurrentFrame().ImageIndex) = 0;

		virtual ImageLayouts GetColorAttachmentImageLayout(uint32_t index) = 0;
		virtual ImageLayouts GetDepthImageLayout() = 0;

		virtual ~FrameBuffer() = default;

		virtual bool HasDepthImage() = 0;
		virtual bool HasColorAttachment() = 0;
		virtual const FrameBufferConfig& GetConfig() const = 0;
		virtual void Resize(Vector2 imageSize) = 0;
	};
	class ScreenFrameBuffer {
	public:
		static Count<ScreenFrameBuffer> Create(uint32_t Width, uint32_t Height);
		virtual void Resize(Vector2 imageSize);
		const Count<FrameBuffer> const GetFrameBuffer() {
			return m_FrameBuffer;
		}
		virtual uint32_t GetFrameWidth();
		virtual uint32_t GetFrameHeight();
		virtual Image GetImage();
	private:
		Count<FrameBuffer> m_FrameBuffer;
	};
}

