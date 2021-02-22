#include "Proofprch.h"
#include "Application.h"


namespace Proof {
    WindowsWindow* Application::MainWindow = nullptr;

    std::pair<int, int> Application::GetWindowSize(){
        return MainWindow->GetWindowSize();
    }

    void Application::SetWindowSize(int width, int height){
        MainWindow->SetWindowSize(width, height);
    }

    Application::Application() {}

    Application::~Application() {}

    void Application::Run() {
        MainWindow = new WindowsWindow(200, 300);
        MainWindow->createWindow();
        while (KeyBoard::KeyPress(KeyBoard::Escape)==false && (glfwWindowShouldClose(MainWindow->GetWindow()) == false)) {
            MainWindow->WindowBegin();
			
            MainWindow->windowPollEvents(false); // false because we are not rendering a new Imgui;
        };
        MainWindow->WindowEnd();
        delete MainWindow;
    }

    void* Application::GetWindow(){
        return MainWindow->GetWindow();
    }

    std::pair<float, float> Application::GetMousePosition(){
        return MainWindow->GetPlatformMouseLocation();
    }
   
}

