#include "Proofprch.h"
#include "WindowsWindow.h"
#include "Proof/Input/KeyCodes.h"
#include "Proof/Events/MouseEvent.h"
#include  "Proof/Events/KeyEvent.h"
#include "Proof/Events/WindowEvent.h"
#include "Platform/CurrentWindow.h"

namespace Proof {
    std::vector<KeyBoardKey> WindowsWindow::KeyboardClicked;
    std::vector<KeyBoardKey> WindowsWindow::KeyboardReleased;
    std::vector<KeyBoardKey> WindowsWindow::KeyboardKeyDoubleClicked;
    std::vector<KeyBoardKey> WindowsWindow::KeyboardKeyRepeat;

    std::vector<MouseButton> WindowsWindow::MouseButtonClicked;
    std::vector<MouseButton> WindowsWindow::MouseButtonReleased;
    std::vector<MouseButton> WindowsWindow::MouseButtonDoubleClicked;
    std::vector<MouseButton> WindowsWindow::MouseButtonRepeat;

    WindowsWindow::WindowsWindow(unsigned int Width, unsigned int Height) {
        this->Width = Width;
        this->Height = Height;
    }
    void WindowsWindow::WindowUpdate(FrameTime DeltaTime) {

       /* Event Handle over hear may change in the future */
        KeyboardClicked.clear();
        KeyboardReleased.clear();
        KeyboardKeyDoubleClicked.clear();
        KeyboardKeyRepeat.clear();
        KeyClickedEvent::Instance->EventHandled = false;
        KeyReleasedEvent::Instance->EventHandled = false;
        KeyDoubleClickEvent::Instance->EventHandled = false;
        KeyRepeatEvent::Instance->EventHandled = false;
       
        MouseButtonClicked.clear();
        MouseButtonReleased.clear();
        MouseButtonDoubleClicked.clear();
        MouseButtonReleased.clear();
        MouseReleasedEvent::Instance->EventHandled = false;
        MouseClickedEvent::Instance->EventHandled = false;
        MouseDoubleClickEvent::Instance->EventHandled = false;
        MouseMoveEvent::Instance->EventHandled = false;
        MouseScrollEvent::Instance->EventHandled = false;
        MouseScrollEvent::Instance->PosX = 0;
        MouseScrollEvent::Instance->PosY = 0;

        /* ----------------------------------------------*/
        /* This are not working like inteded */
        WindowMoveEvent::Instance->EventHandled = false;
        WindowResizeEvent::Instance->EventHandled = false;
        /* -------------------------------------------- */
       
        glfwPollEvents();
        glfwSwapBuffers(MainWindow);
        glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    }
    void WindowsWindow::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
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
            KeyClickedEvent::Instance->KeyClicked = (KeyBoardKey)key;
            KeyClickedEvent::Instance->EventHandled = true;
            break;
        case GLFW_RELEASE:
            KeyboardReleased.emplace_back((KeyBoardKey)key);
            KeyReleasedEvent::Instance->KeyReleased = (KeyBoardKey)key;
            KeyReleasedEvent::Instance->EventHandled = true;
            break;
        case (int)InputEvent::KeyDouble:
            KeyboardKeyDoubleClicked.emplace_back((KeyBoardKey)key);
            KeyDoubleClickEvent::Instance->KeyDoubleClick = (KeyBoardKey)key;
            KeyDoubleClickEvent::Instance->EventHandled = true;
            break;
        case (int)InputEvent::KeyRepeat:
            KeyboardKeyRepeat.emplace_back((KeyBoardKey)key);
            KeyRepeatEvent::Instance->KeyRepeat = (KeyBoardKey)key;
            KeyRepeatEvent::Instance->EventHandled = true;
            break;
        }
    }

    void WindowsWindow::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
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
            MouseButtonClicked.emplace_back((MouseButton)button);
            MouseClickedEvent::Instance->ButtonClicked = (MouseButton)button;
            MouseClickedEvent::Instance->EventHandled = true;
            break;
        case GLFW_RELEASE:
            MouseButtonReleased.emplace_back((MouseButton)button);
            MouseReleasedEvent::Instance->ButtonReleased = (MouseButton)button;
            MouseReleasedEvent::Instance->EventHandled = true;
            break;
        case (int)InputEvent::KeyDouble:
            MouseButtonDoubleClicked.emplace_back((MouseButton)button);
            MouseDoubleClickEvent::Instance->ButtonDoubleClick = (MouseButton)button;
            MouseDoubleClickEvent::Instance->EventHandled = true;
            break;
        }
    }
      
    void WindowsWindow::Window_Resize_Callback(GLFWwindow* window, int width, int height){
        WindowResizeEvent::Instance->EventHandled = true;
        WindowResizeEvent::Instance->Whidt = width;
        WindowResizeEvent::Instance->Height = height;
        CurrentWindow().GetWindowClass().Width = width;
        CurrentWindow().GetWindowClass().Height = height;
    }

    void WindowsWindow::Window_Position_Callback(GLFWwindow* window, int xpos, int ypos){
        WindowMoveEvent::Instance->EventHandled = true;
        WindowMoveEvent::Instance->PosX = xpos;
        WindowMoveEvent::Instance->PosY = ypos;
    }

    void WindowsWindow::Window_Close_Callback(GLFWwindow* window){
        WindowCloseEvent::Instance->EventHandled = true;
    }

    void WindowsWindow::Controller_Callback(int jid, int event){
        if (event == GLFW_CONNECTED){
            PF_ENGINE_INFO("Controller Connect");
        }
        else if (event == GLFW_DISCONNECTED){
            PF_ENGINE_INFO("Controller Disconect");
        }
    }

    void WindowsWindow::Framebuffer_size_callback(GLFWwindow* window, int width, int height){
        glViewport(0, 0, width, height);
    }

    void WindowsWindow::Window_Refresh_callback(GLFWwindow* window){
      
    }

    void WindowsWindow::Window_Input_Focus_callback(GLFWwindow* window, int focused){
        if (focused) {
            WindowFocusEvent::Instance->EventHandled = true;
            PF_ENGINE_INFO("Window Has Input focus");
        }
        else{
            WindowFocusEvent::Instance->EventHandled = false;
            PF_ENGINE_INFO("Window Lost Input Focus");
        }
    }
    
    void WindowsWindow::Mouse_Moved_Callback(GLFWwindow* window, double xpos, double ypos){
        MouseMoveEvent::Instance->EventHandled = true;
        MouseMoveEvent::Instance->PosX = xpos;
        MouseMoveEvent::Instance->PosY = ypos;
    }

    void WindowsWindow::Mouse_Hover_Window(GLFWwindow* window, int entered){
    }

    void WindowsWindow::Mouse_ScrollWhell_Callback(GLFWwindow* window, double xoffset, double yoffset){
        MouseScrollEvent::Instance->EventHandled = true;
        MouseScrollEvent::Instance->PosX = xoffset;
        MouseScrollEvent::Instance->PosY = yoffset;
    }

    int WindowsWindow::createWindow() {
        if (!glfwInit()) {
            PF_ENGINE_ERROR("Could Not Initilize GLFW");
            return -1;
        }
        MainWindow = glfwCreateWindow(Width, Height, "Proof Engine", NULL, NULL);
        if (MainWindow == nullptr) {
            PF_ENGINE_ERROR("Window Is Nullptr");
            glfwTerminate();
            return -1;
        }
     
        glfwMakeContextCurrent(MainWindow);
        glfwSetKeyCallback(MainWindow, key_callback);
        glfwSetMouseButtonCallback(MainWindow, mouse_button_callback);

        glfwSetCursorPosCallback(MainWindow, Mouse_Moved_Callback);
        glfwSetCursorEnterCallback(MainWindow, Mouse_Hover_Window);
        glfwSetScrollCallback(MainWindow, Mouse_ScrollWhell_Callback);
      
        glfwSetWindowSizeCallback(MainWindow, Window_Resize_Callback);
        glfwSetWindowPosCallback(MainWindow, Window_Position_Callback);
        glfwSetWindowCloseCallback(MainWindow, Window_Close_Callback);
        glfwSetWindowRefreshCallback(MainWindow, Window_Refresh_callback);
        glfwSetWindowFocusCallback(MainWindow, Window_Input_Focus_callback);
        glfwSetFramebufferSizeCallback(MainWindow, Framebuffer_size_callback);

        if (glewInit() != GLEW_OK) {
            PF_ENGINE_ERROR("Could Not Initilize Glew");
            return -1;
        }
        PF_ENGINE_TRACE("Open GL window has been created Width {}  Height {} ", Width, Height);
        PF_ENGINE_INFO("{}", glGetString(GL_RENDERER));
        return 0;
    }

    int WindowsWindow::WindowEnd() {
        glfwDestroyWindow(MainWindow);
        glfwTerminate();
        return 0;
    }
}
