#include "Proofprch.h"
#include "Input.h"
#include "Proof/Events/MouseEvent.h"
#include "Proof/Input/Mouse.h"
#include <GLFW/glfw3.h>
namespace Proof
{
	bool Input::IsKeyClicked(KeyBoardKey Key) {
		if(Application::Get()->GetWindow()->IsInputEventEnabled()==false)return false;
		return std::find(Application::Get()->GetWindow()->KeyboardClicked.begin(), Application::Get()->GetWindow()->KeyboardClicked.end(), Key) 
			!= 
			Application::Get()->GetWindow()->KeyboardClicked.end();
	}

	bool Input::IsKeyReleased(KeyBoardKey Key) {
		if (Application::Get()->GetWindow()->IsInputEventEnabled() == false)return false;
		return std::find(Application::Get()->GetWindow()->KeyboardReleased.begin(), Application::Get()->GetWindow()->KeyboardReleased.end(), Key)
			!=
			Application::Get()->GetWindow()->KeyboardReleased.end();
	}

	bool Input::IsKeyHold(KeyBoardKey Key) {
		if (Application::Get()->GetWindow()->IsInputEventEnabled() == false)return false;
		return std::find(Application::Get()->GetWindow()->KeyboardKeyHold.begin(), Application::Get()->GetWindow()->KeyboardKeyHold.end(), Key)
			!=
			Application::Get()->GetWindow()->KeyboardKeyHold.end();
	}
	
	bool Input::IsKeyPressed(KeyBoardKey Key) {
		if (glfwGetKey((GLFWwindow*)Application::Get()->GetWindow()->GetWindow(), (int)Key)) {
			return  true;
		}
		return false;
	}
	
	bool Input::IsKeyDoubleClick(KeyBoardKey Key) {
		if (Application::Get()->GetWindow()->IsInputEventEnabled() == false)return false;
		return std::find(Application::Get()->GetWindow()->KeyboardKeyDoubleClicked.begin(), Application::Get()->GetWindow()->KeyboardKeyDoubleClicked.end(), Key)
			!=
			Application::Get()->GetWindow()->KeyboardKeyDoubleClicked.end();
	}

	bool Input::IsMouseButtonClicked(MouseButton Button) {
		if (Application::Get()->GetWindow()->IsInputEventEnabled() == false)return false;
		return std::find(Application::Get()->GetWindow()->MouseButtonClicked.begin(), Application::Get()->GetWindow()->MouseButtonClicked.end(), Button)
			!=
			Application::Get()->GetWindow()->MouseButtonClicked.end();
	}
	bool Input::IsMouseButtonReleased(MouseButton Button) {
		if (Application::Get()->GetWindow()->IsInputEventEnabled() == false)return false;
		return std::find(Application::Get()->GetWindow()->MouseButtonReleased.begin(), Application::Get()->GetWindow()->MouseButtonReleased.end(), Button)
			!=
			Application::Get()->GetWindow()->MouseButtonReleased.end();
	}

	bool Input::IsMouseButtonPressed(MouseButton Button) {
		if (Application::Get()->GetWindow()->IsInputEventEnabled() == false)return false;
		if (glfwGetMouseButton((GLFWwindow*)Application::Get()->GetWindow()->GetWindow(), (int)Button)) {
			return true;
		}
		return false;
	}
	bool Input::IsMouseButtonDoubleClicked(MouseButton Button) {
		if (Application::Get()->GetWindow()->IsInputEventEnabled() == false)return false;
		return std::find(Application::Get()->GetWindow()->MouseButtonDoubleClicked.begin(), Application::Get()->GetWindow()->MouseButtonDoubleClicked.end(), Button)
			!=
			Application::Get()->GetWindow()->MouseButtonDoubleClicked.end();
	}


	bool Input::IsMouseScrollUp() {
		if (Application::Get()->GetWindow()->IsInputEventEnabled() == false)return false;
		return Mouse::IsScrollUp();
	}

	bool Input::IsMouseScrollDown() {
		if (Application::Get()->GetWindow()->IsInputEventEnabled() == false)return false;
		return Mouse::IsScrollDown();
	}

	bool Input::IsMouseScrolled() {
		if (Application::Get()->GetWindow()->IsInputEventEnabled() == false)return false;
		return Mouse::IsScroll();
	}
	float Input::GetScrollWheelX() {
		if (Application::Get()->GetWindow()->IsInputEventEnabled() == false)return false;
		return Mouse::GetScrollX();
	}
	float Input::GetScrollWheelY() {
		if (Application::Get()->GetWindow()->IsInputEventEnabled() == false)return false;
		return Mouse::GetScrollY();
	}
	float Input::GetMousePosX() {
		if (Application::Get()->GetWindow()->IsInputEventEnabled() == false)return false;
		return Mouse::GetPosX();
	}
	float Input::GetMousePosY() {
		if (Application::Get()->GetWindow()->IsInputEventEnabled() == false)return false;
		return Mouse::GetPosY();
	}
	bool Input::IsControllerClicked(int ID, ControllerButton button){
		for (Controller& controller : Application::Get()->GetWindow()->m_Controllers) {
			if (controller.ID != ID)
				continue;
			return controller.Buttons[(int)button] == (int)InputEvent::KeyClicked;
		}
		return false;
	}
	bool Input::IsAnyControllerClicked(ControllerButton button){
		for (Controller& controller : Application::Get()->GetWindow()->m_Controllers) {
			if (controller.Buttons[(int)button] == (int)InputEvent::KeyClicked)
				return true;
		}
		return false;
	}
	bool Input::IsControllerDoubleClick(int ID, ControllerButton button){
		for (Controller& controller : Application::Get()->GetWindow()->m_Controllers) {
			if (controller.ID != ID)
				continue;
			return controller.Buttons[(int)button] == (int)InputEvent::KeyDouble;
		}
		return false;
	}
	bool Input::IsAnyControllerDoubleClick(ControllerButton button) {
		for (Controller& controller : Application::Get()->GetWindow()->m_Controllers) {
			if (controller.Buttons[(int)button] == (int)InputEvent::KeyDouble)
				return true;
		}
		return false;
	}
	bool Input::IsControllerReleased(int ID, ControllerButton button){
		for (Controller& controller : Application::Get()->GetWindow()->m_Controllers) {
			if (controller.ID != ID)
				continue;
			return controller.Buttons[(int)button] == (int)InputEvent::KeyReleased;
		}
		return false;
	}
	bool Input::IsAnyControllerReleased(ControllerButton button){
		for (Controller& controller : Application::Get()->GetWindow()->m_Controllers) {
			if (controller.Buttons[(int)button] == (int)InputEvent::KeyReleased)
				return true;
		}
		return false;
	}
	bool Input::IsControllerPressed(int ID, ControllerButton button){
		for (Controller& controller : Application::Get()->GetWindow()->m_Controllers) {
			if (controller.ID != ID)
				continue;
			return controller.Buttons[(int)button] == (int)InputEvent::KeyPressed;
		}
		return false;
	}
	bool Input::IsAnyControllerPressed(ControllerButton button){
		for (Controller& controller : Application::Get()->GetWindow()->m_Controllers) {
			if (controller.Buttons[(int)button] == (int)InputEvent::KeyPressed)
				return true;
		}
		return false;
	}
	std::pair<float, float> Input::GetControllerLeftJoystickAxis(int ID) {
		for (Controller& controller : Application::Get()->GetWindow()->m_Controllers) {
			if (controller.ID != ID)
				continue;
			return { controller.LeftJoystickX,controller.LeftJoystickY };
		}
		return { 0,0 };
	}
	std::pair<float, float> Input::GetControllerLeftJoystickAxisDistance(int ID) {
		for (Controller& controller : Application::Get()->GetWindow()->m_Controllers) {
			if (controller.ID != ID)
				continue;
			return { controller.DistanceLeftJoystickX,controller.DistanceLeftJoystickY };
		}
		return { 0,0 };
	}
	std::pair<float, float> Input::GetControllerRightJoystickAxis(int ID) {
		for (Controller& controller : Application::Get()->GetWindow()->m_Controllers) {
			if (controller.ID != ID)
				continue;
			return { controller.RightJoystickX,controller.RightJoystickY };
		}
		return { 0,0 };
	}
	std::pair<float, float> Input::GetControllerRightJoystickAxisDistance(int ID) {
		for (Controller& controller : Application::Get()->GetWindow()->m_Controllers) {
			if (controller.ID != ID)
				continue;
			return { controller.DistanceRightJoystickX,controller.DistanceRightJoystickY };
		}
		return { 0,0 };
	}
	float Input::GetControllerLeftTriggerAxis(int ID) {
		for (Controller& controller : Application::Get()->GetWindow()->m_Controllers) {
			if (controller.ID != ID)
				continue;
			return controller.LeftTriggerAxis;
		}
		return 0;
	}
	float Input::GetControllerLeftTriggerAxisDistance(int ID) {
		for (Controller& controller : Application::Get()->GetWindow()->m_Controllers) {
			if (controller.ID != ID)
				continue;
			return controller.DistanceLeftTriggerAxis;
		}
		return 0;
	}

	float Input::GetControllerRightTriggerAxis(int ID) {
		for (Controller& controller : Application::Get()->GetWindow()->m_Controllers) {
			if (controller.ID != ID)
				continue;
			return controller.RightTriggerAxis;
		}
		return 0;
	}
	float Input::GetControllerRightTriggerAxisDistance(int ID) {
		for (Controller& controller : Application::Get()->GetWindow()->m_Controllers) {
			if (controller.ID != ID)
				continue;
			return controller.DistanceRightTriggerAxis;
		}
		return 0;
	}
	
}