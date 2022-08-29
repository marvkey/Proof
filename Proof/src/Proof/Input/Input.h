#pragma once
#include "Proof/Core/Core.h"
#include "KeyCodes.h"
#include <vector>
namespace Proof
{
	class Proof_API Input {
	public:
		static bool IsKeyClicked(KeyBoardKey Key); // checks if key has been pressed on only render once until key is pressed again
		static bool IsKeyPressed(KeyBoardKey Key); // checks if the key is pressed but has not been released chekcs every frame
		static bool IsKeyReleased(KeyBoardKey Key); // chekcs if the key was pressed the chekcs if release does this every frame
		static bool IsKeyHold(KeyBoardKey Key); // can only render 1input at a time and checks if the key is held
		static bool IsKeyDoubleClick(KeyBoardKey Key); // chekcs if the key has been clicked 2 
		static bool IsMouseButtonClicked(MouseButton Button);// checks if key has been pressed on only render once until key is pressed again
		static bool IsMouseButtonPressed(MouseButton Button); // checks if key is pressed 
		static bool IsMouseButtonReleased(MouseButton Button); // chekcs if the key was pressed the chekcs if release does this every frame
		static bool IsMouseButtonDoubleClicked(MouseButton Button);// chekcs if the key has been clicked 2 times

		static bool IsMouseScrollUp();
		static bool IsMouseScrollDown();
		static bool IsMouseScrolled();
		static float GetScrollWheelX();
		static float GetScrollWheelY();
		static float GetMousePosX();
		static float GetMousePosY();

		// CONTROLLER

		static bool IsControllerClicked(int ID, ControllerButton button);
		static bool IsAnyControllerClicked(ControllerButton button);

		static bool IsControllerDoubleClick(int ID, ControllerButton button);
		static bool IsAnyControllerDoubleClick(ControllerButton button);

		static bool IsControllerReleased(int ID, ControllerButton button);
		static bool IsAnyControllerReleased(ControllerButton button);

		static bool IsControllerPressed(int ID, ControllerButton button);
		static bool IsAnyControllerPressed(ControllerButton button);

		std::pair<float, float>GetControllerLeftJoystickAxis(int ID);
		std::pair<float, float> GetControllerLeftJoystickAxisDistance(int ID);

		std::pair<float, float>GetControllerRightJoystickAxis(int ID);
		std::pair<float, float> GetControllerRightJoystickAxisDistance(int ID);

		float GetControllerLeftTriggerAxis(int ID);
		float GetControllerLeftTriggerAxisDistance(int ID);

		float GetControllerRightTriggerAxis(int ID);
		float GetControllerRightTriggerAxisDistance(int ID);
	};
}