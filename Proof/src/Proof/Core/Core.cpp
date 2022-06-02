#include "Proofprch.h"
#include "Core.h"
#include "Log.h"
std::unordered_map<std::string, Proof::FrameTimersControll::TimeManage> Proof::FrameTimersControll::s_FrameTimers = {};

Proof::RangeTimer::~RangeTimer() {
	float time = m_Timer.TimePassedMillis();
	PF_ENGINE_ERROR("{TIMER} {} %fms", m_Name.c_str(), time);
}