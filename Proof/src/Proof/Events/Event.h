#pragma once
#include "Proof/Core/Core.h"
#include "Platform/CurrentWindow.h"

namespace Proof {
	 enum class EventType:bool {
		WindowClose=false, WindowResize = false, WindowFocus = false, WindowLostFocus = false, WindowMoved = false,
		AppTick = false, AppUpdate = false, AppRender = false
	};
    class Proof_API Event {
    public:
		static bool OnWindowResize(); // Still in work not working properly yet
	private:
		static int* Window1;
		static int* Window2;
    };
}