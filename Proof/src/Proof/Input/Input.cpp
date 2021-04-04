#include "Proofprch.h"
#include "Input.h"
#include "Platform/CurrentWindow.h"
namespace Proof {

	bool Input::IsKeyClicked(KeyBoardKey Key) {
		for (int i = 0; i < WindowsWindow::KeyboardClicked.size(); i++) {
			if (Key == WindowsWindow::KeyboardClicked[i])return true;
		}
		return false;
	}
	bool Input::IsKeyReleased(KeyBoardKey Key) {
		for (int i = 0; i < WindowsWindow::KeyboardReleased.size(); i++) {
			if (Key == WindowsWindow::KeyboardReleased[i])return true;
		}
		return false;
	}

	bool Input::IsKeyHold(KeyBoardKey Key){
		for (int i = 0; i < WindowsWindow::KeyboardKeyRepeat.size(); i++) {
			if (Key == WindowsWindow::KeyboardKeyRepeat[i])return true;
		}
		return false;
	}

	bool Input::IsKeyPressed(KeyBoardKey Key) {
		if (glfwGetKey(CurrentWindow::GetWindow(), (int)Key))return  true;
		return false;
	}

	bool Input::IsKeyDoubleClick(KeyBoardKey Key) {
		for (int i = 0; i < WindowsWindow::KeyboardKeyDoubleClicked.size(); i++) {
			if (Key == WindowsWindow::KeyboardKeyDoubleClicked[i])return true;
		}
		return false;
	}


	bool Input::IsMouseButtonClicked(MouseButton Button) {
		for (int i = 0; i < WindowsWindow::MouseButtonClicked.size(); i++) {
			if (Button == WindowsWindow::MouseButtonClicked[i])return true;
		}
		return false;
	}
	bool Input::IsMouseButtonReleased(MouseButton Button) {
		for (int i = 0; i < WindowsWindow::MouseButtonReleased.size(); i++) {
			if (Button == WindowsWindow::MouseButtonReleased[i])return true;
		}
		return false;
	}

	bool Input::IsMouseButtonDoubleClicked(MouseButton Button){

		for (int i = 0; i < WindowsWindow::MouseButtonDoubleClicked.size(); i++) {
			if (Button == WindowsWindow::MouseButtonDoubleClicked[i])return true;
		}
		return false;
	}
	bool Input::IsMouseButtonPressed(MouseButton Button){
		if (glfwGetKey(CurrentWindow::GetWindow(), (int)Button))return true;
		return false;
	}
}