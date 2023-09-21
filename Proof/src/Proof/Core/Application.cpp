
#include "Proofprch.h"
#include "Application.h"
#include "Proof/ImGui/ImGuiLayer.h"
#include "Proof/Events/Event.h"
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

#include "Proof/Physics/PhysicsEngine.h"
#include "Proof/Utils/FileSystem.h"

#include "Proof/Core/Window.h"
#include "Proof/Core/LayerStack.h"
#include "Proof/Events/KeyEvent.h"
#include "Proof/Events/MouseEvent.h"
#include "Proof/Events/WindowEvent.h"
#include "Proof/Project/Project.h"
#include "Timer.h"
#include "Profile.h"
#include "Proof/Audio/AudioEngine.h"
#ifdef CreateDirectory
#undef CreateDirectory
#undef DeleteFile
#undef MoveFile
#undef CopyFile
#undef SetEnvironmentVariable
#undef GetEnvironmentVariable
#endif
namespace Proof {
    Application* Application::s_Instance = nullptr;
    float Application::FPS = 60.0f;
    float Application::FrameMS = 2.0f;
    float Application::m_ImguiFrameTime;
   
    bool CallReset = false;
    Application::Application(const ApplicationConfiguration& config):
        m_ApplicationConfiguration(config) 
    {
        Build();
    }
    Application::~Application()
    {
        Release();
    }
    void Application::Build()
    {
        Timer buildTimer;
        m_LayerStack = Count<LayerStack>::Create();
        srand(time(NULL));
        Proof::Log::Init();
        s_Instance = this;
        InputManager::Init();

        if (m_ApplicationConfiguration.ProjectPath.empty())
        {
            if (std::filesystem::exists("Proof") == false)
                std::filesystem::create_directory("Proof");
            m_ApplicationConfiguration.ProjectPath = "Proof/Proof.ProofProject";
            ProjectConfig config(std::filesystem::path("Proof/Proof.ProofProject"), "Proof");
            m_Project = Project::New(config);
            // we would scerilize when we save the project
            //ProjectSerilizer projectSerilizer(m_Project.get());
            //projectSerilizer.SerilizeText(m_ProjectPath);
        }
        else
        {
            m_Project = Project::Load(m_ApplicationConfiguration.ProjectPath);
        }
        PF_ENGINE_INFO("Loaded Project {}", m_Project->GetConfig().Name);
        PF_ENGINE_TRACE("     Path {}", m_Project->GetConfig().Project.string());
        PF_ENGINE_TRACE("     AssetManager {}", m_Project->GetConfig().AssetManager.string());
        PF_ENGINE_TRACE("     PROOF_DIR {}", (FileSystem::GetEnvironmentVariable)("PROOF_DIR"));
        
        m_Window = Window::Create(m_ApplicationConfiguration.WindowConfiguration);
        m_Window->SetEventCallback([this](Event& e) {OnEvent(e); });

        Renderer::Init(static_cast<Window*>(m_Window.get()));
        if (m_ApplicationConfiguration.EnableImgui)
        {
            m_ImGuiMainLayer = ImGuiLayer::Create();
            m_LayerStack->PushLayer(m_ImGuiMainLayer);

        }
        PhysicsEngine::Init();
       // ScriptEngine::Init();
        AudioEngine::Init();

        AssetManagerConfiguration assetManagerconfig;
        assetManagerconfig.AssetDirectory = m_Project->GetAssetDirectory();
        assetManagerconfig.AssetManager = m_Project->GetFromSystemProjectDirectory(m_Project->GetConfig().AssetManager);
        AssetManager::Init(assetManagerconfig);

        PF_ENGINE_INFO("Engine Systems Load {}m/s", buildTimer.ElapsedMillis());

    }

    void Application::Release()
    {
        PF_ENGINE_INFO("UnLoaded Project {}", m_Project->GetConfig().Name);
        PF_ENGINE_TRACE("     Path {}", m_Project->GetConfig().Project.string());
        PF_ENGINE_TRACE("     AssetManager {}", m_Project->GetConfig().AssetManager.string());
        PF_ENGINE_TRACE("     PROOF_DIR  {}", (FileSystem::GetEnvironmentVariable)("PROOF_DIR"));

        m_ImGuiMainLayer = nullptr;
        m_LayerStack->Empty();
        m_LayerStack = nullptr;

      //  ScriptEngine::Shutdown();
        PhysicsEngine::Release();
        AudioEngine::ShutDown();

        m_Project = nullptr;
        m_Window->m_SwapChain = nullptr;
        m_Window = nullptr;

        AssetManager::ShutDown();
        Renderer::Shutdown();
        InputManager::Destroy();

        
    }

    void Application::ImguiUpdate(float deltaTime) {
        PF_PROFILE_FUNC();
        Timer time;
        m_ImGuiMainLayer->Begin();
        for (Count<Layer>& layer : m_LayerStack->V_LayerStack)
            layer->OnImGuiDraw(deltaTime);
        m_ImGuiMainLayer->End();
        m_ImguiFrameTime = time.ElapsedMillis();
    }

    void Application::OnEvent(Event& e) {
        PF_PROFILE_FUNC();
        EventDispatcher dispatcher(e);
        /// PUSH LAYERS BACKWARDS
        /// WHEN WE GET UI WE MIGHT WANT TO ONLY RESPODN TO UI FIRST
         if (m_IsRunning == false)
            return;

         dispatcher.Dispatch<WindowResizeEvent>([&](WindowResizeEvent& e) {
             if (e.GetWhidt() != 0 or e.GetHeight() != 0)
             {
                 Renderer::OnWindowResize(e);
                 return false;
             }
             return false;
         });
         for (Count<Layer>& layer : m_LayerStack->V_LayerStack)
         {
             if (e.Handled)
                 break;
             layer->OnEvent(e);
         }

         dispatcher.Dispatch<WindowMinimizeEvent>(PF_BIND_FN(Application::OnWindowMinimizeEvent));
         dispatcher.Dispatch<WindowCloseEvent>(PF_BIND_FN(Application::OnWindowCloseEvent));
    
    }

    bool Application::OnWindowMinimizeEvent(WindowMinimizeEvent& e) {
        if(e.IsWIndowMinimized())
            WindowMinimized = true;
        else
            WindowMinimized = false;

        return true;

    }

    bool Application::OnMouseScrollEVent(MouseScrollEvent& e) {
        return false;
    }

    bool Application::OnKeyClicked(KeyClickedEvent& e) {
        return false;

    }

    bool Application::OnWindowCloseEvent(WindowCloseEvent& e) {
        m_IsRunning = false;
        return true;
    }

    

    void Application::Run() {
        uint64_t FrameCount = 0;
        float PreviousTime = glfwGetTime();
        float CurrentTime;
        while (m_IsRunning == true && m_ApplicationShouldShutdown == false) {
            PF_PROFILE_FRAME("Application::Update");
            Renderer::BeginFrame();
            
            if (Input::IsKeyClicked(KeyBoardKey::O))
            {
                AudioEngine::PlaySoundByPath("Proof/Assets/Sounds/hitHurt.wav");
            }
            float time = (float)glfwGetTime();
            CurrentTime = glfwGetTime();
            FrameCount++;
            const FrameTime DeltaTime = time - LastFrameTime;
            FrameTime::WorldDeltaTime = DeltaTime;

            if (WindowMinimized == false) {
                PF_PROFILE_FUNC("Layer OnUpdate");
                for (Count<Layer>& layer : m_LayerStack->V_LayerStack)
                    layer->OnUpdate(DeltaTime);
            }
            AudioEngine::OnUpdate(DeltaTime);
            if (m_ApplicationConfiguration.EnableImgui == true)
            {
                ImguiUpdate(DeltaTime);
            }
            m_Window->WindowUpdate();
            Renderer::EndFrame();
            if (CurrentTime - PreviousTime >= 1.0) {
                PreviousTime = CurrentTime;
                FrameCount = 0;
            }

            FPS = (1.0 / (CurrentTime - PreviousTime)) * FrameCount;
            FrameMS = ((CurrentTime - PreviousTime) / FrameCount) * 1000;
            LastFrameTime = time;
            ScopePerformanceTimer::ClearTimers();
        };
        if (glfwWindowShouldClose((GLFWwindow*) m_Window->GetWindow()) == GLFW_TRUE)
        {
            m_ApplicationShouldShutdown = true;
        }
        if (CallReset)
        {
            Release();
            Build();
            CallReset = false;
        }
    }

    void Application::PushLayer(Count<Layer> layer) {
        m_LayerStack->PushLayer(layer);
    }

    void Application::PushOverlay(Count<Layer> layer) {
        m_LayerStack->PushOverlay(layer);
    }

    void Application::OpenProject(const std::filesystem::path& path)
    {
        // closes project
        m_IsRunning = false;
        // autmatically call everythign 
        m_ApplicationConfiguration.ProjectPath = path.string();
        CallReset = true;
    }
    void Application::Save()
    {
        AssetManager::SaveAllAssets();

    }
}