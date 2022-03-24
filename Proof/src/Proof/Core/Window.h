#pragma once
#include "Core.h"
#include "Proof/Events/Event.h"
#include <functional>

namespace Proof {
	class Proof_API Window {
	public:
		virtual void SetEventCallback(const std::function<void(Event&)>& callback) = 0;
		virtual void WindowUpdate() = 0;
		virtual  void* GetWindow()=0;
		virtual ~Window(){};
		bool IsFrameBufferResized() { return m_FrameBufferResized; }
	protected:
		bool m_FrameBufferResized = false;
	};
}
