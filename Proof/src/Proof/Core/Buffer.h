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
		//data is in (uint8_t*) just using void* so user can avoid casitng
		Buffer(const void* data, uint64_t size, bool copyData = false):
			Size(size)
		{
			if (copyData && data && size > 0)
			{
				Allocate(size);
				memcpy(Data, data, Size);
			}
			else
			{
				Data = (uint8_t*)data;
			}
		}
		void Copy(const void* data, uint32_t size)
		{
			Release();
			Allocate(size);
			memcpy(Data, data, size);
		}
		void ZeroInitialize()
		{
			if (Data)
				memset(Data, 0, Size);
		}
		static Buffer Copy(Buffer other)
		{
			Buffer result(other.Size);
			memcpy(result.Data, other.Data, other.Size);
			return result;
		}
		// copy to specif region of the buffer
		void SetData(const void* data, uint32_t size, uint32_t offset)
		{
			uint8_t* newData = Data;
			newData += offset;
			memcpy(newData, data, size);
		}
		// copy to specif region of the buffer
		void SetData(const Buffer& buffer,uint32_t offset)
		{
			uint8_t* newData = Data;
			newData += offset;
			memcpy(newData, buffer.Get(), buffer.GetSize());
		}
		void Allocate(uint64_t size)
		{
			if (size == 0)
				return;
			Release();
			Data = new uint8_t[size];
			Size = size;
		}
	
		uint8_t* Get()const
		{
			return Data;
		}
		uint64_t GetSize()const {
			return Size;
		}
		template<typename T>
		T* As()
		{
			return (T*)Data;
		}

		void Fill(uint8_t value)
		{
			if(Size >0)
				std::memset(Data, value, Size*sizeof(uint8_t));
		}

		template<typename T>
		T& Read(uint64_t offset)
		{
			return *static_cast<T*>(static_cast<void*>(Data + offset));
		}
		operator bool() const
		{
			return (bool)Data;
		}
		void Release()
		{
			delete[] Data;
			Data = nullptr;
			Size = 0;
		}
		uint8_t* Data = nullptr;
		uint64_t Size = 0;
	};
}
