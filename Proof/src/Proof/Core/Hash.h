#pragma once
#include <string>
namespace Proof
{
	//https://github.com/Jumballaya/fnv-hash/blob/master/src/fnv1a.cpp
	//https://www.boost.org/doc/libs/1_82_0/libs/unordered/examples/fnv1.hpp
	class Hash
	{
	public:
		static constexpr uint32_t GenerateFNVHash32(std::string_view str)
		{
			constexpr uint32_t FNV_PRIME = 16777619u;
			constexpr uint32_t OFFSET_BASIS = 2166136261u;

			const size_t length = str.length();
			const char* data = str.data();

			uint32_t hash = OFFSET_BASIS;
			for (size_t i = 0; i < length; ++i)
			{
				hash ^= *data++;
				hash *= FNV_PRIME;
			}
			hash ^= '\0';
			hash *= FNV_PRIME;

			return hash;
		}

		static constexpr uint64_t GenerateFNVHash64(std::string_view str)
		{
			// FNV-1a constants for 64-bit
			constexpr uint64_t FNV_PRIME = 1099511628211ull;//ull indicates 64 bit u indicates 32 bits
			constexpr uint64_t FNV_OFFSET_BASIS = 14695981039346656037ull; 

			const size_t length = str.length();
			const char* data = str.data();
			uint64_t hash = FNV_OFFSET_BASIS;

			for (size_t i = 0; i < length; ++i)
			{
				hash ^= *data++;
				hash *= FNV_PRIME;
			}
			hash ^= '\0';
			hash *= FNV_PRIME;
			return hash;
		}
	};
}