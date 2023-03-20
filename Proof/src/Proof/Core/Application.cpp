
#include "Proofprch.h"
#include "Application.h"
#include "Proof/ImGui/ImGuiLayer.h"
#include "Proof/Events/Event.h"
#include "Proof/Renderer/RendererBase.h"
#include "Proof/Events/KeyEvent.h"
#include "Proof/Events/WindowEvent.h"
#include "Proof/Asset/AssetManager.h"
#include "Proof/Math/Random.h"
#include <GLFW/glfw3.h>
#include "Proof/Input/InputManager.h"
#include<chrono>
#include "Proof/Scripting/ScriptEngine.h"
#include "Proof/Project/ProjectSerilizer.h"
#include "Proof/Renderer/Renderer.h"
#include "Proof/Renderer/SwapChain.h"

#include "Proof/Scene/Physics/PhysicsEngine.h"
namespace Proof {
    Application* Application::s_Instance = nullptr;
    float Application::FPS = 60.0f;
    float Application::FrameMS = 2.0f;
    float Application::m_ImguiFrameTime;
   
    Application::Application(const ApplicationConfiguration& config):
        m_ApplicationConfiguration(config) 
    {
        srand(time(NULL));
        Proof::Log::Init();
        s_Instance = this;
        if (m_ApplicationConfiguration.ProjectPath.empty()) {
            if(std::filesystem::exists("Proof")==false)
                std::filesystem::create_directory("Proof");
            (FileSystem::SetAnEnvironmentVariable)("PROOF_PROJECT_DIR", "Proof");
            m_ApplicationConfiguration.ProjectPath = "Proof/Proof.ProofProject";
            ProjectConfig config("Proof/Proof.ProofProject","Proof");
            m_Project = Project::New(config);
            // we would scerilize when we save the project
            //ProjectSerilizer projectSerilizer(m_Project.get());
            //projectSerilizer.SerilizeText(m_ProjectPath);
        }
        else {
            m_Project = Project::Load(m_ApplicationConfiguration.ProjectPath);
            (FileSystem::SetAnEnvironmentVariable)("PROOF_PROJECT_DIR", m_Project->GetProjectDirectory().string());
        }
       
        m_Window = Window::Create(m_ApplicationConfiguration.WindowConfiguration); 
        m_Window->SetEventCallback([this](Event& e) {OnEvent(e); });
        RendererBase::Init(static_cast<Window*>(m_Window.get()));
        InputManager::Init();

        AssetManagerConfiguration assetManagerconfig;
        assetManagerconfig.AssetDirectory = m_Project->GetAssetDirectory();
        assetManagerconfig.AssetManager = m_Project->GetFromSystemProjectDirectory(m_Project->GetConfig().AssetManager);
        AssetManager::Init(assetManagerconfig);

        PhysicsEngine::Init();
        ScriptEngine::Init();
        ImGuiMainLayer = new ImGuiLayer();
        MainLayerStack.PushLayer(ImGuiMainLayer);

        PF_ENGINE_TRACE("Engine Load Done");
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
        dispatcher.Dispatch<WindowResizeEvent>([&](WindowResizeEvent& e) {
            if(e.GetWhidt() != 0 or e.GetHeight()!=0)
                Renderer::OnWindowResize(e);
        });
        /// PUSH LAYERS BACKWARDS
        /// WHEN WE GET UI WE MIGHT WANT TO ONLY RESPODN TO UI FIRST
         if (m_IsRunning == false)
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
        m_IsRunning = false;
    }

    Application::~Application() {
        if (m_ApplicationShouldShutdown)
            std::exit(EXIT_SUCCESS);
    }

    void Application::Run() {
        uint64_t FrameCount = 0;
        float PreviousTime = glfwGetTime();
        float CurrentTime;
        while (m_IsRunning == true) {
            PF_PROFILE_FRAME("Application::Update");
            Renderer::BeginFrame();
            
            float time = (float)glfwGetTime();
            CurrentTime = glfwGetTime();
            FrameCount++;
            const FrameTime DeltaTime = time - LastFrameTime;
            FrameTime::WorldDeltaTime = DeltaTime;

            if (WindowMinimized == false) {
                PF_PROFILE_FUNC("Layer OnUpdate");
                for (Layer* layer : MainLayerStack.V_LayerStack)
                    layer->OnUpdate(DeltaTime);
            }
            if (m_ApplicationConfiguration.EnableImgui == true)
                ImguiUpdate(DeltaTime);
            m_Window->WindowUpdate();
            Renderer::EndFrame();

            FrameTimersControll::s_FrameTimers.clear();

            if (CurrentTime - PreviousTime >= 1.0) {
                PreviousTime = CurrentTime;
                FrameCount = 0;
            }

            FPS = (1.0 / (CurrentTime - PreviousTime)) * FrameCount;
            FrameMS = ((CurrentTime - PreviousTime) / FrameCount) * 1000;
            LastFrameTime = time;
        };
        if (glfwWindowShouldClose((GLFWwindow*) m_Window->GetWindow()) == GLFW_TRUE)
        {
            m_ApplicationShouldShutdown = true;
        }
        MainLayerStack.Empty();
        // remove the swpchain so it cna be deleted in the queue
        AssetManager::SaveAllAssets();
        m_Window->m_SwapChain = nullptr;
        RendererBase::Destroy();

        FileSystem::ClearEnvironmentVariables();
        ScriptEngine::Shutdown();
        PhysicsEngine::Release();
        m_Project = nullptr;
        m_Window = nullptr;
        InputManager::Destroy();
    }

    void Application::PushLayer(Layer* Layer) {
        MainLayerStack.PushLayer(Layer);
    }

    void Application::PushOverlay(Layer* Layer) {
        MainLayerStack.PushOverlay(Layer);
    }

    void Application::OpenProject(const std::filesystem::path& path)
    {
    }
    void Application::Save()
    {
        AssetManager::SaveAllAssets();

    }
}