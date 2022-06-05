#pragma once
#include "Core.h"
#include "Platform/Window/WindowsWindow.h"
#include "Proof/Renderer/ScreenFrameBuffer.h"
#include "Proof/Core/LayerStack.h"
#include "CurrentWindow.h"
#include "Proof/Events/KeyEvent.h"
#include "Proof/Events/MouseEvent.h"
#include "Proof/Events/WindowEvent.h"
#include "Proof/Scene/CSharpScripting/CSharpInitilize.h"
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

        static float GetFPS() {return FPS;}
        static float GetFrameMS() {return FrameMS;};
        static float GetImguiFrameTime() { return m_ImguiFrameTime; }
    protected:
        Application();
        bool WindowMinimized = false;
        bool IsRunning = true;
    private:
        static float m_ImguiFrameTime;
        //InitlizeCSharp ScriptingCharp;
        void OnEvent(Event& e);
        void OnWindowMinimizeEvent(WindowMinimizeEvent& e);
        void OnMouseScrollEVent(MouseScrollEvent& e);
        void OnKeyClicked(KeyClickedEvent& e);

        LayerStack MainLayerStack;
        ImGuiLayer* ImGuiMainLayer;
        static Special<WindowsWindow>MainWindow;
        friend class CurrentWindow;
        friend class FrameBuffer;
        float LastFrameTime;
        static float FPS;
        static float FrameMS;
    };
    Application* CreateApplication();
}
