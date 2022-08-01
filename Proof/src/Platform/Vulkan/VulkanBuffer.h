#pragma once
#include "Proof/Renderer/Buffer.h"
#include "Proof/Core/Core.h"
#include<vulkan/vulkan.h>
#include <glm/glm.hpp>
#include "Proof/Renderer/Vertex.h"
//#include <vulkan/VulkanProofExternalLibs/vk_mem_alloc.h>

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


		//vec2 with float
		Vec2 = VK_FORMAT_R32G32_SFLOAT,
		// vec3 with float
		Vec3 = VK_FORMAT_R32G32B32_SFLOAT,
		//vec4 with float
		Vec4 = VK_FORMAT_R32G32B32A32_SFLOAT,

		//vec2 with double
		DVec2 = VK_FORMAT_R64G64_SFLOAT,
		// vec3 with double
		DVec3 = VK_FORMAT_R64G64B64_SFLOAT,
		//vec4 with double
		DVec4 = VK_FORMAT_R64G64B64A64_SFLOAT,

	};

	struct VulkanVertexInput {
		VulkanVertexInput() {};
		VulkanVertexInput(const std::vector<VkVertexInputBindingDescription>& descriptions, const std::vector<VkVertexInputAttributeDescription>& attributes) {
			m_Descriptions = descriptions;
			m_Attributes = attributes;
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
			VulkanVertexInput(m_Descriptions, m_Attributes);
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
	
	class VulkanVertexBuffer {
	public:
		~VulkanVertexBuffer();
		VulkanVertexBuffer(void* data, size_t size);
		virtual void Bind(VkCommandBuffer commandBuffer);
		virtual void AddData(const void* Data, uint32_t Size, uint32_t SizeOfVertexBuffer = 0){}
		/**
		*removes as Current vertex Buffer
		*/
		virtual void UnBind(){}
		uint32_t GetVertexCount() { return m_VertexCount; }
	private:
		VkBuffer m_VertexBuffer;
		VkDeviceMemory m_VertexBufferMemory;

		//VmaAllocation m_Allocation = nullptr;
		uint32_t m_VertexCount;
	};

	class VulkanIndexBuffer {
	public:
		VulkanIndexBuffer(void* data, uint32_t size);
		~VulkanIndexBuffer();
		virtual void Bind(VkCommandBuffer commandBuffer);

		uint32_t GetIndexCount() {
			return GetSize()/sizeof(uint32_t);
		}
		uint32_t GetSize() {
			return m_Size;
		}
	private:
		uint32_t m_Size;
		VkBuffer m_IndexBuffer;
		VkDeviceMemory m_IndexBufferMemory;
		//VmaAllocation m_Allocation = nullptr;
	};

	
}