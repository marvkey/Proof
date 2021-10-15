#include "Proofprch.h"
#include "Input.h"
#include "Proof/Events/MouseEvent.h"
#include "Proof/Input/Mouse.h"
#include "Platform/Window/WindowsWindow.h"
#include "ImGui/imgui.h"
namespace Proof
{
	bool Input::IsKeyClicked(KeyBoardKey Key) {
		if(CurrentWindow::GetWindowClass().IsInputEventEnabled()==false)return false;
		for (int i = 0; i < Proof::WindowsWindow::KeyboardClicked.size(); i++) {
			if (Key == Proof::WindowsWindow::KeyboardClicked[i]) {
				return true;
			}
		}
		return false;
	}

	bool Input::IsKeyReleased(KeyBoardKey Key) {
		if (CurrentWindow::GetWindowClass().IsInputEventEnabled() == false)return false;
		for (int i = 0; i < Proof::WindowsWindow::KeyboardReleased.size(); i++) {
			if (Key == Proof::WindowsWindow::KeyboardReleased[i]) {
				return true;
			}
		}
		return false;
	}

	bool Input::IsKeyHold(KeyBoardKey Key) {
		if (CurrentWindow::GetWindowClass().IsInputEventEnabled() == false)return false;
		for (int i = 0; i < Proof::WindowsWindow::KeyboardKeyRepeat.size(); i++) {
			if (Key == Proof::WindowsWindow::KeyboardKeyRepeat[i]) {
				return true;
			}
		}
		return false;
	}
	
	bool Input::IsKeyPressed(KeyBoardKey Key) {
		if (CurrentWindow::GetWindowClass().IsInputEventEnabled() == false)return false;
		if (glfwGetKey(Proof::CurrentWindow::GetWindow(),(int)Key)) {
			return  true;
		}
		return false;
	}
	
	bool Input::IsKeyDoubleClick(KeyBoardKey Key) {
		if (CurrentWindow::GetWindowClass().IsInputEventEnabled() == false)return false;
		for (int i = 0; i < Proof::WindowsWindow::KeyboardKeyDoubleClicked.size(); i++) {
			if (Key == Proof::WindowsWindow::KeyboardKeyDoubleClicked[i]) {
				return true;
			}
		}
		return false;
	}

	bool Input::IsMouseButtonClicked(MouseButton Button) {
		if (CurrentWindow::GetWindowClass().IsInputEventEnabled() == false)return false;
		for (int i = 0; i < Proof::WindowsWindow::MouseButtonClicked.size(); i++) {
			if (Button == Proof::WindowsWindow::MouseButtonClicked[i]) {
				return true;
			}
		}
		return false;
	}
	bool Input::IsMouseButtonReleased(MouseButton Button) {
		if (CurrentWindow::GetWindowClass().IsInputEventEnabled() == false)return false;
		for (int i = 0; i < Proof::WindowsWindow::MouseButtonReleased.size(); i++) {
			if (Button == Proof::WindowsWindow::MouseButtonReleased[i]) {
				return true;
			}
		}
		return false;
	}

	bool Input::IsMouseButtonPressed(MouseButton Button) {
		if (CurrentWindow::GetWindowClass().IsInputEventEnabled() == false)return false;
		if (glfwGetMouseButton(Proof::CurrentWindow::GetWindow(),(int)Button)) {
			return true;
		}
		return false;
	}
	bool Input::IsMouseButtonDoubleClicked(MouseButton Button) {
		if (CurrentWindow::GetWindowClass().IsInputEventEnabled() == false)return false;
		for (int i = 0; i < Proof::WindowsWindow::MouseButtonDoubleClicked.size(); i++) {
			if (Button == Proof::WindowsWindow::MouseButtonDoubleClicked[i]) {
				return true;
			}
		}
		return false;
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
}