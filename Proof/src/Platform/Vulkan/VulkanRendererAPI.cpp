#include "Proofprch.h"
#include "VulkanRendererAPI.h"
#include <vulkan/vulkan.h>
#include "VulkanCommandBuffer.h"
#include "VulkanRenderer/VulkanRenderer.h"
#include "VulkanCommandBuffer.h"
#include "VulkanRenderMaterial.h"
#include "VulkanComputePass.h"
#include "VulkanRenderPass.h"
#include "VulkanGraphicsContext.h"
namespace Proof {
	
	VulkanRendererAPI::VulkanRendererAPI() {
	}
	//VulkanRendererAPI::~VulkanRendererAPI()
	//{
	//}
	void VulkanRendererAPI::DrawArrays(Count<class RenderCommandBuffer> commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex , uint32_t firstInstance) {
		vkCmdDraw(commandBuffer.As<VulkanRenderCommandBuffer>()->GetCommandBuffer(Renderer::GetCurrentFrame().FrameinFlight), vertexCount, instanceCount, firstVertex, firstInstance);
	}
	void VulkanRendererAPI::DrawElementIndexed(Count<class RenderCommandBuffer> commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstInstance) {
		vkCmdDrawIndexed(commandBuffer.As<VulkanRenderCommandBuffer>()->GetCommandBuffer(Renderer::GetCurrentFrame().FrameinFlight), indexCount, instanceCount, 0, 0, firstInstance);
	}

	void VulkanRendererAPI::DrawElementIndexed(Count<class RenderCommandBuffer> commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance)
	{
		vkCmdDrawIndexed(commandBuffer.As<VulkanRenderCommandBuffer>()->GetCommandBuffer(Renderer::GetCurrentFrame().FrameinFlight), indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
	}

	void VulkanRendererAPI::BeginRenderPass(Count<class RenderCommandBuffer> commandBuffer, Count<class RenderPass> renderPass, bool explicitClear )
	{
		renderPass.As<VulkanRenderPass>()->BeginRenderPass(commandBuffer, explicitClear);
	}

	void VulkanRendererAPI::BeginRenderMaterialRenderPass(Count<class RenderCommandBuffer> commandBuffer, Count<class RenderPass> renderPass, bool explicitClear )
	{
		renderPass.As<VulkanRenderPass>()->BeginRenderMaterialRenderPass(commandBuffer, explicitClear);
	}

	void VulkanRendererAPI::EndRenderPass(Count<class RenderPass> renderPass)
	{
		renderPass.As<VulkanRenderPass>()->EndRenderPass();
	}

	void VulkanRendererAPI::RenderPassPushRenderMaterial(Count<class RenderPass> renderPass, Count<class RenderMaterial> renderMaterial)
	{
		renderPass.As<VulkanRenderPass>()->RenderPassPushRenderMaterial(renderMaterial);
	}

	void VulkanRendererAPI::BeginComputePass(Count<RenderCommandBuffer> commandBuffer, Count<ComputePass> computePass)
	{
		computePass.As<VulkanComputePass>()->BeginComputePass(commandBuffer);
	}

	void VulkanRendererAPI::BeginRenderMaterialComputePass(Count<RenderCommandBuffer> commandBuffer, Count<ComputePass> computePass)
	{
		computePass.As<VulkanComputePass>()->BeginRenderMaterialComputePass(commandBuffer);
	}

	void VulkanRendererAPI::EndComputePass( Count<ComputePass> computePass)
	{
		computePass.As<VulkanComputePass>()->EndComputePass();
	}

	void VulkanRendererAPI::ComputePassPushRenderMaterial(Count<class ComputePass> computePass, Count<class RenderMaterial> renderMaterial)
	{
		computePass.As<VulkanComputePass>()->ComputePassPushRenderMaterial(renderMaterial);
	}

	void VulkanRendererAPI::SubmitCommandBuffer(Count<class RenderCommandBuffer> commandBuffer) {
		VulkanRenderer::SubmitCommandBuffer(commandBuffer);
	}

	void VulkanRendererAPI::SetGraphicsContext(Count<GraphicsContext> context)
	{
		VulkanRenderer::SetGraphicsContext(context);
	}
	VkFence submitFence;
	void VulkanRendererAPI::Init() {
		VkFenceCreateInfo fenceInfo = {};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.pNext = nullptr;
		fenceInfo.flags = 0;
		vkCreateFence(VulkanRenderer::GetGraphicsContext()->GetDevice(), &fenceInfo, nullptr, &submitFence);
		VulkanRenderer::Init();
	}
	void VulkanRendererAPI::OnWindowResize(WindowResizeEvent& e) {
		VulkanRenderer::OnWindowResize(e);
	}

	void VulkanRendererAPI::Destroy() {
		VulkanRenderer::Destroy();
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
		auto graphicsContext = Renderer::GetGraphicsContext().As<VulkanGraphicsContext>();
		VulkanCommandBuffer* commandBufferContainer = new VulkanCommandBuffer();
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

		for (int i = 0; i < commandBufferContainer->As<VulkanCommandBuffer>()->m_CommandBuffer.size(); i++)
			commandBufferContainer->As<VulkanCommandBuffer>()->m_CommandBuffer[i] = commandBuffer;
		func(commandBufferContainer);
		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(graphicsContext->GetGraphicsQueue(), 1, &submitInfo, submitFence);
		vkWaitForFences(graphicsContext->GetDevice(), 1, &submitFence, VK_TRUE, UINT64_MAX);
		vkResetFences(VulkanRenderer::GetGraphicsContext()->GetDevice(), 1, &submitFence);
		
		vkFreeCommandBuffers(graphicsContext->GetDevice(), graphicsContext->GetCommandPool(), 1, &commandBuffer);
		vkQueueWaitIdle(graphicsContext->GetGraphicsQueue());

		delete commandBufferContainer;
	}
	void VulkanRendererAPI::BeginCommandBuffer(Count<class RenderCommandBuffer> commandBuffer)
	{
		commandBuffer.As<VulkanRenderCommandBuffer>()->BeginRecord(Renderer::GetCurrentFrame().FrameinFlight);
	}
	void VulkanRendererAPI::EndCommandBuffer(Count<class RenderCommandBuffer> commandBuffer)
	{
		commandBuffer.As<VulkanRenderCommandBuffer>()->EndRecord(Renderer::GetCurrentFrame().FrameinFlight);
	}
	CurrentFrame VulkanRendererAPI::GetCurrentFrame() {
		return VulkanRenderer::s_CurrentFrame;
	}
	RendererConfig VulkanRendererAPI::GetConfig() {
		return VulkanRenderer::s_RendererConfig;
	}
	
}