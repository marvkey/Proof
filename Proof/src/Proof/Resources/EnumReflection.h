#pragma once
#include <string>
#define MAGIC_ENUM_RANGE_MIN -10
#define MAGIC_ENUM_RANGE_MAX 5000
#include <magic_enum.hpp>
#include <vector>
#include <unordered_set>
namespace Proof
{
	
	// PURPOSE OF THIS CLASS IS TO HAVE A BETTER API FOR MAGIC ENUM THAT SUITS PROOF
	class EnumReflection {
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
		static auto StringEnumUnchecked(const std::string& name) {
			auto type = magic_enum::enum_cast<E>(name);
			return type;
		}
	};
}