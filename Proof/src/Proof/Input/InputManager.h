#pragma once
#include "KeyCodes.h"
#include "Proof/Events/Event.h"
#include <unordered_map>
#include <functional>
#include <string>
#include "Proof/Events/ControllerEvent.h"
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
		// Layout of the input available
		// number of input to each layout will be stored in the location of the array
		// each are set to 0 as of now
		//KeyReleased = 0,
		//KeyClicked = 1,
		//KeyHold = 2,
		//KeyPressed = 3,
		//KeyDouble = 4
		std::array<int, 5>AvalaibleInputEvents = {0,0,0,0,0};
		//KeyBoard = 0,
		//MouseButton = 1, // mouse button pressed
		//MouseMovement = 2, // moseu scroll to side or up, r wheal up or down
		//ControllerButton = 3,
		//ControllerAxis = 4,
		std::array<int, 5> AvalableDevices = { 0,0,0,0 };
	};
	struct MotionInputType {
	
		MotionInputType(InputDevice inputDevice, int key,float motionValue=1) {
			Device = inputDevice;
			Key = key;
			MotionValue = motionValue;
		}
		InputDevice Device = InputDevice::None;
		int Key = 0; /// value is an enum
		float MotionValue = 1.0;
	};
	struct Motion {
		std::function<void(float motionValue)> FunctionCallback;// mihgt store a vector of this to bind to
		std::vector<MotionInputType> Inputs;
		//KeyBoard = 0,
		//MouseButton = 1, // mouse button pressed
		//MouseMovement = 2, // moseu scroll to side or up, r wheal up or down
		//ControllerButton = 3,
		//ControllerAxis = 4,
		std::array<int, 5> AvalableDevices = { 0,0,0,0 };
	};
	// make this class per player ID
	//call it Input(Something)
	//have an atual input manger class that proccesses event 
	//then that class will send liek keyboard event to the id 
	// this class will be stored in a map with the id signifying the current player
	class InputManager {
	public:
		static void OnEvent(Event& e);

		InputManager() = delete;
		static void BindAction(const std::string& name, InputEvent inputEvent, const std::function<void()>& func);
		static bool ActionAddKey(const std::string& name, InputType inputype);
		static bool AddAction(const std::string& name);

		// MOTION INPUTS
		static void BindMotion(const std::string& name, const std::function<void(float MotionValue)>& func);
		static bool MotionAddKey(const std::string& name, MotionInputType inputType);
		static bool AddMotion(const std::string& name);
	private:
		static void OnKeyClicked(KeyClickedEvent& e);
		static void OnKeyPressed(KeyPressedEvent& e);
		static void OnKeyHold(KeyHoldEvent& e);
		static void OnKeyDoubleClicked(KeyDoubleClickEvent& e);
		static void OnKeyReleased(KeyReleasedEvent& e);

		// MOUSE
		static void OnMouseClicked(MouseButtonClickedEvent& e);
		static void OnMousePressed(MouseButtonPressedEvent& e);
		static void OnMouseReleased(MouseButtonReleasedEvent& e);
		static void OnMouseMoved(MouseMoveEvent& e);

		// CONTROLLER
		static void OnControllerClicked(ControllerButtonClickedEvent& e);
		static void OnControllerPressed(ControllerButtonPressedEvent& e);
		static void OnControllerReleased(ControllerButtonReleasedEvent& e);

		static void ControllerTriggerAxis(ControllerTriggerAxisEvent& e);
		static void ControllerLeftJoystickAxis(ControllerLeftJoystickAxisEvent& e);
		static void ControllerRightJoystickAxis(ControllerRightJoystickAxisEvent& e);
		static std::unordered_map<std::string, Action> S_ActionMapping;
		static std::unordered_map<std::string, Motion> s_MotionMapping;
		friend class Application;
		friend class InputPanel;
	};	
}