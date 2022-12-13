#pragma once
#include "Proof/Core/Core.h"
#include<vulkan/vulkan.h>
#include "VulkanUtils/VulkanTypes.h"
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
	struct VulkanVertexInputDataConfig {
		uint32_t Size;
		VkVertexInputRate InputRate ;
		VulkanVertexInputDataConfig(uint32_t size, VkVertexInputRate inputRate = VK_VERTEX_INPUT_RATE_VERTEX):
			Size(size), InputRate(inputRate)
		{
			
		}
	};
	class VulkanVertexInputData {
	public:
		VulkanVertexInputData(const std::vector<VulkanVertexInputDataConfig>& config) {
			m_Descriptions.resize(config.size());
			for (uint32_t i = 0; i < config.size(); i++) {
				m_Descriptions[i].binding = i;
				m_Descriptions[i].stride = config[i].Size;
				m_Descriptions[i].inputRate = config[i].InputRate;
			}
		}
		void AddData(uint32_t location, VulkanDataFormat format, uint32_t offset, uint32_t binding =0) {
			VkVertexInputAttributeDescription attribute = {};
			attribute.binding = binding;
			attribute.location = location;
			attribute.format = (VkFormat)format;
			attribute.offset = offset;
			m_Attributes.emplace_back(attribute);
		}
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
	class VulkanVertexArray {
	public:
		VulkanVertexArray(const std::vector<VulkanVertexInputDataConfig>& config);
		virtual void SetData(uint32_t Position, VulkanDataFormat Count, size_t Offset,uint32_t binding =0);
		VulkanVertexInput GetData() {
			return m_InputData.GetData();
		}
	private:
		VulkanVertexInputData m_InputData;
	};
}
