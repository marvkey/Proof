#pragma once
#include <Glad/glad.h>
#include <vector>
#include "Proof/Core/Window.h"
#include "Proof/Input/KeyCodes.h"
#include <functional>
#include "Proof/Events/Event.h"

namespace Proof {
    class Proof_API WindowsWindow : public Window {
    public:
        WindowsWindow(unsigned int Width, unsigned int Height);
        WindowsWindow(){
            this->createWindow();
        }
        ~WindowsWindow(){
            this->WindowEnd();
        }
        friend class Application; 
        /* This is temporary the way the key input works but it wil still use std::vector but maybe i will make the window have a friend class with the Input the class so i can make this private*/
        std::vector<KeyBoardKey> KeyboardClicked;
        std::vector<KeyBoardKey> KeyboardReleased;
        std::vector<KeyBoardKey> KeyboardKeyDoubleClicked;
        std::vector<KeyBoardKey> KeyboardKeyRepeat;

        std::vector<MouseButton> MouseButtonClicked;
        std::vector<MouseButton> MouseButtonReleased;
        std::vector<MouseButton> MouseButtonDoubleClicked;
        std::vector<MouseButton> MouseButtonRepeat;
        std::vector<float> MouseScrollX;
        std::vector<float> MouseScrollY;
        
        virtual void WindowUpdate()override;

        void* GetWindow(){
            return m_Window;
        }
        inline void SetEventCallback(const std::function<void(Event&)>& callback) { EventCallback = callback; }
        bool IsInputEventEnabled(){
            return m_InputEventEnabled;
        }
    private:
        virtual int WindowEnd();
        virtual	int createWindow();
        int Width =800, Height=500;
        void* m_Window;
        void key_callback(int key, int scancode, int action, int mods);
        void mouse_button_callback(int button, int action, int mods);
        void Mouse_Moved_Callback(double xpos, double ypos);
        void Mouse_Hover_Window(int entered);
        void Mouse_ScrollWhell_Callback(double xoffset, double yoffset);
        void Window_Close_Callback();
        void Controller_Callback(int jid, int event);
        void Window_Resize_Callback(int width, int height);
        void Window_Position_Callback(int xpos, int ypos);
        void Window_Input_Focus_callback(int focused);
        void ProcessInput();
        friend class CurrentWindow;
        bool Vsync = false;
        std::function<void(Event&)>EventCallback;
        bool m_InputEventEnabled =true;
        // EVENT CALLback is a pointer to a function
        // set event callback is the function we will call whenever we create 
        // a new evvent with calling the object constructor it 
        // will automatically call that function
        // std::bind lets us use the function with std::function
    };
}