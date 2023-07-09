#pragma once
#include <stdint.h>
#include <cstring>
namespace Proof{
	// Non-owning raw buffer class
	struct Buffer
	{
		Buffer() = default;

		Buffer(uint64_t size)
		{
			Allocate(size);
		}
		Buffer(const Buffer&) = default;
		Buffer(uint8_t* data, uint64_t size, bool copyData = false):
			m_Size(size)
		{
			if (copyData && data && size > 0)
			{
				Allocate(size);
				memcpy(m_Data, data, m_Size);
			}
			else
			{
				m_Data = data;
			}
		}
		void Copy(const void* data, uint32_t size)
		{
			Release();
			Allocate(size);
			memcpy(m_Data, data, size);
		}
		static Buffer Copy(Buffer other)
		{
			Buffer result(other.m_Size);
			memcpy(result.m_Data, other.m_Data, other.m_Size);
			return result;
		}
		// copy to specif region of the buffer
		void SetData(const void* data, uint32_t size, uint32_t offset)
		{
			uint8_t* newData = m_Data;
			newData += offset;
			memcpy(newData, data, size);
		}
		// copy to specif region of the buffer
		void SetData(const Buffer& buffer,uint32_t offset)
		{
			uint8_t* newData = m_Data;
			newData += offset;
			memcpy(newData, buffer.Get(), buffer.GetSize());
		}
		void Allocate(uint64_t size)
		{
			if (size == 0)
				return;
			Release();
			m_Data = new uint8_t[size];
			m_Size = size;
		}
	
		uint8_t* Get()const
		{
			return m_Data;
		}
		uint64_t GetSize()const {
			return m_Size;
		}
		template<typename T>
		T* As()
		{
			return (T*)m_Data;
		}

		operator bool() const
		{
			return (bool)m_Data;
		}
		void Release()
		{
			delete[] m_Data;
			m_Data = nullptr;
			m_Size = 0;
		}
	private:
		uint8_t* m_Data = nullptr;
		uint64_t m_Size = 0;
	};
}
