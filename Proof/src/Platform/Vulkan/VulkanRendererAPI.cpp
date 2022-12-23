#include "Proofprch.h"
#include "VulkanRendererAPI.h"
#include <vulkan/vulkan.h>
#include "VulkanCommandBuffer.h"
#include "VulkanRenderer/VulkanRenderer.h"
#include "VulkanCommandBuffer.h"
#include "VulkanRenderPass.h"
namespace Proof {
	CommandBuffer* s_CommandBuffer =nullptr;
	VulkanRendererAPI::VulkanRendererAPI() {
	}
	void VulkanRendererAPI::DrawArrays(Count<class CommandBuffer> commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstInstance ) {
		vkCmdDraw(commandBuffer->As<VulkanCommandBuffer>()->GetCommandBuffer(), vertexCount, instanceCount, 0, firstInstance);
	}
	void VulkanRendererAPI::DrawElementIndexed(Count<class CommandBuffer> commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstInstance) {
		vkCmdDrawIndexed(commandBuffer->As<VulkanCommandBuffer>()->GetCommandBuffer(), indexCount, instanceCount, 0, 0, firstInstance);
	}
	void VulkanRendererAPI::BeginRenderPass(Count<class CommandBuffer> commandBuffer, Count<class RenderPass> renderPass, Count<class ScreenFrameBuffer> frameBuffer, bool viewScreen) {
		renderPass->As<VulkanRenderPass>()->BeginRenderPass(commandBuffer, frameBuffer,viewScreen);
	}
	void VulkanRendererAPI::RecordRenderPass(Count<class RenderPass> renderPass,std::function<void(Count<CommandBuffer>)> data) {
		renderPass->As<VulkanRenderPass>()->RecordRenderPass(data);
	}
	void VulkanRendererAPI::EndRenderPass(Count<class RenderPass> renderPass) {
		renderPass->As<VulkanRenderPass>()->EndRenderPass();
	}
	void VulkanRendererAPI::SubmitCommandBuffer(Count<class CommandBuffer> commandBuffer) {
		VulkanRenderer::SubmitCommandBuffer(commandBuffer);
	}

	void VulkanRendererAPI::Init() {
		VulkanRenderer::Init();
	}
	void VulkanRendererAPI::OnWindowResize(WindowResizeEvent& e) {
		VulkanRenderer::OnWindowResize(e);
	}

	void VulkanRendererAPI::Destroy() {
		VulkanRenderer::Destroy();
		delete s_CommandBuffer;
	}
	void VulkanRendererAPI::BeginFrame() {
		VulkanRenderer::BeginFrame();
	}
	void VulkanRendererAPI::EndFrame() {
		VulkanRenderer::EndFrame();
	}
	void VulkanRendererAPI::SubmitDatafree(std::function<void()> func) {
		VulkanRenderer::SubmitDatafree(func);
	}
	void VulkanRendererAPI::Submit(std::function<void(CommandBuffer*)> func) {
		auto graphicsContext = RendererBase::GetGraphicsContext()->As<VulkanGraphicsContext>();
		if (s_CommandBuffer == nullptr)
			s_CommandBuffer = new VulkanCommandBuffer(true);
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = graphicsContext->GetCommandPool();
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(graphicsContext->GetDevice(), &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		for (int i = 0; i < s_CommandBuffer->As<VulkanCommandBuffer>()->m_CommandBuffer.size(); i++)
			s_CommandBuffer->As<VulkanCommandBuffer>()->m_CommandBuffer[i] = commandBuffer;
		func(s_CommandBuffer);
		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(graphicsContext->GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(graphicsContext->GetGraphicsQueue());

		vkFreeCommandBuffers(graphicsContext->GetDevice(), graphicsContext->GetCommandPool(), 1, &commandBuffer);
	}
	CurrentFrame VulkanRendererAPI::GetCurrentFrame() {
		return VulkanRenderer::s_CurrentFrame;
	}
	RendererConfig VulkanRendererAPI::GetConfig() {
		return VulkanRenderer::s_RendererConfig;
	}
	
}