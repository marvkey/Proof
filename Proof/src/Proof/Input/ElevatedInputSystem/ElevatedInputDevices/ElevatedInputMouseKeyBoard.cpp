#include "Proofprch.h"
#include "ElevatedInputMouseKeyBoard.h"
#include "Proof/Events/MouseEvent.h"
#include "Proof/Events/KeyEvent.h"
namespace Proof
{
	
	namespace Utils
	{
		ElevatedInputKey ConvertMouseButton(MouseButton button)
		{

			switch (button)
			{
				case Proof::MouseButton::None:
					return ElevatedInputKeys::Invalid;
				case Proof::MouseButton::ButtonLeft:
					return ElevatedInputKeys::LeftMouseButton;
				case Proof::MouseButton::ButtonRight:
					return ElevatedInputKeys::RightMouseButton;
				case Proof::MouseButton::ButtonMiddle:
					return ElevatedInputKeys::MiddleMouseButton;
				case Proof::MouseButton::Button3:
					return ElevatedInputKeys::ThumbMouseButton;
				case Proof::MouseButton::Button4:
					return ElevatedInputKeys::ThumbMouseButton2;
				case Proof::MouseButton::Button5:
					PF_CORE_ASSERT("No Support yet");
					break;
				case Proof::MouseButton::Button6:
					PF_CORE_ASSERT("No Support yet");
					break;
				case Proof::MouseButton::Button7:
					PF_CORE_ASSERT("No Support yet");
					break;
			}

			return ElevatedInputKeys::Invalid;
		}
	}

	void ElevatedInputDeviceMouseKeyboard::OnUpdate(float deltaTime)
	{
		ProcessAccumulatedPointerInput(deltaTime);
	}

	void ElevatedInputDeviceMouseKeyboard::OnEvent(Event& e)
	{
		PF_PROFILE_FUNC();
		//https://github.com/EpicGames/UnrealEngine/blob/072300df18a94f18077ca20a14224b5d99fee872/Engine/Plugins/EnhancedInput/Source/InputEditor/Private/EnhancedInputEditorProcessor.cpp#L119
		EventDispatcher dispatcher(e);

		Count< ElevatedInputDeviceMouseKeyboard> inputDevice = this;
		dispatcher.Dispatch<MouseButtonClickedEvent>([&](MouseButtonClickedEvent& mouseButtonClickedEvent)
			{
				ElevatedInputKey inputkey = Utils::ConvertMouseButton(mouseButtonClickedEvent.GetButton());
				ElevatedInputKeyParams params{ inputkey,ElevatedKeyEventType::Clicked, inputDevice, 1.0, 0 };
				return InvokeStep(params);
			});
/*
		dispatcher.Dispatch<MouseButtonPressedEvent>([&](MouseButtonPressedEvent& mouseButtonClickedEvent)
			{
				ElevatedInputKey inputkey = Utils::ConvertMouseButton(mouseButtonClickedEvent.GetButton());
				ElevatedInputKeyParams params{ inputkey,ElevatedKeyEventType::Pressed, inputDevice };
				return InvokeStep(params);
			});
*/
		dispatcher.Dispatch<MouseButtonReleasedEvent>([&](MouseButtonReleasedEvent& mouseButtonClickedEvent)
			{
				ElevatedInputKey inputkey = Utils::ConvertMouseButton(mouseButtonClickedEvent.GetButton());
				ElevatedInputKeyParams params{ inputkey,ElevatedKeyEventType::Released, inputDevice,0.0, 0 };
				return InvokeStep(params);
			});
		/*
		dispatcher.Dispatch<MouseButtonDoubleClickEvent>([&](MouseButtonDoubleClickEvent& mouseButtonClickedEvent)
			{
				ElevatedInputKey inputkey = Utils::ConvertMouseButton(mouseButtonClickedEvent.GetButton());
				ElevatedInputKeyParams params{ inputkey,ElevatedKeyEventType::Double, inputDevice, 1.0,0 };
				return InvokeStep(params);
			});
*/
		dispatcher.Dispatch<MouseMoveEvent>([&](MouseMoveEvent& mouseMovedEvent)
			{
				/*
				bool invokeValue = false;
				if (mouseMovedEvent.GetMovedX() > 0)
				{
					ElevatedInputKeyParams params{ ElevatedInputKeys::MouseMoveX,ElevatedKeyEventType::Clicked, inputDevice, mouseMovedEvent.GetMovedX() };
					invokeValue |= InvokeStep(params);
				}

				if (mouseMovedEvent.GetMovedY() > 0)
				{
					ElevatedInputKeyParams params{ ElevatedInputKeys::MouseMoveY,ElevatedKeyEventType::Clicked, inputDevice, mouseMovedEvent.GetMovedY() };
					invokeValue |= InvokeStep(params);
				}

				ElevatedInputKeyParams params{ ElevatedInputKeys::MouseMoveAxis,ElevatedKeyEventType::Clicked, inputDevice, glm::vec2{mouseMovedEvent.GetMovedX(),mouseMovedEvent.GetMovedY()} };
				invokeValue |= InvokeStep(params);
				*/
				UpdateCachePosition({ mouseMovedEvent.GetMovedX(),mouseMovedEvent.GetMovedY() });
				return false;
			});

		dispatcher.Dispatch<MouseScrollEvent>([&](MouseScrollEvent& mouseScroll)
			{
				auto axis = mouseScroll.GetAxis();
				bool invokeValue = false;
				if (axis == MouseAxis::ScrollUp)
				{
					ElevatedInputKeyParams params{ ElevatedInputKeys::MouseScrollUp,ElevatedKeyEventType::Clicked, inputDevice,1.0,0u };
					invokeValue |= InvokeStep(params);

					params.Event = ElevatedKeyEventType::Released;
					invokeValue |= InvokeStep(params);

				}
				else if (axis == MouseAxis::ScrollDown)
				{
					ElevatedInputKeyParams params{ ElevatedInputKeys::MouseScrollDown,ElevatedKeyEventType::Clicked, inputDevice,1.0,0u };
					invokeValue |= InvokeStep(params);

					params.Event = ElevatedKeyEventType::Released;
					invokeValue |= InvokeStep(params);
				}

				ElevatedInputKeyParams params{ ElevatedInputKeys::MouseWheelAxis,ElevatedKeyEventType::Clicked, inputDevice, mouseScroll.GetScrollY(),1};
				invokeValue |= InvokeStep(params);
				return invokeValue;
			});


		//TODO need support for keyboard keys that need shift to be emplaced
		dispatcher.Dispatch<KeyClickedEvent>([&](KeyClickedEvent& keyClicked)
			{
				ElevatedInputKey inputKey = ElevatedInputKeys::GetKeyBoardKey(keyClicked.GetKey());
				ElevatedInputKeyParams params{ inputKey,ElevatedKeyEventType::Clicked, inputDevice,1.0,inputKey.IsAnalog() ? 1u : 0u};

				return InvokeStep(params);
			});

		dispatcher.Dispatch<KeyReleasedEvent>([&](KeyReleasedEvent& keyReleased)
			{
				ElevatedInputKey inputKey = ElevatedInputKeys::GetKeyBoardKey(keyReleased.GetKey());

				ElevatedInputKeyParams params{ inputKey,ElevatedKeyEventType::Released, inputDevice,0.0,inputKey.IsAnalog() ? 1u : 0u };

				return InvokeStep(params);
			});

		/*
		dispatcher.Dispatch<KeyHoldEvent>([&](KeyHoldEvent& keyHold)
			{
				ElevatedInputKeyParams params{ ElevatedInputKeys::GetKeyBoardKey(keyHold.GetKey()) ,ElevatedKeyEventType::Repeat, inputDevice };

				return InvokeStep(params);
			});

		dispatcher.Dispatch<KeyDoubleClickEvent>([&](KeyDoubleClickEvent& keyDouble)
			{
				ElevatedInputKeyParams params{ ElevatedInputKeys::GetKeyBoardKey(keyDouble.GetKey()) ,ElevatedKeyEventType::Double, inputDevice };

				return InvokeStep(params);
			});
			*/
	}
	
	void ElevatedInputDeviceMouseKeyboard::UpdateCachePosition(glm::vec2 data)
	{
		m_CachedCursorDelta = data;

		m_NumCursorSamplesThisFrame.x++;
		m_NumCursorSamplesThisFrame.y++;
	}
	void ElevatedInputDeviceMouseKeyboard::ProcessAccumulatedPointerInput(float deltaTime)
	{
		Count< ElevatedInputDeviceMouseKeyboard> inputDevice = this;

		{
			ElevatedInputKeyParams params{ ElevatedInputKeys::MouseMoveX,ElevatedKeyEventType::Clicked, inputDevice,m_CachedCursorDelta.x,m_NumCursorSamplesThisFrame.x };
			InvokeStep(params);
		}

		{
			ElevatedInputKeyParams params{ ElevatedInputKeys::MouseMoveY,ElevatedKeyEventType::Clicked, inputDevice,m_CachedCursorDelta.y,m_NumCursorSamplesThisFrame.y };
			InvokeStep(params);
		}
	}
}