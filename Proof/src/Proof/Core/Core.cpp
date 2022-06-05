#include "Proofprch.h"
#include "Core.h"
#include "Log.h"
std::vector<Proof::FrameTimersControll::FrameTimeManage> Proof::FrameTimersControll::s_FrameTimers = {};

Proof::RangeTimer::~RangeTimer() {
	float time = m_Timer.TimePassedMillis();
	PF_ENGINE_ERROR("{TIMER} {} %fms", m_Name.c_str(), time);
}
Proof::RangeTimerMacro::~RangeTimerMacro() {
	float time = m_Timer.TimePassedMillis();
	FrameTimersControll::Add(m_Name, time, m_MaxTime,m_Type);
}