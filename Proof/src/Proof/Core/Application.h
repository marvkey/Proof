#pragma once
#include "Core.h"
#include "Platform/WindowsWindow.h"
#include "Platform/CurrentWindow.h"
#include "Proof/Core/Layer.h"
#include "Proof/Core/LayerStack.h"
#include "Proof/ImGui/ImGuiLayer.h"
namespace Proof {
    class Proof_API Application {
    public:
        virtual ~Application(); // main app//
        void Run();
        void PushLayer(Layer* Layer);
        void PushOverlay(Layer* Layer);
    protected:
        Application();
    private:
        LayerStack MainLayerStack;
        ImGuiLayer* ImGuiMainLayer;
        static WindowsWindow* MainWindow;
        friend class CurrentWindow;
        float LastFrameTime;
        friend int ::main(int argc,char** argv);
        bool WindowMinimized = false;
    };
    Application* CreateApplication();
}