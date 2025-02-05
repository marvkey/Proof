#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Renderer/Buffer.h"
#include "VulkanUtils/VulkanBufferBase.h"
namespace Proof
{
	class RenderBuffer
	{
	public:
		RenderBuffer(uint64_t bufferSize, uint32_t numBuffers = 1)
			: m_BufferSize(bufferSize), m_NumBuffers(numBuffers)
		{
			// Allocate the specified number of buffers
			m_Buffers.resize(m_NumBuffers);
			for (auto& buffer : m_Buffers)
			{
				buffer.Allocate(m_BufferSize);
			}
		}

		// Set data into the active buffer
		void SetData(const void* data, uint32_t size, uint32_t offset = 0)
		{
			std::lock_guard<std::mutex> lock(m_BufferMutex);
			if (size + offset > m_BufferSize)
			{
				throw std::runtime_error("Data size exceeds buffer capacity!");
			}

			// Write data into the current buffer
			m_Buffers[m_CurrentBufferIndex].SetData(data, size, offset);
		}

		// Get the active buffer for rendering
		const Buffer& GetActiveBuffer() const
		{
			std::lock_guard<std::mutex> lock(m_BufferMutex);
			return m_Buffers[m_CurrentBufferIndex];
		}

		// Swap to the next buffer (used after submission)
		void SwapBuffer()
		{
			std::lock_guard<std::mutex> lock(m_BufferMutex);
			m_CurrentBufferIndex = (m_CurrentBufferIndex + 1) % m_NumBuffers;
		}

		// Get the size of the buffer
		uint64_t GetBufferSize() const
		{
			return m_BufferSize;
		}
	private:

	private:
		uint64_t m_BufferSize;                      // Size of each buffer
		uint32_t m_NumBuffers;                      // Number of buffers (e.g., double buffering)
		std::vector<Buffer> m_Buffers;              // Storage for multiple buffers
		mutable std::mutex m_BufferMutex;           // Mutex for thread-safe access
		std::atomic<uint32_t> m_CurrentBufferIndex; // Active buffer index
	};
	class VulkanVertexBuffer : public VertexBuffer {
	public:
		virtual ~VulkanVertexBuffer();
		VulkanVertexBuffer(const void* data, uint64_t size);
		VulkanVertexBuffer(uint64_t size);

		void Resize(uint64_t size);
		void Resize(const void* data, uint64_t size);
		virtual void SetData(const void* data, uint64_t size, uint64_t offset = 0);
		virtual void RT_SetData(const void* data, uint64_t size, uint64_t offset=0);
		uint64_t GetVertexSize()const { return m_VertexSize; }
		virtual void Bind(Count<RenderCommandBuffer> commandBuffer,uint64_t binding=0,uint64_t offset =0)override;

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

	class VulkanIndexBuffer : public IndexBuffer 
	{
	public:
		VulkanIndexBuffer(uint32_t size);
		VulkanIndexBuffer(const void* data, uint32_t size);
		virtual ~VulkanIndexBuffer();
		virtual void Bind(Count<RenderCommandBuffer> commandBuffer);
		virtual void RT_SetData(const void* data, uint32_t size, uint32_t offsetSize = 0);
		virtual void SetData(const void* data, uint32_t size, uint32_t offsetSize = 0);

		void Resize(uint32_t size);
		void Resize(const void* data, uint32_t size);
		uint32_t GetSize()const override { return m_Size; }
		std::vector<uint32_t> GetData()const ;
		virtual Buffer GetDataRaw();
	private:
		void Build();
		void Release();

	private:
		VulkanMemmoryUsage m_Usage;
		uint32_t m_Size;
		VulkanBuffer m_IndexBuffer;
		Buffer m_LocalBuffer;
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