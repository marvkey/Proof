#pragma once
#include "Proof/Core/Core.h"
#include "Image.h"
#include "Renderer.h"
namespace Proof {
	struct FrameBufferImageConfig {
		FrameBufferImageConfig() = default;
		FrameBufferImageConfig(ImageFormat format)
			: Format(format) {}

		ImageFormat Format = ImageFormat::None;
	//	bool Blend = true;
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
		Vector2 Size = { 100.f,100.f };
		float Samples = 1;
		mutable Vector4 ClearColor = { 0,0,0,1.0f };
		FrameBufferAttachments Attachments;
		float DepthClearValue = 1.0f;
		uint32_t StencilClearValue = 0;
		// clear when getting drawn from a render pass
		bool ClearDepthOnLoad = true;
		bool ClearFrameBufferOnLoad = true;
	};
	class FrameBuffer : public RefCounted {
	public:
		static Count<FrameBuffer> Create(const FrameBufferConfig& config);
		// index is hte framebuffer image index
		virtual Count <Image2D> GetColorAttachmentImage(uint32_t index, uint32_t imageIndex = Renderer::GetCurrentFrame().ImageIndex) = 0;
		virtual Count <Image2D> GetDepthImage(uint32_t imageIndex = Renderer::GetCurrentFrame().ImageIndex) = 0;
		virtual ImageLayouts2D GetColorAttachmentImageLayout(uint32_t index) = 0;
		virtual ImageLayouts2D GetDepthImageLayout() = 0;

		virtual ~FrameBuffer() = default;

		virtual bool HasDepthImage() = 0;
		virtual bool HasColorAttachment() = 0;
		virtual const FrameBufferConfig& GetConfig() const = 0;
		virtual void Resize(Vector2 imageSize) = 0;

	};
	class ScreenFrameBuffer : public RefCounted {
	public:
		static Count<ScreenFrameBuffer> Create(uint32_t Width, uint32_t Height);
		static Count<ScreenFrameBuffer> Create(const FrameBufferConfig& config);
		virtual void Resize(Vector2 imageSize);
		const Count<FrameBuffer> const GetFrameBuffer() {
			return m_FrameBuffer;
		}
		virtual uint32_t GetFrameWidth();
		virtual uint32_t GetFrameHeight();
		virtual Count<Image2D> GetImage(uint32_t imageIndex = Renderer::GetCurrentFrame().ImageIndex);
	private:
		Count<FrameBuffer> m_FrameBuffer;
	};
}

