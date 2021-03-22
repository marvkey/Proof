#pragma once
#include "Proof/Core/Core.h"
#include "Platform/WindowsWindow.h"
#include "KeyCodes.h"



namespace Proof {
	
	class Proof_API Input  {
	public:
		static bool  IsKeyPressed(KeyBoardKey Key);
		static bool  IsKeyReleased(KeyBoardKey Key);
		static bool  IsKeyHold(KeyBoardKey Key);
		static bool  IsKeyDoubleClick(KeyBoardKey Key);
		static bool  IsMouseButtonPressed(MouseButton Button);
		static bool  IsMouseButtonReleased(MouseButton Button);
		static bool  IsMouseButtonHold(MouseButton Button); // Needs to be rendered in a different way;
		static bool  IsMouseButtonDoubleClicked(MouseButton Button);
	};
	
}