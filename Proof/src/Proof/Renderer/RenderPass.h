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
		RenderPassConfig(const std::string& name, const FrameBufferConfig& config) {
			DebugName = name;
			for (const auto& attach : config.Attachments.Attachments)
			{
				Attachments.Attachments.emplace_back(attach.Format);
			}
			}
		std::string DebugName;
		RenderPassAttachment Attachments;

		bool MultiView = false;
	};
	class RenderPass {
	public:
		virtual ~RenderPass() =default;
		static	Count<RenderPass> Create(const RenderPassConfig& config);
	};
}
