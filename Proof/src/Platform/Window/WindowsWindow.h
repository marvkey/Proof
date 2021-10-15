#pragma once
#include <GLFW/glfw3.h>
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
        friend class Application; 
        /* This is temporary the way the key input works but it wil still use std::vector but maybe i will make the window have a friend class with the Input the class so i can make this private*/
        static std::vector<KeyBoardKey> KeyboardClicked;
        static std::vector<KeyBoardKey> KeyboardReleased;
        static std::vector<KeyBoardKey> KeyboardKeyDoubleClicked;
        static std::vector<KeyBoardKey> KeyboardKeyRepeat;

        static std::vector<MouseButton> MouseButtonClicked;
        static std::vector<MouseButton> MouseButtonReleased;
        static std::vector<MouseButton> MouseButtonDoubleClicked;
        static std::vector<MouseButton> MouseButtonRepeat;
        static std::vector<float> MouseScrollX;
        static std::vector<float> MouseScrollY;
        
        virtual void WindowUpdate()override;
        virtual	int createWindow()override;
        virtual int WindowEnd()override;

        void* GetWindow(){
            return MainWindow;
        }
        inline void SetEventCallback(const std::function<void(Event&)>& callback) { EventCallback = callback; }
        bool IsInputEventEnabled(){
            return m_InputEventEnabled;
        }
    private:
        int Width =0, Height=0;
        ::GLFWwindow* MainWindow;
        static void key_callback(::GLFWwindow* window, int key, int scancode, int action, int mods);
        static void mouse_button_callback(::GLFWwindow* window, int button, int action, int mods);
        static void Mouse_Moved_Callback(::GLFWwindow* window, double xpos, double ypos);
        static void Mouse_Hover_Window(::GLFWwindow* window, int entered);
        static void Mouse_ScrollWhell_Callback(::GLFWwindow* window, double xoffset, double yoffset);
        static void Window_Close_Callback(::GLFWwindow* window);
        static void Controller_Callback(int jid, int event);
        static void Framebuffer_size_callback(::GLFWwindow* window, int width, int height);

        static void Window_Resize_Callback(::GLFWwindow* window, int width, int height);
        static void Window_Position_Callback(::GLFWwindow* window, int xpos, int ypos);
        static void Window_Refresh_callback(::GLFWwindow* window);
        static void Window_Input_Focus_callback(::GLFWwindow* window, int focused);
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