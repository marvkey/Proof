#pragma once
#include "Proof/Renderer/Buffer.h"
#include "Proof/Core/Core.h"
#include "VulkanUtils/VulkanTypes.h"
#include <glm/glm.hpp>
#include "Proof/Renderer/Vertex.h"
#include "VulkanUtils/VulkanBufferBase.h"
namespace Proof
{

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