#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Core/Delegate.h"
#include "Proof/Core/UUID.h"
namespace Proof
{
	struct ScheduledTimer : RefCounted
	{
		ScheduledTimer(float timeSeconds, bool looping, bool startTimer)
		{
			TimeSeconds = TimeSeconds;
			m_StartTime = TimeSeconds;
			looping = looping;
			StartTimer = startTimer;
		}
		float TimeSeconds = 0;
		bool Looping = false;
		bool StartTimer = true;
		Delegate<void()> Actuated;
		bool IsActuated()
		{
			return m_IsActuated;
		}
		bool Reset()
		{
			TimeSeconds = m_StartTime;
		}
	private:
		float m_StartTime = 0;
		bool m_IsActuated = false;
		UUID ID = 0;
		friend class ScheduledTimersManager;
	};

	class ScheduledTimersManager : RefCounted
	{
	public:
		ScheduledTimersManager()
		{

		}

		void SetTimer(Count<ScheduledTimer> timer)
		{
			timer->ID = UUID();
			m_Timer[timer->ID] = timer;
		}

		Count<ScheduledTimer> GetTimer(UUID id)
		{
			if (m_Timer.contains(id))
				return m_Timer[id];

			return nullptr;
		}
	private:
		std::unordered_map<UUID, Count<ScheduledTimer>> m_Timer;
	};
}