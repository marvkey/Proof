#pragma once
#include "Core.h"
#include "Platform/WindowsWindow.h"


namespace Proof {
    class Proof_API Application {
    public:

        virtual ~Application(); // main app//
        void Run();
        static void* GetWindow();
        static std::pair<float, float>GetMousePosition();
        static std::pair<int, int>GetWindowSize();
        static void SetWindowSize(int width, int height);
        
    protected:
        Application();
    private:
        static WindowsWindow* MainWindow;
    };
    Application* CreateApplication();
}


