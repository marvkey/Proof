#pragma once
namespace Proof{
	class UUID{
	public:
		UUID();
		UUID(uint64_t id){
			m_UUID = id;
		}
		UUID(const UUID&)=default;
		operator uint64_t () { return m_UUID; }
		operator const uint64_t() const { return m_UUID; }
		uint64_t Get()const { return m_UUID; }
	private:
		uint64_t m_UUID=0;
	};

	class UUID32
	{
	public:
		UUID32();
		UUID32(uint32_t uuid);
		UUID32(const UUID32& other);
		uint32_t Get()const { return m_UUID; }

		operator uint32_t () { return m_UUID; }
		operator const uint32_t() const { return m_UUID; }
	private:
		uint32_t m_UUID = 0;
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

	template <>
	struct hash<Proof::UUID32>
	{
		std::size_t operator()(const Proof::UUID32& uuid) const
		{
			return hash<uint32_t>()((uint32_t)uuid);
		}
	};
}