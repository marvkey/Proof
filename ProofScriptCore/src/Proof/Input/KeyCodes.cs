using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Proof{
	public enum KeyBoardKey 
	{
		None = 0,
		Space = 32,
		Apostrophe = 39, /* ' */
		Comma = 44, /* , */
		Minus = 45, /* - */
		Period = 46, /* . */
		Slash = 47, /* / */

		Num0 = 48, /* 0 */
		Num1 = 49, /* 1 */
		Num2 = 50, /* 2 */
		Num3 = 51, /* 3 */
		Num4 = 52, /* 4 */
		Num5 = 53, /* 5 */
		Num6 = 54, /* 6 */
		Num7 = 55, /* 7 */
		Num8 = 56, /* 8 */
		Num9 = 57, /* 9 */
		Semicolon = 59, /* ; */
		Equal = 61, /* = */

		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,

		LeftBracket = 91,  /* [ */
		Backslash = 92,  /* \ */
		RightBracket = 93,  /* ] */
		GraveAccent = 96,  /* ` */

		World1 = 161, /* non-US #1 */
		World2 = 162,/* non-US #2 */
		/* Function keys */
		Escape = 256,
		Enter = 257,
		Tab = 258,
		Backspace = 259,
		Insert = 260,
		Delete = 261,
		RightArrow = 262,
		LeftArrow = 263,
		DownArrow = 264,
		UpArrow = 265,
		PageUp = 266,
		PageDown = 267,
		Home = 268,
		End = 269,
		CapsLock = 280,
		ScrollLock = 281,
		NumLock = 282,
		PrintScreen = 283,
		Pause = 284,

		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,

		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		LeftSuper = 343,
		RightShift = 344,
		RightControl = 345,
		RightAlt = 346,
		RightSuper = 347,
		Menu = 348,
	};
	public enum  MouseButton
	{
		None =-1,
		ButtonLeft = 0,
		ButtonRight = 1,
		ButtonMiddle = 2,
		Button3 = 3,
		Button4 = 4,
		Button5 = 5,
		Button6 = 6,
		Button7 = 7,

		Button0 = ButtonLeft,
		Button1 = ButtonRight,
		Button2 = ButtonMiddle,
	};
	public enum MouseMovementInput
	{ // rname MouseAxis
		X =0,
		Y =1,
		ScrollUp,
		ScrolDown
	};
	public enum JoystickKey
	{
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
	};
	public enum ControllerButton
	{
		None =-1,
		ButtonA = 0,
		ButtonB = 1,
		ButtonX = 2,
		ButtonY = 3,
		ButtonLeftBumper = 4,
		ButtonRightBumper = 5,
		ButtonBack = 6,
		ButtonStart = 7,
		ButtonGuide = 8,
		ButtonLeftThumb = 9,
		ButtonRightThumb = 10,
		ButtonDpadUp= 11,
		ButtonDpadRight = 12,
		ButtonDpadDown= 13,
		ButtonDpadLeft = 14,

		ButtonCROSS = ButtonA,
		ButtonCIRCLE = ButtonB,
		ButtonSQUARE = ButtonX,
		ButtonTRIANGLE = ButtonY,
		ButtonRightTrigger =20, // this are proof defined only used in the proof engine as this values
		ButtonLeftTrigger = 21 // this are proof defined only used in the proof engine as this values
	};

	public enum ControllerAxis
	{
		LeftX =0,
		LeftY =1,
		RightX =2,
		RightY =3,
		LeftTrigger =4,
		RightTrigger =5
	};

	public enum InputState
	{
        None = -1,
        KeyReleased = 0,
        KeyPressed = 1,
        KeyHold = 2,
        KeyClicked = 3,
        //KEY PRESSED AND KEY HOLD IS THE SMAE FOR MOUSE INPUTS
        KeyDouble = 4
    }

	public enum PlayerInputState
	{
		Enabled =0,
		Disabled = 1
	}
}
