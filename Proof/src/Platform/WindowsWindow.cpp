#include "Proofprch.h"
#include "WindowsWindow.h"
#include "Proof/Input/KeyCodes.h"
#include "Proof/Events/MouseEvent.h"
#include  "Proof/Events/KeyEvent.h"
#include "Proof/Events/WindowEvent.h"
#include "Platform/CurrentWindow.h"
namespace Proof {
    std::vector<KeyBoardKey> WindowsWindow::KeyboardPressed;
    std::vector<KeyBoardKey> WindowsWindow::KeyboardReleased;
    std::vector<KeyBoardKey> WindowsWindow::KeyboardKeyDoubleClicked;
    std::vector<KeyBoardKey> WindowsWindow::KeyboardKeyRepeat;

    std::vector<MouseButton> WindowsWindow::MouseButtonPressed;
    std::vector<MouseButton> WindowsWindow::MouseButtonReleased;

    std::vector<MouseButton> WindowsWindow::MouseButtonDoubleClicked;
    std::vector<MouseButton> WindowsWindow::MouseButtonRepeat;
    WindowsWindow::WindowsWindow(unsigned int Width, unsigned int Height) {
        this->Width = Width;
        this->Height = Height;
    }

    void WindowsWindow::WindowUpdate(bool UsingGui) {
       
       /* Event Handle over hear may change in the future */
        KeyboardPressed.clear();
        KeyboardReleased.clear();
        KeyboardKeyDoubleClicked.clear();
        KeyboardKeyRepeat.clear();
        
        KeyPressedEvent::Instance->EventHandled = false;
        KeyReleasedEvent::Instance->EventHandled = false;
        KeyDoubleClickEvent::Instance->EventHandled = false;
        KeyRepeatEvent::Instance->EventHandled = false;
       
        MouseButtonPressed.clear();
        MouseButtonReleased.clear();
        MouseButtonDoubleClicked.clear();
        MouseButtonReleased.clear();
        MouseReleasedEvent::Instance->EventHandled = false;
        MousePressedEvent::Instance->EventHandled = false;
        MouseDoubleClickEvent::Instance->EventHandled = false;
        MouseMoveEvent::Instance->EventHandled = false;

        /* ----------------------------------------------*/
        /* This are not working like inteded */
        WindowMoveEvent::Instance->EventHandled = false;
        WindowResizeEvent::Instance->EventHandled = false;
        /* -------------------------------------------- */
        
        if (UsingGui == true) {
             ImGui::Render();
             ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }

        glfwSwapBuffers(MainWindow);
        glfwPollEvents();
    }
    void WindowsWindow::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        /* This is for when a key is pressed 2 */
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
            KeyboardPressed.push_back((KeyBoardKey)key);
            KeyPressedEvent::Instance->KeyPressed = (KeyBoardKey)key;
            KeyPressedEvent::Instance->EventHandled = true;
            break;
        case GLFW_RELEASE:
            KeyboardReleased.push_back((KeyBoardKey)key);
            KeyReleasedEvent::Instance->KeyReleased = (KeyBoardKey)key;
            KeyReleasedEvent::Instance->EventHandled = true;
            break;
        case (int)InputEvent::KeyDouble:
            KeyboardKeyDoubleClicked.push_back((KeyBoardKey)key);
            KeyDoubleClickEvent::Instance->KeyDoubleClick = (KeyBoardKey)key;
            KeyDoubleClickEvent::Instance->EventHandled = true;
            break;
        case (int) InputEvent::KeyRepeat:
            KeyboardKeyRepeat.push_back((KeyBoardKey)key);
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
            MouseButtonPressed.push_back((MouseButton)button);
            MousePressedEvent::Instance->ButtonPressed = (MouseButton)button;
            MousePressedEvent::Instance->EventHandled = true;
            break;
        case GLFW_RELEASE:
            MouseButtonReleased.push_back((MouseButton)button);
            MouseReleasedEvent::Instance->ButtonReleased = (MouseButton)button;
            MouseReleasedEvent::Instance->EventHandled = true;

            break;
        case (int)InputEvent::KeyDouble:
            MouseButtonDoubleClicked.push_back((MouseButton)button);
            MouseDoubleClickEvent::Instance->ButtonDoubleClick = (MouseButton)button;
            MouseDoubleClickEvent::Instance->EventHandled = true;
            break;
        }
    }
    
    void WindowsWindow::Window_Resize_Callback(GLFWwindow* window, int width, int height){
        WindowResizeEvent::Instance->EventHandled = true;
        WindowResizeEvent::Instance->Whidt = width;
        WindowResizeEvent::Instance->Height = height;
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

    void WindowsWindow::Window_Refresh_callback(GLFWwindow* window){
        //glfwSwapBuffers(window);
       
    }

    void WindowsWindow::Window_Input_Focus_callback(GLFWwindow* window, int focused){
        if (focused) {
            WindowFocusEvent::Instance->EventHandled = true;
        }
        else{
            WindowFocusEvent::Instance->EventHandled = false;
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
        //glfwSetJoystickCallback(Controller_Callback);
        
        glfwSetWindowSizeCallback(MainWindow, Window_Resize_Callback);
        glfwSetWindowPosCallback(MainWindow, Window_Position_Callback);
        glfwSetWindowCloseCallback(MainWindow, Window_Close_Callback);
        glfwSetWindowRefreshCallback(MainWindow, Window_Refresh_callback);
        glfwSetWindowFocusCallback(MainWindow, Window_Input_Focus_callback);
        
        

        if (glewInit() != GLEW_OK) {
            PF_ENGINE_ERROR("Could Not Initilize Glew");
            return -1;
        }

        
        //ImGui::CreateContext();
        //ImGui_ImplGlfw_InitForOpenGL(MainWindow, true);
        //ImGui::StyleColorsDark();
        return 0;
    }

    int WindowsWindow::WindowEnd() {

        //ImGui_ImplOpenGL3_Shutdown();
        //ImGui::DestroyContext();
        glfwDestroyWindow(MainWindow);
        glfwTerminate();
        return 0;
    }

    

}
