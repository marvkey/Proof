#include "Proofprch.h"
#include "Timer.h"
namespace Proof
{
	std::unordered_map<std::string, std::pair<float, uint32_t>> ScopePerformanceTimer::s_Timers = {};
	ScopeTimer::~ScopeTimer() {
		float time = m_Timer.ElapsedMillis();
		PF_EC_TRACE("{TIMER} {} - {}ms", m_Name, time);
	}
	
	ScopePerformanceTimer::~ScopePerformanceTimer()
	{
		std::scoped_lock<std::mutex> lock(m_PerFrameDataMutex);

		float time = m_Timer.ElapsedMillis();
		s_Timers[m_Name].first += time;
		s_Timers[m_Name].second += 1;
	}
	void ScopePerformanceTimer::ClearTimers()
	{
		std::scoped_lock<std::mutex> lock(m_PerFrameDataMutex);
		s_Timers.clear();
	}
}