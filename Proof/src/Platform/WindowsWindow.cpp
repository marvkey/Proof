#include "Proofprch.h"
#include "WindowsWindow.h"
#include "Proof/Input/KeyCodes.h"
namespace Proof {
    std::vector<KeyBoardKey> WindowsWindow::KeyboardPressed;
    std::vector<KeyBoardKey> WindowsWindow::KeyboardReleased;
    std::vector<KeyBoardKey> WindowsWindow::KeyboardKeyDoubleClicked;
    std::vector<KeyBoardKey> WindowsWindow::KeyboardKeyRepeat;

    double WindowsWindow::time;
    int WindowsWindow::CountClicks;

    std::vector<MouseButton> WindowsWindow::MouseButtonPressed;
    std::vector<MouseButton> WindowsWindow::MouseButtonReleased;

    std::vector<MouseButton> WindowsWindow::MouseButtonDoubleClicked;
    std::vector<MouseButton> WindowsWindow::MouseButtonRepeat;
    WindowsWindow::WindowsWindow(unsigned int Width, unsigned int Height) {
        this->Width = Width;
        this->Height = Height;
    }

    void WindowsWindow::NextFrame(bool UsingGui) {
        if (UsingGui == true) {
           // ImGui::Render();
            //ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }
        glfwGetWindowSize(MainWindow, (int*)&Width, (int*)&Height);
        KeyboardPressed.clear();
        KeyboardReleased.clear();
        KeyboardKeyDoubleClicked.clear();
        KeyboardKeyRepeat.clear();

        MouseButtonPressed.clear();
        MouseButtonReleased.clear();
        MouseButtonDoubleClicked.clear();
        MouseButtonReleased.clear();
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
            break;
        case GLFW_RELEASE:
            KeyboardReleased.push_back((KeyBoardKey)key);
            break;
        case (int)InputEvent::KeyDouble:
           
            KeyboardKeyDoubleClicked.push_back((KeyBoardKey)key);
            break;
        case (int) InputEvent::KeyRepeat:
            KeyboardKeyRepeat.push_back((KeyBoardKey)key);
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
            break;
        case GLFW_RELEASE:
            MouseButtonReleased.push_back((MouseButton)button);
            break;
        case (int)InputEvent::KeyDouble:
            MouseButtonDoubleClicked.push_back((MouseButton)button);
            break;
        case (int)InputEvent::KeyRepeat:
            MouseButtonRepeat.push_back((MouseButton)button);
            break;
        }
    }

    int WindowsWindow::createWindow() {
        if (!glfwInit()) {
            PF_ENGINE_ERROR("GlFW Not initilised");
            return -1;
        }
        MainWindow = glfwCreateWindow(Width, Height, "Proof Engine", NULL, NULL);
        if (MainWindow == nullptr) {
            PF_ENGINE_ERROR("Main Window nOt work");
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(MainWindow);
        glfwSetKeyCallback(MainWindow, key_callback);
        glfwSetMouseButtonCallback(MainWindow, mouse_button_callback);

        if (glewInit() != GLEW_OK) {
            PF_ENGINE_ERROR("Glew Init Not defined");
            return -1;
        }
        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(MainWindow, true);
        ImGui::StyleColorsDark();
        return 0;
    }

    int WindowsWindow::WindowEnd() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui::DestroyContext();
        glfwTerminate();
        return 0;
    }

    void WindowsWindow::NewFrame() {
        glClear(GL_COLOR_BUFFER_BIT);
        time = glfwGetTime();
        //ImGui_ImplOpenGL3_NewFrame();
        //ImGui_ImplGlfw_NewFrame();
    }

}
