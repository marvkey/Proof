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
	bool Input::IsControllerState(int ID, ControllerButton button, InputEvent state){
		auto& controllerHodler = Application::Get()->GetWindow()->m_Controllers;
		if (!controllerHodler.contains(ID))
			return false ;
	
		return controllerHodler[ID].Buttons[button] == state;
	}
	bool Input::IsAnyControllerState(ControllerButton button, InputEvent state)
	{
		for (auto& [ID, controller] : Application::Get()->GetWindow()->m_Controllers)
		{
			if (controller.Buttons[button] == state)
				return true;
		}
		return false;
	}
	std::pair<float, float> Input::GetControllerLeftJoystickAxis(int ID) {
		auto& controllerHodler = Application::Get()->GetWindow()->m_Controllers;
		if (!controllerHodler.contains(ID))
			return { 0,0 };
		return { controllerHodler[ID].LeftJoystick.Axis.X,controllerHodler[ID].LeftJoystick.Axis.Y };
	}
	std::pair<float, float> Input::GetControllerLeftJoystickAxisDistance(int ID) {
		auto& controllerHodler = Application::Get()->GetWindow()->m_Controllers;
		if (!controllerHodler.contains(ID))
			return { 0,0 };
		return { controllerHodler[ID].LeftJoystick.Distance.X,controllerHodler[ID].LeftJoystick.Distance.Y };
	}
	std::pair<float, float> Input::GetControllerRightJoystickAxis(int ID) {
		auto& controllerHodler = Application::Get()->GetWindow()->m_Controllers;
		if (!controllerHodler.contains(ID))
			return { 0,0 };
		return { controllerHodler[ID].RightJoystick.Axis.X,controllerHodler[ID].RightJoystick.Axis.Y };
	}
	std::pair<float, float> Input::GetControllerRightJoystickAxisDistance(int ID) {
		auto& controllerHodler = Application::Get()->GetWindow()->m_Controllers;
		if (!controllerHodler.contains(ID))
			return { 0,0 };
		return { controllerHodler[ID].RightJoystick.Distance.X,controllerHodler[ID].RightJoystick.Distance.Y };
	}
	float Input::GetControllerLeftTriggerAxis(int ID) {
		auto& controllerHodler = Application::Get()->GetWindow()->m_Controllers;
		if (!controllerHodler.contains(ID))
			return 0;
		return controllerHodler[ID].LeftTrigger.Axis;
	}
	float Input::GetControllerLeftTriggerAxisDistance(int ID) {
		auto& controllerHodler = Application::Get()->GetWindow()->m_Controllers;
		if (!controllerHodler.contains(ID))
			return 0;
		return controllerHodler[ID].LeftTrigger.DistanceAxis;
	}

	float Input::GetControllerRightTriggerAxis(int ID) {
		auto& controllerHodler = Application::Get()->GetWindow()->m_Controllers;
		if (!controllerHodler.contains(ID))
			return 0;
		return controllerHodler[ID].RightTrigger.Axis;
	}
	float Input::GetControllerRightTriggerAxisDistance(int ID) {
		auto& controllerHodler =Application::Get()->GetWindow()->m_Controllers;
		if (!controllerHodler.contains(ID))
			return 0;
		return controllerHodler[ID].RightTrigger.DistanceAxis;
	}
	
}