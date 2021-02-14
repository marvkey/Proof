#pragma once
#include "Proof/Core/Core.h"
namespace Proof{
			struct Proof_API Joystick {

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

			struct Proof_API GamePad{
						static enum GamePadKeys{
									
									Gampad_Button_A = 0,
									Gampad_Button_B = 1,
									Gampad_Button_X = 2,
									Gampad_Button_Y = 3,
									Gampad_Button_Left_Bumper = 4,
									Gampad_Button_Right_Bumper = 5,
									Gampad_Button_Back = 6,
									Gampad_Button_Start = 7,
									Gampad_Button_Guide = 8,
									Gampad_Button_Left_Thumb = 9,
									Gampad_Button_Right_Thumb = 10,
									Gampad_Button_DPAD_UP = 11,
									Gampad_Button_DPAD_RIGHT = 12,
									Gampad_Button_DPAD_DOWN = 13,
									Gampad_Button_DPAD_LEFT = 14,
									Gampad_Button_LAST = Gampad_Button_DPAD_LEFT,

									Gampad_Button_CROSS     = Gampad_Button_A,
									Gampad_Button_CIRCLE    = Gampad_Button_B,
									Gampad_Button_SQUARE    = Gampad_Button_X,
									Gampad_Button_TRIANGLE  = Gampad_Button_Y,

									GamePad_Axis_LEFT_X = 0,
									GamePad_Axis_LEFT_Y = 1,
									GamePad_Axis_RIGHT_X = 2,
									GamePad_Axis_RIGHT_Y = 3,
									GamePad_Axis_LEFT_TRIGGER = 4,
									GamePad_Axis_RIGHT_TRIGGER = 5,
									GamePad_Axis_LAST = GamePad_Axis_RIGHT_TRIGGER
						};
			};
}
