#pragma once
#include "Core.h"
#include "Platform/WindowsWindow.h"
#include "Platform/CurrentWindow.h"
int main(int argc, char** argv);

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
        friend int ::main(int argc, char** argv);

    };
    Application* CreateApplication();
}


