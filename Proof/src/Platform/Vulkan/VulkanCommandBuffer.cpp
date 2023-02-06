#include "Proofprch.h"
#include "VulkanCommandBuffer.h"
#include "Proof/Renderer/Renderer.h"
#include "VulkanGraphicsContext.h"
#include "VulkanSwapChain.h"
#include "VulkanGraphicsPipeline.h"
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
	

	VulkanRenderCommandBuffer::VulkanRenderCommandBuffer(CommandBuffer* commandBuffer )
		:m_NormalCommandBuffer(commandBuffer)
	{
		if (m_NormalCommandBuffer)
			return;
		Init();
		
	}

	VulkanRenderCommandBuffer::~VulkanRenderCommandBuffer()
	{
		Release();
	}
	void VulkanRenderCommandBuffer::Init()
	{
		auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();
		m_CommandBuffers.resize(Renderer::GetConfig().FramesFlight);

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.pNext = nullptr;
		allocInfo.commandPool = graphicsContext->GetCommandPool();
		allocInfo.commandBufferCount = m_CommandBuffers.size();
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

		if (vkAllocateCommandBuffers(graphicsContext->GetDevice(), &allocInfo, m_CommandBuffers.data()) != VK_SUCCESS)
			PF_CORE_ASSERT(false, "Failed to allocate command buffer");
	}

	void VulkanRenderCommandBuffer::Release()
	{
		if (m_NormalCommandBuffer != nullptr)
		{
			m_NormalCommandBuffer = nullptr;
			return;
		}
		for (uint32_t i = 0; i < Renderer::GetConfig().FramesFlight; i++)
		{
			Renderer::SubmitDatafree([buffer = m_CommandBuffers[i]](){
				auto graphicsContext = Renderer::GetGraphicsContext()->As<VulkanGraphicsContext>();
				vkFreeCommandBuffers(
					graphicsContext->GetDevice(),
					graphicsContext->GetCommandPool(),
					1,
					&buffer);
			});
		}
	}
	VkCommandBuffer VulkanRenderCommandBuffer::GetCommandBuffer(uint32_t frameIndex)
	{
		if (m_NormalCommandBuffer != nullptr)
			return m_NormalCommandBuffer->As<VulkanCommandBuffer>()-> GetCommandBuffer(frameIndex);
		return m_CommandBuffers[frameIndex];
	}

	void VulkanRenderCommandBuffer::BeginRecord(uint32_t frameIndex)
	{
		PF_CORE_ASSERT(m_Recording == false, "cannot start recoridng when command buffer is still recording");
		m_Recording = true;
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.pNext = nullptr;
		if (vkBeginCommandBuffer(m_CommandBuffers[frameIndex], &beginInfo) != VK_SUCCESS)
			PF_CORE_ASSERT(false, "Failed to begin recording command buffer");
	}

	void VulkanRenderCommandBuffer::EndRecord(uint32_t frameIndex)
	{
		PF_CORE_ASSERT(m_Recording == true, "cannot End recording when recoring never started");
		if (vkEndCommandBuffer(m_CommandBuffers[frameIndex]) != VK_SUCCESS)
			PF_CORE_ASSERT(false, "Faied to record command Buffers");
		m_Recording = false;
	}

};
