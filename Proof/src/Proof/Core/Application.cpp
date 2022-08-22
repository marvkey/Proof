
#include "Proofprch.h"
#include "Application.h"
#include "Proof/ImGui/ImGuiLayer.h"
#include "Proof/Events/Event.h"
#include "Proof/Renderer/Renderer.h"
#include "Proof/Events/KeyEvent.h"
#include "Proof/Events/WindowEvent.h"
#include "Proof/Resources/Asset/AssetManager.h"
#include "Proof/Resources/Math/Random.h"
#include <GLFW/glfw3.h>
#include "Proof/Input/InputManager.h"
#include<chrono>
namespace Proof {
    Special <WindowsWindow> Application::MainWindow = nullptr;
    float Application::FPS = 60.0f;
    float Application::FrameMS = 2.0f;
    float Application::m_ImguiFrameTime;
    Application::Application(){
        srand(time(NULL));
        Proof::Log::Init();
        MainWindow = CreateSpecial<WindowsWindow>(); 

        Renderer::Init(static_cast<Window*>(MainWindow.get()));

        MainWindow->SetEventCallback(PF_BIND_FN(Application::OnEvent));
        if (Renderer::GetAPI() != RendererAPI::API::Vulkan) {
            AssetManager::NewInitilizeAssets("config/AssetManager.ProofAssetManager");
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
        
        /// PUSH LAYERS BACKWARDS
        /// WHEN WE GET UI WE MIGHT WANT TO ONLY RESPODN TO UI FIRST
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

        CurrentWindow::SetvSync(true);

        while (glfwWindowShouldClose((GLFWwindow*)CurrentWindow::GetWindowAPI()) == false && Input::IsKeyClicked(KeyBoardKey::Escape)==false) {
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
            if (Renderer::GetAPI() != RendererAPI::API::Vulkan)
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
            AssetManager::NewSaveAllAsset("config/AssetManager.ProofAssetManager");

        Renderer::Destroy();
    }

    void Application::PushLayer(Layer* Layer) {
        MainLayerStack.PushLayer(Layer);
    }

    void Application::PushOverlay(Layer* Layer) {
        MainLayerStack.PushOverlay(Layer);
    }
}