#pragma once
#include "Proof/Core/Core.h"
namespace Proof{
	struct RenderPassConfig {
	
		std::string DebugName;
		Count< class GraphicsPipeline> Pipeline;
		Count<class FrameBuffer> TargetFrameBuffer;
	};
	class RenderPass : public RefCounted {
	public:
		virtual void SetInput(std::string_view name, Count<class UniformBuffer> buffer) = 0;
		virtual void SetInput(std::string_view name, Count<class Texture2D> iamge) = 0;
		virtual void SetInput(std::string_view name, Count<class StorageBuffer> buffer) = 0;
		virtual void SetInput(std::string_view name, Count<class UniformBufferSet> buffer) = 0;
		virtual void SetInput(std::string_view name, Count<class StorageBufferSet> buffer) = 0;
		virtual void SetInput(std::string_view name, Count<class TextureCube> buffer) = 0;
		virtual void SetInput(std::string_view name, const std::vector< Count<class Texture2D>>& images) = 0;
		void virtual SetInput(std::string_view name, Count<class ImageView> imageView) = 0;
		void virtual SetInput(std::string_view name, const std::vector< Count<class ImageView>>& imageViews) = 0;
		void virtual SetInput(std::string_view name, Count<class Image2D>image) = 0;
		void virtual SetInput(std::string_view name, const std::vector< Count<class Image2D>>& images) = 0;

		void virtual AddGlobalInput(Count<class GlobalBufferSet> globalInputs) = 0;
		virtual void PushData(std::string_view name, const void* data) = 0;
		virtual void SetTargetFrameBuffer(Count<FrameBuffer> frame) = 0;
		virtual Count< class GraphicsPipeline> GetPipeline() = 0;
		virtual Count<class FrameBuffer> GetTargetFrameBuffer() = 0;
		virtual const RenderPassConfig& GetConfig() const = 0;
		virtual ~RenderPass() =default;
		static	Count<RenderPass> Create(const RenderPassConfig& config);
	};
}
