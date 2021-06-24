#include "Proofprch.h"
#include "VulkanRendererAPI.h"
#include "vulkan/vulkan.h"
namespace Proof {
	void VulkanRendererAPI::Clear() {
	}
	//void VulkanRendererAPI::DrawIndexed(const Count<VertexArray>& VertexArray) {}
	void VulkanRendererAPI::SetViewPort(int num,int num2,int Width,int Height) {
		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float)Width;
		viewport.height = (float)Height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkRect2D scissor{};
		scissor.offset = {0,0};
		scissor.extent = Graphics->swapChainExtent;

		VkPipelineViewportStateCreateInfo viewportState{};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.pViewports = &viewport;
		viewportState.scissorCount = 1;
		viewportState.pScissors = &scissor;
	}
	void VulkanRendererAPI::EnableDepth(bool Depth) {
		VkPipelineRasterizationStateCreateInfo rasterizer{};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizer.lineWidth = 1.0f;
		rasterizer.depthBiasEnable = VK_FALSE;
	}
	void VulkanRendererAPI::SetClearColor(const glm::vec4& Color) {
		
	}
	void VulkanRendererAPI::SetClearColor(float R,float G,float B,float A) {
	}
}