#include "Proofprch.h"
#include "VulkanCommandBuffer.h"
#include "Proof/Renderer/Renderer.h"
#include "VulkanGraphicsContext.h"
#include "VulkanSwapChain.h"
#include "Proof/Renderer/Renderer.h"
#include "VulkanGraphicsPipeline.h"
#include "Vulkan.h"
#include "VulkanRenderer/VulkanRenderer.h"
namespace Proof
{
	VulkanCommandBuffer::VulkanCommandBuffer()
	{
		m_CommandBuffer.resize(Renderer::GetConfig().FramesFlight);
	}
	VulkanCommandBuffer::~VulkanCommandBuffer() {
		// called by render sumit class
		/*
		for (uint32_t i = 0; i < Renderer::GetConfig().FramesFlight; i++)
		{
			Renderer::SubmitDatafree([buffer = m_CommandBuffer[i]](){
				auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();
				vkFreeCommandBuffers(
					graphicsContext->GetDevice(),
					graphicsContext->GetCommandPool(),
					1,
					&buffer);
			});
		}
		*/
	}


	VulkanRenderCommandBuffer::VulkanRenderCommandBuffer(std::string debugName, bool swapchaing)
		:
		m_DebugName(debugName), m_Swapcahin(swapchaing)
	{
		m_ActiveCommandBufferIndex = Renderer::GetCurrentFrame().FrameinFlight;

		if (m_Swapcahin == true)return;
		Build();
	}

	VulkanRenderCommandBuffer::VulkanRenderCommandBuffer(CommandBuffer* commandBuffer)
		:m_NormalCommandBuffer(commandBuffer)
	{

	}

	VulkanRenderCommandBuffer::~VulkanRenderCommandBuffer()
	{
		Release();
	}
	VkCommandBuffer VulkanRenderCommandBuffer::GetActiveCommandBuffer()
	{
		return m_CommandBuffers[m_ActiveCommandBufferIndex];
	}

	void VulkanRenderCommandBuffer::Build()
	{
		if (m_Swapcahin == true)return;
		auto graphicsContext = VulkanRenderer::GetGraphicsContext();
		m_CommandBuffers.resize(Renderer::GetConfig().FramesFlight);

		VkCommandPoolCreateInfo cmdPoolInfo = {};
		cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		cmdPoolInfo.queueFamilyIndex = graphicsContext->FindPhysicalQueueFamilies().graphicsAndComputeFamily.value();
		cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		VK_CHECK_RESULT(vkCreateCommandPool(graphicsContext->GetDevice(), &cmdPoolInfo, nullptr, &m_CommandPool));
		graphicsContext->SetDebugUtilsObjectName(VK_OBJECT_TYPE_COMMAND_POOL, fmt::format("{} Command Pool", m_DebugName), m_CommandPool);

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.pNext = nullptr;
		allocInfo.commandPool = graphicsContext->GetCommandPool();
		allocInfo.commandBufferCount = m_CommandBuffers.size();
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

		if (vkAllocateCommandBuffers(graphicsContext->GetDevice(), &allocInfo, m_CommandBuffers.data()) != VK_SUCCESS)
			PF_CORE_ASSERT(false, "Failed to allocate command buffer");

		for (uint32_t i = 0; i < m_CommandBuffers.size(); ++i)
			graphicsContext->SetDebugUtilsObjectName(VK_OBJECT_TYPE_COMMAND_BUFFER, fmt::format("{} (frame in flight: {})", m_DebugName, i), m_CommandBuffers[i]);


		VkFenceCreateInfo fenceCreateInfo{};
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
		m_WaitFences.resize(Renderer::GetConfig().FramesFlight);
		for (size_t i = 0; i < m_WaitFences.size(); ++i)
		{
			VK_CHECK_RESULT(vkCreateFence(graphicsContext->GetDevice(), &fenceCreateInfo, nullptr, &m_WaitFences[i]));
			graphicsContext->SetDebugUtilsObjectName(VK_OBJECT_TYPE_FENCE, fmt::format("{} (frame in flight: {}) fence", m_DebugName, i), m_WaitFences[i]);
		}
	}

	void VulkanRenderCommandBuffer::Release()
	{
		if (m_NormalCommandBuffer != nullptr)
		{
			m_NormalCommandBuffer = nullptr;
			return;
		}
		if (m_Swapcahin)
			return;
		for (uint32_t i = 0; i < Renderer::GetConfig().FramesFlight; i++)
		{
			Renderer::SubmitDatafree([buffer = m_CommandBuffers[i], fence = m_WaitFences[i]]() {
				auto graphicsContext = VulkanRenderer::GetGraphicsContext();
				vkFreeCommandBuffers(
					graphicsContext->GetDevice(),
					graphicsContext->GetCommandPool(),
					1,
					&buffer);

				vkDestroyFence(graphicsContext->GetDevice(), fence, nullptr);

			});
		}

		Renderer::SubmitDatafree([commandPool = m_CommandPool]() {
			auto graphicsContext = VulkanRenderer::GetGraphicsContext();
			vkDestroyCommandPool(graphicsContext->GetDevice(), commandPool, nullptr);
		});
	}
	/*
	VkCommandBuffer VulkanRenderCommandBuffer::GetCommandBuffer(uint32_t frameIndex)
	{
		if (m_NormalCommandBuffer != nullptr)
			return m_NormalCommandBuffer->As<VulkanCommandBuffer>()->GetCommandBuffer(frameIndex);

		if (m_Swapcahin)
			return VulkanRenderer::GetGraphicsContext()->GetSwapChain().As<VulkanSwapChain>()->GetCommandBuffer(frameIndex);
		return m_CommandBuffers[frameIndex];
	}
	*/

	void VulkanRenderCommandBuffer::Submit()
	{
		if (m_Swapcahin == true || m_NormalCommandBuffer != nullptr)
			return;
		Count<VulkanRenderCommandBuffer> instance = this;
		Renderer::Submit([instance]()
		{

			auto device = VulkanRenderer::GetGraphicsContext()->GetDevice();

			uint32_t frameIndex = Renderer::RT_GetCurrentFrame().FrameinFlight;

			VkSubmitInfo submitInfo{};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			submitInfo.commandBufferCount = 1;
			VkCommandBuffer commandBuffer = instance->m_CommandBuffers[frameIndex];
			submitInfo.pCommandBuffers = &commandBuffer;

			VK_CHECK_RESULT(vkWaitForFences(device, 1, &instance->m_WaitFences[frameIndex], VK_TRUE, UINT64_MAX));
			VK_CHECK_RESULT(vkResetFences(device, 1, &instance->m_WaitFences[frameIndex]));


			VK_CHECK_RESULT(vkQueueSubmit(VulkanRenderer::GetGraphicsContext()->GetGraphicsQueue(), 1, &submitInfo, instance->m_WaitFences[frameIndex]));
			vkDeviceWaitIdle(VulkanRenderer::GetGraphicsContext()->GetDevice());
			vkResetCommandBuffer(instance->m_CommandBuffers[frameIndex], 0);
		});
	}

	void VulkanRenderCommandBuffer::BeginRecord()
	{
		if (m_Swapcahin == true || m_NormalCommandBuffer != nullptr)
			return;
		Count<VulkanRenderCommandBuffer> instance = this;
		Renderer::Submit([instance]()
		{
			PF_CORE_ASSERT(instance->m_Recording == false, "cannot start recoridng when command buffer is still recording");
			instance->m_Recording = true;

			instance->m_ActiveCommandBufferIndex = Renderer::RT_GetCurrentFrame().FrameinFlight;
			VkCommandBufferBeginInfo beginInfo{};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.pNext = nullptr;
			beginInfo.flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
			if (vkBeginCommandBuffer(instance->m_CommandBuffers[instance->m_ActiveCommandBufferIndex], &beginInfo) != VK_SUCCESS)
				PF_CORE_ASSERT(false, "Failed to begin recording command buffer");
		});
	}

	void VulkanRenderCommandBuffer::EndRecord()
	{

		if (m_Swapcahin == true || m_NormalCommandBuffer != nullptr)
			return;
		Count<VulkanRenderCommandBuffer> instance = this;
		Renderer::Submit([instance]()
		{
			PF_CORE_ASSERT(instance->m_Recording == true, "cannot End recording when recoring never started");
			if (vkEndCommandBuffer(instance->m_CommandBuffers[instance->m_ActiveCommandBufferIndex]) != VK_SUCCESS)
				PF_CORE_ASSERT(false, "Faied to record command Buffers");
			instance->m_Recording = false;
		});
	}

};