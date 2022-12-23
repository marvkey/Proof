#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Renderer/RenderPass.h"
#include <vulkan/vulkan.h>
namespace Proof{

	struct VulkanRenderPassInfo {
		std::vector< VkAttachmentDescription> Attachments;
		std::vector< VkAttachmentReference> AttachmentRef;
		std::vector< VkSubpassDescription> SubPass;
		std::vector < VkSubpassDependency> SubPassDepedency;
		VulkanRenderPassInfo(uint32_t attachmentSize, uint32_t subPassSize, uint32_t subPassDependecySize) {
			Attachments.resize(attachmentSize);
			AttachmentRef.resize(attachmentSize);
			SubPass.resize(subPassSize);
			SubPassDepedency.resize(subPassDependecySize);
		}
		Count<class VulkanGraphicsPipeline> PipeLine;
	};
	
	class VulkanRenderPass : public RenderPass {
	public:
		VulkanRenderPass(RenderPassType type = RenderPassType::World);
		VulkanRenderPass(const VulkanRenderPassInfo& info);
		virtual ~VulkanRenderPass();
		void SetGraphicsPipeline(Count<class GraphicsPipeline> pipeline) {
			m_PipeLine = pipeline;
		}
		VkRenderPass GetRenderPass() {
			return m_RenderPass;
		}
		virtual Count<class GraphicsPipeline> GetPipeLine() {
			return m_PipeLine;
		}
		Count<class CommandBuffer>  GetCommandBuffer() {
			return m_CommandBuffer;
		}
	private:
		friend class VulkanRendererAPI;
		VkRenderPass m_RenderPass = nullptr;
		void BeginRenderPass(Count<class CommandBuffer> command,Count<class ScreenFrameBuffer>frameBuffer,bool viewScreen = false,const glm::vec4& Color = { 0.0,0.0,0.0,1 }, float Depth = 1.0f, uint32_t stencil = 0);
		void EndRenderPass();
		void RecordRenderPass(std::function<void(Count<CommandBuffer> commandBuffer)> func);
		bool m_RenderPassEnabled = false;
		Count<class GraphicsPipeline> m_PipeLine;
		Count<CommandBuffer> m_CommandBuffer;
		Count<ScreenFrameBuffer> m_FrameBuffer;
		friend class VulkanRenderer;
		friend class VulkanRendererAPI;
	};
}
