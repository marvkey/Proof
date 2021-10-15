#pragma once
#include "Core.h"
#include "Platform/Window/WindowsWindow.h"
#include "Proof/Renderer/ScreenFrameBuffer.h"
#include "Proof/Core/LayerStack.h"
#include "CurrentWindow.h"
#include "Proof/Events/KeyEvent.h"
#include "Proof/Events/MouseEvent.h"
#include "Proof/Renderer/GraphicsContext.h"
#include "Proof/Events/WindowEvent.h"
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
        static Count<ScreenFrameBuffer>& GetScreenBuffer() {
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
        void OnEvent(Event& e);
        void OnWindowMinimizeEvent(WindowMinimizeEvent& e);
        void OnMouseScrollEVent(MouseScrollEvent& e);
        void OnKeyClicked(KeyClickedEvent& e);

        static Count<ScreenFrameBuffer> ScreenFrameBuffer;
        LayerStack MainLayerStack;
        ImGuiLayer* ImGuiMainLayer;
        static WindowsWindow* MainWindow;
        friend class CurrentWindow;
        friend class FrameBuffer;
        float LastFrameTime;
        static float FPS;
        static float FrameMS;
        Count<class GraphicsContext>m_GraphicsContext;
    };
    Application* CreateApplication();
}
