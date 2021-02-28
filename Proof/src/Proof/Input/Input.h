#pragma once
#include "Proof/Core/Core.h"
#include "Platform/WindowsWindow.h"
#include "KeyCodes.h"



namespace Proof {
	enum  KeyTypes {
		SHIFT = 0x0001,
		CONTROL = 0x0002,
		ALT = 0x0004

	};

	class Proof_API Input  {
	public:
		static bool  IsKeyPressed(KeyBoardKey Key);
		static bool  IsKeyReleased(KeyBoardKey Key);
		static bool  IsKeyHold(KeyBoardKey Key);
		static bool  IsKeyDoubleClick(KeyBoardKey Key);
		static bool  IsMouseButtonPressed(MouseButton Button);
		static bool  IsMouseButtonReleased(MouseButton Button);
		static bool  IsMouseButtonHold(MouseButton Button);
		static bool  IsMouseButtonDoubleClicked(MouseButton Button);
	};
	
}