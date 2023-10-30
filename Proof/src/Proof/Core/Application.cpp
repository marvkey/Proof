
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
#include "Proof/Renderer/GraphicsContext.h"
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
   
   // bool CallReset = false;
    Application::Application(const ApplicationConfiguration& config):
        m_ApplicationConfiguration(config) , m_RenderThread(config.ThreadingPolicy)
    {
        Build();

    }
    Application::~Application()
    {
        Release();
    }
    void Application::Build()
    {
        srand(time(NULL));
        Timer buildTimer;
        Proof::Log::Init();
        
        s_Instance = this;
        m_RenderThread.Run();

        m_LayerStack = Count<LayerStack>::Create();

        if (m_ApplicationConfiguration.ProjectPath.empty())
            m_ApplicationConfiguration.ProjectPath = "SandboxProject/SandboxProject.ProofProject";

        InputManager::Init();
        m_Project = Project::Load(m_ApplicationConfiguration.ProjectPath);

        //temporary
        m_GraphicsContext = GraphicsContext::Create();
        PF_CORE_ASSERT(m_Project, "Project is not valid");
        PF_ENGINE_INFO("Loaded Project {}", m_Project->GetConfig().Name);
        PF_ENGINE_TRACE("     Path {}", m_Project->GetConfig().Project.string());
        PF_ENGINE_TRACE("     AssetManager {}", m_Project->GetConfig().AssetManager.string());
        
        m_Window = Window::Create(m_ApplicationConfiguration.WindowConfiguration);
        m_Window->SetEventCallback([this](Event& e) {OnEvent(e); });

        Renderer::Init();
        m_RenderThread.Pump();
        if (m_ApplicationConfiguration.EnableImgui)
        {
            m_ImGuiMainLayer = ImGuiLayer::Create();
            m_LayerStack->PushLayer(m_ImGuiMainLayer);

        }
        PhysicsEngine::Init();
        ScriptEngine::Init();
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

        m_RenderThread.Terminate();
        ScriptEngine::ShutDown();
        PhysicsEngine::Release();
        AudioEngine::ShutDown();

        m_Project = nullptr;
        m_Window->m_SwapChain = nullptr;
        m_Window = nullptr;

        AssetManager::ShutDown();
        Renderer::Shutdown();
        InputManager::Destroy();

        
    }

    void Application::ImguiUpdate() {
        PF_PROFILE_FUNC();
        Timer time;
        m_ImGuiMainLayer->Begin();
        for (Count<Layer>& layer : m_LayerStack->V_LayerStack)
            layer->OnImGuiDraw();
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

         dispatcher.Dispatch<WindowMinimizeEvent>(PF_BIND_FN(Application::OnWindowMinimizeEvent));
         for (auto it = m_LayerStack->rbegin(); it != m_LayerStack->rend(); ++it)
         {
             if (e.Handled)
                 break;
             (*it)->OnEvent(e);
         }
         dispatcher.Dispatch<WindowCloseEvent>(PF_BIND_FN(Application::OnWindowCloseEvent));
    
    }

    bool Application::OnWindowMinimizeEvent(WindowMinimizeEvent& e) {
        if(e.IsWIndowMinimized())
            m_WindowMinimized = true;
        else
            m_WindowMinimized = false;

        return false;

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

    void Application::Run() 
    {
        float PreviousTime = glfwGetTime();

        while (m_IsRunning == true && m_ApplicationShouldShutdown == false)
        {
            PF_PROFILE_FRAME("MainThread");

            // render thread
            {
                PF_PROFILE_FUNC("Wait");
                Timer timer;

                m_RenderThread.BlockUntilRenderComplete();

                //m_PerformanceTimers.MainThreadWaitTime = timer.ElapsedMillis();

            }
            static uint64_t frameCounter = 0;
            //ProcessEvents(); // Poll events when both threads are idle


            m_RenderThread.NextFrame();
            // Start rendering previous frame

                m_RenderThread.Kick();

            if (!m_WindowMinimized)
            {
                Timer cpuTimer;

                Renderer::Submit([&]()
                    {
                        m_Window->BeginFrame();
                    });

                Renderer::BeginFrame();
                {
                    PF_PROFILE_FUNC("Application Layer::OnUpdate");
                    //PF_SCOPE_PERF("Application Layer::OnUpdate");

                    for (Count<Layer> layer : m_LayerStack->V_LayerStack)
                        layer->OnUpdate(m_DeltaTime);
                }

                Application* app = this;
                if (m_ApplicationConfiguration.EnableImgui)
                {
                    Renderer::Submit([app]() { app->ImguiUpdate(); });
                }
                Renderer::EndFrame();

                Renderer::Submit([&]() 
                    {
                        m_Window->EndFrame();
                    });
                m_CurrentFrameIndex = (m_CurrentFrameIndex + 1) % Renderer::GetConfig().FramesFlight;
            }

            float time = glfwGetTime();
            m_FrameTime = time - m_LastFrameTime;
            m_DeltaTime = glm::min<float>(m_FrameTime, 0.0333f);
            // limiting tehdelta time to about 30fps because we do not want drastic changes to obectusing delta time
            m_LastFrameTime = time;
            frameCounter++;

            //FrameMS = m_FrameTime * 1000.0f;
            //FPS = 1000.0f / FrameMS;

            FrameTime::WorldDeltaTime = m_DeltaTime;

            if (time - PreviousTime >= 1.0) {
                PreviousTime = time;
                frameCounter = 0;
            }

            FPS = (1.0 / (time - PreviousTime)) * frameCounter;
            FrameMS = ((time - PreviousTime) / frameCounter) * 1000;
            m_Window->WindowUpdate();
        }

        if (glfwWindowShouldClose((GLFWwindow*) m_Window->GetWindow()) == GLFW_TRUE)
        {
            m_ApplicationShouldShutdown = true;
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
    }
    void Application::Save()
    {
        AssetManager::SaveAllAssets();

    }
}