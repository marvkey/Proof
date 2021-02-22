#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Core/Application.h"
#include "Input.h"

namespace Proof {
 class Mouse:Input {
 public:
	 static enum MouseKey {
		 Button0 = 0,
		 Button1 = 1,
		 Button2 = 2,
		 Button3 = 3,
		 Button4 = 4,
		 Button5 = 5,
		 Button6 = 6,
		 Button7 = 7,

		 ButtonLast = Button7,
		 ButtonLeft = Button0,
		 ButtonRight = Button1,
		 ButtonMiddle = Button2
	 };


	 static	float GetMouseX() {
		 return Application::GetMousePosition().first;
	 }
	 static float GetMouseY() {
		 return Application::GetMousePosition().second;
	 }

	 static bool KeyPressed(Mouse::MouseKey Key) {
		 if (glfwGetKey(static_cast<GLFWwindow*>(Application::GetWindow()), Key) == InputKeys::KeyPressed) return true;
		 return false;
	 }

	 static bool KeyReleased(Mouse::MouseKey Key) {
		 if (glfwGetKey(static_cast<GLFWwindow*>(Application::GetWindow()), Key) == InputKeys::KeyReleased) return true;
		 return false;
	 }

 private:

 };
}

