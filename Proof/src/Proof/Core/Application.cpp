#include "Proofprch.h"
#include "Application.h"
#include "Platform/WindowsWindow.h"
#include "Proof/Input/Mouse.h"
#include "Proof/Events/MouseEvent.h"
#include "Proof/Events/KeyEvent.h"
#include "Proof/Events/WindowEvent.h"

namespace Proof {
    WindowsWindow* Application::MainWindow = nullptr;

    Application::Application() {}

    Application::~Application() {}

    void Application::Run() {
        MainWindow = new WindowsWindow(800, 500);
      
        MainWindow->createWindow();
        while ((glfwWindowShouldClose(CurrentWindow::GetWindow()) == false)) {
          
            MainWindow->WindowUpdate(false); // false because we are not rendering a new Imgui;
        };
        MainWindow->WindowEnd();
        delete MainWindow;
    }
   
}