#pragma once
#include "Core.h"
#include "Proof/Core/Window.h"
#include <filesystem>
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
        void PushLayer(Count<Layer> layer);
        void PushOverlay(Count<Layer> layer);
        static Application* Get() {
            return s_Instance;
        }
       class Project* GetProject() {
            return m_Project.get();
        }
        static float GetFPS() {return FPS;}
        static float GetFrameMS() {return FrameMS;};
        static float GetImguiFrameTime() { return m_ImguiFrameTime; }

        void OpenProject(const std::filesystem::path& path);

        void Save();
        Window* GetWindow() {
            return m_Window.get();
        }
        bool GetApplicationShouldShutdown() {
            return m_ApplicationShouldShutdown;
        }
        
        void ShutDown() {
            m_ApplicationShouldShutdown = true;
        }
        ApplicationConfiguration GetConfig() {
            return m_ApplicationConfiguration;
        }
        Count<class ImGuiLayer> GetImguiLayer() { return m_ImGuiMainLayer; }
    protected:
        Application(const ApplicationConfiguration& config);
        bool WindowMinimized = false;
        bool m_IsRunning = true;
    private:
        bool m_ApplicationShouldShutdown = false;
        static Application* s_Instance;
        void LayerUpdate(float deltaTime);
        void ImguiUpdate(float deltaTime);
        static float m_ImguiFrameTime;
        void OnEvent(Event& e);
        bool OnWindowMinimizeEvent(class WindowMinimizeEvent& e);
        bool OnMouseScrollEVent(class MouseScrollEvent& e);
        bool OnKeyClicked(class KeyClickedEvent& e);
        bool OnWindowCloseEvent(class WindowCloseEvent& e);
        Count<LayerStack> m_LayerStack;
        Count<class ImGuiLayer> m_ImGuiMainLayer;
        Special<Window>m_Window;
        friend class CurrentWindow;
        friend class FrameBuffer;
        float LastFrameTime;
        static float FPS;
        static float FrameMS;
        Special<class Project> m_Project;
        ApplicationConfiguration m_ApplicationConfiguration;
    private:
        void Build();
        void Release();
    };
    Application* CreateApplication(int argc, char** argv);
}
