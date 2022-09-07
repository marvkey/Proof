#pragma once
#include "Core.h"
#include "Proof/Core/Window.h"
#include "Proof/Core/LayerStack.h"
#include "Proof/Events/KeyEvent.h"
#include "Proof/Events/MouseEvent.h"
#include "Proof/Events/WindowEvent.h"
#include "Proof/Project/Project.h"
    namespace Proof {
    class Layer;
    class ImGuiLayer;
    class LayerStack;

    struct ApplicationConfiguration {
        std::string Name = "Applicaiton";
        bool EnableImgui = false;
        std::string ProjectPath;
        WindowConfiguration WindowConfiguration;
    };
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
        Application(const ApplicationConfiguration& config);
        bool WindowMinimized = false;
        bool IsRunning = true;
        std::string m_ProjectPath;
    private:
        void LayerUpdate(float deltaTime);
        void ImguiUpdate(float deltaTime);
        static float m_ImguiFrameTime;
        void OnEvent(Event& e);
        void OnWindowMinimizeEvent(WindowMinimizeEvent& e);
        void OnMouseScrollEVent(MouseScrollEvent& e);
        void OnKeyClicked(KeyClickedEvent& e);
        void OnWindowCloseEvent(WindowCloseEvent& e);
        LayerStack MainLayerStack;
        ImGuiLayer* ImGuiMainLayer;
        static Special<Window>MainWindow;
        friend class CurrentWindow;
        friend class FrameBuffer;
        float LastFrameTime;
        static float FPS;
        static float FrameMS;
        ApplicationConfiguration m_ApplicationConfiguration;
    };
    Application* CreateApplication(int argc, char** argv);
}
