#include "Proofprch.h"
#include "VulkanVertexArray.h"

namespace Proof
{
	VulkanVertexArray::VulkanVertexArray(const std::vector<VulkanVertexInputDataConfig>& config):
		m_InputData(config) {
	}
	void VulkanVertexArray::SetData(uint32_t position, VulkanDataFormat count, size_t offset, uint32_t binding) {
		m_InputData.AddData(position, count, offset, binding);
	}
	
}
