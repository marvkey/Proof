#pragma once
#include "Proof/Renderer/Buffer.h"
#include "Proof/Core/Core.h"
#include<vulkan/vulkan.h>
#include <glm/glm.hpp>
#include "Proof/Renderer/Vertex.h"
#include "VulkanUtils/VulkanBufferBase.h"
namespace Proof
{
	enum class VulkanDataFormat{
		Int = VK_FORMAT_R32_SINT,
		Uint32_t = VK_FORMAT_R32_UINT,
		Uint64_t = VK_FORMAT_R64_UINT,
		Float = VK_FORMAT_R32_SFLOAT,
		Double = VK_FORMAT_R64_SFLOAT,
		Char = VK_FORMAT_R4G4_UNORM_PACK8,	

		//signed int vec2
		Ivec2 = VK_FORMAT_R32G32_SINT,
		//signed int vec3
		Ivec3 = VK_FORMAT_R32G32B32_SINT,
		//signed int vec4
		Ivec4 = VK_FORMAT_R32G32B32A32_SINT,


		//vector 2 with float
		Vec2 = VK_FORMAT_R32G32_SFLOAT,
		// vector 3 with float
		Vec3 = VK_FORMAT_R32G32B32_SFLOAT,
		//vector 4 with float
		Vec4 = VK_FORMAT_R32G32B32A32_SFLOAT,

		//vector 2 with double
		DVec2 = VK_FORMAT_R64G64_SFLOAT,
		// vector 3 with double
		DVec3 = VK_FORMAT_R64G64B64_SFLOAT,
		//vector 4 with double
		DVec4 = VK_FORMAT_R64G64B64A64_SFLOAT,

	};

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
		VulkanVertexInputData(uint32_t size, uint32_t binding =0)  {
			m_Descriptions.resize(1);
			m_Descriptions[0].binding = 0;
			m_Descriptions[0].stride = size;
			m_Descriptions[0].inputRate= VK_VERTEX_INPUT_RATE_VERTEX;
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

	};

	void Copy(const void* data, uint32_t size);
	void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	class VulkanVertexBuffer {
	public:
		~VulkanVertexBuffer();
		VulkanVertexBuffer(const void* data, uint32_t size, uint32_t count);
		virtual void Bind(VkCommandBuffer commandBuffer);
	
		virtual void UnBind(){}
		uint32_t GetVertexSize() { return m_VertexSize; }
		uint32_t GetCount() { return m_Count; }
	private:
		VulkanBuffer m_VertexBuffer;
		uint32_t m_VertexSize;
		uint32_t m_Count;
	};

	class VulkanIndexBuffer {
	public:
		VulkanIndexBuffer(const void* data, uint32_t size);
		~VulkanIndexBuffer();
		virtual void Bind(VkCommandBuffer commandBuffer);

		uint32_t GetIndexCount() {
			return m_Count;
		}
		uint32_t GetSize() {
			return m_Size;
		}
	private:
		uint32_t m_Size;
		uint32_t m_Count;
		VulkanBuffer m_IndexBuffer;
	};
}