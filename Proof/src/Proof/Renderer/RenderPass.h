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
		Count< class GraphicsPipeline> Pipeline;
		bool MultiView = false;
	};
	class RenderPass : public RefCounted {
	public:
		virtual void SetInput(std::string_view name, Count<class UniformBuffer> buffer) = 0;
		virtual void SetInput(std::string_view name, Count<class Texture2D> iamge) = 0;
		virtual void SetInput(std::string_view name, Count<class StorageBuffer> buffer) = 0;
		virtual void SetInput(std::string_view name, const std::vector< Count<class Texture2D>>& images) = 0;
		//virtual void SetTargetFrameBuffer(Count<FrameBuffer> frame) = 0;
		virtual Count< class GraphicsPipeline> GetPipeline() = 0;
		virtual Count<class FrameBuffer> GetTargetFrameBuffer() = 0;

		virtual ~RenderPass() =default;
		static	Count<RenderPass> Create(const RenderPassConfig& config);
	};
}
