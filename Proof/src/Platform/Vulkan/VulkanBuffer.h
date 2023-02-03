#pragma once
#include "Proof/Renderer/Buffer.h"
#include "Proof/Core/Core.h"
#include <glm/glm.hpp>
#include "Proof/Renderer/Vertex.h"
#include "VulkanUtils/VulkanBufferBase.h"
namespace Proof
{

	class VulkanVertexBuffer : public VertexBuffer {
	public:
		~VulkanVertexBuffer();
		VulkanVertexBuffer(const void* data, uint32_t size);
		VulkanVertexBuffer(uint32_t size);
		virtual void Bind(Count<RenderCommandBuffer> commandBuffer,uint32_t binding=0)const override;
		virtual void AddData(const void* data, uint32_t size, uint32_t offset=0);
		uint32_t GetVertexSize()const { return m_VertexSize; }
	private:
		VulkanBuffer m_VertexBuffer;
		uint32_t m_VertexSize;
	};

	class VulkanIndexBuffer : public IndexBuffer {
	public:
		VulkanIndexBuffer(const void* data, uint32_t size);
		~VulkanIndexBuffer();
		virtual void Bind(Count<RenderCommandBuffer> commandBuffer)const;

		uint32_t GetCount()const override{
			return m_Count;
		}
		uint32_t GetSize()const override {
			return m_Size;
		}
	private:
		uint32_t m_Size;
		uint32_t m_Count;
		VulkanBuffer m_IndexBuffer;
	};
}