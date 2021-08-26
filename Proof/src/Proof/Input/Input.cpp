#include "Proofprch.h"
#include "Input.h"
#include "Proof/Events/MouseEvent.h"
#include "Proof/Input/Mouse.h"
#include "Platform/Window/WindowsWindow.h"
#include "ImGui/imgui.h"
namespace Proof
{
	bool Input::ViewPoartHovered = true;
	bool Input::IsKeyClicked(KeyBoardKey Key) {
		//if(CurrentWindow::GetWindow() ==nullptr)return false;
		if (ViewPoartHovered == false)return false; // This Temprory will not be released like this
		for (int i = 0; i < Proof::WindowsWindow::KeyboardClicked.size(); i++) {
			if (Key == Proof::WindowsWindow::KeyboardClicked[i]) {
				return true;
			}
		}
		return false;
	}

	bool Input::IsKeyReleased(KeyBoardKey Key) {
		if (CurrentWindow::GetWindow() == nullptr)return false;
		if (ViewPoartHovered == false)return false; // This Temprory will not be released like this
		for (int i = 0; i < Proof::WindowsWindow::KeyboardReleased.size(); i++) {
			if (Key == Proof::WindowsWindow::KeyboardReleased[i]) {
				return true;
			}
		}
		return false;
	}

	bool Input::IsKeyHold(KeyBoardKey Key) {
		if (CurrentWindow::GetWindow() == nullptr)return false;
		if (ViewPoartHovered == false)return false; // This Temprory will not be released like this
		for (int i = 0; i < Proof::WindowsWindow::KeyboardKeyRepeat.size(); i++) {
			if (Key == Proof::WindowsWindow::KeyboardKeyRepeat[i]) {
				return true;
			}
		}
		return false;
	}
	
	bool Input::IsKeyPressed(KeyBoardKey Key) {
		//if (CurrentWindow::GetWindow() == nullptr){return false;}
		if (ViewPoartHovered == false)return false; // This Temprory will not be released like this
		if (glfwGetKey(Proof::CurrentWindow::GetWindow(),(int)Key)) {
			return  true;
		}
		return false;
	}
	
	bool Input::IsKeyDoubleClick(KeyBoardKey Key) {
		if (CurrentWindow::GetWindow() == nullptr)return false;
		if (ViewPoartHovered == false)return false; // This Temprory will not be released like this
		for (int i = 0; i < Proof::WindowsWindow::KeyboardKeyDoubleClicked.size(); i++) {
			if (Key == Proof::WindowsWindow::KeyboardKeyDoubleClicked[i]) {
				return true;
			}
		}
		return false;
	}

	bool Input::IsMouseButtonClicked(MouseButton Button) {
		if (CurrentWindow::GetWindow() == nullptr)return false;
		if (ViewPoartHovered == false)return false; // This Temprory will not be released like this
		for (int i = 0; i < Proof::WindowsWindow::MouseButtonClicked.size(); i++) {
			if (Button == Proof::WindowsWindow::MouseButtonClicked[i]) {
				return true;
			}
		}
		return false;
	}
	bool Input::IsMouseButtonReleased(MouseButton Button) {
		if (CurrentWindow::GetWindow() == nullptr)return false;
		if (ViewPoartHovered == false)return false; // This Temprory will not be released like this
		for (int i = 0; i < Proof::WindowsWindow::MouseButtonReleased.size(); i++) {
			if (Button == Proof::WindowsWindow::MouseButtonReleased[i]) {
				return true;
			}
		}
		return false;
	}

	bool Input::IsMouseButtonDoubleClicked(MouseButton Button) {
		if (CurrentWindow::GetWindow() == nullptr)return false;
		if (ViewPoartHovered == false)return false; // This Temprory will not be released like this
		for (int i = 0; i < Proof::WindowsWindow::MouseButtonDoubleClicked.size(); i++) {
			if (Button == Proof::WindowsWindow::MouseButtonDoubleClicked[i]) {
				return true;
			}
		}
		return false;
	}

	bool Input::IsMouseScrollWheelUp() {
		if (CurrentWindow::GetWindow() == nullptr)return false;
		if (ViewPoartHovered == false)return false; // This Temprory will not be released like this
		float ScrollUp = 1.0f;
		for (int i = 0; i < Proof::WindowsWindow::MouseScrollY.size(); i++) {
			if (ScrollUp == Proof::WindowsWindow::MouseScrollY[i]) {
				return true;
			}
		}
		return false;
	}

	bool Input::IsMouseScrollWheelDown() {
		if (CurrentWindow::GetWindow() == nullptr)return false;
		if (ViewPoartHovered == false)return false; // This Temprory will not be released like this
		float ScrollDown = -1.0f;
		for (int i = 0; i < Proof::WindowsWindow::MouseScrollX.size(); i++) {
			if (ScrollDown == Proof::WindowsWindow::MouseScrollX[i]) {
				return true;
			}
		}
		return false;
	}

	bool Input::IsMouseButtonPressed(MouseButton Button) {
		if (CurrentWindow::GetWindow() == nullptr)return false;
		if (ViewPoartHovered == false)return false; // This Temprory will not be released like this
		if (glfwGetMouseButton(Proof::CurrentWindow::GetWindow(),(int)Button)) {
			return true;
		}
		return false;
	}
	float Input::GetScrollWheelX() {
		if (ViewPoartHovered == false)return 0.0f; // This Temprory will not be released like this
		Proof::MouseScrollEvent ScrollWheel;
		return ScrollWheel.GetPosX();
	}
	float Input::GetScrollWheelY() {
		if (ViewPoartHovered == false)return 0.0f; // This Temprory will not be released like this
		Proof::MouseScrollEvent ScrollWheel;
		return ScrollWheel.GetPosY();
	}
	float Input::GetMousePosX() {
		if (ViewPoartHovered == false)return 0.0f; // This Temprory will not be released like this
		return Proof::Mouse::GetMouseX();
	}
	float Input::GetMousePosY() {
		if (ViewPoartHovered == false)return 0.0f; // This Temprory will not be released like this
		return Proof::Mouse::GetMouseY();
	}
}