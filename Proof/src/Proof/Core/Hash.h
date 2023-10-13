#pragma once
#include <string>
namespace Proof
{

	class Hash
	{
	public:
		static constexpr uint32_t GenerateFNVHash(std::string_view str)
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

		uint64_t GenerateFNV1aHash64(const void* data, std::size_t size) {

			// FNV-1a constants for 64-bit
			constexpr uint64_t FNV_OFFSET_BASIS = 14695981039346656037ull;
			constexpr uint64_t FNV_PRIME = 1099511628211ull;
			const unsigned char* bytes = static_cast<const unsigned char*>(data);
			uint64_t hash = FNV_OFFSET_BASIS;

			for (std::size_t i = 0; i < size; ++i)
			{
				hash ^= static_cast<uint64_t>(bytes[i]);
				hash *= FNV_PRIME;
			}

			return hash;
		}
	};
}