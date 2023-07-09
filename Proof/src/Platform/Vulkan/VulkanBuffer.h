#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Renderer/Buffer.h"
#include <glm/glm.hpp>
#include "VulkanUtils/VulkanBufferBase.h"
namespace Proof
{
	class VulkanVertexBuffer : public VertexBuffer {
	public:
		~VulkanVertexBuffer();
		VulkanVertexBuffer(const void* data, uint32_t size);
		VulkanVertexBuffer(uint32_t size);

		void Resize(uint32_t size);
		void Resize(const void* data, uint32_t size);
		virtual void Bind(Count<RenderCommandBuffer> commandBuffer,uint32_t binding=0)const override;
		virtual void SetData(const void* data, uint32_t size, uint32_t offset=0);
		uint32_t GetVertexSize()const { return m_VertexSize; }
		static VmaAllocator GetGraphicsAllocator();
		template<typename T>
		std::vector<T> GetData();

		Buffer GetDataRaw();
	private:
		void Release();
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

		std::vector<uint32_t> GetData()const ;
	private:
		uint32_t m_Size;
		uint32_t m_Count;
		VulkanBuffer m_IndexBuffer;
	};
	template<typename T>
	inline std::vector<T> VulkanVertexBuffer::GetData()
	{

		std::vector<T> data;
		void* vertexData;
		vmaMapMemory(VulkanVertexBuffer::GetGraphicsAllocator(), m_VertexBuffer.Allocation, &vertexData);
		T* vertices = static_cast<T*>(vertexData);
		for (size_t i = 0; i < m_VertexSize/sizeof(T); i++)
		{
			data.emplace_back(vertices[i]);
		}
		vmaUnmapMemory(VulkanVertexBuffer::GetGraphicsAllocator(), m_VertexBuffer.Allocation);
		return data;
	}
}