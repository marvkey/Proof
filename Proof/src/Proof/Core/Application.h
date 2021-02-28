#pragma once
#include "Core.h"
#include "Platform/WindowsWindow.h"
#include "Platform/CurrentWindow.h"

namespace Proof {
    class Proof_API Application {
    public:
        virtual ~Application(); // main app//
        void Run();
    protected:
        Application();
    private:
        static WindowsWindow* MainWindow;
        friend class CurrentWindow;
    };
    Application* CreateApplication();
}


