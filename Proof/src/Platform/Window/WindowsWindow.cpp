#include "Proofprch.h"
#include "WindowsWindow.h"
#include "Proof/Input/KeyCodes.h"
#include "Proof/Events/MouseEvent.h"
#include  "Proof/Events/KeyEvent.h"
#include "Proof/Events/WindowEvent.h"
#include "Proof/Core/CurrentWindow.h"

#include "Proof/Core/FrameTime.h"

#include "Proof/Renderer/GraphicsContext.h"
#include "Proof/Core/Core.h"
#include "ImGui/imgui.h"
#include <GLFW/glfw3.h>

namespace Proof {



    WindowsWindow::WindowsWindow(unsigned int Width, unsigned int Height) {
        this->Width = Width;
        this->Height = Height;
        // setting all values to false
        for (int i = 0; i < KeyPressed.size(); i++) {
            KeyPressed[i] = false;
        }
        for (int i = -0; i < MouseButtonPressed.size(); i++) {
            MouseButtonPressed[i] = false;
        }
        this->createWindow();
    }
    void WindowsWindow::WindowUpdate() {
        glfwSwapInterval(Vsync);
        
        KeyboardClicked.clear();
        KeyboardReleased.clear();
        KeyboardKeyDoubleClicked.clear();
        KeyboardKeyHold.clear();
       
        MouseButtonClicked.clear();
        MouseButtonReleased.clear();
        MouseButtonDoubleClicked.clear();
        MouseButtonReleased.clear();

        MouseScrollX.clear();
        MouseScrollY.clear();
        
        //HAVE TO DO THIS SINCE SOMETIEMS GLFW DOES NOT RECIEVE THE KEY RELEASED DUE TO SO MANY KEYS 
        // BEING PRESSED AT THE SAME TIME
        // WE ARE BASICALLY GOING TO DO THIS EVENT FOR THE INPUT MANAGER AXIS
        

        // Checking to see if every key clicked in the last frame is still clicked 
        // if teh key is not pressed we remove it from the list, set it false in the key pressed list
        for (int i = 0; i < m_KeyPressedEventCheck.size(); i++) {
            KeyBoardKey key = m_KeyPressedEventCheck[i];
            if (glfwGetKey((GLFWwindow*)CurrentWindow::GetWindowAPI(), (int)key)) {
                KeyPressedEvent pressedEvent(key);
                EventCallback(pressedEvent);
            }
            else {
                KeyPressed[(int)key] = false;
                m_KeyPressedEventCheck.erase(m_KeyPressedEventCheck.begin() + i);
            }
        }

        for (int i = 0; i < m_MouseButtonPressedEventCheck.size(); i++) {
            MouseButton key = m_MouseButtonPressedEventCheck[i];
            if (glfwGetMouseButton((GLFWwindow*)CurrentWindow::GetWindowAPI(), (int)key)) {
                MouseButtonPressedEvent pressedEvent(key);
                EventCallback(pressedEvent);
            }
            else {
                MouseButtonPressed[(int)key] = false;
                m_MouseButtonPressedEventCheck.erase(m_MouseButtonPressedEventCheck.begin() + i);
            }
        }
        glfwSwapBuffers((GLFWwindow*)m_Window);
        glfwPollEvents();
    }
    void WindowsWindow::key_callback(int key, int scancode, int action, int mods) {
        /* This is for when a key is Clicked 2 */
        if (action == GLFW_RELEASE) {
            static auto before = std::chrono::system_clock::now();
            auto now = std::chrono::system_clock::now();
            double diff_ms = std::chrono::duration <double, std::milli>(now - before).count();
            before = now;
            if (diff_ms > 10 && diff_ms < 200) {
                action = (int)InputEvent::KeyDouble;
            }
        }     
        
        switch (action) {
        case GLFW_PRESS:
            KeyboardClicked.emplace_back((KeyBoardKey)key);
            {
                // have to use this because GLFW someties will not send that a key hasb een released
                KeyPressed[key] = true;
                m_KeyPressedEventCheck.emplace_back((KeyBoardKey)key);
                KeyClickedEvent keyevent((KeyBoardKey)key);
                EventCallback(keyevent);
                
            }
            break;
        case GLFW_RELEASE:
            KeyboardReleased.emplace_back((KeyBoardKey)key);
            {
                // have to use this because GLFW someties will not send that a key hasb een released
                KeyPressed[key] = false;
                KeyReleasedEvent keyevent((KeyBoardKey)key);
                EventCallback(keyevent);
            }
            break;
        case (int)InputEvent::KeyDouble:
            KeyboardKeyDoubleClicked.emplace_back((KeyBoardKey)key);
            {
                KeyDoubleClickEvent keyevent((KeyBoardKey)key);
                EventCallback(keyevent);
            }
            break;
        case (int)InputEvent::KeyHold:
            KeyboardKeyHold.emplace_back((KeyBoardKey)key);
            {
                KeyHoldEvent keyevent((KeyBoardKey)key);
                EventCallback(keyevent);
            }
            break;
        }
    }

    void WindowsWindow::mouse_button_callback(int button, int action, int mods) {
        if (action == GLFW_RELEASE) {
            static auto before = std::chrono::system_clock::now();
            auto now = std::chrono::system_clock::now();
            double diff_ms = std::chrono::duration <double, std::milli>(now - before).count();
            before = now;
            if (diff_ms > 10 && diff_ms < 200) {
                action = (int)InputEvent::KeyDouble;
            }
        }
        switch (action) {
        case GLFW_PRESS:
        {
            MouseButtonClicked.emplace_back((MouseButton)button);
            {
                m_MouseButtonPressedEventCheck.emplace_back((MouseButton)button);
                MouseButtonPressed[button] = true;
                MouseButtonClickedEvent mouseEvent((MouseButton)button);
                EventCallback(mouseEvent);
            }
            break;
        }
        case GLFW_RELEASE:
            MouseButtonReleased.emplace_back((MouseButton)button);
            {
                MouseButtonReleasedEvent mouseEvent((MouseButton)button);
                EventCallback(mouseEvent);
                MouseButtonPressed[button] = false;
            }
            break;
        case (int)InputEvent::KeyDouble:
            MouseButtonDoubleClicked.emplace_back((MouseButton)button);
            {
                MouseButtonDoubleClickEvent mouseEvent((MouseButton)button);
                EventCallback(mouseEvent);
            }
            break;
        }
    }
      
    void WindowsWindow::Window_Resize_Callback(int width, int height){
        WindowResizeEvent windowResizeEvent(width,height);
        EventCallback(windowResizeEvent);
        if(width==0 || height==0){
            WindowMinimizeEvent Minimizedevent(true);
            EventCallback(Minimizedevent);

        }else if (Width==0 || Height==0){
            WindowMinimizeEvent Minimizedevent(false);
            EventCallback(Minimizedevent);
        }
        Width = width;
        Height = height;
    }

    void WindowsWindow::Window_Position_Callback(int xpos, int ypos){
        WindowMoveEvent windowEvent(xpos,ypos);
        EventCallback(windowEvent);
    }

    void WindowsWindow::Window_Close_Callback(){
        WindowCloseEvent windowEvent(true);
        EventCallback(windowEvent);
    }

    void WindowsWindow::Controller_Callback(int jid, int event){
        if (event == GLFW_CONNECTED){
            PF_INFO("Controller Connect");
        }
        else if (event == GLFW_DISCONNECTED){
            PF_INFO("Controller Disconect");
        }
    }

    void WindowsWindow::Window_Input_Focus_callback(int focused){
        WindowFocusEvent windowEvent(true);
        EventCallback(windowEvent);
    }

    void WindowsWindow::ProcessInput(){
       
    }
    
    void WindowsWindow::Mouse_Moved_Callback(double xpos, double ypos){
        MouseMoveEvent mouseEvent(xpos,ypos);
        EventCallback(mouseEvent);
    }

    void WindowsWindow::Mouse_Hover_Window(int entered){
    }

    void WindowsWindow::Mouse_ScrollWhell_Callback(double xoffset, double yoffset){
        if (m_InputEventEnabled == false)
            return;
        MouseScrollEvent mouseEvent(xoffset,yoffset);
        EventCallback(mouseEvent);
        MouseScrollX.emplace_back(xoffset);
        MouseScrollY.emplace_back(yoffset);
    }

    int WindowsWindow::createWindow() {
        if (!glfwInit()) {
            PF_CORE_ASSERT(false,"Could Not Initilize GLFW");
            return -1;
        }
       // glfwWindowHint(GLFW_MAXIMIZED,GLFW_TRUE); // when using this meathod teh window will have some glithy meathod

        m_Window = glfwCreateWindow(Width,Height, "Proof", nullptr,NULL);
        if (m_Window == nullptr) {
            PF_CORE_ASSERT(false,"Window Is Nullptr");
            glfwTerminate();
            return -1;
        }
        glfwSetWindowUserPointer((GLFWwindow*)m_Window, this);
        glfwMaximizeWindow((GLFWwindow*)m_Window);

        glfwGetWindowSize((GLFWwindow*)m_Window,&Width,&Height);
       
        glfwMakeContextCurrent((GLFWwindow*)m_Window);
        glfwSetKeyCallback((GLFWwindow*)m_Window, [](::GLFWwindow* window, int key, int scancode, int action, int mods)mutable {
            WindowsWindow& proofWindow = *(WindowsWindow*)glfwGetWindowUserPointer(window);
            proofWindow.key_callback(key, scancode, action, mods);
        });

        glfwSetMouseButtonCallback((GLFWwindow*)m_Window, [](::GLFWwindow* window, int button, int action, int mods){
                WindowsWindow& proofWindow = *(WindowsWindow*)glfwGetWindowUserPointer(window);
                proofWindow.mouse_button_callback(button,action, mods);
        });
        glfwSetCursorPosCallback((GLFWwindow*)m_Window, [](::GLFWwindow* window, double xpos, double ypos) {
            WindowsWindow& proofWindow = *(WindowsWindow*)glfwGetWindowUserPointer(window);
            proofWindow.Mouse_Moved_Callback(xpos,ypos);
        });
        glfwSetCursorEnterCallback((GLFWwindow*)m_Window, [](::GLFWwindow* window, int entered) {
            WindowsWindow& proofWindow = *(WindowsWindow*)glfwGetWindowUserPointer(window);
            proofWindow.Mouse_Hover_Window(entered);
        });
        glfwSetScrollCallback((GLFWwindow*)m_Window, [](::GLFWwindow* window, double xoffset, double yoffset) {
            WindowsWindow& proofWindow = *(WindowsWindow*)glfwGetWindowUserPointer(window);
            proofWindow.Mouse_ScrollWhell_Callback(xoffset, yoffset);
        });

        glfwSetWindowSizeCallback((GLFWwindow*)m_Window, [](::GLFWwindow* window, int width, int height) {
            WindowsWindow& proofWindow = *(WindowsWindow*)glfwGetWindowUserPointer(window);
            proofWindow.Window_Resize_Callback(width,height);
        });

        glfwSetWindowPosCallback((GLFWwindow*)m_Window, [](::GLFWwindow* window, int xpos, int ypos) {
            WindowsWindow& proofWindow = *(WindowsWindow*)glfwGetWindowUserPointer(window);
            proofWindow.Window_Position_Callback(xpos,ypos);
        });
        glfwSetWindowCloseCallback((GLFWwindow*)m_Window, [](::GLFWwindow* window) {
            WindowsWindow& proofWindow = *(WindowsWindow*)glfwGetWindowUserPointer(window);
            proofWindow.Window_Close_Callback();
        });
        
        glfwSetWindowFocusCallback((GLFWwindow*)m_Window, [](::GLFWwindow* window, int focused) {
            WindowsWindow& proofWindow = *(WindowsWindow*)glfwGetWindowUserPointer(window);
            proofWindow.Window_Input_Focus_callback(focused);
        });
        PF_INFO("Window created widht %i height %i",Width,Height);
        return 0;
    }

    int WindowsWindow::WindowEnd() {
        glfwDestroyWindow((GLFWwindow*)m_Window);
        glfwTerminate();
        return 0;
    }
}
