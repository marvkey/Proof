#pragma once
#include <vector>
#include <unordered_map>
#include <type_traits>
typedef uint64_t EntityID;
namespace Proof{
	template<typename T>
	struct ComponentPool {
		
	};  
	class NewECS{
	public:
		template<class T>
		T* AddComponent(EntityID id) {
			return m_entityHolder.at(id).emplace_back<T>;
		}
	private:
		std::unordered_map<EntityID, std::vector<void*>>m_entityHolder;
	};

}