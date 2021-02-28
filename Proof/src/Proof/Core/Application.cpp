#include "Proofprch.h"
#include "Application.h"
#include "Proof/Events/Event.h"
#include "Platform/WindowsWindow.h"
#include "Proof/Input/Mouse.h"

namespace Proof {
    WindowsWindow* Application::MainWindow = nullptr;

    Application::Application() {}

    Application::~Application() {}

    void Application::Run() {
        MainWindow = new WindowsWindow(200, 300);
        MainWindow->createWindow();
        while ((glfwWindowShouldClose(CurrentWindow::GetWindow()) == false)) {
            MainWindow->NewFrame();
            if (Input::IsMouseButtonHold(MouseButton::ButtonLeft)) {
                PF_ENGINE_INFO("PLES");
            }

            MainWindow->NextFrame(false); // false because we are not rendering a new Imgui;
        };
        MainWindow->WindowEnd();
        delete MainWindow;
    }
}