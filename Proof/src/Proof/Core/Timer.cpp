#include "Proofprch.h"
#include "Timer.h"
#include "Proof/Utils/StringUtils.h"
namespace Proof
{
	std::unordered_map<std::string, std::pair<float, uint32_t>> ScopePerformanceTimer::s_Timers = {};
	ScopeTimer::~ScopeTimer() {
		if (m_TimerRef == nullptr)
		{
			float time = m_Timer.ElapsedMillis();

			//PF_EC_INFO("TIMER {} - {}ms", m_Name, time);
			PF_EC_INFO("TIMER {} - {}", m_Name, Utils::String::DurationToString( time));
		}
		else
		{
			*m_TimerRef = m_Timer.ElapsedMillis();
		}
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