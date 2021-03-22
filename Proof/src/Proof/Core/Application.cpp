#include "Proofprch.h"
#include "Application.h"
#include "Platform/WindowsWindow.h"
#include "Proof/Input/Mouse.h"
#include "Proof/Events/MouseEvent.h"
#include "Proof/Events/KeyEvent.h"
#include "Proof/Events/WindowEvent.h"
#include "Proof/Core/FrameTime.h"
#include "Proof/Events/Event.h"


namespace Proof {
    WindowsWindow* Application::MainWindow = nullptr;

    Application::Application() {}

    Application::~Application() {}

    void Application::Run() {
        MainWindow = new WindowsWindow(800, 500);
        
        MainWindow->createWindow();
        
        FrameTime DeltaTime;
        /* Loop until the user closes the window */
        bool show_demo_window = true;
        bool show_another_window = false;
        float Num = 5;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        
       ImGui::CreateContext();
       ImGui_ImplGlfw_InitForOpenGL(CurrentWindow::GetWindow(), true);
       ImGui::StyleColorsDark();

        while ((glfwWindowShouldClose(CurrentWindow::GetWindow()) == false) && !(Input::IsKeyPressed(KeyBoardKey::Escape) ==true)) {
            
            float time = (float)glfwGetTime();
            DeltaTime = time - LastFrameTime;
            LastFrameTime = time;

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            ImGui::Begin("Hello, world!");
           
          
            ImGui::End();
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            MainWindow->WindowUpdate(DeltaTime); 
        };
        ImGui_ImplOpenGL3_Shutdown();
         ImGui::DestroyContext();
        MainWindow->WindowEnd();
        delete MainWindow;
    }
   
}