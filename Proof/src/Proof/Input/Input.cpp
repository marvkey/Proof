#include "Proofprch.h"
#include "Input.h"
#include "Proof/Events/MouseEvent.h"
#include "Proof/Input/Mouse.h"
#include "ImGui/imgui.h"
#include "Proof/Core/CurrentWindow.h"
#include <GLFW/glfw3.h>	
namespace Proof
{
	bool Input::IsKeyClicked(KeyBoardKey Key) {
		if(CurrentWindow::GetWindowClass().IsInputEventEnabled()==false)return false;
		return std::find(CurrentWindow::GetWindowClass().KeyboardClicked.begin(), CurrentWindow::GetWindowClass().KeyboardClicked.end(), Key) 
			!= 
			CurrentWindow::GetWindowClass().KeyboardClicked.end();
	}

	bool Input::IsKeyReleased(KeyBoardKey Key) {
		if (CurrentWindow::GetWindowClass().IsInputEventEnabled() == false)return false;
		return std::find(CurrentWindow::GetWindowClass().KeyboardReleased.begin(), CurrentWindow::GetWindowClass().KeyboardReleased.end(), Key)
			!=
			CurrentWindow::GetWindowClass().KeyboardReleased.end();
	}

	bool Input::IsKeyHold(KeyBoardKey Key) {
		if (CurrentWindow::GetWindowClass().IsInputEventEnabled() == false)return false;
		return std::find(CurrentWindow::GetWindowClass().KeyboardKeyHold.begin(), CurrentWindow::GetWindowClass().KeyboardKeyHold.end(), Key)
			!=
			CurrentWindow::GetWindowClass().KeyboardKeyHold.end();
	}
	
	bool Input::IsKeyPressed(KeyBoardKey Key) {
		if (glfwGetKey((GLFWwindow*)CurrentWindow::GetWindowAPI(), (int)Key)) {
			return  true;
		}
		return false;
	}
	
	bool Input::IsKeyDoubleClick(KeyBoardKey Key) {
		if (CurrentWindow::GetWindowClass().IsInputEventEnabled() == false)return false;
		return std::find(CurrentWindow::GetWindowClass().KeyboardKeyDoubleClicked.begin(), CurrentWindow::GetWindowClass().KeyboardKeyDoubleClicked.end(), Key)
			!=
			CurrentWindow::GetWindowClass().KeyboardKeyDoubleClicked.end();
	}

	bool Input::IsMouseButtonClicked(MouseButton Button) {
		if (CurrentWindow::GetWindowClass().IsInputEventEnabled() == false)return false;
		return std::find(CurrentWindow::GetWindowClass().MouseButtonClicked.begin(), CurrentWindow::GetWindowClass().MouseButtonClicked.end(), Button)
			!=
			CurrentWindow::GetWindowClass().MouseButtonClicked.end();
	}
	bool Input::IsMouseButtonReleased(MouseButton Button) {
		if (CurrentWindow::GetWindowClass().IsInputEventEnabled() == false)return false;
		return std::find(CurrentWindow::GetWindowClass().MouseButtonReleased.begin(), CurrentWindow::GetWindowClass().MouseButtonReleased.end(), Button)
			!=
			CurrentWindow::GetWindowClass().MouseButtonReleased.end();
	}

	bool Input::IsMouseButtonPressed(MouseButton Button) {
		if (CurrentWindow::GetWindowClass().IsInputEventEnabled() == false)return false;
		if (glfwGetMouseButton((GLFWwindow*)CurrentWindow::GetWindowAPI(), (int)Button)) {
			return true;
		}
		return false;
	}
	bool Input::IsMouseButtonDoubleClicked(MouseButton Button) {
		if (CurrentWindow::GetWindowClass().IsInputEventEnabled() == false)return false;
		return std::find(CurrentWindow::GetWindowClass().MouseButtonDoubleClicked.begin(), CurrentWindow::GetWindowClass().MouseButtonDoubleClicked.end(), Button)
			!=
			CurrentWindow::GetWindowClass().MouseButtonDoubleClicked.end();
	}


	bool Input::IsMouseScrollUp() {
		if (CurrentWindow::GetWindowClass().IsInputEventEnabled() == false)return false;
		return Mouse::IsScrollUp();
	}

	bool Input::IsMouseScrollDown() {
		if (CurrentWindow::GetWindowClass().IsInputEventEnabled() == false)return false;
		return Mouse::IsScrollDown();
	}

	bool Input::IsMouseScrolled() {
		if (CurrentWindow::GetWindowClass().IsInputEventEnabled() == false)return false;
		return Mouse::IsScroll();
	}
	float Input::GetScrollWheelX() {
		if (CurrentWindow::GetWindowClass().IsInputEventEnabled() == false)return false;
		return Mouse::GetScrollX();
	}
	float Input::GetScrollWheelY() {
		if (CurrentWindow::GetWindowClass().IsInputEventEnabled() == false)return false;
		return Mouse::GetScrollY();
	}
	float Input::GetMousePosX() {
		if (CurrentWindow::GetWindowClass().IsInputEventEnabled() == false)return false;
		return Mouse::GetPosX();
	}
	float Input::GetMousePosY() {
		if (CurrentWindow::GetWindowClass().IsInputEventEnabled() == false)return false;
		return Mouse::GetPosY();
	}
	bool Input::IsControllerClicked(int ID, ControllerButton button){
		for (Controller& controller : CurrentWindow::GetWindowClass().m_Controllers) {
			if (controller.ID != ID)
				continue;
			return controller.Buttons[(int)button] == (int)InputEvent::KeyClicked;
		}
		return false;
	}
	bool Input::IsAnyControllerClicked(ControllerButton button){
		for (Controller& controller : CurrentWindow::GetWindowClass().m_Controllers) {
			if (controller.Buttons[(int)button] == (int)InputEvent::KeyClicked)
				return true;
		}
		return false;
	}
	bool Input::IsControllerDoubleClick(int ID, ControllerButton button){
		for (Controller& controller : CurrentWindow::GetWindowClass().m_Controllers) {
			if (controller.ID != ID)
				continue;
			return controller.Buttons[(int)button] == (int)InputEvent::KeyDouble;
		}
		return false;
	}
	bool Input::IsAnyControllerDoubleClick(ControllerButton button) {
		for (Controller& controller : CurrentWindow::GetWindowClass().m_Controllers) {
			if (controller.Buttons[(int)button] == (int)InputEvent::KeyDouble)
				return true;
		}
		return false;
	}
	bool Input::IsControllerReleased(int ID, ControllerButton button){
		for (Controller& controller : CurrentWindow::GetWindowClass().m_Controllers) {
			if (controller.ID != ID)
				continue;
			return controller.Buttons[(int)button] == (int)InputEvent::KeyReleased;
		}
		return false;
	}
	bool Input::IsAnyControllerReleased(ControllerButton button){
		for (Controller& controller : CurrentWindow::GetWindowClass().m_Controllers) {
			if (controller.Buttons[(int)button] == (int)InputEvent::KeyReleased)
				return true;
		}
		return false;
	}
	bool Input::IsControllerPressed(int ID, ControllerButton button){
		for (Controller& controller : CurrentWindow::GetWindowClass().m_Controllers) {
			if (controller.ID != ID)
				continue;
			return controller.Buttons[(int)button] == (int)InputEvent::KeyPressed;
		}
		return false;
	}
	bool Input::IsAnyControllerPressed(ControllerButton button){
		for (Controller& controller : CurrentWindow::GetWindowClass().m_Controllers) {
			if (controller.Buttons[(int)button] == (int)InputEvent::KeyPressed)
				return true;
		}
		return false;
	}
}