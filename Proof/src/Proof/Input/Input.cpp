#include "Proofprch.h"
#include "Input.h"
#include "Platform/CurrentWindow.h"
#include "Proof/Events/MouseEvent.h"
#include "Proof/Input/Mouse.h"
namespace Proof {
	bool Input::IsKeyClicked(KeyBoardKey Key) {
		for (int i = 0; i < WindowsWindow::KeyboardClicked.size(); i++) {
			if (Key == WindowsWindow::KeyboardClicked[i]) {
				auto it = std::find(WindowsWindow::KeyboardClicked.begin(), WindowsWindow::KeyboardClicked.begin() + i, WindowsWindow::KeyboardClicked[i]);
				WindowsWindow::KeyboardClicked.erase(it);
				return true;
			}
		}
		return false;
	}

	bool Input::IsKeyReleased(KeyBoardKey Key) {
		for (int i = 0; i < WindowsWindow::KeyboardReleased.size(); i++) {
			if (Key == WindowsWindow::KeyboardReleased[i]) {
				auto it = std::find(WindowsWindow::KeyboardReleased.begin(), WindowsWindow::KeyboardReleased.begin() + i, WindowsWindow::KeyboardReleased[i]);
				WindowsWindow::KeyboardReleased.erase(it);
				return true;
			}
		}
		return false;
	}

	bool Input::IsKeyHold(KeyBoardKey Key){
		for (int i = 0; i < WindowsWindow::KeyboardKeyRepeat.size(); i++) {
			if (Key == WindowsWindow::KeyboardKeyRepeat[i]) {
				auto it = std::find(WindowsWindow::KeyboardKeyRepeat.begin(), WindowsWindow::KeyboardKeyRepeat.begin() + i, WindowsWindow::KeyboardKeyRepeat[i]);
				WindowsWindow::KeyboardKeyRepeat.erase(it);
				return true;
			}
		}
		return false;
	}

	bool Input::IsKeyPressed(KeyBoardKey Key) {
		if (ImGui::IsAnyWindowHovered()) return false;
		if (ImGui::IsAnyItemHovered())return false;
		if (glfwGetKey(CurrentWindow::GetWindow(), (int)Key))return  true;
		return false;
	}

	bool Input::IsKeyDoubleClick(KeyBoardKey Key) {
		for (int i = 0; i < WindowsWindow::KeyboardKeyDoubleClicked.size(); i++) {
			if (Key == WindowsWindow::KeyboardKeyDoubleClicked[i]) {
				auto it = std::find(WindowsWindow::KeyboardKeyDoubleClicked.begin(), WindowsWindow::KeyboardKeyDoubleClicked.begin() + i, WindowsWindow::KeyboardKeyDoubleClicked[i]);
				WindowsWindow::KeyboardKeyDoubleClicked.erase(it);
				return true;
			}
		}
		return false;
	}

	bool Input::IsMouseButtonClicked(MouseButton Button) {
		for (int i = 0; i < WindowsWindow::MouseButtonClicked.size(); i++) {
			if (Button == WindowsWindow::MouseButtonClicked[i]) {
				auto it = std::find(WindowsWindow::MouseButtonClicked.begin(), WindowsWindow::MouseButtonClicked.begin() + i, WindowsWindow::MouseButtonClicked[i]);
				WindowsWindow::MouseButtonClicked.erase(it);
				return true;
			}
		}
		return false;
	}
	bool Input::IsMouseButtonReleased(MouseButton Button) {
		for (int i = 0; i < WindowsWindow::MouseButtonReleased.size(); i++) {
			if (Button == WindowsWindow::MouseButtonReleased[i]) {
				auto it = std::find(WindowsWindow::MouseButtonReleased.begin(), WindowsWindow::MouseButtonReleased.begin() + i, WindowsWindow::MouseButtonReleased[i]);
				WindowsWindow::MouseButtonReleased.erase(it);
				return true;
			}
		}
		return false;
	}

	bool Input::IsMouseButtonDoubleClicked(MouseButton Button){
		for (int i = 0; i < WindowsWindow::MouseButtonDoubleClicked.size(); i++) {
			if (Button == WindowsWindow::MouseButtonDoubleClicked[i]) {
				auto it = std::find(WindowsWindow::MouseButtonDoubleClicked.begin(), WindowsWindow::MouseButtonDoubleClicked.begin() + i, WindowsWindow::MouseButtonDoubleClicked[i]);
				WindowsWindow::MouseButtonDoubleClicked.erase(it);
				return true;
			}
		}
		return false;
	}
	
	bool Input::IsMouseButtonPressed(MouseButton Button){
		if (ImGui::IsAnyWindowHovered()) return false;
		if (ImGui::IsAnyItemHovered())return false;
		if (glfwGetMouseButton(CurrentWindow::GetWindow(), (int)Button))return true;
		return false;
	}
	float Input::GetScrollWheelX(){
		MouseScrollEvent ScrollWheel;
		return ScrollWheel.GetPosX();
	}
	float Input::GetScrollWheelY(){
		MouseScrollEvent ScrollWheel;
		return ScrollWheel.GetPosY();
	}
	float Input::GetMousePosX(){
		return Mouse::GetMouseX();
	}
	float Input::GetMouswPosY(){
		return Mouse::GetMouseY();
	}
}