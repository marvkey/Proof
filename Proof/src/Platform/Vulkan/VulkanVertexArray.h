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
	class VulkanVertexInputData {
	public:
		VulkanVertexInputData(uint32_t size, uint32_t binding = 0) {
			m_Descriptions.resize(1);
			m_Descriptions[0].binding = 0;
			m_Descriptions[0].stride = size;
			m_Descriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		}
		void AddData(uint32_t location, VulkanDataFormat format, uint32_t offset) {

			VkVertexInputAttributeDescription attribute = {};
			attribute.binding = m_binding;
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
		uint32_t m_binding = 0;
		std::vector<VkVertexInputBindingDescription> m_Descriptions;
		std::vector<VkVertexInputAttributeDescription> m_Attributes;
	};
	class VulkanVertexArray {
	public:
		VulkanVertexArray(uint32_t size);
		virtual void SetData(uint32_t Position, VulkanDataFormat Count, size_t Offset);
		VulkanVertexInput GetData() {
			return m_InputData.GetData();
		}
	private:
		VulkanVertexInputData m_InputData;
	};
}
