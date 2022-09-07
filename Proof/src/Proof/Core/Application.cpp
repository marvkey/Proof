
#include "Proofprch.h"
#include "Application.h"
#include "Proof/ImGui/ImGuiLayer.h"
#include "Proof/Events/Event.h"
#include "Proof/Renderer/Renderer.h"
#include "Proof/Events/KeyEvent.h"
#include "Proof/Events/WindowEvent.h"
#include "Proof/Asset/AssetManager.h"
#include "Proof/Math/Random.h"
#include <GLFW/glfw3.h>
#include "Proof/Input/InputManager.h"
#include<chrono>
#include "Proof/Scripting/ScriptEngine.h"
#include "CurrentWindow.h"
#include "Proof/Project/ProjectSerilizer.h"
namespace Proof {
    Special <Window> Application::MainWindow = nullptr;
    float Application::FPS = 60.0f;
    float Application::FrameMS = 2.0f;
    float Application::m_ImguiFrameTime;
    Application::Application(const ApplicationConfiguration& config):
        m_ApplicationConfiguration(config) 
    {
        srand(time(NULL));
        Proof::Log::Init();
        Project::SetApp(this);
        if (m_ApplicationConfiguration.ProjectPath.empty()) {
            if(std::filesystem::exists("Proof")==false)
                std::filesystem::create_directory("Proof");
            (FileSystem::SetAEnvironmentVariable)("PROOF_PROJECT_DIR", "Proof");
            m_ApplicationConfiguration.ProjectPath = "Proof/Proof.ProofProject";
            m_ProjectPath = m_ApplicationConfiguration.ProjectPath;
            Project::Get()->m_Path = m_ProjectPath;
            ProjectSerilizer projectSerilizer(Project::Get());
            projectSerilizer.SerilizeText(m_ProjectPath);
        }
        else {
            m_ProjectPath = m_ApplicationConfiguration.ProjectPath;
            Project::Get()->m_Path = m_ProjectPath;
            (FileSystem::SetAEnvironmentVariable)("PROOF_PROJECT_DIR", Project::Get()->m_Path.root_directory().string());
            ProjectSerilizer projectSerilizer(Project::Get());
            projectSerilizer.DeSerilizeText(m_ProjectPath);
        }
       

        MainWindow = Window::Create(m_ApplicationConfiguration.WindowConfiguration); 
        MainWindow->SetEventCallback([this](Event& e) {OnEvent(e); });
        auto projdir = Project::GetProjectDir();
        AssetManagerConfiguration assetManagerconfig;
        assetManagerconfig.AssetDirectory = Project::Get()->m_AssetDirectory;
        assetManagerconfig.AssetManager = Project::Get()->m_AssetManager;
        AssetManager::Init(assetManagerconfig);

        Renderer::Init(static_cast<Window*>(MainWindow.get()));
        ScriptEngine::Init();
        
        if (Renderer::GetAPI() != RendererAPI::API::Vulkan) {
            AssetManager::InitilizeAssets();
            ImGuiMainLayer = new ImGuiLayer();
            MainLayerStack.PushLayer(ImGuiMainLayer);
        }
        
        PF_ENGINE_TRACE("Engine Load Done");
    }

    void Application::LayerUpdate(float deltaTime) {
        PF_PROFILE_FUNC();
        Renderer::BeginFrame();
        for (Layer* layer : MainLayerStack.V_LayerStack)
            layer->OnUpdate(deltaTime);
        Renderer::EndFrame();
    }

    void Application::ImguiUpdate(float deltaTime) {
        PF_PROFILE_FUNC("Application::ImguiUpdate");
        Timer time;
        ImGuiMainLayer->Begin();
        for (Layer* layer : MainLayerStack.V_LayerStack)
            layer->OnImGuiDraw(deltaTime);
        ImGuiMainLayer->End();
        m_ImguiFrameTime = time.TimePassedMillis();
    }

    void Application::OnEvent(Event& e) {
        PF_PROFILE_FUNC();
        
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowMinimizeEvent>(PF_BIND_FN(Application::OnWindowMinimizeEvent));
        dispatcher.Dispatch<WindowCloseEvent>(PF_BIND_FN(Application::OnWindowCloseEvent));
       
        /// PUSH LAYERS BACKWARDS
        /// WHEN WE GET UI WE MIGHT WANT TO ONLY RESPODN TO UI FIRST
         if (IsRunning == false)
            return;
         for (Layer* layer : MainLayerStack.V_LayerStack)
            layer->OnEvent(e);
    }

    void Application::OnWindowMinimizeEvent(WindowMinimizeEvent& e) {
        if(e.IsWIndowMinimized())
            WindowMinimized = true;
        else
            WindowMinimized = false;
    }

    void Application::OnMouseScrollEVent(MouseScrollEvent& e) {

    }

    void Application::OnKeyClicked(KeyClickedEvent& e) {
    }

    void Application::OnWindowCloseEvent(WindowCloseEvent& e) {
        IsRunning = false;
    }

    Application::~Application() {

    }

    void Application::Run() {
            
        uint64_t FrameCount = 0;
        float PreviousTime = glfwGetTime();
        float CurrentTime;
        if (Renderer::GetAPI() != RendererAPI::API::Vulkan)
            glEnable(GL_BLEND);
     //   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
     //   glBlendFuncSeparate(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ZERO);


        while (IsRunning  == true) {
            PF_PROFILE_FRAME("Application::Update");
            float time = (float)glfwGetTime();
            CurrentTime = glfwGetTime();
            FrameCount++;
            const FrameTime DeltaTime = time - LastFrameTime;
            FrameTime::WorldDeltaTime = DeltaTime;
            if (Renderer::GetAPI() != RendererAPI::API::Vulkan)
                RendererCommand::Enable(ProofRenderTest::DepthTest);

            if (WindowMinimized == false) 
                LayerUpdate(DeltaTime);
            if (Renderer::GetAPI() != RendererAPI::API::Vulkan && m_ApplicationConfiguration.EnableImgui == true)
                ImguiUpdate(DeltaTime);

            MainWindow->WindowUpdate();
            FrameTimersControll::s_FrameTimers.clear();

            if (CurrentTime - PreviousTime >= 1.0) {
                PreviousTime = CurrentTime;
                FrameCount = 0;
            }

            FPS = (1.0 / (CurrentTime - PreviousTime)) * FrameCount;
            FrameMS = ((CurrentTime - PreviousTime) / FrameCount) * 1000;
            LastFrameTime = time;
        };
        IsRunning = false;
        if (Renderer::GetAPI() != RendererAPI::API::Vulkan)
            AssetManager::SaveAllAssets();

        Renderer::Destroy();
    }

    void Application::PushLayer(Layer* Layer) {
        MainLayerStack.PushLayer(Layer);
    }

    void Application::PushOverlay(Layer* Layer) {
        MainLayerStack.PushOverlay(Layer);
    }
}