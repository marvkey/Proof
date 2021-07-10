#pragma once
#include "Proof/Core/Core.h"
#include<iostream>
namespace Proof{

	class Proof_API Event {
	protected:
	public:
		virtual std::string ToString()const { return "NULL"; };
		friend std::ostream& operator<<(std::ostream& os,const Event& e);
		friend class EventDispatcher;
	};
	
	class Proof_API EventDispatcher {
		EventDispatcher(Event& event)
			: m_Event(event) {}

		template<typename T,typename F>
		bool Dispatch(const F& func) {
			if (m_Event.m_EventType == T.m_EventType) {
				m_Event.Handled |= func(static_cast<T&>(m_Event));
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
		
	};
	inline std::ostream& operator<<(std::ostream& os,const Event& e) {
		return os << e.ToString();
	};

}

