
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
#include "Proof/Renderer/Font.h"

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

#include "Proof/Input/ElevatedInputSystem/InputTypes.h"
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

        Proof::LogManager::Init();
        
        s_Instance = this;
        m_RenderThread.Run();

        m_LayerStack = Count<LayerStack>::Create();

        if (m_ApplicationConfiguration.ProjectPath.empty())
            m_ApplicationConfiguration.ProjectPath = "SandboxProject/SandboxProject.ProofProject";

        InputManager::Init();
        ElevatedInputKeys::Init();
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
        Font::Init();
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
        Font::ShutDown();

        m_Project = nullptr;

        AssetManager::ShutDown();
        Renderer::Shutdown();
        m_Window = nullptr;
        ElevatedInputKeys::ShutDown();
        InputManager::Destroy();

        m_EventCallbacks.clear();
        
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

    void Application::OnEvent(Event& e) 
    {
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
         dispatcher.Dispatch<WindowResizeEvent>(PF_BIND_FN(Application::OnWindowResizeEvent));
    
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

    bool Application::OnWindowResizeEvent(WindowResizeEvent& e)
    {
        /*
        const uint32_t width = e.GetWhidt(), height = e.GetHeight();
        if (width == 0 || height == 0)
        {
            return false;
        }
        PF_CORE_ASSERT(false);
        auto& window = m_Window;
        Renderer::Submit([&window, width, height]() mutable
            {
                window->GetSwapChain()->Resize({ width, height });
            });
        return false;
        */
        return false;
    }

    void Application::ProcessEvents()
    {
        std::scoped_lock<std::mutex> lock(m_EventQueueMutex);

        // Process custom event queue
        while (m_EventQueue.size() > 0)
        {
            auto& func = m_EventQueue.front();
            func();
            m_EventQueue.pop();
        }
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
            ProcessEvents(); // Poll events when both threads are idle
// has to be here because the frames are runnign with m_world delta time being 0 have to be initilized first
#if 1
            static bool s_firstFrame = true;
            float time = glfwGetTime();
            m_FrameTime = time - m_LastFrameTime;
            m_DeltaTime = glm::min<float>(m_FrameTime, 0.0333f);
            // limiting the delta time to about 30fps because we do not want drastic changes to obectusing delta time
            m_LastFrameTime = time;
            frameCounter++;

            //FrameMS = m_FrameTime * 1000.0f;
            //FPS = 1000.0f / FrameMS;

            FrameTime::WorldDeltaTime = m_DeltaTime;
            FrameTime::FrameCount += 1;

            if (time - PreviousTime >= 1.0) {
                PreviousTime = time;
                frameCounter = 0;
            }

            FPS = (1.0 / (time - PreviousTime)) * frameCounter;
            FrameMS = ((time - PreviousTime) / frameCounter) * 1000;
#else

            // Compute & sanitize dt (no upper clamp)
            static bool s_firstFrame = true;

            double now = glfwGetTime();
            if (s_firstFrame) { m_LastFrameTime = now; PreviousTime = now; s_firstFrame = false; }

            double rawDt = now - m_LastFrameTime;
            m_LastFrameTime = now;

            // If dt is NaN/Inf/<=0, use a safe fallback; otherwise only enforce a tiny minimum
            if (!std::isfinite(rawDt) || rawDt <= 0.0) {
                rawDt = 1.0 / 60.0;                 // fallback for first frame / timer glitch
            }
            else {
                const double minDt = 1e-6;          // tiny epsilon to avoid dt==0
                rawDt = std::max(rawDt, minDt);     // <-- ONLY MIN CLAMP
            }

            m_FrameTime = (float)rawDt;
            m_DeltaTime = (float)rawDt;

            FrameTime::WorldDeltaTime = m_DeltaTime;
            FrameTime::FrameCount += 1;

            // Rolling FPS window (safe math; no divide-by-zero)
            frameCounter++;
            double window = now - PreviousTime;
            if (window >= 1.0) {
                FPS = (double)frameCounter / window;
                FrameMS = (window / std::max(uint64_t(1), frameCounter)) * 1000.0;
                PreviousTime = now;
                frameCounter = 0;
            }
#endif

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