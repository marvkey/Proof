#include "Proofprch.h"
#include "VulkanVertexArray.h"

namespace Proof
{
	VulkanVertexArray::VulkanVertexArray(uint32_t size):
		m_InputData(size) {
	}
	void VulkanVertexArray::SetData(uint32_t position, VulkanDataFormat count, size_t offset) {
		m_InputData.AddData(position, count, offset);
	}
}
