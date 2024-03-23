#include "Proofprch.h"
#include "InputTypes.h"
#include <unordered_map>

//https://github.com/EpicGames/UnrealEngine/blob/072300df18a94f18077ca20a14224b5d99fee872/Engine/Source/Runtime/InputCore/Private/InputCoreTypes.cpp
namespace Proof
{
	const ElevatedInputKey ElevatedInputKeys::Invalid(ElevatedInputKeyDeviceType::None,"Invalid");
	const ElevatedInputKey ElevatedInputKeys::MouseMoveX(ElevatedInputKeyDeviceType::Mouse,"MouseX", ElevatedInputKey::Axis1D | ElevatedInputKey::MouseButton);
	const ElevatedInputKey ElevatedInputKeys::MouseMoveY(ElevatedInputKeyDeviceType::Mouse,"MouseY", ElevatedInputKey::Axis1D | ElevatedInputKey::MouseButton);
	const ElevatedInputKey ElevatedInputKeys::MouseMoveAxis(ElevatedInputKeyDeviceType::Mouse,"Mouse2D", ElevatedInputKey::Axis2D);
	const ElevatedInputKey ElevatedInputKeys::MouseScrollUp(ElevatedInputKeyDeviceType::Mouse,"MouseScrollUp", ElevatedInputKey::AxisButton | ElevatedInputKey::MouseButton);
	const ElevatedInputKey ElevatedInputKeys::MouseScrollDown(ElevatedInputKeyDeviceType::Mouse,"MouseScrollDown", ElevatedInputKey::AxisButton | ElevatedInputKey::MouseButton);
	const ElevatedInputKey ElevatedInputKeys::MouseWheelAxis(ElevatedInputKeyDeviceType::Mouse,"MouseWheelAxis", ElevatedInputKey::Axis1D | ElevatedInputKey::MouseButton);

	const ElevatedInputKey ElevatedInputKeys::LeftMouseButton(ElevatedInputKeyDeviceType::Mouse, "LeftMouseButton", ElevatedInputKey::MouseButton);
	const ElevatedInputKey ElevatedInputKeys::RightMouseButton(ElevatedInputKeyDeviceType::Mouse, "RightMouseButton", ElevatedInputKey::MouseButton);
	const ElevatedInputKey ElevatedInputKeys::MiddleMouseButton(ElevatedInputKeyDeviceType::Mouse, "MiddleMouseButton", ElevatedInputKey::MouseButton);
	const ElevatedInputKey ElevatedInputKeys::ThumbMouseButton(ElevatedInputKeyDeviceType::Mouse, "ThumbMouseButton", ElevatedInputKey::MouseButton);
	const ElevatedInputKey ElevatedInputKeys::ThumbMouseButton2(ElevatedInputKeyDeviceType::Mouse, "ThumbMouseButton2", ElevatedInputKey::MouseButton);

	const ElevatedInputKey ElevatedInputKeys::BackSpace(ElevatedInputKeyDeviceType::KeyBoard, "BackSpace");
	const ElevatedInputKey ElevatedInputKeys::Tab(ElevatedInputKeyDeviceType::KeyBoard, "Tab");
	const ElevatedInputKey ElevatedInputKeys::Enter(ElevatedInputKeyDeviceType::KeyBoard, "Enter");
	const ElevatedInputKey ElevatedInputKeys::Pause(ElevatedInputKeyDeviceType::KeyBoard, "Pause");

	const ElevatedInputKey ElevatedInputKeys::CapsLock(ElevatedInputKeyDeviceType::KeyBoard, "CapsLock");
	const ElevatedInputKey ElevatedInputKeys::Escape(ElevatedInputKeyDeviceType::KeyBoard, "Escape");
	const ElevatedInputKey ElevatedInputKeys::SpaceBar(ElevatedInputKeyDeviceType::KeyBoard, "SpaceBar");
	const ElevatedInputKey ElevatedInputKeys::PageUp(ElevatedInputKeyDeviceType::KeyBoard, "PageUp");
	const ElevatedInputKey ElevatedInputKeys::PageDown(ElevatedInputKeyDeviceType::KeyBoard, "PageDown");
	const ElevatedInputKey ElevatedInputKeys::End(ElevatedInputKeyDeviceType::KeyBoard, "End");
	const ElevatedInputKey ElevatedInputKeys::Home(ElevatedInputKeyDeviceType::KeyBoard, "Home");

	const ElevatedInputKey ElevatedInputKeys::Left(ElevatedInputKeyDeviceType::KeyBoard, "Left");
	const ElevatedInputKey ElevatedInputKeys::Up(ElevatedInputKeyDeviceType::KeyBoard, "Up");
	const ElevatedInputKey ElevatedInputKeys::Right(ElevatedInputKeyDeviceType::KeyBoard, "Right");
	const ElevatedInputKey ElevatedInputKeys::Down(ElevatedInputKeyDeviceType::KeyBoard, "Down");

	const ElevatedInputKey ElevatedInputKeys::Insert(ElevatedInputKeyDeviceType::KeyBoard, "Insert");
	const ElevatedInputKey ElevatedInputKeys::Delete(ElevatedInputKeyDeviceType::KeyBoard, "Delete");

	const ElevatedInputKey ElevatedInputKeys::Zero(ElevatedInputKeyDeviceType::KeyBoard, "0");
	const ElevatedInputKey ElevatedInputKeys::One(ElevatedInputKeyDeviceType::KeyBoard, "1");
	const ElevatedInputKey ElevatedInputKeys::Two(ElevatedInputKeyDeviceType::KeyBoard, "2");
	const ElevatedInputKey ElevatedInputKeys::Three(ElevatedInputKeyDeviceType::KeyBoard, "3");
	const ElevatedInputKey ElevatedInputKeys::Four(ElevatedInputKeyDeviceType::KeyBoard, "4");
	const ElevatedInputKey ElevatedInputKeys::Five(ElevatedInputKeyDeviceType::KeyBoard, "5");
	const ElevatedInputKey ElevatedInputKeys::Six(ElevatedInputKeyDeviceType::KeyBoard, "6");
	const ElevatedInputKey ElevatedInputKeys::Seven(ElevatedInputKeyDeviceType::KeyBoard, "7");
	const ElevatedInputKey ElevatedInputKeys::Eight(ElevatedInputKeyDeviceType::KeyBoard, "8");
	const ElevatedInputKey ElevatedInputKeys::Nine(ElevatedInputKeyDeviceType::KeyBoard, "9");

	const ElevatedInputKey ElevatedInputKeys::A(ElevatedInputKeyDeviceType::KeyBoard, "A");
	const ElevatedInputKey ElevatedInputKeys::B(ElevatedInputKeyDeviceType::KeyBoard, "B");
	const ElevatedInputKey ElevatedInputKeys::C(ElevatedInputKeyDeviceType::KeyBoard, "C");
	const ElevatedInputKey ElevatedInputKeys::D(ElevatedInputKeyDeviceType::KeyBoard, "D");
	const ElevatedInputKey ElevatedInputKeys::E(ElevatedInputKeyDeviceType::KeyBoard, "E");
	const ElevatedInputKey ElevatedInputKeys::F(ElevatedInputKeyDeviceType::KeyBoard, "F");
	const ElevatedInputKey ElevatedInputKeys::G(ElevatedInputKeyDeviceType::KeyBoard, "G");
	const ElevatedInputKey ElevatedInputKeys::H(ElevatedInputKeyDeviceType::KeyBoard, "H");
	const ElevatedInputKey ElevatedInputKeys::I(ElevatedInputKeyDeviceType::KeyBoard, "I");
	const ElevatedInputKey ElevatedInputKeys::J(ElevatedInputKeyDeviceType::KeyBoard, "J");
	const ElevatedInputKey ElevatedInputKeys::K(ElevatedInputKeyDeviceType::KeyBoard, "K");
	const ElevatedInputKey ElevatedInputKeys::L(ElevatedInputKeyDeviceType::KeyBoard, "L");
	const ElevatedInputKey ElevatedInputKeys::M(ElevatedInputKeyDeviceType::KeyBoard, "M");
	const ElevatedInputKey ElevatedInputKeys::N(ElevatedInputKeyDeviceType::KeyBoard, "N");
	const ElevatedInputKey ElevatedInputKeys::O(ElevatedInputKeyDeviceType::KeyBoard, "O");
	const ElevatedInputKey ElevatedInputKeys::P(ElevatedInputKeyDeviceType::KeyBoard, "P");
	const ElevatedInputKey ElevatedInputKeys::Q(ElevatedInputKeyDeviceType::KeyBoard, "Q");
	const ElevatedInputKey ElevatedInputKeys::R(ElevatedInputKeyDeviceType::KeyBoard, "R");
	const ElevatedInputKey ElevatedInputKeys::S(ElevatedInputKeyDeviceType::KeyBoard, "S");
	const ElevatedInputKey ElevatedInputKeys::T(ElevatedInputKeyDeviceType::KeyBoard, "T");
	const ElevatedInputKey ElevatedInputKeys::U(ElevatedInputKeyDeviceType::KeyBoard, "U");
	const ElevatedInputKey ElevatedInputKeys::V(ElevatedInputKeyDeviceType::KeyBoard, "V");
	const ElevatedInputKey ElevatedInputKeys::W(ElevatedInputKeyDeviceType::KeyBoard, "W");
	const ElevatedInputKey ElevatedInputKeys::X(ElevatedInputKeyDeviceType::KeyBoard, "X");
	const ElevatedInputKey ElevatedInputKeys::Y(ElevatedInputKeyDeviceType::KeyBoard, "Y");
	const ElevatedInputKey ElevatedInputKeys::Z(ElevatedInputKeyDeviceType::KeyBoard, "Z");

	const ElevatedInputKey ElevatedInputKeys::NumPadZero(ElevatedInputKeyDeviceType::KeyBoard, "NumPad 0");
	const ElevatedInputKey ElevatedInputKeys::NumPadOne(ElevatedInputKeyDeviceType::KeyBoard, "NumPad 1");
	const ElevatedInputKey ElevatedInputKeys::NumPadTwo(ElevatedInputKeyDeviceType::KeyBoard, "NumPad 2");
	const ElevatedInputKey ElevatedInputKeys::NumPadThree(ElevatedInputKeyDeviceType::KeyBoard, "NumPad 3");
	const ElevatedInputKey ElevatedInputKeys::NumPadFour(ElevatedInputKeyDeviceType::KeyBoard, "NumPad 4");
	const ElevatedInputKey ElevatedInputKeys::NumPadFive(ElevatedInputKeyDeviceType::KeyBoard, "NumPad 5");
	const ElevatedInputKey ElevatedInputKeys::NumPadSix(ElevatedInputKeyDeviceType::KeyBoard, "NumPad 6");
	const ElevatedInputKey ElevatedInputKeys::NumPadSeven(ElevatedInputKeyDeviceType::KeyBoard, "NumPad 7");
	const ElevatedInputKey ElevatedInputKeys::NumPadEight(ElevatedInputKeyDeviceType::KeyBoard, "NumPad 8");
	const ElevatedInputKey ElevatedInputKeys::NumPadNine(ElevatedInputKeyDeviceType::KeyBoard, "NumPad 9");

	const ElevatedInputKey ElevatedInputKeys::NumPadMultiply(ElevatedInputKeyDeviceType::KeyBoard, "NumPad Multiply");
	const ElevatedInputKey ElevatedInputKeys::NumPadAdd(ElevatedInputKeyDeviceType::KeyBoard, "NumPad Add");
	const ElevatedInputKey ElevatedInputKeys::NumPadSubtract(ElevatedInputKeyDeviceType::KeyBoard, "NumPad Subtract");
	const ElevatedInputKey ElevatedInputKeys::NumPadDecimal(ElevatedInputKeyDeviceType::KeyBoard, "NumPad Decimal");
	const ElevatedInputKey ElevatedInputKeys::NumPadDivide(ElevatedInputKeyDeviceType::KeyBoard, "NumPad Divide");

	const ElevatedInputKey ElevatedInputKeys::F1(ElevatedInputKeyDeviceType::KeyBoard, "F1");
	const ElevatedInputKey ElevatedInputKeys::F2(ElevatedInputKeyDeviceType::KeyBoard, "F2");
	const ElevatedInputKey ElevatedInputKeys::F3(ElevatedInputKeyDeviceType::KeyBoard, "F3");
	const ElevatedInputKey ElevatedInputKeys::F4(ElevatedInputKeyDeviceType::KeyBoard, "F4");
	const ElevatedInputKey ElevatedInputKeys::F5(ElevatedInputKeyDeviceType::KeyBoard, "F5");
	const ElevatedInputKey ElevatedInputKeys::F6(ElevatedInputKeyDeviceType::KeyBoard, "F6");
	const ElevatedInputKey ElevatedInputKeys::F7(ElevatedInputKeyDeviceType::KeyBoard, "F7");
	const ElevatedInputKey ElevatedInputKeys::F8(ElevatedInputKeyDeviceType::KeyBoard, "F8");
	const ElevatedInputKey ElevatedInputKeys::F9(ElevatedInputKeyDeviceType::KeyBoard, "F9");
	const ElevatedInputKey ElevatedInputKeys::F10(ElevatedInputKeyDeviceType::KeyBoard, "F10");
	const ElevatedInputKey ElevatedInputKeys::F11(ElevatedInputKeyDeviceType::KeyBoard, "F11");
	const ElevatedInputKey ElevatedInputKeys::F12(ElevatedInputKeyDeviceType::KeyBoard, "F12");

	const ElevatedInputKey ElevatedInputKeys::NumLock(ElevatedInputKeyDeviceType::KeyBoard, "NumLock");
	const ElevatedInputKey ElevatedInputKeys::ScrollLock(ElevatedInputKeyDeviceType::KeyBoard, "ScrollLock");
	const ElevatedInputKey ElevatedInputKeys::Menu(ElevatedInputKeyDeviceType::KeyBoard, "Menu");

	const ElevatedInputKey ElevatedInputKeys::LeftShift(ElevatedInputKeyDeviceType::KeyBoard, "LeftShift",ElevatedInputKey::ModifierKey);
	const ElevatedInputKey ElevatedInputKeys::RightShift(ElevatedInputKeyDeviceType::KeyBoard, "RightShift", ElevatedInputKey::ModifierKey);
	const ElevatedInputKey ElevatedInputKeys::LeftControl(ElevatedInputKeyDeviceType::KeyBoard, "LeftControl", ElevatedInputKey::ModifierKey);
	const ElevatedInputKey ElevatedInputKeys::RightControl(ElevatedInputKeyDeviceType::KeyBoard, "RightControl", ElevatedInputKey::ModifierKey);
	const ElevatedInputKey ElevatedInputKeys::LeftAlt(ElevatedInputKeyDeviceType::KeyBoard, "LeftAlt", ElevatedInputKey::ModifierKey);
	const ElevatedInputKey ElevatedInputKeys::RightAlt(ElevatedInputKeyDeviceType::KeyBoard, "RightAlt", ElevatedInputKey::ModifierKey);
	const ElevatedInputKey ElevatedInputKeys::LeftCommand(ElevatedInputKeyDeviceType::KeyBoard, "LeftCommand", ElevatedInputKey::ModifierKey);
	const ElevatedInputKey ElevatedInputKeys::RightCommand(ElevatedInputKeyDeviceType::KeyBoard, "RightCommand", ElevatedInputKey::ModifierKey);

	const ElevatedInputKey ElevatedInputKeys::Semicolon(ElevatedInputKeyDeviceType::KeyBoard, ";");
	const ElevatedInputKey ElevatedInputKeys::Equals(ElevatedInputKeyDeviceType::KeyBoard, "=");
	const ElevatedInputKey ElevatedInputKeys::Comma(ElevatedInputKeyDeviceType::KeyBoard, ",");
	const ElevatedInputKey ElevatedInputKeys::Underscore(ElevatedInputKeyDeviceType::KeyBoard, "_");
	const ElevatedInputKey ElevatedInputKeys::Hyphen(ElevatedInputKeyDeviceType::KeyBoard, "-");
	const ElevatedInputKey ElevatedInputKeys::Period(ElevatedInputKeyDeviceType::KeyBoard, ".");
	const ElevatedInputKey ElevatedInputKeys::Slash(ElevatedInputKeyDeviceType::KeyBoard, "/");
	const ElevatedInputKey ElevatedInputKeys::GraveAccent(ElevatedInputKeyDeviceType::KeyBoard, "`");
	const ElevatedInputKey ElevatedInputKeys::LeftBracket(ElevatedInputKeyDeviceType::KeyBoard, "[");
	const ElevatedInputKey ElevatedInputKeys::Backslash(ElevatedInputKeyDeviceType::KeyBoard, "\\");
	const ElevatedInputKey ElevatedInputKeys::RightBracket(ElevatedInputKeyDeviceType::KeyBoard, "]");
	const ElevatedInputKey ElevatedInputKeys::Apostrophe(ElevatedInputKeyDeviceType::KeyBoard, "'");

	const ElevatedInputKey ElevatedInputKeys::Ampersand(ElevatedInputKeyDeviceType::KeyBoard, "&");
	const ElevatedInputKey ElevatedInputKeys::Asterix(ElevatedInputKeyDeviceType::KeyBoard, "*");
	const ElevatedInputKey ElevatedInputKeys::Caret(ElevatedInputKeyDeviceType::KeyBoard, "^");
	const ElevatedInputKey ElevatedInputKeys::Colon(ElevatedInputKeyDeviceType::KeyBoard, ":");
	const ElevatedInputKey ElevatedInputKeys::Dollar(ElevatedInputKeyDeviceType::KeyBoard, "$");
	const ElevatedInputKey ElevatedInputKeys::Exclamation(ElevatedInputKeyDeviceType::KeyBoard, "!");
	const ElevatedInputKey ElevatedInputKeys::LeftParantheses(ElevatedInputKeyDeviceType::KeyBoard, "(");
	const ElevatedInputKey ElevatedInputKeys::RightParantheses(ElevatedInputKeyDeviceType::KeyBoard, ")");
	const ElevatedInputKey ElevatedInputKeys::Quote(ElevatedInputKeyDeviceType::KeyBoard, "\"");

	const ElevatedInputKey ElevatedInputKeys::A_AccentGrave(ElevatedInputKeyDeviceType::KeyBoard, std::string(1,char(224)));
	const ElevatedInputKey ElevatedInputKeys::E_AccentGrave(ElevatedInputKeyDeviceType::KeyBoard, std::string(1, char(232)));
	const ElevatedInputKey ElevatedInputKeys::E_AccentAigu(ElevatedInputKeyDeviceType::KeyBoard, std::string(1, char(233)));
	const ElevatedInputKey ElevatedInputKeys::C_Cedille(ElevatedInputKeyDeviceType::KeyBoard, std::string(1, char(231)));
	const ElevatedInputKey ElevatedInputKeys::Section(ElevatedInputKeyDeviceType::KeyBoard, std::string(1, char(167)));

	const ElevatedInputKey ElevatedInputKeys::ControllerLeftStick2D(ElevatedInputKeyDeviceType::Controller, "ControllerLeftStick2D",ElevatedInputKey::Axis1D | ElevatedInputKey::ControllerButton);
	const ElevatedInputKey ElevatedInputKeys::ControllerRightStick2D(ElevatedInputKeyDeviceType::Controller, "ControllerRightStick2D", ElevatedInputKey::Axis1D | ElevatedInputKey::ControllerButton);
	const ElevatedInputKey ElevatedInputKeys::ControllerLeftStickX(ElevatedInputKeyDeviceType::Controller, "ControllerLeftStickX", ElevatedInputKey::Axis1D | ElevatedInputKey::ControllerButton);
	const ElevatedInputKey ElevatedInputKeys::ControllerLeftStickY(ElevatedInputKeyDeviceType::Controller, "ControllerLeftStickY", ElevatedInputKey::Axis1D | ElevatedInputKey::ControllerButton);
	const ElevatedInputKey ElevatedInputKeys::ControllerRightStickX(ElevatedInputKeyDeviceType::Controller, "ControllerRightStickX", ElevatedInputKey::Axis1D | ElevatedInputKey::ControllerButton);
	const ElevatedInputKey ElevatedInputKeys::ControllerRightStickY(ElevatedInputKeyDeviceType::Controller, "ControllerRightStickY", ElevatedInputKey::Axis1D | ElevatedInputKey::ControllerButton);
	const ElevatedInputKey ElevatedInputKeys::ControllerLeftTriggerAxis(ElevatedInputKeyDeviceType::Controller, "ControllerLeftTriggerAxis", ElevatedInputKey::Axis1D | ElevatedInputKey::ControllerButton);
	const ElevatedInputKey ElevatedInputKeys::ControllerRightTriggerAxis(ElevatedInputKeyDeviceType::Controller, "ControllerRightTriggerAxis", ElevatedInputKey::Axis1D | ElevatedInputKey::ControllerButton);

	const ElevatedInputKey ElevatedInputKeys::ControllerLeftThumbstick(ElevatedInputKeyDeviceType::Controller, "ControllerLeftThumbstick", ElevatedInputKey::ControllerButton | ElevatedInputKey::Axis2D);
	const ElevatedInputKey ElevatedInputKeys::ControllerRightThumbstick(ElevatedInputKeyDeviceType::Controller, "ControllerRightThumbstick", ElevatedInputKey::ControllerButton | ElevatedInputKey::Axis2D);

	const ElevatedInputKey ElevatedInputKeys::ControllerBack(ElevatedInputKeyDeviceType::Controller, "ControllerBack", ElevatedInputKey::ControllerButton);
	const ElevatedInputKey ElevatedInputKeys::ControllerStart(ElevatedInputKeyDeviceType::Controller, "ControllerStart", ElevatedInputKey::ControllerButton);
	const ElevatedInputKey ElevatedInputKeys::ControllerGuide(ElevatedInputKeyDeviceType::Controller, "ControllerGuide", ElevatedInputKey::ControllerButton);
	const ElevatedInputKey ElevatedInputKeys::ControllerSpecialRight(ElevatedInputKeyDeviceType::Controller, "ControllerSpecialRight", ElevatedInputKey::ControllerButton);
	const ElevatedInputKey ElevatedInputKeys::ControllerFaceButtonBottom(ElevatedInputKeyDeviceType::Controller, "ControllerFaceButtonBottom", ElevatedInputKey::ControllerButton);
	const ElevatedInputKey ElevatedInputKeys::ControllerFaceButtonRight(ElevatedInputKeyDeviceType::Controller, "ControllerFaceButtonRight", ElevatedInputKey::ControllerButton);
	const ElevatedInputKey ElevatedInputKeys::ControllerFaceButtonLeft(ElevatedInputKeyDeviceType::Controller, "ControllerFaceButtonLeft", ElevatedInputKey::ControllerButton);
	const ElevatedInputKey ElevatedInputKeys::ControllerFaceButtonTop(ElevatedInputKeyDeviceType::Controller, "ControllerFaceButtonTop", ElevatedInputKey::ControllerButton);

	const ElevatedInputKey ElevatedInputKeys::ControllerLeftShoulder(ElevatedInputKeyDeviceType::Controller, "ControllerLeftShoulder", ElevatedInputKey::ControllerButton);
	const ElevatedInputKey ElevatedInputKeys::ControllerRightShoulder(ElevatedInputKeyDeviceType::Controller, "ControllerRightShoulder", ElevatedInputKey::ControllerButton);
	const ElevatedInputKey ElevatedInputKeys::ControllerLeftTrigger(ElevatedInputKeyDeviceType::Controller, "ControllerLeftTrigger", ElevatedInputKey::ControllerButton | ElevatedInputKey::AxisButton);
	const ElevatedInputKey ElevatedInputKeys::ControllerRightTrigger(ElevatedInputKeyDeviceType::Controller, "ControllerRightTrigger", ElevatedInputKey::ControllerButton | ElevatedInputKey::AxisButton);

	const ElevatedInputKey ElevatedInputKeys::ControllerDPadUp(ElevatedInputKeyDeviceType::Controller, "ControllerDPadUp", ElevatedInputKey::ControllerButton);
	const ElevatedInputKey ElevatedInputKeys::ControllerDPadDown(ElevatedInputKeyDeviceType::Controller, "ControllerDPadDown", ElevatedInputKey::ControllerButton);
	const ElevatedInputKey ElevatedInputKeys::ControllerDPadRight(ElevatedInputKeyDeviceType::Controller, "ControllerDPadRight", ElevatedInputKey::ControllerButton);
	const ElevatedInputKey ElevatedInputKeys::ControllerDPadLeft(ElevatedInputKeyDeviceType::Controller, "ControllerDPadLeft", ElevatedInputKey::ControllerButton);
	const ElevatedInputKey ElevatedInputKeys::ControllerDPad(ElevatedInputKeyDeviceType::Controller, "ControllerDPad", ElevatedInputKey::ControllerButton | ElevatedInputKey::Axis2D);

	const ElevatedInputKey ElevatedInputKeys::ControllerLeftStickUp(ElevatedInputKeyDeviceType::Controller, "ControllerLeftStickUp", ElevatedInputKey::ControllerButton | ElevatedInputKey::AxisButton);
	const ElevatedInputKey ElevatedInputKeys::ControllerLeftStickDown(ElevatedInputKeyDeviceType::Controller, "ControllerLeftStickDown", ElevatedInputKey::ControllerButton | ElevatedInputKey::AxisButton);
	const ElevatedInputKey ElevatedInputKeys::ControllerLeftStickRight(ElevatedInputKeyDeviceType::Controller, "ControllerLeftStickRight", ElevatedInputKey::ControllerButton | ElevatedInputKey::AxisButton);
	const ElevatedInputKey ElevatedInputKeys::ControllerLeftStickLeft(ElevatedInputKeyDeviceType::Controller, "ControllerLeftStickLeft", ElevatedInputKey::ControllerButton | ElevatedInputKey::AxisButton);

	const ElevatedInputKey ElevatedInputKeys::ControllerRightStickUp(ElevatedInputKeyDeviceType::Controller, "ControllerRightStickUp", ElevatedInputKey::ControllerButton | ElevatedInputKey::AxisButton);
	const ElevatedInputKey ElevatedInputKeys::ControllerRightStickDown(ElevatedInputKeyDeviceType::Controller, "ControllerRightStickDown", ElevatedInputKey::ControllerButton | ElevatedInputKey::AxisButton);
	const ElevatedInputKey ElevatedInputKeys::ControllerRightStickRight(ElevatedInputKeyDeviceType::Controller, "ControllerRightStickRight", ElevatedInputKey::ControllerButton | ElevatedInputKey::AxisButton);
	const ElevatedInputKey ElevatedInputKeys::ControllerRightStickLeft(ElevatedInputKeyDeviceType::Controller, "ControllerRightStickLeft", ElevatedInputKey::ControllerButton | ElevatedInputKey::AxisButton);

	struct InputKeysData
	{
		std::unordered_map<KeyBoardKey, std::string> KeyBoardKeys;
		std::unordered_map<std::string, const ElevatedInputKey*> Keys;
	};
	static InputKeysData* s_InputKeysData;

	static void AddKeyBoardKey(KeyBoardKey key, const ElevatedInputKey* inputKey)
	{
		PF_CORE_ASSERT(!s_InputKeysData->KeyBoardKeys.contains(key), fmt::format("Contains {} Already", inputKey->GetKeyName()));
		PF_CORE_ASSERT(!s_InputKeysData->Keys.contains(inputKey->GetKeyName()));
		s_InputKeysData->KeyBoardKeys[key] = { inputKey->GetKeyName() };
		s_InputKeysData->Keys[inputKey->GetKeyName()] = { inputKey };
	}

	static void AddKey(const ElevatedInputKey* inputKey)
	{
		PF_CORE_ASSERT(!s_InputKeysData->Keys.contains(inputKey->GetKeyName()), fmt::format("Contains {} Already", inputKey->GetKeyName()));
		s_InputKeysData->Keys[inputKey->GetKeyName()] = { inputKey };
	}

	static void AddPairedKey(const ElevatedInputKey* key, const ElevatedInputKey* keyXAxis, const ElevatedInputKey* keyYAxis)
	{
		PF_CORE_ASSERT(!s_InputKeysData->Keys.contains(key->GetKeyName()),fmt::format("Contains {} Already",key->GetKeyName()));

		PF_CORE_ASSERT(key->IsAxis2D(), fmt::format("Paired Key must be a Paired Axis2D {}", key->GetKeyName()));

		PF_CORE_ASSERT(keyXAxis->IsAxis1D(), fmt::format("Axis Key must be a Axis1D {}", keyXAxis->GetKeyName()));
		PF_CORE_ASSERT(keyYAxis->IsAxis1D(), fmt::format("Axis Key must be a Axis1D {}", keyYAxis->GetKeyName()));

		s_InputKeysData->Keys[key->GetKeyName()] = { key };

		// Temporarily remove const-ness to modify the objects
		ElevatedInputKey* nonConstKeyXAxis = const_cast<ElevatedInputKey*>(keyXAxis);
		ElevatedInputKey* nonConstKeyYAxis = const_cast<ElevatedInputKey*>(keyYAxis);

		// Modify the objects
		nonConstKeyXAxis->m_AxisParing = ElevatedAxisPairing::X;
		nonConstKeyYAxis->m_AxisParing = ElevatedAxisPairing::Y;


		nonConstKeyXAxis->m_AxisParingKey = key;
		nonConstKeyYAxis->m_AxisParingKey = key;
	}
	ElevatedInputKey ElevatedInputKeys::GetKeyBoardKey(KeyBoardKey key)
	{
		if (s_InputKeysData->KeyBoardKeys.contains(key))
		{
			auto keyString = s_InputKeysData->KeyBoardKeys.at(key);

			const ElevatedInputKey* elevatedKey = s_InputKeysData->Keys.at(keyString);
			return *elevatedKey;
		}
		return ElevatedInputKeys::Invalid;
	}
	
	void ElevatedInputKeys::Init()
	{
		ScopeTimer timer(__FUNCTION__);
		PF_PROFILE_FUNC();
		//https://github.com/EpicGames/UnrealEngine/blob/072300df18a94f18077ca20a14224b5d99fee872/Engine/Source/Runtime/InputCore/Private/InputCoreTypes.cpp#L1007
		s_InputKeysData = new InputKeysData();

		AddKey(&Invalid);

		//mouse
		AddKey(&MouseMoveX);
		AddKey(&MouseMoveY);
		AddPairedKey(&MouseMoveAxis, &MouseMoveX, &MouseMoveY);
		AddKey(&MouseScrollUp);
		AddKey(&MouseScrollDown);
		AddKey(&MouseWheelAxis);

		AddKey(&LeftMouseButton);
		AddKey(&RightMouseButton);
		AddKey(&MiddleMouseButton);
		AddKey(&ThumbMouseButton2);

		//keybaord
		AddKeyBoardKey(KeyBoardKey::Tab, &Tab);
		AddKeyBoardKey(KeyBoardKey::Enter, &Enter);
		AddKeyBoardKey(KeyBoardKey::Pause, &Pause);
		AddKeyBoardKey(KeyBoardKey::Backspace, &BackSpace);
		AddKeyBoardKey(KeyBoardKey::CapsLock, &CapsLock);
		AddKeyBoardKey(KeyBoardKey::Escape, &Escape);
		AddKeyBoardKey(KeyBoardKey::Space, &SpaceBar);
		AddKeyBoardKey(KeyBoardKey::PageUp, &PageUp);
		AddKeyBoardKey(KeyBoardKey::PageDown, &PageDown);
		AddKeyBoardKey(KeyBoardKey::End, &End);
		AddKeyBoardKey(KeyBoardKey::Home, &Home);

		AddKeyBoardKey(KeyBoardKey::LeftArrow, &Left);
		AddKeyBoardKey(KeyBoardKey::UpArrow, &Up);
		AddKeyBoardKey(KeyBoardKey::RightArrow, &Right);
		AddKeyBoardKey(KeyBoardKey::DownArrow, &Down);

		AddKeyBoardKey(KeyBoardKey::Insert, &Insert);
		AddKeyBoardKey(KeyBoardKey::Delete, &Delete);

		AddKeyBoardKey(KeyBoardKey::Num0, &Zero);
		AddKeyBoardKey(KeyBoardKey::Num1, &One);
		AddKeyBoardKey(KeyBoardKey::Num2, &Two);
		AddKeyBoardKey(KeyBoardKey::Num3, &Three);
		AddKeyBoardKey(KeyBoardKey::Num4, &Four);
		AddKeyBoardKey(KeyBoardKey::Num5, &Five);
		AddKeyBoardKey(KeyBoardKey::Num6, &Six);
		AddKeyBoardKey(KeyBoardKey::Num7, &Seven);
		AddKeyBoardKey(KeyBoardKey::Num8, &Eight);
		AddKeyBoardKey(KeyBoardKey::Num9, &Nine);

		AddKeyBoardKey(KeyBoardKey::A, &A);
		AddKeyBoardKey(KeyBoardKey::B, &B);
		AddKeyBoardKey(KeyBoardKey::C, &C);
		AddKeyBoardKey(KeyBoardKey::D, &D);
		AddKeyBoardKey(KeyBoardKey::E, &E);
		AddKeyBoardKey(KeyBoardKey::F, &F);
		AddKeyBoardKey(KeyBoardKey::G, &G);
		AddKeyBoardKey(KeyBoardKey::H, &H);
		AddKeyBoardKey(KeyBoardKey::I, &I);
		AddKeyBoardKey(KeyBoardKey::J, &J);
		AddKeyBoardKey(KeyBoardKey::K, &K);
		AddKeyBoardKey(KeyBoardKey::L, &L);
		AddKeyBoardKey(KeyBoardKey::M, &M);
		AddKeyBoardKey(KeyBoardKey::N, &N);
		AddKeyBoardKey(KeyBoardKey::O, &O);
		AddKeyBoardKey(KeyBoardKey::P, &P);
		AddKeyBoardKey(KeyBoardKey::Q, &Q);
		AddKeyBoardKey(KeyBoardKey::R, &R);
		AddKeyBoardKey(KeyBoardKey::S, &S);
		AddKeyBoardKey(KeyBoardKey::T, &T);
		AddKeyBoardKey(KeyBoardKey::U, &U);
		AddKeyBoardKey(KeyBoardKey::V, &V);
		AddKeyBoardKey(KeyBoardKey::W, &W);
		AddKeyBoardKey(KeyBoardKey::X, &X);
		AddKeyBoardKey(KeyBoardKey::Y, &Y);
		AddKeyBoardKey(KeyBoardKey::Z, &Z);

		AddKeyBoardKey(KeyBoardKey::NumPad0, &NumPadZero);
		AddKeyBoardKey(KeyBoardKey::NumPad1, &NumPadOne);
		AddKeyBoardKey(KeyBoardKey::NumPad2, &NumPadTwo);
		AddKeyBoardKey(KeyBoardKey::NumPad3, &NumPadThree);
		AddKeyBoardKey(KeyBoardKey::NumPad4, &NumPadFour);
		AddKeyBoardKey(KeyBoardKey::NumPad5, &NumPadFive);
		AddKeyBoardKey(KeyBoardKey::NumPad6, &NumPadSix);
		AddKeyBoardKey(KeyBoardKey::NumPad7, &NumPadSeven);
		AddKeyBoardKey(KeyBoardKey::NumPad8, &NumPadEight);
		AddKeyBoardKey(KeyBoardKey::NumPad9, &NumPadNine);

		AddKeyBoardKey(KeyBoardKey::NumPadMultiply, &NumPadMultiply);
		AddKeyBoardKey(KeyBoardKey::NumPadAdd, &NumPadAdd);
		AddKeyBoardKey(KeyBoardKey::NumPadSubtract, &NumPadSubtract);
		AddKeyBoardKey(KeyBoardKey::NumPadDecimal, &NumPadDecimal);
		AddKeyBoardKey(KeyBoardKey::NumPadDivide, &NumPadDivide);

		AddKeyBoardKey(KeyBoardKey::F1, &F1);
		AddKeyBoardKey(KeyBoardKey::F2, &F2);
		AddKeyBoardKey(KeyBoardKey::F3, &F3);
		AddKeyBoardKey(KeyBoardKey::F4, &F4);
		AddKeyBoardKey(KeyBoardKey::F5, &F5);
		AddKeyBoardKey(KeyBoardKey::F6, &F6);
		AddKeyBoardKey(KeyBoardKey::F7, &F7);
		AddKeyBoardKey(KeyBoardKey::F8, &F8);
		AddKeyBoardKey(KeyBoardKey::F9, &F9);
		AddKeyBoardKey(KeyBoardKey::F10, &F10);
		AddKeyBoardKey(KeyBoardKey::F11, &F11);
		AddKeyBoardKey(KeyBoardKey::F12, &F12);

		AddKeyBoardKey(KeyBoardKey::NumLock, &NumLock);
		AddKeyBoardKey(KeyBoardKey::ScrollLock, &ScrollLock);
		AddKeyBoardKey(KeyBoardKey::Menu, &Menu);

		AddKeyBoardKey(KeyBoardKey::LeftShift, &LeftShift);
		AddKeyBoardKey(KeyBoardKey::RightShift, &RightShift);
		AddKeyBoardKey(KeyBoardKey::LeftControl, &LeftControl);
		AddKeyBoardKey(KeyBoardKey::RightControl, &RightControl);
		AddKeyBoardKey(KeyBoardKey::LeftAlt, &LeftAlt);
		AddKeyBoardKey(KeyBoardKey::RightAlt, &RightAlt);
		AddKeyBoardKey(KeyBoardKey::LeftSuper, &LeftCommand);
		AddKeyBoardKey(KeyBoardKey::RightSuper, &RightCommand);

		AddKeyBoardKey(KeyBoardKey::Semicolon, &Semicolon);
		AddKeyBoardKey(KeyBoardKey::Equal, &Equals);
		AddKeyBoardKey(KeyBoardKey::Comma, &Comma);
		AddKeyBoardKey(KeyBoardKey::Minus, &Hyphen);
		AddKeyBoardKey(KeyBoardKey::Period, &Period);
		AddKeyBoardKey(KeyBoardKey::Slash, &Slash);
		AddKeyBoardKey(KeyBoardKey::GraveAccent, &GraveAccent);
		AddKeyBoardKey(KeyBoardKey::LeftBracket, &LeftBracket);
		AddKeyBoardKey(KeyBoardKey::Backslash, &Backslash);
		AddKeyBoardKey(KeyBoardKey::RightBracket, &RightBracket);
		AddKeyBoardKey(KeyBoardKey::Apostrophe, &Apostrophe);

		AddKey(&Underscore);
		AddKey(&Ampersand);
		AddKey(&Asterix);
		AddKey(&Caret);
		AddKey(&Colon);
		AddKey(&Dollar);
		AddKey(&Exclamation);
		AddKey(&LeftParantheses);
		AddKey(&RightParantheses);
		AddKey(&Quote);

		AddKey(&A_AccentGrave);
		AddKey(&E_AccentGrave);
		AddKey(&E_AccentAigu);
		AddKey(&C_Cedille);
		AddKey(&Section);
	}
	void ElevatedInputKeys::ShutDown()
	{
		delete s_InputKeysData;
		s_InputKeysData = nullptr;
	}
}