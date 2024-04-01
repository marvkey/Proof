using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Proof
{
    public enum MouseCursorMode
    {

        Normal = 0,
        Hidden = 1,
        Locked = 2
    }
	public static class Mouse
	{
		public static void SetCursorMode(MouseCursorMode mode) => InternalCalls.Mouse_SetCursorMode(mode);
		public static MouseCursorMode GetCursorMode() => InternalCalls.Mouse_GetCursorMode();
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
    [RegisterCoreClassStruct]
    public class InputAction : Asset
	{

        internal InputAction() { m_ID = AssetID.Invalid; }
        internal InputAction(AssetID handle) { m_ID = handle; }
    }
    [RegisterCoreClassStruct]
    public class InputBindingContext : Asset
    {
        internal InputBindingContext() { m_ID = AssetID.Invalid; }
        internal InputBindingContext(AssetID handle) { m_ID = handle; }
    }
    [Flags]
    public enum InteractionEvent
    {
        // Indicates that no significant changes have occurred in the trigger state and there are no active device inputs.
        None = 0,

        // Represents a triggering event that occurred after one or more frames.
        Triggered = 1 << 0,    // InteractionEvent (None -> Triggered, Ongoing -> Triggered, Triggered -> Triggered)

        // Indicates that an event has initiated trigger evaluation. This event is fired first before Triggered.
        Started = 1 << 1,    // InteractionEvent (None -> Ongoing, None -> Triggered)

        // Represents a continuing process of triggering. For example, Release a key while the key is clicked it would be Ongoing, as the key has not been triggered yet
        Ongoing = 1 << 2,    // InteractionEvent (Ongoing -> Ongoing)

        // Indicates that triggering has been canceled. For example, user did not double click key in a specific threshold;
        Canceled = 1 << 3,    // InteractionEvent (Ongoing -> None)

        // Indicates that the trigger state transitioned from Triggered to None this frame, indicating the completion of triggering.
        Completed = 1 << 4,    // InteractionEvent (Triggered -> None)
    }

    public enum InputActionOutputType
    {
        Bool,
		Float,
		Vector2D,
		Vector3D
    };

    [RegisterCoreClassStruct]
    [StructLayout(LayoutKind.Sequential)]
    public struct InputActionOutput
    {
        private Vector3 m_Output;
        private InputActionOutputType m_OutputType;

        public InputActionOutput(bool inValue)
        {
            m_Output = new Vector3(inValue ? 1f : 0f, 0f, 0f);
            m_OutputType = InputActionOutputType.Bool;
        }

        public InputActionOutput(float inValue)
        {
            m_Output = new Vector3(inValue, 0f, 0f);
            m_OutputType = InputActionOutputType.Float;
        }

        public InputActionOutput(Vector2 inValue)
        {
            m_Output = new Vector3(inValue.X, inValue.Y, 0f);
            m_OutputType = InputActionOutputType.Vector2D;
        }

        public InputActionOutput(Vector3 inValue)
        {
            m_Output = inValue;
            m_OutputType = InputActionOutputType.Vector3D;
        }

        public InputActionOutput(InputActionOutputType inValueType, Vector3 value)
        {
            m_Output = value;
            m_OutputType = inValueType;

            // Adjust value components based on the specified type
            if (m_OutputType == InputActionOutputType.Bool || m_OutputType == InputActionOutputType.Float)
            {
                m_Output.Y = 0f;
                m_Output.Z = 0f;
            }
            else if (m_OutputType == InputActionOutputType.Vector2D)
            {
                m_Output.Z = 0f;
            }
        }

        public InputActionOutputType GetOutputType()
        {
            return m_OutputType;
        }

        public T Get<T>()
        {
            switch (typeof(T))
            {
                case Type boolType when boolType == typeof(bool):
                    if (m_OutputType != InputActionOutputType.Bool)
                    {
                        Log.Warn("Input Action is not a Bool Value ");
                    }
                    return (T)(object)(IsNonZero());

                case Type floatType when floatType == typeof(float):
                    if (m_OutputType != InputActionOutputType.Float)
                        Log.Warn("Input Action is not a float Value ");
                    return (T)(object)m_Output.X;

                case Type vector2Type when vector2Type == typeof(Vector2):
                    if (m_OutputType != InputActionOutputType.Vector2D)
                        Log.Warn("Input Action is not a vector2 Value ");
                    return (T)(object)new Vector2(m_Output.X, m_Output.Y);

                case Type vector3Type when vector3Type == typeof(Vector3):
                    if (m_OutputType != InputActionOutputType.Vector3D)
                        Log.Warn("Input Action is not a vector3 Value ");
                    return (T)(object)new Vector3(m_Output.X, m_Output.Y, m_Output.Z);

                default:
                    Log.Error($"Input Action is not {typeof(T).FullName}");
                    break;
            }
            return (T)(object)m_Output;
        }
        public bool IsNonZero(float tolerance = float.Epsilon)
        {
            return (m_Output.X * m_Output.X + m_Output.Y * m_Output.Y + m_Output.Z * m_Output.Z) >= tolerance * tolerance;
        }

        public float this[int index]
        {
            get { return m_Output[index]; }
        }

        public float GetMagnitudeSq()
        {
            switch (GetOutputType())
            {
                case InputActionOutputType.Bool:
                case InputActionOutputType.Float:
                    return m_Output.X * m_Output.X;
                case InputActionOutputType.Vector2D:
                    return m_Output.X * m_Output.X + m_Output.Y * m_Output.Y;
                case InputActionOutputType.Vector3D:
                    return m_Output.X * m_Output.X + m_Output.Y * m_Output.Y + m_Output.Z * m_Output.Z;
                default:
                    return 0f;
            }
        }

    }
}
