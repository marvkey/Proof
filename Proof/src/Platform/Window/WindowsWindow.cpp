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
    
namespace Proof {
    std::vector<KeyBoardKey> WindowsWindow::KeyboardClicked;
    std::vector<KeyBoardKey> WindowsWindow::KeyboardReleased;
    std::vector<KeyBoardKey> WindowsWindow::KeyboardKeyDoubleClicked;
    std::vector<KeyBoardKey> WindowsWindow::KeyboardKeyRepeat;

    std::vector<MouseButton> WindowsWindow::MouseButtonClicked;
    std::vector<MouseButton> WindowsWindow::MouseButtonReleased;
    std::vector<MouseButton> WindowsWindow::MouseButtonDoubleClicked;
    std::vector<MouseButton> WindowsWindow::MouseButtonRepeat;
    std::vector<float>WindowsWindow::MouseScrollX;
    std::vector<float>WindowsWindow::MouseScrollY;


    WindowsWindow::WindowsWindow(unsigned int Width, unsigned int Height) {
        this->Width = Width;
        this->Height = Height;
        this->createWindow();
    }
    void WindowsWindow::WindowUpdate() {
        glfwSwapInterval(Vsync);
        KeyboardClicked.clear();
        KeyboardReleased.clear();
        KeyboardKeyDoubleClicked.clear();
        KeyboardKeyRepeat.clear();
       
        MouseButtonClicked.clear();
        MouseButtonReleased.clear();
        MouseButtonDoubleClicked.clear();
        MouseButtonReleased.clear();

        MouseScrollX.clear();
        MouseScrollY.clear();
        glfwSwapBuffers(MainWindow);
        glfwPollEvents();
    }
    void WindowsWindow::key_callback(::GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (CurrentWindow::GetWindowClass().m_InputEventEnabled == false)
            return;
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
                KeyClickedEvent keyevent((KeyBoardKey)key);
                CurrentWindow::GetWindowClass().EventCallback(keyevent);
            }
            break;
        case GLFW_RELEASE:
            KeyboardReleased.emplace_back((KeyBoardKey)key);
            {
                KeyReleasedEvent keyevent((KeyBoardKey)key);
                CurrentWindow::GetWindowClass().EventCallback(keyevent);
            }
            break;
        case (int)InputEvent::KeyDouble:
            KeyboardKeyDoubleClicked.emplace_back((KeyBoardKey)key);
            {
                KeyDoubleClickEvent keyevent((KeyBoardKey)key);
                CurrentWindow::GetWindowClass().EventCallback(keyevent);
            }
            break;
        case (int)InputEvent::KeyRepeat:
            KeyboardKeyRepeat.emplace_back((KeyBoardKey)key);
            {
                KeyPressedEvent keyevent((KeyBoardKey)key);
                CurrentWindow::GetWindowClass().EventCallback(keyevent);
            }
            break;
        }
    }

    void WindowsWindow::mouse_button_callback(::GLFWwindow* window, int button, int action, int mods) {
        if (CurrentWindow::GetWindowClass().m_InputEventEnabled == false)
            return;
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
                MouseButtonClickedEvent mouseEvent((MouseButton)button);
                CurrentWindow::GetWindowClass().EventCallback(mouseEvent);
            }
            break;
        }
        case GLFW_RELEASE:
            MouseButtonReleased.emplace_back((MouseButton)button);
            {
                MouseButtonReleasedEvent mouseEvent((MouseButton)button);
                CurrentWindow::GetWindowClass().EventCallback(mouseEvent);
            }
            break;
        case (int)InputEvent::KeyDouble:
            MouseButtonDoubleClicked.emplace_back((MouseButton)button);
            {
                MouseButtonDoubleClickEvent mouseEvent((MouseButton)button);
                CurrentWindow::GetWindowClass().EventCallback(mouseEvent);
            }
            break;
        }
    }
      
    void WindowsWindow::Window_Resize_Callback(::GLFWwindow* window, int width, int height){
        WindowResizeEvent windowEvent(width,height);
        CurrentWindow::GetWindowClass().EventCallback(windowEvent);
        if(width==0 || height==0){
            WindowMinimizeEvent Minimizedevent(true);
            CurrentWindow::GetWindowClass().EventCallback(Minimizedevent);

        }else if (CurrentWindow().GetWindowClass().Width==0 || CurrentWindow().GetWindowClass().Height==0){
            WindowMinimizeEvent Minimizedevent(false);
            CurrentWindow::GetWindowClass().EventCallback(Minimizedevent);
        }
        CurrentWindow().GetWindowClass().Width = width;
        CurrentWindow().GetWindowClass().Height = height;
    }

    void WindowsWindow::Window_Position_Callback(::GLFWwindow* window, int xpos, int ypos){
        WindowMoveEvent windowEvent(xpos,ypos);
        CurrentWindow::GetWindowClass().EventCallback(windowEvent);
    }

    void WindowsWindow::Window_Close_Callback(::GLFWwindow* window){
        WindowCloseEvent windowEvent(true);
        CurrentWindow::GetWindowClass().EventCallback(windowEvent);
    }

    void WindowsWindow::Controller_Callback(int jid, int event){
        if (event == GLFW_CONNECTED){
            PF_ENGINE_INFO("Controller Connect");
        }
        else if (event == GLFW_DISCONNECTED){
            PF_ENGINE_INFO("Controller Disconect");
        }
    }

    void WindowsWindow::Framebuffer_size_callback(::GLFWwindow* window, int width, int height){
    }

    void WindowsWindow::Window_Refresh_callback(::GLFWwindow* window){
      
    }

    void WindowsWindow::Window_Input_Focus_callback(::GLFWwindow* window, int focused){
        WindowFocusEvent windowEvent(true);
        CurrentWindow::GetWindowClass().EventCallback(windowEvent);
    }

    void WindowsWindow::ProcessInput(){
       
    }
    
    void WindowsWindow::Mouse_Moved_Callback(::GLFWwindow* window, double xpos, double ypos){
        if (CurrentWindow::GetWindowClass().m_InputEventEnabled == false)
            return;
        MouseMoveEvent mouseEvent(xpos,ypos);
        CurrentWindow::GetWindowClass().EventCallback(mouseEvent);
    }

    void WindowsWindow::Mouse_Hover_Window(::GLFWwindow* window, int entered){
    }

    void WindowsWindow::Mouse_ScrollWhell_Callback(::GLFWwindow* window, double xoffset, double yoffset){
        if (CurrentWindow::GetWindowClass().m_InputEventEnabled == false)
            return;
        MouseScrollEvent mouseEvent(xoffset,yoffset);
        CurrentWindow::GetWindowClass().EventCallback(mouseEvent);
        MouseScrollX.emplace_back(xoffset);
        MouseScrollY.emplace_back(yoffset);
    }

    int WindowsWindow::createWindow() {
        if (!glfwInit()) {
            PF_CORE_ASSERT(false,"Could Not Initilize GLFW");
            return -1;
        }
       // glfwWindowHint(GLFW_MAXIMIZED,GLFW_TRUE); // when using this meathod teh window will have some glithy meathod

        MainWindow = glfwCreateWindow(Width,Height, "Proof", nullptr,NULL);
        if (MainWindow == nullptr) {
            PF_CORE_ASSERT(false,"Window Is Nullptr");
            glfwTerminate();
            return -1;
        }
        glfwMaximizeWindow(MainWindow);

        glfwGetWindowSize(MainWindow,&Width,&Height);
       
        glfwMakeContextCurrent(MainWindow);
        glfwSetKeyCallback(MainWindow,key_callback);
        glfwSetMouseButtonCallback(MainWindow,mouse_button_callback);

        glfwSetCursorPosCallback(MainWindow,Mouse_Moved_Callback);
        glfwSetCursorEnterCallback(MainWindow,Mouse_Hover_Window);
        glfwSetScrollCallback(MainWindow,Mouse_ScrollWhell_Callback);

        glfwSetWindowSizeCallback(MainWindow,Window_Resize_Callback);
        glfwSetWindowPosCallback(MainWindow,Window_Position_Callback);
        glfwSetWindowCloseCallback(MainWindow,Window_Close_Callback);
        glfwSetWindowRefreshCallback(MainWindow,Window_Refresh_callback);
        glfwSetWindowFocusCallback(MainWindow,Window_Input_Focus_callback);
        glfwSetFramebufferSizeCallback(MainWindow,Framebuffer_size_callback);
        PF_ENGINE_TRACE("Window created widht %i height %i",Width,Height);
        return 0;
    }

    int WindowsWindow::WindowEnd() {
        glfwDestroyWindow(MainWindow);
        glfwTerminate();
        return 0;
    }
}
