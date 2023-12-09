#pragma once
#include<vulkan/vulkan.h>
#include "Proof/Renderer/CommandBuffer.h"
#include<vector>
namespace Proof
{
	class VulkanSwapChain;
	class VulkanGraphicsPipeline;
	// for submiting straight to quee so does not need funcitonality
	class VulkanCommandBuffer : public CommandBuffer {
	public:
		VulkanCommandBuffer();
		virtual ~VulkanCommandBuffer();
		VkCommandBuffer GetCommandBuffer(uint32_t frameInFlight){
			return m_CommandBuffer[frameInFlight];
		}
	private:
		friend class VulkanSwapChain;
		std::vector<VkCommandBuffer> m_CommandBuffer;
		friend class VulkanRenderPass;
		friend class VulkanRendererAPI;
		friend class VulkanTextureCube;
		uint32_t m_ActiveCommandBufferIndex = 0;
	};


	class VulkanRenderCommandBuffer : public RenderCommandBuffer {
	public:
		VulkanRenderCommandBuffer(std::string debugName = "", bool swapchaing = false);
		VulkanRenderCommandBuffer(CommandBuffer* buffer);
		virtual ~VulkanRenderCommandBuffer();

		//returns query id
		uint32_t BeginTimestampQuery();
		void EndTimestampQuery(uint32_t queryID);
		//VkCommandBuffer GetCommandBuffer(uint32_t frameInFlight );

		VkCommandBuffer GetActiveCommandBuffer();
		virtual float GetExecutionGPUTime(uint32_t frameIndex, uint32_t queryIndex = 0) const;
	private:
		void Submit();
		void BeginRecord();
		void EndRecord();
		void Build();
		void Release();

	private:
		uint32_t m_ActiveCommandBufferIndex = 0;
		std::vector<VkCommandBuffer> m_CommandBuffers;
		std::vector<VkFence> m_WaitFences;
		std::string m_DebugName;
		VkCommandPool m_CommandPool = nullptr;
		CommandBuffer* m_NormalCommandBuffer =nullptr;

		bool m_Recording = false;
		bool m_Swapcahin = false;

		//timestep
		uint32_t m_TimestampNextAvailableQuery = 2;
		uint32_t m_TimestampQueryCount = 0;
		std::vector<VkQueryPool> m_TimestampQueryPools;
		// vecotr for frames , adn vecotr for query index
		std::vector<std::vector<float>> m_ExecutionGPUTimes;
		std::vector<std::vector<uint64_t>> m_TimestampQueryResults;

		// pipeline querry count
		uint32_t m_PipelineQueryCount = 0;
		std::vector<PipelineStatistics> m_PipelineStatisticsQueryResults;
		std::vector<VkQueryPool> m_PipelineStatisticsQueryPools;

		friend class VulkanRenderPass;
		friend class VulkanSwapChain;
		friend class VulkanRenderer;
		friend class VulkanRendererAPI;
	};
}
