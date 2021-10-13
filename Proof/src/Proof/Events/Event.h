#pragma once
#include "Proof/Core/Core.h"
#include<iostream>
namespace Proof{
	enum class EventType {
		None = 0,
		WindowClose,WindowResize,WindowFocus,WindowMoved,WindowMinimize,
		KeyPressed,KeyReleased,KeyClicked,KeyDoubleClick,
		MouseButtonPressed,MouseButtonReleased,MouseMoved,MouseScrolled,MouseButtonClicked,MouseButtonDoubleClick
	};

	enum EventCategory {
		None = 0,
		EventCategoryWindow = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouseMovement = BIT(3),
		EventCategoryMouseButton = BIT(4)
	};

	#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

	#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class Event {
	public:
		virtual ~Event() = default;

		bool Handled = false;

		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		bool IsInCategory(EventCategory category) {
			return GetCategoryFlags() & category;
		}
	};

	class EventDispatcher {
	public:
		EventDispatcher(Event& event)
			: m_Event(event) {}

		template<typename T,typename F>
		bool Dispatch(F func) {
			if (m_Event.GetEventType() == T::GetStaticType()) {
				m_Event.Handled =true; 
				func(static_cast<T&>(m_Event)); // everything fine we are calling the function here 
				return true;
			}
			return false;
		}
		// the reason we are not using a normal funciton pointer but using std::bind is that we would
		// have to create a static function everytime if we want to send ifnofrmation to a specific
		//function so instead we can use std::bind to help us call items
	private:
		Event& m_Event;
	};
	
	inline std::ostream& operator<<(std::ostream& os,const Event& e) {
		return os << e.ToString();
	}
	

}

