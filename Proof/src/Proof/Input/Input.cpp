#include "Proofprch.h"
#include "Input.h"
#include "Platform/CurrentWindow.h"
#include "Proof/Events/MouseEvent.h"
#include "Proof/Input/Mouse.h"
std::vector<KeyBoardKey> Input::KeyPressed;
int Input::KeyPressedIndex;
std::vector<MouseButton> Input::MouseButtonPressed;
int Input::MousePressedIndex;

bool Input::IsKeyClicked(KeyBoardKey Key) {
	for (int i = 0; i < Proof::WindowsWindow::KeyboardClicked.size(); i++) {
		if (Key == Proof::WindowsWindow::KeyboardClicked[i]) {
			return true;
		}
	}
	return false;
}

bool Input::IsKeyReleased(KeyBoardKey Key) {
	for (int i = 0; i < Proof::WindowsWindow::KeyboardReleased.size(); i++) {
		if (Key == Proof::WindowsWindow::KeyboardReleased[i]) {
			return true;
		}
	}
	return false;
}

bool Input::IsKeyHold(KeyBoardKey Key) {
	for (int i = 0; i < Proof::WindowsWindow::KeyboardKeyRepeat.size(); i++) {
		if (Key == Proof::WindowsWindow::KeyboardKeyRepeat[i]) {
			return true;
		}
	}
	return false;
}

bool Input::IsKeyPressed(KeyBoardKey Key) {
	if (glfwGetKey(Proof::CurrentWindow::GetWindow(),(int)Key)) {
		return  true;
	}
	return false;
}

bool Input::IsKeyDoubleClick(KeyBoardKey Key) {
	for (int i = 0; i < Proof::WindowsWindow::KeyboardKeyDoubleClicked.size(); i++) {
		if (Key == Proof::WindowsWindow::KeyboardKeyDoubleClicked[i]) {
			return true;
		}
	}
	return false;
}

bool Input::IsMouseButtonClicked(MouseButton Button) {
	for (int i = 0; i < Proof::WindowsWindow::MouseButtonClicked.size(); i++) {
		if (Button == Proof::WindowsWindow::MouseButtonClicked[i]) {
			return true;
		}
	}
	return false;
}
bool Input::IsMouseButtonReleased(MouseButton Button) {
	for (int i = 0; i < Proof::WindowsWindow::MouseButtonReleased.size(); i++) {
		if (Button == Proof::WindowsWindow::MouseButtonReleased[i]) {
			return true;
		}
	}
	return false;
}

bool Input::IsMouseButtonDoubleClicked(MouseButton Button) {
	for (int i = 0; i < Proof::WindowsWindow::MouseButtonDoubleClicked.size(); i++) {
		if (Button == Proof::WindowsWindow::MouseButtonDoubleClicked[i]) {
			return true;
		}
	}
	return false;
}

bool Input::IsMouseScrollWheelUp() {
	float ScrollUp = 1.0f;
	for (int i = 0; i < Proof::WindowsWindow::MouseScrollY.size(); i++) {
		if (ScrollUp == Proof::WindowsWindow::MouseScrollY[i]) {
			return true;
		}
	}
	return false;
}

bool Input::IsMouseScrollWheelDown() {
	float ScrollDown = -1.0f;
	for (int i = 0; i < Proof::WindowsWindow::MouseScrollX.size(); i++) {
		if (ScrollDown == Proof::WindowsWindow::MouseScrollX[i]) {
			return true;
		}
	}
	return false;
}

bool Input::IsMouseButtonPressed(MouseButton Button) {
	if (ImGui::IsAnyWindowHovered())return false;
	if (ImGui::IsAnyItemHovered())return false;
	if (glfwGetMouseButton(Proof::CurrentWindow::GetWindow(),(int)Button)) {
		return true;
	}
	return false;
}
float Input::GetScrollWheelX() {
	Proof::MouseScrollEvent ScrollWheel;
	return ScrollWheel.GetPosX();
}
float Input::GetScrollWheelY() {
	Proof::MouseScrollEvent ScrollWheel;
	return ScrollWheel.GetPosY();
}
float Input::GetMousePosX() {
	return Proof::Mouse::GetMouseX();
}
float Input::GetMouswPosY() {
	return Proof::Mouse::GetMouseY();
}