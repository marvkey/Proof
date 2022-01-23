#pragma once
#include <glm/glm.hpp>
#include "KeyCodes.h"
#include "Proof/Events/Event.h"
#include <unordered_map>
#include <functional>
namespace Proof {
	struct InputType {
		InputType(InputDevice inputDevice, int key) {
			Device = inputDevice;
			Key = key;
		}
		InputDevice Device = InputDevice::None;
		int Key = 0; /// value is an enum
	};
	struct Action {
		std::function<void()> FunctionCallback;
		std::vector<InputType> m_Inputs;
	};
	class InputManager {
	public:
		InputManager() = delete;
		static void BindAction(const std::string& name, InputEvent inputEvent, const std::function<void()>& func) {
			//auto it = m_ActionMapping.find(name);
			//if (it == m_ActionMapping.end())return;
			//it->second.FunctionCallback = func;
		}
		static bool ActionAddKey(const std::string& name, InputType inputype) {
			//auto it = m_ActionMapping.find(name);
			//if (it == m_ActionMapping.end())return false;
			//it->second.m_Inputs.emplace_back(inputype);
			return true;
		}
		static bool AddAction(const std::string& name) {
			//if (m_ActionMapping.find(name) == m_ActionMapping.end()) {
			//	PF_ENGINE_INFO("already has action ");
			//	return false;
			//}
			//m_ActionMapping.insert({ name,Action() });
			return true;
		}
	private:
		static void OnKeyClicked(KeyClickedEvent& e);
		static std::unordered_map<std::string, Action> m_ActionMapping;
		static void OnEvent(Event& e);
		friend class Application;
		friend class InputPanel;
	};	
}