#include "Proofprch.h"
#include "InputManager.h"
#include "Proof/Events/Event.h"
namespace Proof {
	std::unordered_map<std::string, Action> InputManager::S_ActionMapping = {};
	std::unordered_map<std::string, Motion> InputManager::s_MotionMapping = {};
	void InputManager::OnEvent(Event& e){
		EventDispatcher dispatcher(e);
		// KEYBOARD
		{
			dispatcher.Dispatch< KeyClickedEvent>(InputManager::OnKeyClicked);
			dispatcher.Dispatch<KeyHoldEvent>(InputManager::OnKeyHold);
			dispatcher.Dispatch<KeyDoubleClickEvent>(InputManager::OnKeyDoubleClicked);
			dispatcher.Dispatch<KeyPressedEvent>(InputManager::OnKeyPressed);
		}
		//MOUSE
		{
			dispatcher.Dispatch<MouseMoveEvent>(InputManager::OnMouseMoved);
			dispatcher.Dispatch<MouseButtonClickedEvent>(InputManager::OnMouseClicked);
			dispatcher.Dispatch<MouseButtonReleasedEvent>(InputManager::OnMouseReleased);
		}
		// CONTROLLER
		{
			dispatcher.Dispatch<ControllerButtonClickedEvent>(InputManager::OnControllerClicked);
			dispatcher.Dispatch<ControllerButtonPressedEvent>(InputManager::OnControllerPressed);
			dispatcher.Dispatch<ControllerButtonReleasedEvent>(InputManager::OnControllerReleased);
			
			// AXIS
			{
				dispatcher.Dispatch<ControllerLeftJoystickAxisEvent>(InputManager::ControllerLeftJoystickAxis);
				dispatcher.Dispatch<ControllerRightJoystickAxisEvent>(InputManager::ControllerRightJoystickAxis);

				dispatcher.Dispatch<ControllerTriggerAxisEvent>(InputManager::ControllerTriggerAxis);
			}
		}
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
		it->second.AvalableDevices[(int)inputype.Device] ++; // increasing the amount of input for certain device
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
		it->second.AvalableDevices[(int)inputType.Device] ++;  // increasing the amount of input for certain device
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
			if(action.AvalableDevices[(int)InputDevice::KeyBoard] == 0)
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
			if(action.AvalableDevices[(int)InputDevice::KeyBoard] == 0)
				continue;
			for (auto& inputs : action.m_Inputs) {
				if (inputs.Key == (int)e.GetKey()) {
					action.FunctionCallback();
				}
			}
		}
	}
	
	void InputManager::OnKeyClicked(KeyClickedEvent& e){
;		for (const auto& [name, action] : S_ActionMapping) {
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
		// Motion movement
		{
			for (const auto& [name, motion] : s_MotionMapping) {
				if (motion.AvalableDevices[(int)InputDevice::KeyBoard] == 0)
					continue;
				for (auto& inputs : motion.Inputs) {
					if (inputs.Key == (int)e.GetKey()) {
						motion.FunctionCallback(inputs.MotionValue);
					}
				}
			}
		}

		// Action 
		{
			for (const auto& [name, action] : S_ActionMapping) {
			// checking if key Pressed is an available format
				if (action.AvalaibleInputEvents[(int)InputEvent::KeyPressed] == 0)
					continue;
				if(action.AvalableDevices[(int)InputDevice::KeyBoard] == 0)
					continue;
				for (auto& inputs : action.m_Inputs) {
					if (inputs.Key == (int)e.GetKey()) {
						action.FunctionCallback();
					}
				}
			}
		}
	}

	void InputManager::OnMouseClicked(MouseButtonClickedEvent& e){
		for (const auto& [name, action] : S_ActionMapping) {
		// checking if key Pressed is an available format
			if (action.AvalaibleInputEvents[(int)InputEvent::KeyClicked] == 0)
				continue;
			if(action.AvalableDevices[(int)InputDevice::MouseButton] == 0)
				continue;
			for (auto& inputs : action.m_Inputs) {
				if (inputs.Device != InputDevice::MouseButton) // if it is the mouse input
					continue;
				if (inputs.Key == (int)e.GetButton()) {
					action.FunctionCallback();
				}
			}
		}
	}

	void InputManager::OnMousePressed(MouseButtonPressedEvent& e){
		for (const auto& [name, action] : S_ActionMapping) {
		// checking if key Pressed is an available format
			if (action.AvalaibleInputEvents[(int)InputEvent::KeyPressed] == 0)
				continue;
			if(action.AvalableDevices[(int)InputDevice::MouseButton] == 0)
				continue;
			for (auto& inputs : action.m_Inputs) {
				if (inputs.Device != InputDevice::MouseButton) // if it is the mouse input
					continue;
				if (inputs.Key == (int)e.GetButton()) {
					action.FunctionCallback();
				}
			}
		}
		// Motion movement
		{
			for (const auto& [name, motion] : s_MotionMapping) {
				if (motion.AvalableDevices[(int)InputDevice::MouseButton] == 0)
					continue;
				for (auto& inputs : motion.Inputs) {
					if (inputs.Device != InputDevice::MouseButton) // if it is the mouse input
						continue;
					if (inputs.Key == (int)e.GetButton())
						motion.FunctionCallback(inputs.MotionValue);
				}
			}
		}
	}

	void InputManager::OnMouseReleased(MouseButtonReleasedEvent& e){
		{
			for (const auto& [name, action] : S_ActionMapping) {
			// checking if key Pressed is an available format
				if (action.AvalaibleInputEvents[(int)InputEvent::KeyReleased] == 0)
					continue;
				if(action.AvalableDevices[(int)InputDevice::MouseButton] == 0)
					continue;
				for (auto& inputs : action.m_Inputs) {
					if (inputs.Device != InputDevice::MouseButton) // if it is the mouse input
						continue;
					if (inputs.Key == (int)e.GetButton()) {
						action.FunctionCallback();
					}
				}
			}
		}
	}
	void InputManager::OnMouseMoved(MouseMoveEvent& e){
		float movedX = e.GetMovedX();
		float movedY = e.GetMovedY();
		// Motion Movement
		{
			for (const auto& [name, motion] : s_MotionMapping) {
				if (motion.AvalableDevices[(int)InputDevice::MouseMovement] == 0)
					continue;
				for (auto& inputs : motion.Inputs) {
					if (inputs.Device != InputDevice::MouseMovement)
						continue;
					if (inputs.Key == (int)MouseMovementInput::X) {
						motion.FunctionCallback(inputs.MotionValue * e.GetMovedX()); // mihgt not mutltiply
					}
					else if (inputs.Key == (int)MouseMovementInput::Y) {
						motion.FunctionCallback(inputs.MotionValue * e.GetMovedY()); // mihgt not mutltiply
					}
				}
			}
		}
	}
	void InputManager::OnControllerClicked(ControllerButtonClickedEvent& e){
		for (const auto& [name, action] : S_ActionMapping) {
			// checking if key Pressed is an available format
			if (action.AvalaibleInputEvents[(int)InputEvent::KeyClicked] == 0)
				continue;
			if (action.AvalableDevices[(int)InputDevice::ControllerButton] == 0)
				continue;
			for (auto& inputs : action.m_Inputs) {
				if (inputs.Device != InputDevice::ControllerButton) // if it is the mouse input
					continue;
				if (inputs.Key == (int)e.GetButton()) {
					action.FunctionCallback();
				}
			}
		}
	}
	void InputManager::OnControllerPressed(ControllerButtonPressedEvent& e){
		// ACTION
		{
			for (const auto& [name, action] : S_ActionMapping) {
				// checking if key Pressed is an available format
				if (action.AvalaibleInputEvents[(int)InputEvent::KeyPressed] == 0)
					continue;
				if (action.AvalableDevices[(int)InputDevice::ControllerButton] == 0)
					continue;
				for (auto& inputs : action.m_Inputs) {
					if (inputs.Device != InputDevice::ControllerButton) // if it is the mouse input
						continue;
					if (inputs.Key == (int)e.GetButton()) {
						action.FunctionCallback();
					}
				}
			}
		}
		// M0TIION
		{
			for (const auto& [name, motion] : s_MotionMapping) {
				if (motion.AvalableDevices[(int)InputDevice::ControllerButton] == 0) // controller button supported
					continue;
				for (auto& inputs : motion.Inputs) {
					if (inputs.Device != InputDevice::ControllerButton) // if it is the controller button 
						continue;
					if (inputs.Key == (int)e.GetButton())
						motion.FunctionCallback(inputs.MotionValue);
				}
			}
		}
	}
	void InputManager::OnControllerReleased(ControllerButtonReleasedEvent& e){
		for (const auto& [name, action] : S_ActionMapping) {
			// checking if key Pressed is an available format
			if (action.AvalaibleInputEvents[(int)InputEvent::KeyReleased] == 0)
				continue;
			if (action.AvalableDevices[(int)InputDevice::ControllerButton] == 0)
				continue;
			for (auto& inputs : action.m_Inputs) {
				if (inputs.Device != InputDevice::ControllerButton) // if it is the mouse input
					continue;
				if (inputs.Key == (int)e.GetButton()) {
					action.FunctionCallback();
				}
			}
		}
	}
	void InputManager::ControllerTriggerAxis(ControllerTriggerAxisEvent& e){
		for (const auto& [name, motion] : s_MotionMapping) {
			if (motion.AvalableDevices[(int)InputDevice::ControllerAxis] == 0) // controller button supported
				continue;
			for (auto& inputs : motion.Inputs) {
				if (inputs.Device != InputDevice::ControllerAxis) // if it is the controller button 
					continue;
				if (inputs.Key == (int)e.GetTriggerAxis())
					motion.FunctionCallback(inputs.MotionValue* e.GetAxis());
			}
		}
	}
	void InputManager::ControllerLeftJoystickAxis(ControllerLeftJoystickAxisEvent& e){
		for (const auto& [name, motion] : s_MotionMapping) {
			if (motion.AvalableDevices[(int)InputDevice::ControllerAxis] == 0) // controller button supported
				continue;
			for (auto& inputs : motion.Inputs) {
				if (inputs.Device != InputDevice::ControllerAxis) // if it is the controller button 
					continue;
				if (inputs.Key == (int)ControllerAxis::LeftX)
					motion.FunctionCallback(inputs.MotionValue * e.GetX());
				else if(inputs.Key == (int)ControllerAxis::LeftY)
					motion.FunctionCallback(inputs.MotionValue * e.GetY());
			}
		}
	}
	void InputManager::ControllerRightJoystickAxis(ControllerRightJoystickAxisEvent& e)
	{
		for (const auto& [name, motion] : s_MotionMapping) {
			if (motion.AvalableDevices[(int)InputDevice::ControllerAxis] == 0) // controller button supported
				continue;
			for (auto& inputs : motion.Inputs) {
				if (inputs.Device != InputDevice::ControllerAxis) // if it is the controller button 
					continue;
				if (inputs.Key == (int)ControllerAxis::RightX)
					motion.FunctionCallback(inputs.MotionValue * e.GetX());
				else if (inputs.Key == (int)ControllerAxis::RightY)
					motion.FunctionCallback(inputs.MotionValue * e.GetY());
			}
		}
	}
}
