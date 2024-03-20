#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Input/KeyCodes.h"
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

namespace Proof
{
	enum class InputAxisType
	{
		None,
		Button,			// Whilst the physical input is an analog axis the InputKey uses it to emulate a digital button.
		Axis1D,
		Axis2D,
		Axis3D,
	};
	enum class ElevatedInputKeyDeviceType
	{
		None = 0,
		Mouse,
		KeyBoard,
		Controller
	};

	enum class ElevatedAxisPairing
	{
		Unpaired,			// This key is unpaired
		X,					// This key represents the X axis of its PairedAxisKey
		Y,					// This key represents the Y axis of its PairedAxisKey
		Z,					// This key represents the Z axis of its PairedAxisKey - Currently unused
	};
	struct ElevatedInputKey
	{
		enum class InputAxisType
		{
			None,
			Button,			// Whilst the physical input is an analog axis the InputKey uses it to emulate a digital button.
			Axis1D,
			Axis2D,
			Axis3D,
		};

		enum KeyFlags
		{
			ControllerButton = 1 << 0,
			Touch = 1 << 1,
			MouseButton = 1 << 2,
			/*
			Modifier keys are keys on a keyboard that are used to modify the function of other keys when pressed simultaneously.
			Here are some common modifier keys:
			Shift: Used to type capital letters and access symbols above numbers on the keyboard.
			Control (Ctrl): Often used in combination with other keys to perform shortcuts and commands.
			Alt (Alternate): Also known as Option on Mac keyboards, it is used to access alternate functions of other keys.
			Command (Cmd): Found on Mac keyboards, used to perform various system and application-specific commands.
			Windows key: Found on Windows keyboards, used to access the Start menu and perform system shortcuts.
			Meta key: Found on some Unix-based systems, used similarly to the Windows key or Command key.
			Function (Fn): Found on laptops and used to access additional functions assigned to keys on the keyboard.
			*/
			ModifierKey = 1 << 3,
			// Denotes a one-dimensional axis input, typically used for analog inputs like triggers or thumbsticks.
			Axis1D = 1 << 4,
			//Denotes a two-dimensional axis input, often used for movement or camera control in 2D space
			Axis2D = 1 << 5,
			// Represents a three-dimensional axis input, often used for motion or orientation tracking.
			Axis3D = 1 << 6,
			// Gesture input types such as Flick, Pinch, and Rotate
			Gesture = 1 << 7,

			//An axis acting as a button like for example right stick of controller input
			AxisButton = 1 << 8,

			/*
			*
				The UpdateAxisWithoutSamples flag determines whether an axis should be 
				updated even when no continuous input samples are available. 
				This is particularly useful for handling inputs that are binary in nature,
				such as button presses, where continuous sampling isn't necessary. 
				For instance, imagine a game where pressing a button toggles the player's flashlight. 
				In this scenario, setting UpdateAxisWithoutSamples to true for the flashlight button ensures that the flashlight state 
				is updated immediately upon pressing the button, without waiting for continuous input samples.
			*/
			UpdateAxisWithoutSamples = 1 << 9,

			NoFlags = 0,
		};
		ElevatedInputKey(ElevatedInputKeyDeviceType inputDevice, const std::string& keyName, const uint32_t keyFlags = 0) :
			Device(inputDevice), KeyName(keyName), KeyFlags(keyFlags)
		{
			Init(keyFlags);
		}

		bool IsModifierKey() const { return m_IsModifierKey != 0; }
		bool IsControllerKey() const { return m_IsControllerKey != 0; }
		bool IsTouch() const { return m_IsTouch != 0; }
		bool IsMouseButton() const { return m_IsMouseButton != 0; }
		bool IsAxis1D() const { return m_AxisType == InputAxisType::Axis1D; }
		bool IsAxis2D() const { return m_AxisType == InputAxisType::Axis2D; }
		bool IsAxis3D() const { return m_AxisType == InputAxisType::Axis3D; }
		bool IsAxisButton() const { return m_AxisType == InputAxisType::Button; }	// Analog 1D axis emulating a digital button press.
		//if key is axis1d axis2d or axis 3d
		/**
		*
			Analog input refers to input devices or controls that provide a continuous range of
			values rather than discrete on/off states.
			Unlike digital input, which has binary states (e.g., pressed or not pressed), analog input can produce a wide range of values within a certain range.

			Common examples of analog input include:

				Joysticks and Gamepads: These input devices often have analog thumbsticks or triggers that can produce a range of values based on how much they are pushed or tilted.

				Mouse Movement: The movement of a mouse across a surface can be considered analog input,
				as it produces continuous values representing the distance and direction traveled.

				Scroll Wheels: Some mice and other input devices feature scroll wheels that produce analog input, allowing users to scroll through content with varying speeds.

				Pressure-Sensitive Inputs: Certain input devices, such as graphics tablets or pressure-sensitive styluses, can detect varying levels of pressure applied by the user.

			In contrast, digital input devices produce discrete values, typically representing either an "on" or "off" state.
			Examples of digital input include keyboard keys, mouse buttons, and most touchscreen interactions (where a touch is typically detected as either "touched" or "not touched").

			Analog input is often used in applications where fine-grained control or precision is required,
			such as gaming, graphic design, or music production.
			The ability to detect and interpret a wide range of input values allows for more nuanced and expressive interactions with digital systems.
		*/

		bool IsAnalog() const { return IsAxis1D() || IsAxis2D() || IsAxis3D(); }
		// not an axis key
		bool IsDigital() const { return !IsAnalog(); }
		bool IsGesture() const { return m_IsGesture != 0; }

		bool ShouldUpdateAxisWithoutSamples ()const { return m_UpdateAxisWithoutSamples; }
		InputAxisType GetAxisType()const { return m_AxisType; }
		ElevatedAxisPairing GetPairedAxis()const { return m_AxisParing; }
		friend bool operator==(const ElevatedInputKey& KeyA, const ElevatedInputKey& KeyB) { return KeyA.KeyName == KeyB.KeyName; }
		friend bool operator!=(const ElevatedInputKey& KeyA, const ElevatedInputKey& KeyB) { return KeyA.KeyName != KeyB.KeyName; }
	public:
		const std::string KeyName;
		const ElevatedInputKeyDeviceType Device = ElevatedInputKeyDeviceType::None;
		const uint32_t KeyFlags;
		const ElevatedInputKey* AxisParingKey = nullptr;

		
	private:
		void Init(uint32_t keyFlags)
		{
			m_IsModifierKey = ((keyFlags & KeyFlags::ModifierKey) != 0);
			m_IsControllerKey = ((keyFlags & KeyFlags::ControllerButton) != 0);
			m_IsTouch = ((keyFlags & KeyFlags::Touch) != 0);
			m_IsMouseButton = ((keyFlags & KeyFlags::MouseButton) != 0);
			m_IsGesture = ((keyFlags & KeyFlags::Gesture) != 0);
			m_UpdateAxisWithoutSamples = ((keyFlags & KeyFlags::UpdateAxisWithoutSamples) != 0);

			if ((keyFlags & KeyFlags::AxisButton) != 0)
			{
				//ensure((keyFlags& (KeyFlags::Axis1D | KeyFlags::Axis2D | KeyFlags::Axis3D)) == 0);
				m_AxisType = InputAxisType::Button;
			}
			else if ((keyFlags & KeyFlags::Axis1D) != 0)
			{
				//ensure((keyFlags& (KeyFlags::Axis2D | KeyFlags::Axis3D)) == 0);
				m_AxisType = InputAxisType::Axis1D;
			}
			else if ((keyFlags & KeyFlags::Axis2D) != 0)
			{
				//ensure((keyFlags& KeyFlags::Axis3D) == 0);
				m_AxisType = InputAxisType::Axis2D;
			}
			else if ((keyFlags & KeyFlags::Axis3D) != 0)
			{
				m_AxisType = InputAxisType::Axis3D;
			}
			else
			{
				m_AxisType = InputAxisType::None;
			}
		}
	private:
		friend void AddPairedKey(const ElevatedInputKey* key, const ElevatedInputKey* keyXAxis, const ElevatedInputKey* keyYAxis);
		InputAxisType m_AxisType;
		ElevatedAxisPairing m_AxisParing;
		bool m_IsModifierKey = false;
		bool m_IsControllerKey = false;
		bool m_IsTouch = false;
		bool m_IsMouseButton = false;
		bool m_IsGesture = false;
		
		bool m_UpdateAxisWithoutSamples = false;
	};
	
	struct ElevatedInputKeys
	{
		static const ElevatedInputKey Invalid;

		static const ElevatedInputKey MouseMoveX;
		static const ElevatedInputKey MouseMoveY;
		static const ElevatedInputKey MouseMoveAxis;
		static const ElevatedInputKey MouseScrollUp;
		static const ElevatedInputKey MouseScrollDown;
		static const ElevatedInputKey MouseWheelAxis;

		static const ElevatedInputKey LeftMouseButton;
		static const ElevatedInputKey RightMouseButton;
		static const ElevatedInputKey MiddleMouseButton;
		static const ElevatedInputKey ThumbMouseButton;
		static const ElevatedInputKey ThumbMouseButton2;

		static const ElevatedInputKey BackSpace;
		static const ElevatedInputKey Tab;
		static const ElevatedInputKey Enter;
		static const ElevatedInputKey Pause;

		static const ElevatedInputKey CapsLock;
		static const ElevatedInputKey Escape;
		static const ElevatedInputKey SpaceBar;
		static const ElevatedInputKey PageUp;
		static const ElevatedInputKey PageDown;
		static const ElevatedInputKey End;
		static const ElevatedInputKey Home;

		static const ElevatedInputKey Left;
		static const ElevatedInputKey Up;
		static const ElevatedInputKey Right;
		static const ElevatedInputKey Down;

		static const ElevatedInputKey Insert;
		static const ElevatedInputKey Delete;

		static const ElevatedInputKey Zero;
		static const ElevatedInputKey One;
		static const ElevatedInputKey Two;
		static const ElevatedInputKey Three;
		static const ElevatedInputKey Four;
		static const ElevatedInputKey Five;
		static const ElevatedInputKey Six;
		static const ElevatedInputKey Seven;
		static const ElevatedInputKey Eight;
		static const ElevatedInputKey Nine;

		static const ElevatedInputKey A;
		static const ElevatedInputKey B;
		static const ElevatedInputKey C;
		static const ElevatedInputKey D;
		static const ElevatedInputKey E;
		static const ElevatedInputKey F;
		static const ElevatedInputKey G;
		static const ElevatedInputKey H;
		static const ElevatedInputKey I;
		static const ElevatedInputKey J;
		static const ElevatedInputKey K;
		static const ElevatedInputKey L;
		static const ElevatedInputKey M;
		static const ElevatedInputKey N;
		static const ElevatedInputKey O;
		static const ElevatedInputKey P;
		static const ElevatedInputKey Q;
		static const ElevatedInputKey R;
		static const ElevatedInputKey S;
		static const ElevatedInputKey T;
		static const ElevatedInputKey U;
		static const ElevatedInputKey V;
		static const ElevatedInputKey W;
		static const ElevatedInputKey X;
		static const ElevatedInputKey Y;
		static const ElevatedInputKey Z;

		static const ElevatedInputKey NumPadZero;
		static const ElevatedInputKey NumPadOne;
		static const ElevatedInputKey NumPadTwo;
		static const ElevatedInputKey NumPadThree;
		static const ElevatedInputKey NumPadFour;
		static const ElevatedInputKey NumPadFive;
		static const ElevatedInputKey NumPadSix;
		static const ElevatedInputKey NumPadSeven;
		static const ElevatedInputKey NumPadEight;
		static const ElevatedInputKey NumPadNine;

		static const ElevatedInputKey NumPadMultiply;
		static const ElevatedInputKey NumPadAdd;
		static const ElevatedInputKey NumPadSubtract;
		static const ElevatedInputKey NumPadDecimal;
		static const ElevatedInputKey NumPadDivide;

		static const ElevatedInputKey F1;
		static const ElevatedInputKey F2;
		static const ElevatedInputKey F3;
		static const ElevatedInputKey F4;
		static const ElevatedInputKey F5;
		static const ElevatedInputKey F6;
		static const ElevatedInputKey F7;
		static const ElevatedInputKey F8;
		static const ElevatedInputKey F9;
		static const ElevatedInputKey F10;
		static const ElevatedInputKey F11;
		static const ElevatedInputKey F12;

		static const ElevatedInputKey NumLock;
		static const ElevatedInputKey ScrollLock;
		static const ElevatedInputKey Menu;

		static const ElevatedInputKey LeftShift;
		static const ElevatedInputKey RightShift;
		static const ElevatedInputKey LeftControl;
		static const ElevatedInputKey RightControl;
		static const ElevatedInputKey LeftAlt;
		static const ElevatedInputKey RightAlt;
		static const ElevatedInputKey LeftCommand;
		static const ElevatedInputKey RightCommand;

		static const ElevatedInputKey Semicolon;
		static const ElevatedInputKey Equals;
		static const ElevatedInputKey Comma;
		static const ElevatedInputKey Underscore;
		static const ElevatedInputKey Hyphen;
		static const ElevatedInputKey Period;
		static const ElevatedInputKey Slash;
		static const ElevatedInputKey GraveAccent;
		static const ElevatedInputKey LeftBracket;
		static const ElevatedInputKey Backslash;
		static const ElevatedInputKey RightBracket;
		static const ElevatedInputKey Apostrophe;

		static const ElevatedInputKey Ampersand;
		static const ElevatedInputKey Asterix;
		static const ElevatedInputKey Caret;
		static const ElevatedInputKey Colon;
		static const ElevatedInputKey Dollar;
		static const ElevatedInputKey Exclamation;
		static const ElevatedInputKey LeftParantheses;
		static const ElevatedInputKey RightParantheses;
		static const ElevatedInputKey Quote;

		static const ElevatedInputKey A_AccentGrave;
		static const ElevatedInputKey E_AccentGrave;
		static const ElevatedInputKey E_AccentAigu;
		static const ElevatedInputKey C_Cedille;
		static const ElevatedInputKey Section;

		// Controller Keys
		static const ElevatedInputKey ControllerLeftStick2D;
		static const ElevatedInputKey ControllerRightStick2D;
		static const ElevatedInputKey ControllerLeftStickX;
		static const ElevatedInputKey ControllerLeftStickY;
		static const ElevatedInputKey ControllerRightStickX;
		static const ElevatedInputKey ControllerRightStickY;
		static const ElevatedInputKey ControllerLeftTriggerAxis;
		static const ElevatedInputKey ControllerRightTriggerAxis;

		static const ElevatedInputKey ControllerLeftThumbstick;
		static const ElevatedInputKey ControllerRightThumbstick;
		static const ElevatedInputKey ControllerBack;
		static const ElevatedInputKey ControllerStart;
		static const ElevatedInputKey ControllerGuide;
		static const ElevatedInputKey ControllerSpecialRight;
		static const ElevatedInputKey ControllerFaceButtonBottom;
		static const ElevatedInputKey ControllerFaceButtonRight;
		static const ElevatedInputKey ControllerFaceButtonLeft;
		static const ElevatedInputKey ControllerFaceButtonTop;
		static const ElevatedInputKey ControllerLeftShoulder;
		static const ElevatedInputKey ControllerRightShoulder;
		static const ElevatedInputKey ControllerLeftTrigger;
		static const ElevatedInputKey ControllerRightTrigger;
		static const ElevatedInputKey ControllerDPadUp;
		static const ElevatedInputKey ControllerDPadDown;
		static const ElevatedInputKey ControllerDPadRight;
		static const ElevatedInputKey ControllerDPadLeft;

		// Virtual key codes used for input axis button press/release emulation
		static const ElevatedInputKey ControllerLeftStickUp;
		static const ElevatedInputKey ControllerLeftStickDown;
		static const ElevatedInputKey ControllerLeftStickRight;
		static const ElevatedInputKey ControllerLeftStickLeft;

		static const ElevatedInputKey ControllerRightStickUp;
		static const ElevatedInputKey ControllerRightStickDown;
		static const ElevatedInputKey ControllerRightStickRight;
		static const ElevatedInputKey ControllerRightStickLeft;

	public:
		static ElevatedInputKey GetKeyBoardKey(enum KeyBoardKey key);
	private:
		static void Init();
	};

	enum class ElevatedKeyEventType
	{
		None = 0,
		Clicked = 1,
		Repeat = 2, //Repeat
		//Pressed = 3,
		Released = 4,
		Double = 5,
		//Axis1D,
		//Axis2D,
		//Axis3D
	};

	// Axis event should be used as Clicked
	struct ElevatedInputKeyParams
	{
		ElevatedInputKeyParams() = default;

		ElevatedInputKeyParams(ElevatedInputKey InKey, ElevatedKeyEventType InEvent, Count<class ElevatedInputDevice> inputDevice, glm::vec3 axis, uint32_t numSamples, bool bGamepadOverride = false)
			: Key(InKey)
			, Event(InEvent)
			, Axis(axis)
			, InputDevice(inputDevice)
			, NumSamples(numSamples)
			, bIsGamepadOverride(bGamepadOverride)
		{};

		ElevatedInputKeyParams(ElevatedInputKey InKey, ElevatedKeyEventType InEvent, Count<class ElevatedInputDevice> inputDevice, float axis, uint32_t numSamples, bool bGamepadOverride = false)
			: Key(InKey)
			, Event(InEvent)
			, NumSamples(numSamples)
			, Axis(glm::vec3(axis, 0.0, 0.0))
			, InputDevice(inputDevice)
			, bIsGamepadOverride(bGamepadOverride)
		{};

		ElevatedInputKeyParams(ElevatedInputKey InKey, ElevatedKeyEventType InEvent, Count<class ElevatedInputDevice> inputDevice, glm::vec2 axis, uint32_t numSamples, bool bGamepadOverride = false)
			: Key(InKey)
			, Event(InEvent)
			, NumSamples(numSamples)
			, Axis(glm::vec3(axis.x, axis.y, 0.0))
			, InputDevice(inputDevice)
			, bIsGamepadOverride(bGamepadOverride)
		{};

		ElevatedInputKeyParams(ElevatedInputKey InKey, ElevatedKeyEventType InEvent, Count<class ElevatedInputDevice> inputDevice, bool bGamepadOverride = false)
			: Key(InKey)
			, Event(InEvent)
			, Axis(0)
			, InputDevice(inputDevice)
			, bIsGamepadOverride(bGamepadOverride)
		{};
		Count<class ElevatedInputDevice> InputDevice;
		/** The key that has been pressed */
		ElevatedInputKey Key = ElevatedInputKeys::Invalid;

		/** The event that has caused a Button key to be considered */
		ElevatedKeyEventType Event = ElevatedKeyEventType::None;

		/** The Delta that the given key/axis has been changed by */
		/*if key clicked Axis.x = 1 if reelaes axis.x =0*/
		glm::vec3 Axis = glm::vec3(0.0f);

		/** If set to true, treat this input event as if it were from a gamepad, whether the InputKey is a gamepad key or not. */
		bool bIsGamepadOverride = false;

		/*
			The NumSamples parameter in the struct is used to indicate the number of
			samples taken into account with the input event. Its purpose is to provide information about the number of times the input event occurred within a given frame.

			In the provided code, the value of NumSamples is set differently depending on the type of input event being handled:

			Keyboard Events (HandleKeyDownEvent and HandleKeyUpEvent):

			If the key is analog (e.g., a gamepad trigger), NumSamples is set to 1 to indicate that a single sample is being considered.
			Otherwise, NumSamples is set to 0, indicating that the event is not related to analog input and does not involve multiple samples.
			Mouse Events (HandleAnalogInputEvent, HandleMouseMoveEvent, HandleMouseButtonDownEvent, HandleMouseButtonUpEvent, HandleMouseButtonDoubleClickEvent, and HandleMouseWheelOrGestureEvent):

			For mouse analog input events (HandleAnalogInputEvent), NumSamples is set to 1, as it represents a single analog input sample.
			For mouse movement events (HandleMouseMoveEvent), NumSamples is not relevant, so it is not set.
			For mouse button events (HandleMouseButtonDownEvent, HandleMouseButtonUpEvent,
			HandleMouseButtonDoubleClickEvent), NumSamples is set to 0, as these events are discrete and do not involve multiple samples.
			For mouse wheel events (HandleMouseWheelOrGestureEvent), two
			FInputKeyParams objects are constructed, one representing the pressed state and the other representing the released state.
			In this case, NumSamples is set to 0 for both objects to indicate that they are discrete events.
			In summary, the NumSamples parameter is used to provide additional context about the nature of the input event,
			particularly for analog input, where multiple samples may be relevant.
			The specific values assigned to NumSamples depend on the type of input event being handled and whether it involves analog input or discrete events.
		*/
		uint32_t NumSamples = 0;
		/** Returns true if the Key used for this input is a gamepad key */
		bool IsGamepad() const { return Key.IsControllerKey() || bIsGamepadOverride; }

		/** Get the delta of the given axis for 1D axis */
		float Get1DAxis() const { return Axis.x; }

		/** Get the delta of the given axis for 2D axis */
		glm::vec2 Get2DAxis() const { return glm::vec2((float)Axis.x, (float)Axis.y); }

		/** Get the delta of the given axis for 3D axis */
		glm::vec3 Get3DAxis() const { return Axis; }
	};

    /**
    * Trigger states are a light weight interpretation of the provided input values used in trigger UpdateState responses.
    */
    enum class TriggerState 
    {
        // No inputs
        None,

        // Triggering is being monitored, but not yet been confirmed (e.g. a time based trigger that requires the trigger state to be maintained over several frames)
        Ongoing,

        // The trigger state has been met
        Triggered,
    };
	// Define bitwise operators for enum classes to enable flag-based operations

    enum class TriggerEvent
    {
        // No significant changes occurred in trigger state and there are no active device inputs
        None = (0x0),
        // A triggering event occurred after one or more processing ticks
        Triggered = (1 << 0),    // TriggerState (None -> Triggered, Ongoing -> Triggered, Triggered -> Triggered)

        // An event has initiated trigger evaluation. Note: Triggered may also occur this frame, but this event will always be fired first.
        Started = (1 << 1),    // TriggerState (None -> Ongoing, None -> Triggered)

        // Triggering is still in progress. For instance, an action with a "Press and Hold" trigger
        // will be "Ongoing" while the user is holding down the key but the time threshold has not been met yet. 
        Ongoing = (1 << 2),    // TriggerState (Ongoing -> Ongoing)

        // Triggering has been canceled. For example, the user released a key before the "Press and Hold" time threshold.
        // The action began evaluation but was not completed. 
        Canceled = (1 << 3),    // TriggerState (Ongoing -> None)

        // The trigger state transitioned from Triggered to None this frame, indicating the completion of triggering.
        // Note: Using this event limits you to one set of triggers for Started/Completed events. You may prefer two actions, each with its own trigger rules.
        // Completed will not fire if any trigger reports Ongoing on the same frame, but both should fire. For example, Tick 2 of Hold (= Ongoing) + Pressed (= None) combo will raise Ongoing event only.
        Completed = (1 << 4),    // TriggerState (Triggered -> None)
    };
	DEFINE_ENUM_CLASS_FLAGS(TriggerEvent);
    
	enum class TriggerEventInternal 
	{
		None,					// No significant trigger state changes occurred
		Completed,				// Triggering stopped after one or more triggered ticks										TriggerState (Triggered -> None)
		Started,				// Triggering has begun																		TriggerState (None -> Ongoing)
		Ongoing,				// Triggering is still being processed														TriggerState (Ongoing -> Ongoing)
		Canceled,				// Triggering has been canceled	mid processing												TriggerState (Ongoing -> None)
		StartedAndTriggered,	// Triggering occurred in a single tick (fires both started and triggered events)			TriggerState (None -> Triggered)
		Triggered,				// Triggering occurred after one or more processing ticks									TriggerState (Ongoing -> Triggered, Triggered -> Triggered)
	};

	static inline TriggerEventInternal GetTriggerStateChangeEvent(TriggerState LastTriggerState, TriggerState NewTriggerState) 
	{
		// LastTState	NewTState     Event

		// None		 -> Ongoing		= Started
		// None		 -> Triggered	= Started + Triggered
		// Ongoing	 -> None		= Canceled
		// Ongoing	 -> Ongoing		= Ongoing
		// Ongoing	 -> Triggered	= Triggered
		// Triggered -> Triggered	= Triggered
		// Triggered -> Ongoing		= Ongoing
		// Triggered -> None	    = Completed

		switch (LastTriggerState)
		{
		case TriggerState::None:
			if (NewTriggerState == TriggerState::Ongoing)
			{
				return TriggerEventInternal::Started;
			}
			else if (NewTriggerState == TriggerState::Triggered)
			{
				return TriggerEventInternal::StartedAndTriggered;
			}
			break;
		case TriggerState::Ongoing:
			if (NewTriggerState == TriggerState::None)
			{
				return TriggerEventInternal::Canceled;
			}
			else if (NewTriggerState == TriggerState::Ongoing)
			{
				return TriggerEventInternal::Ongoing;
			}
			else if (NewTriggerState == TriggerState::Triggered)
			{
				return TriggerEventInternal::Triggered;
			}
			break;
		case TriggerState::Triggered:
			if (NewTriggerState == TriggerState::Triggered)
			{
				return TriggerEventInternal::Triggered;	// Don't re-raise Started event for multiple completed ticks.
			}
			else if (NewTriggerState == TriggerState::Ongoing)
			{
				return TriggerEventInternal::Ongoing;
			}
			else if (NewTriggerState == TriggerState::None)
			{
				return TriggerEventInternal::Completed;
			}
			break;
		}

		return TriggerEventInternal::None;
	}

	static inline TriggerEvent ConvertInternalTriggerEvent(TriggerEventInternal InternalEvent) 
	{
		switch (InternalEvent)
		{
		case TriggerEventInternal::None:
			return TriggerEvent::None;
		case TriggerEventInternal::Started:
			return TriggerEvent::Started;
		case TriggerEventInternal::Ongoing:
			return TriggerEvent::Ongoing;
		case TriggerEventInternal::Canceled:
			return TriggerEvent::Canceled;
		case TriggerEventInternal::StartedAndTriggered:
		case TriggerEventInternal::Triggered:
			return TriggerEvent::Triggered;
		case TriggerEventInternal::Completed:
			return TriggerEvent::Completed;
		}
		return TriggerEvent::None;
	}
    

}

namespace std 
{
	template <>
	struct hash<Proof::ElevatedInputKey>
	{
		std::size_t operator()(const Proof::ElevatedInputKey& key) const noexcept
		{
			return std::hash<std::string>{}(key.KeyName);
		}
	};
};