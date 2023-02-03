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
		VkRenderPass GetRenderPass() {
			return m_RenderPass;
		}
		Count<class RenderCommandBuffer>  GetCurrentCommandBuffer() {
			return m_CommandBuffer;
		}
	private:
		void AddColorAttachment(const RenderPassImageConfig& config);
		void SetDepthAttachment(const RenderPassImageConfig& config);

		void Init();
		void CreateRenderPass();

		void BeginRenderPass(Count<class RenderCommandBuffer> command,Count<class FrameBuffer>frameBuffer);
		void RecordRenderPass(Count<class GraphicsPipeline>pipline,std::function<void(Count<RenderCommandBuffer> commandBuffer)> func);
		void EndRenderPass();

		void Release();
		Count<RenderCommandBuffer> m_CommandBuffer;
		Count<FrameBuffer> m_CurrentFrameBuffer;
		RenderPassConfig m_Config;
		VulkanRenderPassAttach m_DepthAttachment;
		std::vector< VulkanRenderPassAttach> m_ColorAttachments;
		ImageFormat m_DepthFormat = ImageFormat::None;

		bool m_RenderPassEnabled = false;
		VkRenderPass m_RenderPass = nullptr;

		friend class VulkanRenderer;
		friend class VulkanRendererAPI;
	};
}
