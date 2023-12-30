#include "Proofprch.h"
#include "VulkanCommandBuffer.h"
#include "Proof/Renderer/Renderer.h"
#include "VulkanGraphicsContext.h"
#include "VulkanSwapChain.h"
#include "Proof/Renderer/Renderer.h"
#include "VulkanGraphicsPipeline.h"
#include "Vulkan.h"
#include "VulkanRenderer.h"
#include "VulkanDevice.h"
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
		m_ActiveCommandBufferIndex = Renderer::RT_GetCurrentFrameInFlight();

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
		return m_CommandBuffers[Renderer::RT_GetCurrentFrameInFlight()];
	}

	void VulkanRenderCommandBuffer::Build()
	{
		if (m_Swapcahin == true)return;

		const uint32_t framesInFlight = Renderer::GetConfig().FramesFlight;
		auto graphicsContext = VulkanGraphicsContext::Get();
		auto device = VulkanGraphicsContext::Get()->GetDevice();
		m_CommandBuffers.resize(framesInFlight);

		// Timestamp queries
		const uint32_t maxUserQueries = 16;
		m_TimestampQueryCount = 2 + 2 * maxUserQueries;
		m_TimestampQueryPools.resize(framesInFlight);
		m_ExecutionGPUTimes.resize(framesInFlight);
		for (auto& executionGPUTimes : m_ExecutionGPUTimes)
			executionGPUTimes.resize(m_TimestampQueryCount / 2);

		m_TimestampQueryPools.resize(framesInFlight);

		VkQueryPoolCreateInfo queryPoolCreateInfo = {};
		queryPoolCreateInfo.sType = VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO;
		queryPoolCreateInfo.pNext = nullptr;
		queryPoolCreateInfo.queryType = VK_QUERY_TYPE_TIMESTAMP;
		queryPoolCreateInfo.queryCount = m_TimestampQueryCount;

		for (auto& timestampQueryPool : m_TimestampQueryPools)
			VK_CHECK_RESULT(vkCreateQueryPool(device->GetVulkanDevice(), &queryPoolCreateInfo, nullptr, &timestampQueryPool));

		// pipline stats

		m_PipelineQueryCount = 7;
		queryPoolCreateInfo.queryType = VK_QUERY_TYPE_PIPELINE_STATISTICS;
		queryPoolCreateInfo.queryCount = m_PipelineQueryCount;
		queryPoolCreateInfo.pipelineStatistics =
			VK_QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_VERTICES_BIT |
			VK_QUERY_PIPELINE_STATISTIC_INPUT_ASSEMBLY_PRIMITIVES_BIT |
			VK_QUERY_PIPELINE_STATISTIC_VERTEX_SHADER_INVOCATIONS_BIT |
			VK_QUERY_PIPELINE_STATISTIC_CLIPPING_INVOCATIONS_BIT |
			VK_QUERY_PIPELINE_STATISTIC_CLIPPING_PRIMITIVES_BIT |
			VK_QUERY_PIPELINE_STATISTIC_FRAGMENT_SHADER_INVOCATIONS_BIT |
			VK_QUERY_PIPELINE_STATISTIC_COMPUTE_SHADER_INVOCATIONS_BIT;

		m_PipelineStatisticsQueryPools.resize(framesInFlight);
		for (auto& pipelineStatisticsQueryPools : m_PipelineStatisticsQueryPools)
			VK_CHECK_RESULT(vkCreateQueryPool(device->GetVulkanDevice(), &queryPoolCreateInfo, nullptr, &pipelineStatisticsQueryPools));
		m_PipelineStatisticsQueryResults.resize(framesInFlight);

		/// command buffer
		VkCommandPoolCreateInfo cmdPoolInfo = {};
		cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		cmdPoolInfo.queueFamilyIndex = device->GetPhysicalDevice()->GetQueueFamilyIndices().Graphics;
		cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		VK_CHECK_RESULT(vkCreateCommandPool(device->GetVulkanDevice(), &cmdPoolInfo, nullptr, &m_CommandPool));
		VulkanUtils::SetDebugUtilsObjectName(device->GetVulkanDevice(), VK_OBJECT_TYPE_COMMAND_POOL, fmt::format("{} Command Pool", m_DebugName), m_CommandPool);

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.pNext = nullptr;
		allocInfo.commandPool = m_CommandPool;
		allocInfo.commandBufferCount = m_CommandBuffers.size();
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

		if (vkAllocateCommandBuffers(device->GetVulkanDevice(), &allocInfo, m_CommandBuffers.data()) != VK_SUCCESS)
			PF_CORE_ASSERT(false, "Failed to allocate command buffer");

		for (uint32_t i = 0; i < m_CommandBuffers.size(); ++i)
			VulkanUtils::SetDebugUtilsObjectName(device->GetVulkanDevice(),VK_OBJECT_TYPE_COMMAND_BUFFER, fmt::format("{} (frame in flight: {})", m_DebugName, i), m_CommandBuffers[i]);


		VkFenceCreateInfo fenceCreateInfo{};
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
		m_WaitFences.resize(Renderer::GetConfig().FramesFlight);
		for (size_t i = 0; i < m_WaitFences.size(); ++i)
		{
			VK_CHECK_RESULT(vkCreateFence(device->GetVulkanDevice(), &fenceCreateInfo, nullptr, &m_WaitFences[i]));
			VulkanUtils::SetDebugUtilsObjectName(device->GetVulkanDevice(),VK_OBJECT_TYPE_FENCE, fmt::format("{} (frame in flight: {}) fence", m_DebugName, i), m_WaitFences[i]);
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
			Renderer::SubmitResourceFree([commandPool = m_CommandPool,buffer = m_CommandBuffers[i], fence = m_WaitFences[i]]()
			{
				VkDevice device = VulkanGraphicsContext::Get()->GetDevice()->GetVulkanDevice();

				auto graphicsContext = VulkanRenderer::GetGraphicsContext();
				vkFreeCommandBuffers(
					device,
					commandPool,
					1,
					&buffer);

				vkDestroyFence(device, fence, nullptr);

			});
		}

		Renderer::SubmitResourceFree([commandPool = m_CommandPool]() 
		{
			VkDevice device = VulkanGraphicsContext::Get()->GetDevice()->GetVulkanDevice();
			vkDestroyCommandPool(device, commandPool, nullptr);
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

			auto device = VulkanRenderer::GetGraphicsContext()->GetDevice()->GetVulkanDevice();

			uint32_t frameIndex = Renderer::RT_GetCurrentFrameInFlight();

			VkSubmitInfo submitInfo{};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			submitInfo.commandBufferCount = 1;
			VkCommandBuffer commandBuffer = instance->m_CommandBuffers[frameIndex];
			submitInfo.pCommandBuffers = &commandBuffer;

			VK_CHECK_RESULT(vkWaitForFences(device, 1, &instance->m_WaitFences[frameIndex], VK_TRUE, UINT64_MAX));
			VK_CHECK_RESULT(vkResetFences(device, 1, &instance->m_WaitFences[frameIndex]));


			VK_CHECK_RESULT(vkQueueSubmit(VulkanRenderer::GetGraphicsContext()->GetDevice()->GetGraphicsQueue(), 1, &submitInfo, instance->m_WaitFences[frameIndex]));

#if 0
			vkGetQueryPoolResults(device, instance->m_TimestampQueryPools[frameIndex], 0, instance->m_TimestampNextAvailableQuery,
				instance->m_TimestampNextAvailableQuery * sizeof(uint64_t), instance->m_TimestampQueryResults[frameIndex].data(), sizeof(uint64_t), VK_QUERY_RESULT_64_BIT);

			for (uint32_t i = 0; i < instance->m_TimestampNextAvailableQuery; i += 2)
			{
				uint64_t startTime = instance->m_TimestampQueryResults[frameIndex][i];
				uint64_t endTime = instance->m_TimestampQueryResults[frameIndex][i + 1];
				float nsTime = endTime > startTime ? (endTime - startTime) * VulkanRenderer::GetGraphicsContext()->GetDevice()->GetPhysicalDevice()->GetLimits().timestampPeriod : 0.0f;
				instance->m_ExecutionGPUTimes[frameIndex][i / 2] = nsTime * 0.000001f; // Time in ms
			}

			// Retrieve pipeline stats results
			vkGetQueryPoolResults(device, instance->m_PipelineStatisticsQueryPools[frameIndex], 0, 1,
				sizeof(PipelineStatistics), &instance->m_PipelineStatisticsQueryResults[frameIndex], sizeof(uint64_t), VK_QUERY_RESULT_64_BIT);
#endif
			vkDeviceWaitIdle(device);
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

			instance->m_ActiveCommandBufferIndex = Renderer::RT_GetCurrentFrameInFlight();
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
			PF_CORE_ASSERT(instance->m_Recording == true, "cannot End recording when recording never started");
			if (vkEndCommandBuffer(instance->m_CommandBuffers[instance->m_ActiveCommandBufferIndex]) != VK_SUCCESS)
				PF_CORE_ASSERT(false, "Faied to record command Buffers");
			instance->m_Recording = false;
		});
	}
	uint32_t VulkanRenderCommandBuffer::BeginTimestampQuery()
	{
		PF_CORE_ASSERT(m_TimestampNextAvailableQuery >= m_TimestampQueryCount, fmt::format("Max amount: {} of timestamp require reached", m_TimestampQueryCount));

		uint32_t queryIndex = m_TimestampNextAvailableQuery;
		m_TimestampNextAvailableQuery += 2;
		Count<VulkanRenderCommandBuffer> instance = this;
		Renderer::Submit([instance, queryIndex]()
		{
				uint32_t frameIndex = Renderer::RT_GetCurrentFrameInFlight();
				VkCommandBuffer commandBuffer = instance->m_CommandBuffers[frameIndex];
				vkCmdWriteTimestamp(commandBuffer, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, instance->m_TimestampQueryPools[frameIndex], queryIndex);
		});
		return queryIndex;
	}

	void VulkanRenderCommandBuffer::EndTimestampQuery(uint32_t queryID)
	{
		Count<VulkanRenderCommandBuffer> instance = this;
		Renderer::Submit([instance, queryID]()
			{
				uint32_t frameIndex = Renderer::RT_GetCurrentFrameInFlight();
				VkCommandBuffer commandBuffer = instance->m_CommandBuffers[frameIndex];
				vkCmdWriteTimestamp(commandBuffer, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, instance->m_TimestampQueryPools[frameIndex], queryID + 1);
			});
	}

	float VulkanRenderCommandBuffer::GetExecutionGPUTime(uint32_t frameIndex, uint32_t queryIndex) const 
	{
		if (queryIndex == UINT32_MAX || queryIndex / 2 >= m_TimestampNextAvailableQuery / 2)
			return 0.0f;

		return m_ExecutionGPUTimes[frameIndex][queryIndex / 2];
	}
};