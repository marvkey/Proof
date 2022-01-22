
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
namespace Proof {
    Special <WindowsWindow> Application::MainWindow = nullptr;
    float Application::FPS = 60.0f;
    float Application::FrameMS = 2.0f;
    Application::Application(){
        srand(time(NULL));
        Proof::Log::Init();
        MainWindow = CreateSpecial<WindowsWindow>(); 

        m_GraphicsContext =GraphicsContext::Create(static_cast<Window*>(MainWindow.get()));
        Renderer::Init();
        ImGuiMainLayer = new ImGuiLayer();
        MainLayerStack.PushLayer(ImGuiMainLayer);
        MainWindow->SetEventCallback(PF_BIND_FN(Application::OnEvent));
        AssetManager::NewInitilizeAssets("config/AssetManager.ProofAssetManager");
        //AssetManager::InitilizeAssets("content");

    }

    void Application::OnEvent(Event& e) {
        EventDispatcher dispatcher(e);
        InputManager::OnEvent(e);
        dispatcher.Dispatch<WindowMinimizeEvent>(PF_BIND_FN(Application::OnWindowMinimizeEvent));
        dispatcher.Dispatch<MouseScrollEvent>(PF_BIND_FN(Application::OnMouseScrollEVent));
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
        glEnable(GL_BLEND);
     //   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
     //   glBlendFuncSeparate(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ZERO);
        CurrentWindow::SetvSync(false);
        while (glfwWindowShouldClose((GLFWwindow*)CurrentWindow::GetWindowAPI()) == false && Input::IsKeyClicked(KeyBoardKey::Escape)==false) {
            float FrameStart = glfwGetTime();
            float time = (float)glfwGetTime();
            CurrentTime = glfwGetTime();
            FrameCount++;
            const FrameTime DeltaTime = time - LastFrameTime;
            RendererCommand::Enable(ProofRenderTest::DepthTest);
            if (WindowMinimized == false) 
            {
                for (Layer* layer : MainLayerStack.V_LayerStack)
                    layer->OnUpdate(DeltaTime);
            }

            ImGuiMainLayer->Begin();
            for (Layer* layer : MainLayerStack.V_LayerStack)
                layer->OnImGuiDraw(DeltaTime);
            ImGuiMainLayer->End();

            MainWindow->WindowUpdate();
            if (CurrentTime - PreviousTime >= 1.0) {
                PreviousTime = CurrentTime;
                FrameCount = 0;
            }
            FPS = (1.0 / (CurrentTime - PreviousTime)) * FrameCount;
            FrameMS = ((CurrentTime - PreviousTime) / FrameCount) * 1000;

            LastFrameTime = time;
        };
        IsRunning = false;
        AssetManager::SaveAllAsset();
    }

    void Application::PushLayer(Layer* Layer) {
        MainLayerStack.PushLayer(Layer);
    }

    void Application::PushOverlay(Layer* Layer) {
        MainLayerStack.PushOverlay(Layer);
    }
}