#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Renderer/Texture.h"
#include "FrameBuffer.h"
namespace Proof{
	struct RenderPassImageConfig {
		RenderPassImageConfig() = default;
		RenderPassImageConfig(ImageFormat format)
			: Format(format) {}

		ImageFormat Format = ImageFormat::None;
		bool PresentKHr = false;
	};
	struct RenderPassAttachment {
		RenderPassAttachment() = default;
		RenderPassAttachment(std::initializer_list<RenderPassImageConfig> attahcments)
			:
			Attachments(attahcments)
		{

		}
		std::vector<RenderPassImageConfig> Attachments;
	};
	struct RenderPassConfig {
		RenderPassConfig() {

		}
		RenderPassConfig(const std::string& debugName, Count<FrameBuffer> buffer) {
			DebugName = debugName;
			for (const auto& attach : buffer->GetConfig().Attachments.Attachments)
			{
				Attachments.Attachments.emplace_back(attach.Format);
			}
			TargetBuffer = buffer;
		}
		RenderPassConfig(const std::string& debugName, const FrameBufferConfig& conif) {
			DebugName = debugName;
			for (const auto& attach : conif.Attachments.Attachments)
			{
				Attachments.Attachments.emplace_back(attach.Format);
			}
		}
		std::string DebugName;
		RenderPassAttachment Attachments;
		Count<FrameBuffer> TargetBuffer;
		bool MultiView = false;
	};
	class RenderPass {
	public:
		virtual void SetTargetFrameBuffer(Count<FrameBuffer> frame) = 0;
		virtual ~RenderPass() =default;
		static	Count<RenderPass> Create(const RenderPassConfig& config);
	};
}
