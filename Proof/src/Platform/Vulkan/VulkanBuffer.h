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
		VulkanVertexBuffer(const void* data, uint64_t size);
		VulkanVertexBuffer(uint64_t size);

		void Resize(uint64_t size);
		void Resize(const void* data, uint64_t size);
		virtual void SetData(const void* data, uint64_t size, uint64_t offset=0);
		uint64_t GetVertexSize()const { return m_VertexSize; }
		virtual void Bind(Count<RenderCommandBuffer> commandBuffer,uint64_t binding=0,uint64_t offset =0)const override;

		static VmaAllocator GetGraphicsAllocator();
		template<typename T>
		std::vector<T> GetData();
		Buffer GetDataRaw();
	private:
		void Release();
		void Build();
		VulkanMemmoryUsage m_Usage;
		VulkanBuffer m_VertexBuffer;
		uint64_t m_VertexSize;
		Buffer m_LocalBuffer;
	};

	class VulkanIndexBuffer : public IndexBuffer {
	public:
		VulkanIndexBuffer(uint32_t count);
		VulkanIndexBuffer(const void* data, uint32_t count);
		~VulkanIndexBuffer();
		virtual void Bind(Count<RenderCommandBuffer> commandBuffer)const;
		virtual void SetData(const void* data, uint32_t count, uint32_t offsetCount = 0);

		void Resize(uint32_t count);
		void Resize(const void* data, uint32_t count);
		uint32_t GetCount()const override{ return m_Count;}
		uint32_t GetSize()const override { return m_Count* sizeof(uint32_t);}

		std::vector<uint32_t> GetData()const ;
	private:
		VulkanMemmoryUsage m_Usage;
		uint32_t m_Count;
		VulkanBuffer m_IndexBuffer;

		void Build();
		void Release();
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