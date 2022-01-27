#include "Proofprch.h"
#include "InputManager.h"
#include "Proof/Events/Event.h"
namespace Proof {
	std::unordered_map<std::string, Action> InputManager::S_ActionMapping;
	std::unordered_map<std::string, Motion> InputManager::s_MotionMapping;
	void InputManager::OnEvent(Event& e){
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch< KeyClickedEvent>(InputManager::OnKeyClicked);
		dispatcher.Dispatch<KeyHoldEvent>(InputManager::OnKeyHold);
		dispatcher.Dispatch<KeyDoubleClickEvent>(InputManager::OnKeyDoubleClicked);
		dispatcher.Dispatch<KeyPressedEvent>(InputManager::OnKeyPressed);
	}
	void InputManager::BindAction(const std::string& name, InputEvent inputEvent, const std::function<void()>& func) {
		auto it = S_ActionMapping.find(name);
		if (it == S_ActionMapping.end())return;
		int location = (int)inputEvent;
		if (location == -1) {
			PF_INFO("Cannot pass none to bind an input event");
			return;
		}
		it->second.AvalaibleInputEvents[location]++;
		it->second.FunctionCallback = func;
	}

	bool InputManager::ActionAddKey(const std::string& name, InputType inputype){
		auto it = S_ActionMapping.find(name);
		if (it == S_ActionMapping.end())return false;
		it->second.m_Inputs.emplace_back(inputype);
		return true;
	}

	bool InputManager::AddAction(const std::string& name){
		if (S_ActionMapping.find(name) != S_ActionMapping.end()) {
			PF_ENGINE_INFO("already has action ");
			return false;
		}
		S_ActionMapping.insert({ name,Action() });
		return true;
	}

	void InputManager::BindMotion(const std::string& name, const std::function<void(float MotionValue)>& func) {
		auto it = s_MotionMapping.find(name);
		if (it == s_MotionMapping.end())return;
		it->second.FunctionCallback = func;
	}

	bool InputManager::MotionAddKey(const std::string& name, MotionInputType inputType){
		auto it = s_MotionMapping.find(name);
		if (it == s_MotionMapping.end())return false;
		it->second.Inputs.emplace_back(inputType);
		return true;
	}

	bool InputManager::AddMotion(const std::string& name){
		if (s_MotionMapping.find(name) != s_MotionMapping.end()) {
			PF_ENGINE_INFO("already has Motion");
			return false;
		}
		s_MotionMapping.insert({ name,Motion() });
		return true;
	}

	void InputManager::OnKeyHold(KeyHoldEvent& e){
		for (const auto& [name, action] : S_ActionMapping) {
			// checking if key hold is an available format
			if (action.AvalaibleInputEvents[(int)InputEvent::KeyHold] == 0) 
				continue;
			for (auto& inputs : action.m_Inputs) {
				if (inputs.Key == (int)e.GetKey()) {
					action.FunctionCallback();
				}
			}
		}
	}
	void InputManager::OnKeyDoubleClicked(KeyDoubleClickEvent& e){
		for (const auto& [name, action] : S_ActionMapping) {
			// checking if KeyDouble is an available format
			if (action.AvalaibleInputEvents[(int)InputEvent::KeyDouble] == 0)
				continue;
			for (auto& inputs : action.m_Inputs) {
				if (inputs.Key == (int)e.GetKey()) {
					action.FunctionCallback();
				}
			}
		}
	}
	void InputManager::OnKeyReleased(KeyReleasedEvent& e){
		for (const auto& [name, action] : S_ActionMapping) {
			// checking if key Releaed is an available format
			if (action.AvalaibleInputEvents[(int)InputEvent::KeyReleased] == 0)
				continue;
			for (auto& inputs : action.m_Inputs) {
				if (inputs.Key == (int)e.GetKey()) {
					action.FunctionCallback();
				}
			}
		}
	}
	
	void InputManager::OnKeyClicked(KeyClickedEvent& e){
		for (const auto& [name, action] : S_ActionMapping) {
			// checking if key CLicked is an available format
			if (action.AvalaibleInputEvents[(int)InputEvent::KeyClicked] == 0)
				continue;
			for (auto& inputs : action.m_Inputs) {
				if (inputs.Key == (int)e.GetKey()) {
					action.FunctionCallback();
				}
			}
		}
	}
	void InputManager::OnKeyPressed(KeyPressedEvent& e){
		for (const auto& [name, motion] : s_MotionMapping) {
			for (auto& inputs : motion.Inputs) {
				if (inputs.Key == (int)e.GetKey()) {
					motion.FunctionCallback(inputs.MotionValue);
				}
			}
		}
	}
}
