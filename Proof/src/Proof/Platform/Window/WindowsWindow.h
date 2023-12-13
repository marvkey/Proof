#pragma once
#include "Proof/Core/Window.h"
#include <vector>
#include <GLFW/glfw3.h>
namespace Proof {
 
    class Proof_API WindowsWindow : public Window {
    public:
        WindowsWindow(const WindowConfiguration& configuration);
        virtual ~WindowsWindow();
        inline void SetEventCallback(const std::function<void(Event&)>& callback) { EventCallback = callback; }
        void CenterWindow();
    private:

        virtual void BeginFrame();
        virtual void EndFrame();
        virtual void SetVsync(bool vsync)override;
        virtual Vector2 GetMousePosition();

        std::function<void(Event&)>EventCallback;
        int End();
        int Init();
        virtual void WindowUpdate();

        bool m_FrameBufferResized = false;
        void key_callback(int key, int scancode, int action, int mods);
        void mouse_button_callback(int button, int action, int mods);
        void Mouse_Moved_Callback(double xpos, double ypos);
        void Mouse_Hover_Window(int entered);
        void Mouse_ScrollWhell_Callback(double xoffset, double yoffset);
        void Window_Close_Callback();
        void ControllerCallbackConnect(int jid, int event);
        void Window_Resize_Callback(int width, int height);
        void Window_Position_Callback(int xpos, int ypos);
        void Window_Input_Focus_callback(int focused);
        void ControllerEventCallback();
        void ContollerButtonCallback(Controller& controller);
        void ContollerAxisCallback(Controller& controller);
        void FrameBufferResizedCallback(int width, int height);

        //keyID,key
        inline static std::unordered_map<uint32_t,ControllerButton> s_ControllerLastKeyClicked; // the last key for controller register
        static InputEvent ControllerGetState(Controller& controller, GLFWgamepadstate& state, ControllerButton button);
        inline static WindowsWindow* s_lastWindow;
        bool m_InputEventEnabled =true;
        float m_MousePreviousLocationX = 0;
        float m_MousePreviousLocationY = 0;
        
    };
}