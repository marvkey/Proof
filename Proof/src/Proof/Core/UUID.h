#pragma once
#include "Core.h"

namespace Proof{
	class UUID{
	public:
		UUID();
		UUID(uint64_t id){
			m_UUID = id;
		}
		UUID(const UUID&)=default;
		operator uint64_t() const{return m_UUID;};
	private:
		uint64_t m_UUID=0;
	};
}

namespace std{
	template<>
	struct hash<Proof::UUID>
	{
		std::size_t operator()(const Proof::UUID& uuid)const{
			return hash<uint64_t>()((uint64_t) uuid);
		}
	};
}