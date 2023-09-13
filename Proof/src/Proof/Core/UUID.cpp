#include "Proofprch.h"
#include "UUID.h"
#include<random>
namespace Proof
{
	static std::random_device s_RandomDevice;
	static std::mt19937_64 s_Engine(s_RandomDevice());
	static std::uniform_int_distribution<uint64_t> s_UniformDistribution = std::uniform_int_distribution<uint64_t>(1, (std::numeric_limits<uint64_t>::max)());
	static std::uniform_int_distribution<uint32_t> s_UniformDistribution32 = std::uniform_int_distribution<uint32_t>(1, (std::numeric_limits<uint32_t>::max)());
	UUID::UUID()
		:m_UUID(s_UniformDistribution(s_Engine))
	{
	
	}

	UUID32::UUID32()
		: m_UUID(s_UniformDistribution32(s_Engine))
	{
	}

	UUID32::UUID32(uint32_t uuid)
		: m_UUID(uuid)
	{
	}

	UUID32::UUID32(const UUID32& other)
		: m_UUID(other.m_UUID)
	{
	}
}