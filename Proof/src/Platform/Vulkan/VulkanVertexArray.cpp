#include "Proofprch.h"
#include "VulkanVertexArray.h"

namespace Proof
{
	namespace Utils
	{
		VkVertexInputRate ProofFormatToVulkanFormat(VertexInputRate rate) {
			switch (rate) {
				case Proof::VertexInputRate::Vertex:
					return VkVertexInputRate::VK_VERTEX_INPUT_RATE_VERTEX;
					break;
				case Proof::VertexInputRate::Instance:
					return VkVertexInputRate::VK_VERTEX_INPUT_RATE_INSTANCE;
			}
			PF_CORE_ASSERT("Format not supported");
		}

		VkFormat ProofFormatToVulkanFormat(DataType data) {
			switch (data) {
				case Proof::DataType::Int:
					return VK_FORMAT_R32_SINT;
					break;
				case Proof::DataType::Uint32_t:
					return VK_FORMAT_R32_UINT;
					break;
				case Proof::DataType::Uint64_t:
					return VK_FORMAT_R64_UINT;
					break;
				case Proof::DataType::Float:
					return VK_FORMAT_R32_SFLOAT;
					break;
				case Proof::DataType::Double:
					return VK_FORMAT_R64_SFLOAT;
					break;
				case Proof::DataType::Char:
					return VK_FORMAT_R4G4_UNORM_PACK8;
					break;
				case Proof::DataType::Ivec2:
					return VK_FORMAT_R32G32_SINT;
					break;
				case Proof::DataType::Ivec3:
					return VK_FORMAT_R32G32B32_SINT;
					break;
				case Proof::DataType::Ivec4:
					return VK_FORMAT_R32G32B32A32_SINT;
					break;
				case Proof::DataType::Vec2:
					return VK_FORMAT_R32G32_SFLOAT;
					break;
				case Proof::DataType::Vec3:
					return VK_FORMAT_R32G32B32_SFLOAT;
					break;
				case Proof::DataType::Vec4:
					return VK_FORMAT_R32G32B32A32_SFLOAT;
					break;
				case Proof::DataType::DVec2:
					return VK_FORMAT_R64G64_SFLOAT;
					break;
				case Proof::DataType::DVec3:
					return VK_FORMAT_R64G64B64_SFLOAT;
					break;
				case Proof::DataType::DVec4:
					return VK_FORMAT_R64G64B64A64_SFLOAT;
					break;
				case Proof::DataType::UVec4:
					return VK_FORMAT_R8G8B8A8_UNORM;
					break;
				default:
					break;
			}
			PF_CORE_ASSERT("Format not supported");
		}
	}
	VulkanVertexArray::VulkanVertexArray(const std::vector<VertexInputDataConfig>& config):
		m_InputData(config) {
	}
	void VulkanVertexArray::AddData(uint32_t position, DataType count, size_t offset, uint32_t binding) {
		m_InputData.AddData(position, count, offset, binding);
	}
	
	VulkanVertexInputData::VulkanVertexInputData(const std::vector<VertexInputDataConfig>& config) {
		m_Descriptions.resize(config.size());
		for (uint32_t i = 0; i < config.size(); i++) {
			m_Descriptions[i].binding = i;
			m_Descriptions[i].stride = config[i].Size;
			m_Descriptions[i].inputRate = Utils::ProofFormatToVulkanFormat(config[i].InputRate);
		}
	}

	void VulkanVertexInputData::AddData(uint32_t location, DataType format, uint32_t offset, uint32_t binding) {
		VkVertexInputAttributeDescription attribute = {};
		attribute.binding = binding;
		attribute.location = location;
		attribute.format = Utils::ProofFormatToVulkanFormat(format);
		attribute.offset = offset;
		m_Attributes.emplace_back(attribute);
	}

}
