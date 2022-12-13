#pragma once
#include "Proof/Core/Core.h"
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
	enum class VulkanRenderPassDefaultType {
		World,
		Other
	};
	class VulkanRenderPass {
	public:
		VulkanRenderPass(VulkanRenderPassDefaultType type = VulkanRenderPassDefaultType::World);
		VulkanRenderPass(const VulkanRenderPassInfo& info);
		virtual ~VulkanRenderPass();
		void SetGraphicsPipeLine(Count<class VulkanGraphicsPipeline> pipeline) {
			m_PipeLine = pipeline;
		}
		VkRenderPass GetRenderPass() {
			return m_RenderPass;
		}
		Count<class VulkanGraphicsPipeline> GetPipeLine() {
			return m_PipeLine;
		}
		VkRenderPass m_RenderPass = nullptr;

	private:
		void BeginRenderPass(Count<class VulkanCommandBuffer> command,Count<class ScreenFrameBuffer>frameBuffer,const glm::vec4& Color = { 0.0,0.0,0.0,1 }, float Depth = 1.0f, uint32_t stencil = 0);
		void EndRenderPass(Count<class VulkanCommandBuffer> commandBuffer);
		bool m_RenderPassEnabled = false;
		Count<class VulkanGraphicsPipeline> m_PipeLine;
		friend class VulkanRenderer;
	};
}
