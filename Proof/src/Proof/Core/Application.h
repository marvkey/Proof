#pragma once
#include "Core.h"
#include "Platform/Window/WindowsWindow.h"
#include "CurrentWindow.h"
#include "Proof/Renderer/FrameBuffer.h"
#include "Proof/Core/LayerStack.h"

namespace Proof {
    class Layer;
    class ImGuiLayer;
    class LayerStack;
    class Proof_API Application {
    public:
        
        virtual ~Application(); // main app//
        void Run();
        void PushLayer(Layer* Layer);
        void PushOverlay(Layer* Layer);
        static Count<FrameBuffer>& GetScreenBuffer() {
            return ScreenFrameBuffer;
        }

        static uint32_t ViewPortWidth;
        static uint32_t ViewPortHeight;
        static float GetFPS() {return FPS;}
        static float GetFrameMS() {return FrameMS;};
    protected:
        Application();
        bool WindowMinimized = false;
        bool IsRunning = true;
    private:
        static Count<FrameBuffer> ScreenFrameBuffer;
        LayerStack MainLayerStack;
        ImGuiLayer* ImGuiMainLayer;
        static WindowsWindow* MainWindow;
        friend class CurrentWindow;
        friend class FrameBuffer;
        float LastFrameTime;
        friend int ::main(int argc,char** argv);
        static float FPS;
        static float FrameMS;
        Count<class GraphicsContext>m_GraphicsContext;
    };
    Application* CreateApplication();
}
