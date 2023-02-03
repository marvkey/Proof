#pragma once
#include <glm/glm.hpp>
#include "RendererBase.h"
namespace Proof {
	struct  RendererConfig {
		uint32_t FramesFlight = 2;
		uint32_t MaxImageCount = 2;
	};

	struct CurrentFrame {
		uint32_t FrameinFlight;
		uint32_t ImageIndex;
	};
	

	class Proof_API RendererAPI {
	public:
		enum class API {
			None =0, OpenGL =1, Vulkan=2
		};
		virtual ~RendererAPI(){};
		virtual void BeginCommandBuffer(Count<class RenderCommandBuffer> commandBuffer) = 0;
		virtual void EndCommandBuffer(Count<class RenderCommandBuffer> commandBuffer) = 0;
		virtual void DrawArrays(Count<class RenderCommandBuffer> commandBuffer,uint32_t vertexCount, uint32_t instanceCount,uint32_t firstInstance=0)=0;
		virtual void DrawElementIndexed(Count<class RenderCommandBuffer> commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstInstance =0) = 0;
		
		virtual void BeginRenderPass(Count<class RenderCommandBuffer> commandBuffer, Count<class RenderPass> renderPass, Count<class FrameBuffer> frameBuffer) = 0;
		virtual void BeginRenderPass(Count<class RenderCommandBuffer> commandBuffer, Count<class RenderPass> renderPass, Count<class ScreenFrameBuffer> frameBuffer);

		virtual void RecordRenderPass(Count<class RenderPass> renderPass, Count<class GraphicsPipeline>pipeline, std::function<void(Count<RenderCommandBuffer> commandBuffer)> data) = 0;
		virtual void EndRenderPass(Count<class RenderPass> renderPass) =0;
		virtual void SubmitCommandBuffer(Count<class RenderCommandBuffer>commandBuffer) = 0;
		virtual void Submit(std::function<void(class CommandBuffer*)> func ) = 0;
		virtual void SubmitDatafree(std::function<void()> func) = 0;
		virtual void OnWindowResize(WindowResizeEvent& e) = 0;
		virtual CurrentFrame GetCurrentFrame() = 0;
		virtual RendererConfig GetConfig() = 0;
		inline static API GetAPI() { return ActiveAPI; }
	private:
		virtual void Init() = 0;
		virtual void Destroy() =0;
		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;
		friend class RendererBase;
		friend class Renderer;
		static API ActiveAPI;
	};
}