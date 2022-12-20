#pragma once
#include "Core.h"
#include <functional>
#include "Proof/Input/KeyCodes.h"
#include "Proof/Events/Event.h"
#include "Proof/Input/Controller.h"
#include "Proof/Math/Vector.h"
namespace Proof {
	struct WindowConfiguration {
		uint32_t Width = 0;
		uint32_t Height = 0;
		bool startWindowedFullScreen = false;
		bool startFullScreen = false;
		bool Vsync = false;
		bool Decorated = true;
		std::string Title = "Window";
	};
	class Proof_API Window {
	public:
		virtual void SetEventCallback(const std::function<void(Event&)>& callback) = 0;
		virtual void WindowUpdate() = 0;
		virtual ~Window(){};
		Window(const WindowConfiguration& configuration) {
			m_WindowConfiguration = configuration;
			 // setting all values to false
			for (int i = 0; i < KeyPressed.size(); i++) {
				KeyPressed[i] = false;
			}
			for (int i = 0; i < MouseButtonPressed.size(); i++) {
				MouseButtonPressed[i] = false;
			}
		}
		void* GetWindow() {
			return m_Window;
		}
		bool IsInputEventEnabled() {
			return m_InputEventEnabled;
		}
		bool IsMouseMoved() {
			return m_MouseMoved;
		}
		uint32_t GetHeight() {
			return m_WindowConfiguration.Height;
		}
		uint32_t GetWidth() {
			return m_WindowConfiguration.Width;
		}
		virtual Vector2 GetMousePosition() = 0;
		virtual void SetVsync(bool vsync) = 0;
		bool IsVsync() {
			return m_WindowConfiguration.Vsync;
		}
		bool IsFrameBufferResized() { return m_FrameBufferResized; }
		virtual void CenterWindow() = 0;

		void SetWindowInputEvent(bool inputEvent) {
			m_InputEventEnabled = inputEvent;
		}
		Vector2 GetMouseScrollWheel() {
			float x = 0, y = 0;
			if(MouseScrollX.size()>0)
				x = MouseScrollX[0];
			if (MouseScrollY.size() > 0)
				y = MouseScrollY[0];
			return { x,y };
		}

		static Special<Window> Create(const WindowConfiguration& configuration);
		friend class Application;
		friend class Input;
		friend class CurrentWindow;
		friend class Editore3D;
		Count<class SwapChain> GetSwapChain(){
			return m_SwapChain;
		}
	protected:
		Count<class SwapChain> m_SwapChain;
		bool m_FrameBufferResized = false;
		bool m_MouseMoved = false;
		void* m_Window = nullptr;
		bool m_InputEventEnabled = true;
		WindowConfiguration m_WindowConfiguration;

				/* This is temporary the way the key input works but it wil still use std::vector but maybe i will make the window have a friend class with the Input the class so i can make this private*/
		std::vector<KeyBoardKey> KeyboardClicked;
		std::vector<KeyBoardKey> KeyboardReleased;
		std::vector<KeyBoardKey> KeyboardKeyDoubleClicked;
		std::vector<KeyBoardKey> KeyboardKeyHold;

		std::vector<MouseButton> MouseButtonClicked;
		std::vector<MouseButton> MouseButtonReleased;
		std::vector<MouseButton> MouseButtonDoubleClicked;
		std::vector<MouseButton> MouseButtonRepeat;
		std::vector<float> MouseScrollX;
		std::vector<float> MouseScrollY;
		std::array<bool, 1024>KeyPressed; // This might need to be in another way, taking 
		std::array<bool, 10>MouseButtonPressed; // This might need to be in another way, taking 
				// have to use this because GLFW someties will not send that a key hasb een released
		std::vector<KeyBoardKey> m_KeyPressedEventCheck;
		std::vector<MouseButton> m_MouseButtonPressedEventCheck;
		std::vector<Controller> m_Controllers;
		friend class RendererBase;
	};
}
