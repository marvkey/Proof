#pragma once
#include "Core.h"
#include "Proof/Core/Window.h"
#include <filesystem>
#include "RenderThread.h"
    namespace Proof {
    class Layer;
    class ImGuiLayer;
    class LayerStack;

    struct ApplicationConfiguration {
        std::string Name = "Applicaiton";
        bool EnableImgui = false;
        std::string ProjectPath;
        WindowConfiguration WindowConfiguration;
        ThreadingPolicy ThreadingPolicy = ThreadingPolicy::SingleThreaded;
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
       Count<class Project> GetProject() {
            return m_Project;
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
        Count<class GraphicsContext> GetGraphicsContext() { return m_GraphicsContext; }

    protected:
        Application(const ApplicationConfiguration& config);
        bool m_WindowMinimized = false;
        bool m_IsRunning = true;
        ApplicationConfiguration m_ApplicationConfiguration;

    private:
        bool m_ApplicationShouldShutdown = false;
        static Application* s_Instance;
        void ImguiUpdate();
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
        Count<class Project> m_Project;
        Count<class GraphicsContext> m_GraphicsContext;
        uint32_t m_CurrentFrameIndex = 0;

    private:
        RenderThread m_RenderThread;
        float m_DeltaTime;
        float m_LastFrameTime;
        float m_FrameTime;
        void Build();
        void Release();
        friend class Renderer;
    };
    Application* CreateApplication(int argc, char** argv);
}
