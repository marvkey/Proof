#pragma once
#include <string>
#define MAGIC_ENUM_RANGE_MIN -10
#define MAGIC_ENUM_RANGE_MAX 5000
#include <magic_enum.hpp>
#include <vector>
#include <unordered_set>
#include <type_traits>

namespace Proof
{
	
	// PURPOSE OF THIS CLASS IS TO HAVE A BETTER API FOR MAGIC ENUM THAT SUITS PROOF
	class EnumReflection 
	{
	public:
		template <typename E>
		static std::string EnumString(E value) {
			auto name = magic_enum::enum_name<E>(value);
			return std::string(name);
		}
		template <typename E, typename Func>
		static void ForEach(Func func) {
			magic_enum::enum_for_each<E>([&](E val) {
				func(val);
			});
		}
		template <typename E>
		static uint64_t GetCount() {
			return (uint64_t)magic_enum::enum_count<E>();
		}

		template<typename E>
		static std::vector<std::string> GetNames(const std::unordered_set<E>& unused = {}) {
			std::vector<std::string> temp;
			magic_enum::enum_for_each<E>([&](auto val) {
				if (!unused.contains(val))
				{
					auto name = magic_enum::enum_name<E>(val);
					std::string s = std::string(name);
					temp.emplace_back(std::string(name));
				}
				
			});
			return temp;
		}
		
		template<typename E>
		static E StringEnum(const std::string& name) {
			auto type = magic_enum::enum_cast<E>(name);
			if (type.has_value())
				return type.value();
			for (int i = MAGIC_ENUM_RANGE_MIN; i < MAGIC_ENUM_RANGE_MAX + 1; i++)
			{
				if (magic_enum::enum_contains<E>(i))
				{
					return (E)i;
				}
			}
			return (E)0;
		}

		// does not return a defaultValue if 
		//the name is not equal to an enum
		template<typename E>
		static auto StringEnumUnchecked(const std::string& name) 
		{
			auto type = magic_enum::enum_cast<E>(name);
			return type;
		}

		template<typename Enum>
		static bool HasAllFlags(Enum Flags, Enum Contains)
		{
			using UnderlyingType = std::underlying_type_t<Enum>;
			return (static_cast<UnderlyingType>(Flags) & static_cast<UnderlyingType>(Contains)) == static_cast<UnderlyingType>(Contains);
		}

		template<typename Enum>
		static bool HasAnyFlags(Enum Flags, Enum Contains)
		{
			using UnderlyingType = std::underlying_type_t<Enum>;
			return (static_cast<UnderlyingType>(Flags) & static_cast<UnderlyingType>(Contains)) != 0;
		}


		template<typename Enum>
		static bool HasOnlyFlags(Enum Flags, Enum Contains)
		{
			using UnderlyingType = std::underlying_type_t<Enum>;
			UnderlyingType allFlags = static_cast<UnderlyingType>(Flags);
			UnderlyingType containsFlags = static_cast<UnderlyingType>(Contains);

			// Check if Flags contains only the flags specified in Contains
			return (allFlags & containsFlags) == containsFlags && allFlags == containsFlags;
		}

		template<typename Enum>
		static bool HasMultipleFlags(Enum Flags, Enum Contains)
		{
			using UnderlyingType = std::underlying_type_t<Enum>;
			UnderlyingType allFlags = static_cast<UnderlyingType>(Flags);
			UnderlyingType containsFlags = static_cast<UnderlyingType>(Contains);

			// Count the number of set bits (flags) in Flags that are also set in Contains
			UnderlyingType commonFlags = allFlags & containsFlags;

			// If the count of common flags is greater than 1, return true
			return Popcount(commonFlags) > 1;
		}

		template<typename Enum>
		static int CountFlags(Enum Flags)
		{
			using UnderlyingType = std::underlying_type_t<Enum>;
			UnderlyingType allFlags = static_cast<UnderlyingType>(Flags);

			// Count the number of set bits (flags) in Flags
			int count = Popcount(allFlags);
			return count;
		}

		template<typename Enum>
		static void AddFlags(Enum& Flags, Enum FlagsToAdd)
		{
			using UnderlyingType = std::underlying_type_t<Enum>;
			Flags = static_cast<Enum>(static_cast<UnderlyingType>(Flags) | static_cast<UnderlyingType>(FlagsToAdd));
		}

		template<typename Enum>
		static void RemoveFlags(Enum& Flags, Enum FlagsToRemove)
		{
			using UnderlyingType = std::underlying_type_t<Enum>;
			Flags = static_cast<Enum>(static_cast<UnderlyingType>(Flags) & ~static_cast<UnderlyingType>(FlagsToRemove));
		}
	private:
		/*
		*
		The Popcount function calculates the number of set bits (bits with a value of 1) in an integer. Here's a breakdown of how it works:

		Input:

		The function takes an integer n as input.
		Loop:

		Inside the function, there's a while loop that continues until n becomes zero.
		In each iteration of the loop, the least significant bit (LSB) of n is cleared. This is done by performing a bitwise AND operation between n and n - 1. This operation effectively clears the rightmost set bit in n.
		After clearing the LSB, the loop increments the count of set bits.
		Count:

		The function keeps track of the count of set bits encountered during the loop iterations.
		Return:

		Once all bits in n are cleared (i.e., n becomes zero), the function returns the count of set bits.
		In essence, the Popcount function calculates the Hamming weight of the integer n, which represents the number of set bits in its binary representation.

		This function is useful for various tasks, such as counting the number of set bits in a bit mask, determining the parity of a number, or implementing certain algorithms like population count-based sorting algorithms.
			**/
		template<typename T>
		static int Popcount(T n) 
		{
			int count = 0;
			while (n) {
				n &= (n - 1);
				count++;
			}
			return count;
		}
	};
}