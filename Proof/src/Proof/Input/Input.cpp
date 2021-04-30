#include "Proofprch.h"
#include "Input.h"
#include "Platform/CurrentWindow.h"
#include "Proof/Events/MouseEvent.h"
#include "Proof/Input/Mouse.h"
namespace Proof {
	std::vector<KeyBoardKey> Input::KeyPressed;
	int Input::KeyPressedIndex;
	std::vector<MouseButton> Input::MouseButtonPressed;
	int Input::MousePressedIndex;
	bool Input::IsKeyClicked(KeyBoardKey Key) {
		for (int i = 0; i < WindowsWindow::KeyboardClicked.size(); i++) {
			if (Key == WindowsWindow::KeyboardClicked[i]) {
				//auto it = std::find(WindowsWindow::KeyboardClicked.begin(), WindowsWindow::KeyboardClicked.begin() + i, WindowsWindow::KeyboardClicked[i]);
				//WindowsWindow::KeyboardClicked.erase(it);
				return true;
			}
		}
		return false;
	}

	bool Input::IsKeyReleased(KeyBoardKey Key) {
		for (int i = 0; i < WindowsWindow::KeyboardReleased.size(); i++) {
			if (Key == WindowsWindow::KeyboardReleased[i]) {
				//auto it = std::find(WindowsWindow::KeyboardReleased.begin(), WindowsWindow::KeyboardReleased.begin() + i, WindowsWindow::KeyboardReleased[i]);
				//WindowsWindow::KeyboardReleased.erase(it);
				return true;
			}
		}
		return false;
	}

	bool Input::IsKeyHold(KeyBoardKey Key){
		for (int i = 0; i < WindowsWindow::KeyboardKeyRepeat.size(); i++) {
			if (Key == WindowsWindow::KeyboardKeyRepeat[i]) {
				//auto it = std::find(WindowsWindow::KeyboardKeyRepeat.begin(), WindowsWindow::KeyboardKeyRepeat.begin() + i, WindowsWindow::KeyboardKeyRepeat[i]);
				//WindowsWindow::KeyboardKeyRepeat.erase(it);
				return true;
			}
		}
		return false;
	}

	bool Input::IsKeyPressed(KeyBoardKey Key) {
		//if (ImGui::IsAnyWindowHovered())return;
		//if (ImGui::IsAnyItemHovered())return;
		if (glfwGetKey(CurrentWindow::GetWindow(), (int)Key)) {
			/*
			for (int i = 0; i < KeyPressed.size(); i++) {
				if (Key == KeyPressed[i]) {
					goto a;
				}
			}
			KeyPressed.emplace_back(Key);
			KeyPressedIndex++;
			*/
			return  true;
		}
		/*
		a:
		auto it = std::find(KeyPressed.begin(), KeyPressed.begin() + KeyPressedIndex, Key);
		if (it != KeyPressed.end()) {
			KeyPressed.erase(it);
			KeyPressedIndex--;
		}
		*/
		return false;
	}

	bool Input::IsKeyDoubleClick(KeyBoardKey Key) {
		for (int i = 0; i < WindowsWindow::KeyboardKeyDoubleClicked.size(); i++) {
			if (Key == WindowsWindow::KeyboardKeyDoubleClicked[i]) {
				//auto it = std::find(WindowsWindow::KeyboardKeyDoubleClicked.begin(), WindowsWindow::KeyboardKeyDoubleClicked.begin() + i, WindowsWindow::KeyboardKeyDoubleClicked[i]);
				//WindowsWindow::KeyboardKeyDoubleClicked.erase(it);
				return true;
			}
		}
		return false;
	}

	bool Input::IsMouseButtonClicked(MouseButton Button) {
		for (int i = 0; i < WindowsWindow::MouseButtonClicked.size(); i++) {
			if (Button == WindowsWindow::MouseButtonClicked[i]) {
				//auto it = std::find(WindowsWindow::MouseButtonClicked.begin(), WindowsWindow::MouseButtonClicked.begin() + i, WindowsWindow::MouseButtonClicked[i]);
				//WindowsWindow::MouseButtonClicked.erase(it);
				return true;
			}
		}
		return false;
	}
	bool Input::IsMouseButtonReleased(MouseButton Button) {
		for (int i = 0; i < WindowsWindow::MouseButtonReleased.size(); i++) {
			if (Button == WindowsWindow::MouseButtonReleased[i]) {
				//auto it = std::find(WindowsWindow::MouseButtonReleased.begin(), WindowsWindow::MouseButtonReleased.begin() + i, WindowsWindow::MouseButtonReleased[i]);
				//WindowsWindow::MouseButtonReleased.erase(it);
				return true;
			}
		}
		return false;
	}

	bool Input::IsMouseButtonDoubleClicked(MouseButton Button){
		for (int i = 0; i < WindowsWindow::MouseButtonDoubleClicked.size(); i++) {
			if (Button == WindowsWindow::MouseButtonDoubleClicked[i]) {
				//auto it = std::find(WindowsWindow::MouseButtonDoubleClicked.begin(), WindowsWindow::MouseButtonDoubleClicked.begin() + i, WindowsWindow::MouseButtonDoubleClicked[i]);
				//WindowsWindow::MouseButtonDoubleClicked.erase(it);
				return true;
			}
		}
		return false;
	}

	bool Input::IsMouseScrollWheelUp(){
		float ScrollUp = 1.0f;
		for (int i = 0; i < WindowsWindow::MouseScrollY.size(); i++) {
			if (ScrollUp == WindowsWindow::MouseScrollY[i]) {
				//auto it = std::find(WindowsWindow::MouseScrollY.begin(), WindowsWindow::MouseScrollY.begin() + i, WindowsWindow::MouseScrollY[i]);
				//WindowsWindow::MouseScrollY.erase(it);
				return true;
			}
		}
		return false;
	}

	bool Input::IsMouseScrollWheelDown(){
		float ScrollDown = -1.0f;
		for (int i = 0; i < WindowsWindow::MouseScrollX.size(); i++) {
			if (ScrollDown == WindowsWindow::MouseScrollX[i]) {
				//auto it = std::find(WindowsWindow::MouseScrollX.begin(), WindowsWindow::MouseScrollX.begin() + i, WindowsWindow::MouseScrollX[i]);
				//WindowsWindow::MouseScrollX.erase(it);
				return true;
			}
		}
		return false;
	}
	
	bool Input::IsMouseButtonPressed(MouseButton Button){
		if (ImGui::IsAnyWindowHovered())return false;
		if (ImGui::IsAnyItemHovered())return false;
		if (glfwGetMouseButton(CurrentWindow::GetWindow(), (int)Button)) {
			/*
			for (int i = 0; i < MouseButtonPressed.size(); i++) {
				if (Button == MouseButtonPressed[i]) {
					goto a;
				}
			}
			MouseButtonPressed.emplace_back(Button);
			MousePressedIndex++;
			*/
			return true;
		}
		/*
		a:
		auto it = std::find(MouseButtonPressed.begin(), MouseButtonPressed.begin() + MousePressedIndex, Button);
		if (it != MouseButtonPressed.end()) {
			MouseButtonPressed.erase(it);
			MousePressedIndex--;
		}
		*/
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