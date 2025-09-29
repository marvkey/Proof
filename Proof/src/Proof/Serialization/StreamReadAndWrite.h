#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Core/Buffer.h"
#include "Proof/Core/Assert.h"

#include <vector>
#include <set>
namespace Proof
{
	class StreamWriter
	{
	public:
		virtual ~StreamWriter() = default;
		virtual bool IsStreamGood() const = 0;
		virtual uint64_t GetStreamPosition()  = 0;
		virtual void SetStreamPosition(uint64_t position) = 0;
		virtual bool WriteData(const char* data, size_t size) = 0;

		operator bool() const { return IsStreamGood(); }

		void WriteBuffer(const Buffer& buffer, bool writeSize = true)
		{
			if (writeSize)
				WriteData((char*)&buffer.Size, sizeof(uint32_t));

			WriteData((char*)buffer.Data, buffer.Size);
		}
		void WriteString(const std::string& string)
		{
			size_t size = string.size();
			WriteData((char*)&size, sizeof(size_t));
			WriteData((char*)string.data(), sizeof(char) * string.size());
		}
		void WriteZero(uint64_t size)
		{
			char zero = 0;
			for (uint64_t i = 0; i < size; i++)
				WriteData(&zero, 1);
		}

		template<typename T>
		void WriteRaw(const T& data)
		{
			PF_CORE_ASSERT(WriteData(reinterpret_cast<const char*>(&data), sizeof(T)));
		}

		template<typename T>
		void WriteObject(const T& obj) { T::Serialize(this, obj); }


		template<typename T>
		void WriteArray(const std::set<T>& array, bool writeSize = true)
		{
			if (writeSize)
				WriteRaw<uint32_t>((uint32_t)array.size());

			for (const auto& element : array)
			{
				if constexpr (std::is_trivial<T>())
					WriteRaw<T>(element);
				else
					WriteObject<T>(element);
			}
		}

		template<typename T>
		void WriteArray(const std::vector<T>& array, bool writeSize = true)
		{
			if (writeSize)
				WriteRaw<uint32_t>((uint32_t)array.size());

			for (const auto& element : array)
			{
				if constexpr (std::is_trivial<T>())
					WriteRaw<T>(element);
				else
					WriteObject<T>(element);
			}
		}
	};


	class StreamReader
	{
	public:
		virtual ~StreamReader() = default;
		virtual bool IsStreamGood() const = 0;
		virtual uint64_t GetStreamPosition() = 0;
		virtual void SetStreamPosition(uint64_t position) = 0;
		virtual bool ReadData(char* dst, size_t size) = 0;

		operator bool() const { return IsStreamGood(); }

		void ReadBufferAndSize(Buffer& buffer)
		{
			uint32_t size;
			ReadData((char*)&buffer.Size, sizeof(uint32_t));

			buffer.Allocate(buffer.Size);
			ReadData((char*)buffer.Data, buffer.Size);
		}

		void ReadBufferWithSize(Buffer& buffer, uint32_t size)
		{
			buffer.Size = size;
			buffer.Allocate(buffer.Size);
			ReadData((char*)buffer.Data, buffer.Size);
		}


		void ReadString(std::string& string)
		{
			size_t size;
			ReadData((char*)&size, sizeof(size_t));

			string.resize(size);
			ReadData((char*)string.data(), sizeof(char) * size);
		}

		template<typename T>
		void ReadRaw(T& data)
		{
			ReadData(reinterpret_cast<char*>(&data), sizeof(T));
		}

		template<typename T>
		void ReadObject(T& obj) { T::Deserialize(this, obj); }


		// if saved the size before saving array
		template<typename T>
		void ReadArrayAndSize(std::set<T>& set)
		{
			uint32_t size;
			ReadRaw<uint32_t>(size);

			for (uint32_t i = 0; i < size; i++)
			{
				T value;
				if constexpr (std::is_trivial<T>())
					ReadRaw<T>(value);
				else
					ReadObject<T>(value);
				set.emplace(value);
			}
		}

		// did not save size before saving array
		template<typename T>
		void ReadArrayWithSize(std::set<T>& array, uint32_t size)
		{

			for (uint32_t i = 0; i < size; i++)
			{
				if constexpr (std::is_trivial<T>())
					ReadRaw<T>(array[i]);
				else
					ReadObject<T>(array[i]);

				array.emplace(array[i]);
			}
		}


		// if saved the size before saving array
		template<typename T>
		void ReadArrayAndSize(std::vector<T>& array)
		{
			uint32_t size;
			ReadRaw<uint32_t>(size);
			array.resize(size); 

			for (uint32_t i = 0; i < size; i++)
			{
				T value;
				if constexpr (std::is_trivial<T>())
					ReadRaw<T>(value);
				else
					ReadObject<T>(value);
				array[i] = value;
			}
		}

		// did not save size before saving array
		template<typename T>
		void ReadArrayWithSize(std::vector<T>& array, uint32_t size)
		{
			array.resize(size);
			for (uint32_t i = 0; i < size; i++)
			{
				if constexpr (std::is_trivial<T>())
					ReadRaw<T>(array[i]);
				else
					ReadObject<T>(array[i]);
			}
		}
	};

}