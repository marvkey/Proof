#pragma once
#include "Proof/Core/Core.h"
#include<vulkan/vulkan.h>
#include "Proof/Renderer/VertexArray.h"
namespace Proof{
	struct VulkanVertexInput {
		VulkanVertexInput() {};
		VulkanVertexInput(const std::vector<VkVertexInputBindingDescription>& descriptions, const std::vector<VkVertexInputAttributeDescription>& attributes) {
			m_Attributes = attributes;
			m_Descriptions = descriptions;
		}
		const std::vector<VkVertexInputBindingDescription>& GetDescriptions()const {
			return m_Descriptions;
		}
		const std::vector<VkVertexInputAttributeDescription>& GetAttributes()const {
			return m_Attributes;
		}
	private:
		std::vector<VkVertexInputBindingDescription> m_Descriptions;
		std::vector<VkVertexInputAttributeDescription> m_Attributes;
	};
	
	class VulkanVertexInputData {
	public:
		VulkanVertexInputData(const std::vector<VertexInputDataConfig>& config);
		void AddData(uint32_t location, DataType format, uint32_t offset, uint32_t binding = 0);
		VulkanVertexInput GetData() {
			return VulkanVertexInput(m_Descriptions, m_Attributes);
		}
		const std::vector<VkVertexInputBindingDescription>& GetDescriptions()const {
			return m_Descriptions;
		}
		const std::vector<VkVertexInputAttributeDescription>& GetAttributes()const {
			return m_Attributes;
		}
	private:
		std::vector<VkVertexInputBindingDescription> m_Descriptions;
		std::vector<VkVertexInputAttributeDescription> m_Attributes;
	};
	class VulkanVertexArray : public VertexArray {
	public:
		VulkanVertexArray(const std::vector<VertexInputDataConfig>& config);
		virtual void AddData(uint32_t Position, DataType Count, size_t Offset,uint32_t binding =0);
		VulkanVertexInput GetData() {
			return m_InputData.GetData();
		}
	private:
		VulkanVertexInputData m_InputData;
	};
}
