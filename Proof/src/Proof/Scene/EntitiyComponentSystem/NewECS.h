#pragma once
#include <vector>
#include <unordered_map>
#include <type_traits>
namespace Proof{
	//template<typename T>
	struct ComponentPool {
	private:

		inline char* get(size_t index) {
			return static_cast<char*>(m_ComponentPool[index]);
		}
	private:
		std::vector<void*> m_ComponentPool;
	};
	using EntityID = uint64_t;
	class NewECS{
	public:
		template<typename T> 
		T* AddComponent(EntityID uniqeid){
			bool temp= std::is_same<T,m_Pool[0]>::value;
			return nullptr;
			m_Pool.resize(1);
		}
	private:
		std::vector<EntityID>m_Pool;
		std::unordered_map<EntityID,ComponentPool>m_Componet;
	};

}