#pragma once
#include "Proof/Core/Core.h"
#include "KeyCodes.h"
#include <vector>
#include "Mouse.h"
namespace Proof
{
	class Proof_API Input {
	public:
		static bool IsKeyClicked(KeyBoardKey Key); // checks if key has been pressed on only render once until key is pressed again
		static bool	IsKeyPressed(KeyBoardKey Key); // checks if the key is pressed but has not been released chekcs every frame
		static bool IsKeyReleased(KeyBoardKey Key); // chekcs if the key was pressed the chekcs if release does this every frame
		static bool IsKeyHold(KeyBoardKey Key); // can only render 1input at a time and checks if the key is held
		static bool IsKeyDoubleClick(KeyBoardKey Key); // chekcs if the key has been clicked 2 times

		static bool IsMouseButtonClicked(MouseButton Button);// checks if key has been pressed on only render once until key is pressed again
		static bool	IsMouseButtonPressed(MouseButton Button); // checks if key is pressed 
		static bool IsMouseButtonReleased(MouseButton Button); // chekcs if the key was pressed the chekcs if release does this every frame
		static bool IsMouseButtonDoubleClicked(MouseButton Button);// chekcs if the key has been clicked 2 times
		static bool	IsMouseScrollWheelUp();
		static bool	IsMouseScrollWheelDown();
		static bool IsMouseScrolled();
		static float GetScrollWheelX();
		static float GetScrollWheelY();
		static float GetMousePosX();
		static float GetMousePosY();
		static bool ViewPoartHovered;  // This Should Be Temporary;
	};
}