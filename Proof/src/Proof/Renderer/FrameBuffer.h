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
		
		Count<class Image> ExistingImage = nullptr;
	};
	struct FrameBufferAttachments {
		FrameBufferAttachments() = default;
		FrameBufferAttachments(std::initializer_list<FrameBufferImageConfig> attachments)
			: Attachments(attachments) {}
		std::vector<ImageFormat> GetAttachmentsImages()const
		{
			std::vector<ImageFormat> imagesFormats;
			for (auto& attach : Attachments)
				imagesFormats.emplace_back(attach.Format);

			return imagesFormats;
		}
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
		std::vector<uint32_t> ExistingImageLayers;

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
		virtual ~FrameBuffer() = default;

		virtual const FrameBufferConfig& GetConfig() const = 0;
		virtual Count<Image> GetOutput(uint32_t imageIndex) = 0;

		//uses getoutput to get find the depth index and returns it
		virtual Count<Image> GetDepthOutput() = 0;

		virtual void Resize(const glm::uvec2 size) =0;

		virtual bool HasDepthImage() = 0;
		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
	};
}

