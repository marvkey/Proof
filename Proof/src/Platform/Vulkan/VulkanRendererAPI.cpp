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
	VulkanRendererAPI::~VulkanRendererAPI()
	{
		delete s_CommandBuffer;
	}
	void VulkanRendererAPI::DrawArrays(Count<class RenderCommandBuffer> commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstInstance ) {
		vkCmdDraw(commandBuffer->As<VulkanRenderCommandBuffer>()->GetCommandBuffer(), vertexCount, instanceCount, 0, firstInstance);
	}
	void VulkanRendererAPI::DrawElementIndexed(Count<class RenderCommandBuffer> commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstInstance) {
		vkCmdDrawIndexed(commandBuffer->As<VulkanRenderCommandBuffer>()->GetCommandBuffer(), indexCount, instanceCount, 0, 0, firstInstance);
	}
	void VulkanRendererAPI::BeginRenderPass(Count<class RenderCommandBuffer> commandBuffer, Count<class RenderPass> renderPass, Count<class FrameBuffer> frameBuffer) {
		renderPass->As<VulkanRenderPass>()->BeginRenderPass(commandBuffer, frameBuffer);
	}
	void VulkanRendererAPI::RecordRenderPass(Count<class RenderPass> renderPass, Count<class GraphicsPipeline>pipeline, std::function<void(Count<RenderCommandBuffer> commandBuffer)> data) {
		renderPass->As<VulkanRenderPass>()->RecordRenderPass(pipeline,data);
	}
	void VulkanRendererAPI::EndRenderPass(Count<class RenderPass> renderPass) {
		renderPass->As<VulkanRenderPass>()->EndRenderPass();
	}
	void VulkanRendererAPI::SubmitCommandBuffer(Count<class RenderCommandBuffer> commandBuffer) {
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
			s_CommandBuffer = new VulkanCommandBuffer();
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
	void VulkanRendererAPI::BeginCommandBuffer(Count<class RenderCommandBuffer> commandBuffer)
	{
		commandBuffer->As<VulkanRenderCommandBuffer>()->BeginRecord();
	}
	void VulkanRendererAPI::EndCommandBuffer(Count<class RenderCommandBuffer> commandBuffer)
	{
		commandBuffer->As<VulkanRenderCommandBuffer>()->EndRecord();
	}
	CurrentFrame VulkanRendererAPI::GetCurrentFrame() {
		return VulkanRenderer::s_CurrentFrame;
	}
	RendererConfig VulkanRendererAPI::GetConfig() {
		return VulkanRenderer::s_RendererConfig;
	}
	
}