#pragma once
#include "Proof/Core/Core.h"
#include "Proof/Input/KeyCodes.h"
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

namespace Proof
{
	enum class ElevatedInputKeyDeviceType
	{
		None = 0,
		Mouse,
		KeyBoard,
		Controller
	};

	enum class ElevatedAxisPairing
	{
		None,			
		X,					
		Y,					
		Z,					
	};
	enum class ElevatedInputKeyInputAxisType
	{
		None,
		Button,			// a joystick click
		Axis1D,
		Axis2D,
		Axis3D,
	};

	enum class ElevatedInputKeyFlags
	{
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
		ModifierKey = BIT(0),
		Axis1D = BIT(1),
		Axis2D = BIT(2),
		Axis3D = BIT(3),
		Gesture = BIT(4),
		Touch = BIT(5),

		//An axis acting as a button like clcikcing on right joystick
		AxisButton = BIT(6),

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
		UpdateAxisWithoutSamples = BIT(7),
		NoFlags = 0,
	};

	struct ElevatedInputKey
	{
		
		ElevatedInputKey(ElevatedInputKeyDeviceType inputDevice, const std::string& keyName, const ElevatedInputKeyFlags keyFlags = ElevatedInputKeyFlags::NoFlags) :
			m_Device(inputDevice), m_KeyName(keyName), m_KeyFlags(keyFlags)
		{
			Init(keyFlags);
		}

		bool IsModifierKey() const { return m_IsModifierKey != 0; }
		bool IsTouch() const { return m_IsTouch != 0; }
		bool IsAxis1D() const { return m_AxisType == ElevatedInputKeyInputAxisType::Axis1D; }
		bool IsAxis2D() const { return m_AxisType == ElevatedInputKeyInputAxisType::Axis2D; }
		bool IsAxis3D() const { return m_AxisType == ElevatedInputKeyInputAxisType::Axis3D; }
		bool IsAxisButton() const { return m_AxisType == ElevatedInputKeyInputAxisType::Button; }	// Analog 1D axis emulating a digital button press.
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
		ElevatedInputKeyInputAxisType GetAxisType()const { return m_AxisType; }
		ElevatedAxisPairing GetPairedAxis()const { return m_AxisParing; }
		friend bool operator==(const ElevatedInputKey& KeyA, const ElevatedInputKey& KeyB) { return KeyA.m_KeyName == KeyB.m_KeyName; }
		friend bool operator!=(const ElevatedInputKey& KeyA, const ElevatedInputKey& KeyB) { return KeyA.m_KeyName != KeyB.m_KeyName; }
	public:
		// Getters for all members
		const std::string& GetKeyName() const { return m_KeyName; }
		ElevatedInputKeyDeviceType GetDevice() const { return m_Device; }
		ElevatedInputKeyFlags GetKeyFlags() const { return m_KeyFlags; }
		const ElevatedInputKey* GetAxisParingKey() const { return m_AxisParingKey; }

		
	private:
		void Init(ElevatedInputKeyFlags keyFlags);
	private:
		friend void AddPairedKey(const ElevatedInputKey* key, const ElevatedInputKey* keyXAxis, const ElevatedInputKey* keyYAxis);
		ElevatedInputKeyInputAxisType m_AxisType;
		ElevatedAxisPairing m_AxisParing;
		bool m_IsModifierKey = false;
		bool m_IsTouch = false;
		bool m_IsGesture = false;
		
		bool m_UpdateAxisWithoutSamples = false;

		std::string m_KeyName;
		ElevatedInputKeyDeviceType m_Device;
		ElevatedInputKeyFlags m_KeyFlags;
		const ElevatedInputKey* m_AxisParingKey;

	};
	DEFINE_ENUM_CLASS_FLAGS(ElevatedInputKeyFlags);
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
		static const ElevatedInputKey ControllerDPad; // 2d axis

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
		static ElevatedInputKey GetKeyByName(const std::string& name);// returns invalid if name not found

		static const std::unordered_map<std::string, const ElevatedInputKey*>& GetKeys();
		static const std::unordered_map<ElevatedInputKeyDeviceType, std::vector<const ElevatedInputKey*>>& GetDeviceKeys();

	private:
		static void Init();
		static void ShutDown();

		friend class Application;
	};

	enum class ElevatedKeyEventType
	{
		None = 0,
		Clicked = 1,
		Repeat = 2, 
		Released = 4,
		Double = 5,
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
		ElevatedInputKey Key = ElevatedInputKeys::Invalid;

		ElevatedKeyEventType Event = ElevatedKeyEventType::None;

		/*if key clicked Axis.x = 1 if reelaes axis.x =0*/
		glm::vec3 Axis = glm::vec3(0.0f);

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
		bool IsGamepad() const { return Key.GetDevice() == ElevatedInputKeyDeviceType::Controller || bIsGamepadOverride; }

		float Get1DAxis() const { return Axis.x; }

		glm::vec2 Get2DAxis() const { return glm::vec2((float)Axis.x, (float)Axis.y); }

		glm::vec3 Get3DAxis() const { return Axis; }
	};

    enum class InteractionState 
    {
        // No inputs
        None,

        // Interaction is being checked but it is not triggered yet, like a double click key hte first click has been met
        Ongoing,

        // The trigger state has been met
        Triggered,
    };

    enum class InteractionEvent
    {
		// Indicates that no significant changes have occurred in the trigger state and there are no active device inputs.
		None = 0,

		// Represents a triggering event that occurred after one or more frames.
		Triggered = BIT(0),    // InteractionEvent (None -> Triggered, Ongoing -> Triggered, Triggered -> Triggered)

		// Indicates that an event has initiated trigger evaluation. This event is fired first before Triggered.
		Started = BIT(1),    // InteractionEvent (None -> Ongoing, None -> Triggered)

		// Represents a continuing process of triggering. For example, Release a key while the key is clicked it would be Ongoing, as the key has not been triggerd yet
		Ongoing = BIT(2),    // InteractionEvent (Ongoing -> Ongoing)

		// Indicates that triggering has been canceled. For example, user did not double click key in a specific threshold;
		Canceled = BIT(3),    // InteractionEvent (Ongoing -> None)

		// Indicates that the trigger state transitioned from Triggered to None this frame, indicating the completion of triggering.
		Completed = BIT(4),    // InteractionEvent (Triggered -> None)
    };
	DEFINE_ENUM_CLASS_FLAGS(InteractionEvent);
    
	static inline InteractionEvent GetInteractionStateChangeEvent(InteractionState lastTriggerState, InteractionState newTriggerState)
	{
		switch (lastTriggerState)
		{
			case InteractionState::None:
				if (newTriggerState== InteractionState::Ongoing)
				{
					return InteractionEvent::Started;
				}
				else if (newTriggerState== InteractionState::Triggered)
				{
					return InteractionEvent::Started | InteractionEvent::Triggered;
				}
				break;
			case InteractionState::Ongoing:
				if (newTriggerState== InteractionState::None)
				{
					return InteractionEvent::Canceled;
				}
				else if (newTriggerState== InteractionState::Ongoing)
				{
					return InteractionEvent::Ongoing;
				}
				else if (newTriggerState== InteractionState::Triggered)
				{
					return InteractionEvent::Triggered;
				}
				break;
			case InteractionState::Triggered:
				if (newTriggerState== InteractionState::Triggered)
				{
					return InteractionEvent::Triggered;
				}
				else if (newTriggerState== InteractionState::Ongoing)
				{
					return InteractionEvent::Ongoing;
				}
				else if (newTriggerState== InteractionState::None)
				{
					return InteractionEvent::Completed;
				}
				break;
		}

		return InteractionEvent::None;
	}
	
}

namespace std 
{
	template <>
	struct hash<Proof::ElevatedInputKey>
	{
		std::size_t operator()(const Proof::ElevatedInputKey& key) const noexcept
		{
			return std::hash<std::string>{}(key.GetKeyName());
		}
	};
};