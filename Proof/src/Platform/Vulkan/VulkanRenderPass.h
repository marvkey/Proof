#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Renderer/RenderPass.h"
#include <vulkan/vulkan.h>
namespace Proof{
	struct VulkanRenderPassAttach {
		VkAttachmentDescription AttchmentDescription;
		VkAttachmentReference AttachmentRef;
	};
	class VulkanRenderPass : public RenderPass {
	public:
		VulkanRenderPass(const RenderPassConfig& config);
		virtual ~VulkanRenderPass();
		virtual void SetTargetFrameBuffer(Count<FrameBuffer> frame) { PF_CORE_ASSERT(frame, "Cannot be null"); m_Config.TargetBuffer = frame; }

		VkRenderPass GetRenderPass() {
			return m_RenderPass;
		}
		Count<class RenderCommandBuffer>  GetCurrentCommandBuffer() {
			return m_CommandBuffer;
		}

		//Todo remove for imgui draw 
		// Dont use elseWHere
		void BeginRenderPass(Count<class RenderCommandBuffer> command);

	//	void RecordRenderPass(Count<class GraphicsPipeline>pipline, VkViewport viewport, VkRect2D scissor);

	private:
		void AddColorAttachment(const RenderPassImageConfig& config);
		void SetDepthAttachment(const RenderPassImageConfig& config);

		void Init();
		void CreateRenderPass();
		void BeginRenderPass(Count<class RenderCommandBuffer> command, Count<class GraphicsPipeline>pipline,Viewport vieport, ViewportScissor scisscor);
		void BeginRenderPass(Count<class RenderCommandBuffer> command, Count<class GraphicsPipeline>pipline);
		void EndRenderPass();
		void Release();
		Count<RenderCommandBuffer> m_CommandBuffer;
		RenderPassConfig m_Config;
		VulkanRenderPassAttach m_DepthAttachment;
		std::vector< VulkanRenderPassAttach> m_ColorAttachments;
		ImageFormat m_DepthFormat = ImageFormat::None;
		Count<class GraphicsPipeline>m_GraphicsPipline;
		bool m_RenderPassEnabled = false;
		VkRenderPass m_RenderPass = nullptr;

		friend class VulkanRenderer;
		friend class VulkanRendererAPI;
	};
}
