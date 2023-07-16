#pragma once
#include "Proof/Core/Log.h"
#include "FrameTime.h"

#include "Imgui/imgui.h"
#include <iostream>
namespace Proof {
	class Layer : public RefCounted {
	public:
		Layer(const std::string& Name = "Debug Layer"):
			m_DebugName(Name)
		{

		}
		virtual ~Layer() { PF_ENGINE_TRACE("Layer {} Deleted", m_DebugName); };
		virtual void OnUpdate(FrameTime DeltaTime) {}
		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnImGuiDraw(FrameTime DeltaTime) {}
		virtual void OnEvent(Event&e) {}
		virtual const std::string& GetName() const { return m_DebugName; }
	private:
		std::string m_DebugName;
	};
}