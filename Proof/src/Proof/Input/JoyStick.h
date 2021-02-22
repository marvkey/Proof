#pragma once
#include "Proof/Core/Core.h"
#include "Input.h"

namespace Proof {
	class Proof_API Joystick :public Input {

		static enum JoystickKey {
			Joystick1 = 0,
			Joystick2 = 1,
			Joystick3 = 2,
			Joystick4 = 3,
			Joystick5 = 4,
			Joystick6 = 5,
			Joystick7 = 6,
			Joystick8 = 7,
			Joystick9 = 8,
			Joystick10 = 9,
			Joystick11 = 10,
			Joystick12 = 11,
			Joystick13 = 12,
			Joystick14 = 13,
			Joystick15 = 14,
			Joystick16 = 15,
			JoystickLast = Joystick16
		};
	};
}
