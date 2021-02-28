#include "Proofprch.h"
#include "Input.h"
namespace Proof {

	bool Input::IsKeyPressed(KeyBoardKey Key) {
		for (int i = 0; i < WindowsWindow::KeyboardPressed.size(); i++) {
			if (Key == WindowsWindow::KeyboardPressed[i])return true;
		}
		return false;
	}
	bool Input::IsKeyReleased(KeyBoardKey Key) {
		for (int i = 0; i < WindowsWindow::KeyboardReleased.size(); i++) {
			if (Key == WindowsWindow::KeyboardReleased[i])return true;
		}
		return false;
	}

	bool Proof_API Input::IsKeyHold(KeyBoardKey Key) {
		for (int i = 0; i < WindowsWindow::KeyboardKeyRepeat.size(); i++) {
			if (Key == WindowsWindow::KeyboardKeyRepeat[i])return true;
		}
		return false;
		/*
		if (glfwGetKey(static_cast<GLFWwindow*>(CurrentWindow::GetWindow()), (int)Key) == GLFW_PRESS) return true;
		return false;
		*/
	}

	bool Input::IsKeyDoubleClick(KeyBoardKey Key) {
		for (int i = 0; i < WindowsWindow::KeyboardKeyDoubleClicked.size(); i++) {
			if (Key == WindowsWindow::KeyboardKeyDoubleClicked[i])return true;
		}
		return false;
	}

	bool Input::IsMouseButtonPressed(MouseButton Button) {
		for (int i = 0; i < WindowsWindow::MouseButtonPressed.size(); i++) {
			if (Button == WindowsWindow::MouseButtonPressed[i])return true;
		}
		return false;
	}
	bool Input::IsMouseButtonReleased(MouseButton Button) {
		for (int i = 0; i < WindowsWindow::MouseButtonReleased.size(); i++) {
			if (Button == WindowsWindow::MouseButtonReleased[i])return true;
		}
		return false;
	}
	bool Input::IsMouseButtonHold(MouseButton Button) {
		if (glfwGetMouseButton(static_cast<GLFWwindow*>(CurrentWindow::GetWindow()), (int)Button) == (int)InputEvent::KeyPressed) return true;
		
		return false;
		
	}
	bool Input::IsMouseButtonDoubleClicked(MouseButton Button){

		for (int i = 0; i < WindowsWindow::MouseButtonDoubleClicked.size(); i++) {
			if (Button == WindowsWindow::MouseButtonDoubleClicked[i])return true;
		}
		return false;
	}
}