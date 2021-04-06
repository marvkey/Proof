#pragma once
#include "Core.h"
#include "Platform/WindowsWindow.h"
#include "Platform/CurrentWindow.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "ImGUIOpenGL/imgui.h"
#include "ImGUIOpenGL/imgui_impl_glfw.h"
#include "ImGUIOpenGL/imgui_impl_opengl3.h"
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
        float LastFrameTime;
        friend int ::main(int argc, char** argv);
    };
    Application* CreateApplication();
}


