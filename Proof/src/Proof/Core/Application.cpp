#include "Proofprch.h"
#include "Application.h"


namespace Proof {
    WindowsWindow* Application::MainWindow = nullptr;

    Application::Application() {}

    Application::~Application() {}

    void Application::Run() {
        MainWindow = new WindowsWindow(200, 300);

        MainWindow->createWindow();
        while ((glfwGetKey(MainWindow->GetWindow(), KeyBoard::Escape) != KeyBoard::KeyPressed) && (glfwWindowShouldClose(MainWindow->GetWindow()) == false)) {
            MainWindow->WindowBegin();

            MainWindow->windowPollEvents(false); // false because we are not rendering a new Imgui;
        };
        MainWindow->WindowEnd();
    }
}

