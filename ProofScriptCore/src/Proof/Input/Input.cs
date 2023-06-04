using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Proof
{
	public static class Mouse
	{
		public static void Capture(bool capture) => InternalCalls.Mouse_CaptureMouse(capture);
		public static bool IsMouseCaptured() => InternalCalls.Mouse_IsMouseCaptured();
		public static Vector2 GetPositon()
		{
			Vector2 vec = new Vector2();
			InternalCalls.Mouse_GetPosition(out vec);
			return vec;
		}
    }
    public static class Input
    {
        // checks if key has been pressed on only render once until key is pressed again

        public static bool IsKeyClicked(KeyBoardKey key) => InternalCalls.Input_IsKeyClicked((int)key);
        public static bool IsKeyPressed(KeyBoardKey key) => InternalCalls.Input_IsKeyPressed((int)key);
		public static bool IsKeyReleased(KeyBoardKey key)=> InternalCalls.Input_IsKeyReleased((int)key);
		public static bool IsKeyHold(KeyBoardKey key)=>InternalCalls.Input_IsKeyHold((int)key);
		public static bool IsKeyDoubleClick(KeyBoardKey key)=>InternalCalls.Input_IsKeyDoubleClick((int)key);   

		public static bool IsMouseButtonClicked(MouseButton button)=>InternalCalls.Input_IsMouseButtonClicked((int)button);
        public static bool IsMouseButtonPressed(MouseButton button)=>InternalCalls.Input_IsMouseButtonPressed((int)button);	
		public static bool IsMouseButtonReleased(MouseButton button)=>InternalCalls.Input_IsMouseButtonPressed((int)button);
		public static bool IsMouseButtonDoubleClicked(MouseButton button) => InternalCalls.Input_IsMouseButtonDoubleClicked((int)button);
		
	}
}
